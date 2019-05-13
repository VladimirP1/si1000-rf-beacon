//-----------------------------------------------------------------------------
// F97x_Oscillator_SmaRTClock.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to use the smaRTClock oscillator as a low
// frequency system clock.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
// 2) Connect the USB Debug Adapter to H8.
// 3) Place the VDD SELECT switch in the VREG position and power the board.
// 4) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 5) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 6) Measure the frequency output on P0.1 to be approximately 16.4 kHz.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 09 MAY 2014

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums

#include "F97x_SmaRTClock.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void PORT_Init (void);
void OSCILLATOR_Init (void);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = PCA0_PAGE;
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;      // Disable the watchdog timer

   PORT_Init ();                         // Initialize crossbar and GPIO
   RTC_Init (RTC_USE_LFO);               // Initialize SmaRTClock
   OSCILLATOR_Init ();                   // Initialize Oscillator

   while(1);
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
// This function initializes the system clock to use the SmaRTClock Oscillator.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   // Switch to RTC OSC
   CLKSEL = CLKSEL_CLKSL__RTC |
            CLKSEL_CLKDIV__SYSCLK_DIV_1;

   // Wait for RTC OSC to ready
   while ((CLKSEL & CLKSEL_CLKRDY__BMASK) == CLKSEL_CLKRDY__NOT_SET);


   SFRPAGE = SFRPAGE_save;
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
// P0.1   digital    push-pull             /SYSCLK
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;                // Save the current SFRPAGE
   SFRPAGE = CONFIG_PAGE;

   // Buffered System Clock Output
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
