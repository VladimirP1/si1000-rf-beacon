//-----------------------------------------------------------------------------
// F97x_PCA0_Software_Timer_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use the PCA as a software timer
//
// This program uses the PCA's Software Timer Mode to periodically schedule
// an interrupt.  Inside the ISR, a port pin is toggled to blink an LED on the
// target board.
//
// In this example, PCA Module 0 is used to generate the interrupt, and the
// port pin driving the LED is configured for push-pull mode.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) LED05 will blink.
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

#define LED_FREQUENCY   5L             // Frequency to blink LED at in Hz
#define T0_CLOCKS       200L           // Use 200 clocks per T0 Overflow

// SYSCLK cycles per interrupt
#define PCA_TIMEOUT ((SYSCLK/T0_CLOCKS)/LED_FREQUENCY/2)

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SBIT (LED,   SFR_P0, 5);              // '0' means ON, '1' means OFF

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
            CLKSEL_CLKDIV__SYSCLK_DIV_1; // 20MHz/1 = 20MHz

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
// P0.5 - digital  push-pull   LED
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;                // Save the current SFRPAGE
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x20;                          // Enable LED as a push-pull

   XBR1    = XBR1_XBARE__ENABLED |           // Enable crossbar
             XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups

   SFRPAGE = SFRPAGE_save;                   // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base to use T0 Overflows as a clock
// source.  Timer0 is also configured by this routine for 8-bit auto-reload
// mode, where T0_CLOCKS is the number of System clocks per T0 Overflow.
// The code then sets up Software Timer mode for Module 0 (CEX0 pin).
//
// On every interrupt from Module 0, software toggles the port I/O pin for the
// LED. The frequency of the LED toggling is determined by the parameter
// CEX0_FREQUENCY.
//
// The maximum frequency output for this example is about 50 kHz.
// The minimum frequency output for this example is about 1 Hz
//
// The PCA time base in this example is configured to use T0 overflows, and
// SYSCLK is set up for the internal oscillator running at 24.5 MHz.
// Using different PCA clock sources, different T0 reload values, or a
// different processor clock will result in a different frequency for the LED
// square wave, and different maximum and minimum options.
//
//    -------------------------------------------------------------------------
//    How "Software Timer Mode" Works:
//
//       The PCA's Software Timer Mode works by generating an interrupt for the
//    associated module every time the PCA0 register increments and the new
//    16-bit PCA0 counter value matches the module's capture/compare
//    register (PCA0CPn).
//
//    By loading the PCA0CPn register with the next match value every time a
//    match happens, arbitrarily timed interrupts can be generated.
//    -------------------------------------------------------------------------
//
// When setting the capture/compare register for the next match value, the low
// byte of the PCA0CPn register (PCA0CPLn) should be written first, followed
// by the high byte (PCA0CPHn).  Writing the low byte clears the ECOMn bit,
// and writing the high byte will restore it.  This ensures that a match does
// not occur until the full 16-bit value has been written to the compare
// register.  Writing the high byte first will result in the ECOMn bit being
// set to '0' after the 16-bit write, and the next match will not occur at
// the correct time.
//
// It is best to update the capture/compare register as soon after a match
// occurs as possible so that the PCA counter will not have incremented past
// the next desired edge value. This code implements the compare register
// update in the PCA ISR upon a match interrupt.
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = TIMER01_PAGE;

   TMOD &= 0xF0;                       // Clear all T0 control bits
   TMOD |= 0x02;                       // Timer0 in 8-bit reload mode

   CKCON |= 0x04;                      // T0 uses SYSCLK

   TH0 = (U8) -T0_CLOCKS;              // Set up reload value
   TL0 = (U8) -T0_CLOCKS;              // Set up initial value


   // Configure PCA time base; overflow interrupt disabled
   SFRPAGE = PCA0_PAGE;

   PCA0CN = PCA0CN_CR__STOP;           // Stop counter; clear all flags
   PCA0MD = PCA0MD_CPS__T0_OVERFLOW;   // Use Timer 0 as time base

   PCA0CPM0 = PCA0CPM0_ECOM__ENABLED | // Module 0 = Software Timer Mode,
              PCA0CPM0_MAT__ENABLED  | // Enable Module 0 Interrupt flag,
              PCA0CPM0_ECCF__ENABLED;  // Enable ECOM bit

   PCA0L = 0x00;                       // Reset PCA Counter Value to 0x0000
   PCA0H = 0x00;

   PCA0CPL0 = PCA_TIMEOUT & 0x00FF;    // Set up first match
   PCA0CPH0 = (PCA_TIMEOUT & 0xFF00) >> 8;

   // Set up the variable for the following match
   Next_Compare_Value = PCA0CP0 + PCA_TIMEOUT;

   EIE1 |= EIE1_EPCA0__ENABLED;        // Enable PCA interrupts

   PCA0CN_CR = 1;                      // Start PCA

   TCON_TR0 = 1;                       // Start Timer 0

   SFRPAGE = SFRPAGE_save;
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
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = PCA0_PAGE;

   if (PCA0CN_CCF0 != 0)                      // If Module 0 caused the interrupt
   {
      PCA0CN_CCF0 = 0;                        // Clear module 0 interrupt flag.

      PCA0CPL0 = (Next_Compare_Value & 0x00FF);
      PCA0CPH0 = (Next_Compare_Value & 0xFF00)>>8;

      LED = !LED;                             // Toggle LED05

      // Set up the variable for the following edge
      Next_Compare_Value = PCA0CP0 + PCA_TIMEOUT;
   }
   else // Interrupt was caused by other bits.
   {
      PCA0CN &= ~(PCA0CN_CF__BMASK   |       // Clear other interrupt flags for PCA
                  PCA0CN_CCF2__BMASK |
                  PCA0CN_CCF1__BMASK);
   }

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
