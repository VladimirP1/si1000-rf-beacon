//-----------------------------------------------------------------------------
// F97x_PCA0_High_Speed_Output.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use the PCA to generate a high frequency signal (KHz)
//
// This program sends a square wave out on an I/O pin, using the PCA's
// High-Speed Output Mode.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P0.2.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) Connect an oscilloscope to view the configured frequency on P0.2.
//    SYSCLK may be viewed by connecting an oscilloscope to P0.1.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 30 APR 2014


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK          20000000       // SYSCLK frequency in Hz

#define CEX0_FREQUENCY    100000       // Frequency to output on CEX0 (Hz)

#define DIVIDE_RATIO (SYSCLK/CEX0_FREQUENCY/2) // SYSCLK cycles per interrupt

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void PCA0_Init (void);

INTERRUPT_PROTO (PCA0_ISR, PCA0_IRQn);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

U16 Next_Compare_Value;                // Next edge to be sent out in HSO mode

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = PCA0_PAGE;
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable watchdog timer

   SYSCLK_Init ();                     // Initialize system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   PCA0_Init ();                       // Initialize PCA0

   IE_EA = 1;                          // Enable global interrupts

   while (1);
}


//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure system clock to low power internal oscillator / 8
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;            // Save the current SFRPAGE
   SFRPAGE = LEGACY_PAGE;

   CLKSEL = CLKSEL_CLKSL__LPOSC |        // Use Low Power Int Osc
            CLKSEL_CLKDIV__SYSCLK_DIV_1; // 20MHz/8 = 20MHz

   SFRPAGE = SFRPAGE_save;               // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the Crossbar and GPIO ports.
//
// P0.1 - digital  push-pull   SYSCLK
// P0.2 - digital  push-pull   PCA0 CEX0
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = CONFIG_PAGE;

   P0SKIP |= 0x01;                     // Skip P0.0

   P0MDOUT |= 0x02;                    // SYSCLK is push-pull
   P0MDOUT |= 0x04;                    // CEX0 is push-pull


   XBR0 |= XBR0_SYSCKE__ENABLED |      // SYSCLK to P0.1
           XBR0_PCA0ME__CEX0;          // PCA0 CEX0 freq to P0.2

   XBR1  = XBR1_XBARE__ENABLED |       // Enable crossbar
           XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups

   SFRPAGE = SFRPAGE_save;             // Restore saved SFRPAGE
}


//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base, and sets up High-Speed output
// mode for Module 0 (CEX0 pin).
//
// The frequency of the square wave generated at the CEX0 pin is defined by
// the parameter CEX0_FREQUENCY.
//
// The maximum square wave frequency output for this example is about 180 kHz.
// The minimum square wave frequency output for this example is about 155 Hz
//
// The PCA time base in this example is configured to use SYSCLK, and SYSCLK
// is set up to use the internal oscillator running at 20 MHz.
// Using different PCA clock sources or a different processor clock will
// result in a different frequency for the square wave, and different
// maximum and minimum options.
//
//    -------------------------------------------------------------------------
//    How "High Speed Output Mode" Works:
//
//       The PCA's High Speed Output Mode works by toggling the output pin
//    associated with the module every time the PCA0 register increments and
//    the new 16-bit PCA0 counter value matches the module's capture/compare
//    register (PCA0CPn). When initially enabled in high-speed output mode, the
//    CEXn pin associated with the module will be at a logic high state.  The
//    first match will cause a falling edge on the pin.  The next match will
//    cause a rising edge on the pin, and so on.
//
//    By loading the PCA0CPn register with the next match value every time a
//    match happens, arbitrary waveforms can be generated on the pin with high
//    levels of precision.
//    -------------------------------------------------------------------------
//
// When setting the capture/compare register for the next edge value, the low
//  byte of the PCA0CPn register (PCA0CPLn) should be written first, followed
//  by the high byte (PCA0CPHn).  Writing the low byte clears the ECOMn bit,
//  and writing the high byte will restore it.  This ensures that a match does
//  not occur until the full 16-bit value has been written to the compare
//  register.  Writing the high byte first will result in the ECOMn bit being
//  set to '0' after the 16-bit write, and the next match will not occur at
// the correct time.
//
// It is best to update the capture/compare register as soon after a match
//  occurs as possible so that the PCA counter will not have incremented past
//  the next desired edge value. This code implements the compare register
//  update in the PCA ISR upon a match interrupt.
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{

   SFRPAGE = PCA0_PAGE;

   PCA0CN = PCA0CN_CCF0__NOT_SET |     // Disable PCA0 and
            PCA0CN_CCF1__NOT_SET |     // clear flags
            PCA0CN_CCF2__NOT_SET |
            PCA0CN_CR__STOP      |
            PCA0CN_CF__NOT_SET;


   PCA0MD &= ~PCA0MD_CPS__FMASK;       // Timebase = SYSCLK
   PCA0MD |= PCA0MD_CPS__SYSCLK;


   PCA0CPM0 = PCA0CPM0_ECCF__ENABLED  | // High Speed Output
              PCA0CPM0_PWM__DISABLED  |
              PCA0CPM0_TOG__ENABLED   |
              PCA0CPM0_MAT__ENABLED   |
              PCA0CPM0_CAPN__DISABLED |
              PCA0CPM0_CAPP__DISABLED |
              PCA0CPM0_ECOM__ENABLED  |
              PCA0CPM0_PWM16__8_BIT;

   PCA0L = 0x00;                       // Reset PCA Counter Value to 0x0000
   PCA0H = 0x00;

   PCA0CPL0 = DIVIDE_RATIO & 0x00FF;   // Set up first edge
   PCA0CPH0 = (DIVIDE_RATIO & 0xFF00) >> 8;

   // Set up the variable for the following edge
   Next_Compare_Value = PCA0CP0 + DIVIDE_RATIO;


   EIE1 |= EIE1_EPCA0__ENABLED;

   PCA0CN_CR = 1;                     // Enable PCA0 counter
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This is the ISR for the PCA.  It handles the case when a match occurs on
// channel 0, and updates the PCA0CPn compare register with the value held in
// the global variable "Next_Compare_Value".
//
//-----------------------------------------------------------------------------
INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
   SFRPAGE = PCA0_PAGE;

   if (PCA0CN_CCF0 != 0){
           PCA0CN_CCF0 = 0;   // Clear module 0 interrupt flag.

           PCA0CPL0 = (Next_Compare_Value & 0x00FF);
           PCA0CPH0 = (Next_Compare_Value & 0xFF00)>>8;

           // Set up the variable for the following edge
           Next_Compare_Value = PCA0CP0 + DIVIDE_RATIO;
   }
   else // Interrupt was caused by other bits.
   {
      PCA0CN &= ~(PCA0CN_CF__BMASK   |   // Clear other interrupt flags for PCA
                  PCA0CN_CCF2__BMASK |
                  PCA0CN_CCF1__BMASK);
   }
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
