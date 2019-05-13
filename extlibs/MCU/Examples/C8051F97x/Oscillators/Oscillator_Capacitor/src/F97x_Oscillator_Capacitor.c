//-----------------------------------------------------------------------------
// F97x_Oscillator_Capacitor.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the system clock using with an
// external capacitor.
//
//
// How To Test:
// ------------
// 1) Connect a 33pF capacitor between P0.3 (XTAL2) and GND. And
//    specify the desired XFCN setting by modifying the constant <XFCN>.
// 2) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
// 3) Connect the USB Debug Adapter to H8.
// 4) Place the VDD SELECT switch (SW1) in the VREG position and power the board.
// 5) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 6) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 7) Measure the frequency output on P0.1. Note that the SYSCLK routed on I/O
//    pin is half of the oscillator frequency for C and RC modes.
//
//
// The oscillation frequency and the required External Oscillator Frequency
// Control value (XFCN) can be determined by the following equation:
//
//       Clock frequency (MHz) = KF(XFCN) / (C * VDD)
//
//       where
//       KF(XFCN) is a factor based on XFCN.
//       C = capacitor value on the XTAL2 pin in pF
//       Vdd = power supply voltage in Volts
//
//       XFCN | K Factor
//       ---------------
//         0  |    0.87
//         1  |    2.6
//         2  |    7.7
//         3  |   22
//         4  |   65
//         5  |  180
//         6  |  664
//         7  | 1590
//
// The actual frequency is best determined by measurement. In this example,
// a 33pF capacitor was used with VDD = 3.3V. The actual measured SYSCLK
// frequency on P0.1 was 109.9 kHz when XFCN is 3.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (CM2/JL)
//    - 29 MAY 2014


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h>

//-----------------------------------------------------------------------------
// External Oscillator RC Frequency Selection
//-----------------------------------------------------------------------------
//

// XFCN Setting Macro
#define XFCN             3             // XFCN/KF setting.

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
// This function initializes the system clock to use the external C oscillator
// mode.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   OSCXCN = OSCXCN_XOSCMD__C |   // Enable External Capacitor Oscillator Mode
            XFCN;                // with appropriate XFCN setting

   // Wait for External OSC to ready
   while ((OSCXCN & OSCXCN_XCLKVLD__BMASK) == OSCXCN_XCLKVLD__NOT_SET);

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
// P1.1   analog                  XTAL2 (C Oscillator Mode)
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;             // Save the current SFRPAGE
   SFRPAGE = CONFIG_PAGE;

   // Instructions based off of 26.4.1 in datasheet
   P1MDIN &= ~0x02;                       // P1.1 (XTAL2) is analog
   P1 |= 0x02;
   P1SKIP |= 0x02;


   P0SKIP = 0x01;                         // P0.0 has a capacitor on it
   P0MDOUT |= 0x02;                       // P0.1 is push-pull

   XBR0 = XBR0_SYSCKE__ENABLED;
   XBR1 = XBR1_XBARE__ENABLED |           // Enable crossbar
          XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups

   SFRPAGE = SFRPAGE_save;                // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------