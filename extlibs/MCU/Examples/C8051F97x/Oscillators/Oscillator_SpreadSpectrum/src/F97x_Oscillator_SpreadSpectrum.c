//-----------------------------------------------------------------------------
// F97x_Oscillator_SpreadSpectrum.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the oscillator to use the
// internal precision oscillator with spread spectrum clock dithering enabled.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
// 2) Connect the USB Debug Adapter to H8.
// 3) Place the VDD SELECT switch (SW1) in the VREG position and power the board.
// 4) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 5) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 6) Measure the frequency output on P0.1 to be approximately 24.5 MHz.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2/JL)
//    - 29 MAY 2014


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = PCA0_PAGE;
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // WDTE = 0 (clear watchdog timer
                                       // enable)

   PORT_Init();                        // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator

   while (1);                          // Infinite Loop

}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal precision
// oscillator with spread spectrum enabled.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   FLSCL = FLSCL_BYPASS__SYSCLK;              // Set FLRT bit for >14 MHz

   OSCICL |= OSCICL_SSE__ENABLED;             // Enable Spread Spectrum

   REG0CN |= REG0CN_OSCBIAS__ENABLED;         // Force precision osc bias
   OSCICN |= OSCICN_IOSCEN__ENABLED;          // Enable High Presicion Int Osc

   // Wait for HFOSC to ready
   while ((OSCICN & OSCICN_IFRDY__BMASK) == OSCICN_IFRDY__NOT_SET);

   // Switch to HFOSC
   CLKSEL = CLKSEL_CLKSL__HFOSC |
            CLKSEL_CLKDIV__SYSCLK_DIV_1;

   // Wait for divider to be applied
   while ((CLKSEL & CLKSEL_CLKRDY__BMASK) == CLKSEL_CLKRDY__NOT_SET);

   PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;   // Clear all wake-up source flags
                                       // This allows low power oscillator
                                       // to be turned off
   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P0.1   digital    push-pull             /SYSCLK
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;                // Save the current SFRPAGE
   SFRPAGE = CONFIG_PAGE;

   P0SKIP = 0x01;                            // P0.0 has a capacitor on it
   P0MDOUT |= 0x02;                          // P0.1 is push-pull

   XBR0 = XBR0_SYSCKE__ENABLED;
   XBR1 = XBR1_XBARE__ENABLED |              // Enable crossbar
          XBR1_WEAKPUD__PULL_UPS_ENABLED;    // with weak pullups

   SFRPAGE = SFRPAGE_save;                   // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------