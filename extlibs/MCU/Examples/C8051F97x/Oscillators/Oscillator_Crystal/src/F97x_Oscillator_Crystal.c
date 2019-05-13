//-----------------------------------------------------------------------------
// F97x_Oscillator_Crystal.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the oscillator to use an
// external crystal.
//
//
// How To Test:
// ------------
// 1) Populate Y2 with a crystal (20 MHz) and update CRYSTAL_FREQUENCY with the
//    frequency in Hz.
// 2) Populate R3 (10 MOhm) and C5/C4 according to the crystal datasheet
//    (12 pF), and remove SH16 and SH17 resistors.
// 3) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
// 4) Connect the USB Debug Adapter to H8.
// 5) Place the VDD SELECT switch (SW1) in the VREG position and power the board.
// 6) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 7) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 8) Place a scope probe on P0.1. The oscilloscope should show a waveform
//    of the crystal frequency.
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
// External Oscillator Crystal Frequency Selection
//-----------------------------------------------------------------------------
//
// Please initialize the following constant to the crystal frequency
//
#define CRYSTAL_FREQUENCY    20000000     //  Crystal Frequency in Hz

// XFCN Setting Macro
#if  (CRYSTAL_FREQUENCY <= 20000)
   #define XFCN 0
#elif(CRYSTAL_FREQUENCY <= 58000)
   #define XFCN 1
#elif(CRYSTAL_FREQUENCY <= 155000)
   #define XFCN 2
#elif(CRYSTAL_FREQUENCY <= 415000)
   #define XFCN 3
#elif(CRYSTAL_FREQUENCY <= 1100000)
   #define XFCN 4
#elif(CRYSTAL_FREQUENCY <= 3100000)
   #define XFCN 5
#elif(CRYSTAL_FREQUENCY <= 8200000)
   #define XFCN 6
#elif(CRYSTAL_FREQUENCY <= 25000000)
   #define XFCN 7
#else
   #error "Crystal Frequency must be less than 25MHz"
#endif

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Port_Init (void);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = PCA0_PAGE;
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // WDTE = 0 (clear watchdog timer
                                       // enable)

   Port_Init();                        // Initialize Port I/O
   Oscillator_Init ();                 // Initialize Oscillator

   while (1);                          // Infinite Loop

}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use an external crystal.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   // If system clock is > 14 MHz, bypass (disable) the one-shot timer
   FLSCL |= FLSCL_BYPASS__SYSCLK;      // Set the BYPASS bit

   OSCXCN = OSCXCN_XOSCMD__CRYSTAL |
            XFCN;

   // Wait for the external crystal to be ready
   while ((OSCXCN & OSCXCN_XCLKVLD__BMASK) == OSCXCN_XCLKVLD__NOT_SET);

   CLKSEL = CLKSEL_CLKSL__EXTOSC |
            CLKSEL_CLKDIV__SYSCLK_DIV_1;

   // Wait for divider to be applied
   while ((CLKSEL & CLKSEL_CLKRDY__BMASK) == CLKSEL_CLKRDY__NOT_SET);

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Port_Init
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
// P1.0   analog                  XTAL1
// P1.1   analog                  XTAL2
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = CONFIG_PAGE;

   // Instructions based off of 26.4.1 in datasheet
   P1MDIN &= ~0x03;                    // P1.0 and P1.1 (Crystal Pins) are
                                       // analog
   P1MDOUT &= ~0x03;
   P1SKIP = 0x03;
   P1 = 0xFF;

   P0SKIP = 0x01;                      // P0.0 has a capacitor on it
   P0MDOUT |= 0x02;                    // P0.1 is push-pull

   XBR0 = XBR0_SYSCKE__ENABLED;
   XBR1 = XBR1_XBARE__ENABLED |
          XBR1_WEAKPUD__PULL_UPS_ENABLED;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------