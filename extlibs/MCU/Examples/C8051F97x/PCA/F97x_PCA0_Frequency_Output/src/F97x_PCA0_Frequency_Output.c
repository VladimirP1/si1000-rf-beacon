//-----------------------------------------------------------------------------
// F97x_PCA0_Frequency_Output.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use the PCA to generate a high frequency signal (MHz)
//
// This program sends a square wave out of an I/O pin, using the PCA's
// Frequency Output Mode.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P0.2. For comparison
// SYSCLK is routed to P0.1.
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

#define SYSCLK       20000000/8        // SYSCLK frequency in Hz

#define CEX0_FREQUENCY   50000L        // Frequency to output on CEX0 (Hz)

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void PCA0_Init (void);

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
            CLKSEL_CLKDIV__SYSCLK_DIV_8; // 20MHz/8 = 2.5MHz

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
// This function configures the PCA time base, and sets up frequency output
// mode for Module 0 (CEX0 pin).
//
// The frequency generated at the CEX0 pin is equal to CEX0_FREQUENCY Hz,
// which is defined in the "Global Constants" section at the beginning of the
// file.
//
// The PCA time base in this example is configured to use SYSCLK.
// The frequency range that can be generated using this example is ~5 kHz to
// ~1 MHz when the processor clock is 2.5 MHz.  Using different PCA clock
// sources or a different processor clock will generate different frequency
// ranges.
//
//    -------------------------------------------------------------------------
//    How "Frequency Output Mode" Works:
//
//       The PCA's Frequency Output Mode works by toggling an output pin every
//    "N" PCA clock cycles.  The value of "N" should be loaded into the PCA0CPH
//    register for the module being used (in this case, module 0).  Whenever
//    the register PCA0L (PCA counter low byte) and the module's PCA0CPL value
//    are equivalent, two things happen:
//
//    1) The port pin associated with the PCA module toggles logic state
//    2) The value stored in PCA0CPH is added to PCA0CPL.
//
//    Using this mode, a square wave is produced at the output port pin of the
//    PCA module. The speed of the waveform can be changed by changing the
//    value of the PCA0CPH register.
//    -------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = PCA0_PAGE;

   PCA0CN = PCA0CN_CCF0__NOT_SET |     // Disable PCA0 and
            PCA0CN_CCF1__NOT_SET |     // clear flags
            PCA0CN_CCF2__NOT_SET |
            PCA0CN_CR__STOP      |
            PCA0CN_CF__NOT_SET;


   PCA0MD &= ~PCA0MD_CPS__FMASK;       // Timebase = SYSCLK
   PCA0MD |= PCA0MD_CPS__SYSCLK;


   PCA0CPM0 = PCA0CPM0_ECCF__DISABLED | // 16-bit PWM mode and
              PCA0CPM0_PWM__ENABLED   | // enable ECOM0 match and
              PCA0CPM0_TOG__ENABLED   | // interrupt flags
              PCA0CPM0_MAT__DISABLED  |
              PCA0CPM0_CAPN__DISABLED |
              PCA0CPM0_CAPP__DISABLED |
              PCA0CPM0_ECOM__ENABLED  |
              PCA0CPM0_PWM16__8_BIT;

   // Configure frequency for CEX0
   // PCA0CPH0 = (SYSCLK)/(2*CEX0_FREQUENCY), where:
   // SYSCLK = PCA time base
   // CEX0_FREQUENCY = desired frequency
   PCA0CPH0 = SYSCLK/(2*CEX0_FREQUENCY);

   PCA0CN_CR = 1;                      // Enable PCA0 counter

   SFRPAGE = SFRPAGE_save;             // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
