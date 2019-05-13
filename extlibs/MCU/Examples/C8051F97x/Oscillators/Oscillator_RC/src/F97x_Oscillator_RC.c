//-----------------------------------------------------------------------------
// F97x_Oscillator_RC.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the system clock using with an
// external RC network.
//
//
// How To Test:
// ------------
// 1) Connect a 33pF capacitor between P0.3 (XTAL2) and GND, and a
//    330kOhm resistor between P0.3 (XTAL2) and Vcc (3.3V). And
//    specify the desired frequency by changing the constant <RC_FREQUENCY>
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
//    pin is half of the oscillator frequency for RC and C modes.
//
//
// The capacitor should be no greater than 100 pF and the resistor should be no
// smaller than 10 kOhm.
// The oscillation frequency can be determined by the following equation:
//
//       Clock frequency (MHz) = 1.23 x 10^3 / ( R * C )
//
//       where
//       R = pull-up resistor value in kOhm
//       C = capacitor value on the XTAL2 pin in pF
//
// Note: Add 2pF stray capacitance to C.
//
// For a resistor value of 330K, and a capacitor value of 33pF, the
// expected RC_FREQUENCY is 112.9 kHz. The actual measured SYSCLK frequency
// on P0.1 was 54.5 kHz.
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
// Please initialize the following constant to the target frequency
//
#define RC_FREQUENCY  200000         //  Target RC Frequency in Hz

// XFCN Setting Macro
#if  (RC_FREQUENCY <= 25000)
   #define XFCN 0
#elif(RC_FREQUENCY <= 50000)
   #define XFCN 1
#elif(RC_FREQUENCY <= 100000)
   #define XFCN 2
#elif(RC_FREQUENCY <= 200000)
   #define XFCN 3
#elif(RC_FREQUENCY <= 400000)
   #define XFCN 4
#elif(RC_FREQUENCY <= 800000)
   #define XFCN 5
#elif(RC_FREQUENCY <= 1600000)
   #define XFCN 6
#elif(RC_FREQUENCY <= 3200000)
   #define XFCN 7
#else
   #error "RC Frequency must be less than or equal to 3.2 MHz"
#endif

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
// This function initializes the system clock to use the external RC
// oscillator.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   OSCXCN = OSCXCN_XOSCMD__RC |  // Enable External RC Oscillator Mode
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
// P1.1   analog                  XTAL2 (RC)
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