//-----------------------------------------------------------------------------
// F97x_Ports_SwitchLED.c
//-----------------------------------------------------------------------------
// Copyright 2010 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Configure port pins as different types of inputs and outputs
//
// This program demonstrates how to configure port pins as digital inputs
// and outputs.  The C8051F970 target board has 2 LEDs connected to port pins.
// The program turn on/off these 2 LEDs alternatively.
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
// 5) Push BTN03 or BTN04 buttons and see that the corresponding
//    LED05 or LED15 turns on.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 02 MAY 2014

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SBIT (LED05,   SFR_P0, 5);         // '0' means ON, '1' means OFF
SBIT (LED15,   SFR_P1, 5);         // '0' means ON, '1' means OFF

SBIT (BTN03,   SFR_P0, 3);         // '0' means ON, '1' means OFF
SBIT (BTN04,   SFR_P0, 4);         // '0' means ON, '1' means OFF


//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       20000000/8    // SYSCLK frequency in Hz

#define LED_ON             0
#define LED_OFF            1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;

   SYSCLK_Init ();                      // Initialize system clock
   PORT_Init ();                        // Initialize crossbar and GPIO


   LED05 = LED_ON;
   LED15 = LED_OFF;

   while (1)
   {
      if (BTN03 == 0)                   // If switch pressed
      {
         LED05 = LED_ON;                // Turn off LED
      }
      else
      {
         LED05 = LED_OFF;               // Turn off LED
      }


      if (BTN04 == 0)                   // If switch pressed
      {
         LED15 = LED_ON;                // Turn off LED
      }
      else
      {
         LED15 = LED_OFF;               // Turn off LED
      }
   }
}


//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// This routine initializes the system clock to use the internal 20 MHz
// oscillator as its clock source divided by 8.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   CLKSEL = CLKSEL_CLKSL__LPOSC |         // Use Low Power Int Osc
            CLKSEL_CLKDIV__SYSCLK_DIV_8;  // 20MHz/8 = 2.5MHz
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
// P1.5 - digital  push-pull   LED
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x10;                       // P0.5 is push-pull
   P1MDOUT |= 0x10;                       // P1.5 is push-pull


   XBR1 = XBR1_XBARE__ENABLED |           // Enable crossbar
          XBR1_WEAKPUD__PULL_UPS_ENABLED; //  with weak pullups

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
