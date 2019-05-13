//-----------------------------------------------------------------------------
// F97x_Oscillator_CMOS.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the oscillator to use an
// external CMOS clock.
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
// 6) Measure the frequency output on P0.1 to be approximately 20 MHz.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (CM2/JL)
//    - 28 MAY 2014


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

   while (1);
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
// This function initializes the system clock to use the internal low power
// oscillator.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   // If system clock is > 14 MHz, bypass (disable) the one-shot timer
   FLSCL |= FLSCL_BYPASS__SYSCLK;      // Set the BYPASS bit

   OSCXCN = OSCXCN_XOSCMD__CMOS;       // External CMOS mode

   // In CMOS mode, the XCLKVLD flag will never be set
   //while ((OSCXCN & OSCXCN_XCLKVLD__BMASK) == OSCXCN_XCLKVLD__NOT_SET);

   // Switch to EXTOSC
   CLKSEL = CLKSEL_CLKSL__EXTOSC |
            CLKSEL_CLKDIV__SYSCLK_DIV_1;

   // Wait for divider to be applied
   while ((CLKSEL & CLKSEL_CLKRDY__BMASK) == CLKSEL_CLKRDY__NOT_SET);

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
// The oscillator pins should be configured as analog inputs when using the
// external oscillator in crystal mode.
//
// P0.1   digital    push-pull    /SYSCLK
//
// P1.1   digital    push-pull    XTAL2 (CMOS)
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;             // Save the current SFRPAGE
   SFRPAGE = CONFIG_PAGE;

   P0SKIP = 0x01;                         // P0.0 has a capacitor on it

   P0MDOUT |= 0x02;                       // P0.1 is push-pull

   // Oscillator Pin
   P1MDIN  |=  0x02;                      // P1.1 is digital CMOS input
   P1MDOUT &= ~0x02;                      //  Change mode to open drain
   P1      |=  0x02;                      //  Disable output drivers


   XBR0 = XBR0_SYSCKE__ENABLED;
   XBR1 = XBR1_XBARE__ENABLED |           // Enable crossbar
          XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups

   SFRPAGE = SFRPAGE_save;                // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------