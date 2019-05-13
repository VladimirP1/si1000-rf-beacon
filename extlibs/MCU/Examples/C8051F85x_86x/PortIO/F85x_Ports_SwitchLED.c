//-----------------------------------------------------------------------------
// F85x_Ports_SwitchLED.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates how to configure port pins as digital inputs
// and outputs.  The C8051F85x MCU card has two push-button switches
// and two LEDs connected to port pins.  The program constantly checks the
// status of the switches and if S1 is closed, it turns off LED1; otherwise,
// LED1 is on.  S2 controls LED2 in the same manner.
//
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - J27: DS6 P1.1 - P1.1/ADC0.9
//    - J27: S1 - P1.7
//    - J27: S2 - P2.1
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to the 'F85x UDP MCU board.
// 4) Run the code.
// 5) Push the buttons (S1 or S2) and see that the corresponding LED (DS5 or
//    DS6) turns off.
//
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 19 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>            // SFR declarations

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SBIT(LED1, SFR_P1, 0);                 // DS5 P1.0 LED
SBIT(LED2, SFR_P1, 1);                 // DS6 P1.1 LED
SBIT(S1, SFR_P1, 7);
SBIT(S2, SFR_P2, 1);

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK      24500000/8         // Clock speed in Hz (default)

#define SW_PRESSED           0
#define SW_NOT_PRESSED       1

#define LED_ON               0
#define LED_OFF              1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Port_Init (void);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   Port_Init();                        // Initialize Port I/O

   while (1)
   {
      // Set the state of LED1
      if (S1 == SW_PRESSED)
      {
         LED1 = LED_OFF;
      }
      else
      {
         LED1 = LED_ON;
      }

      // Set the state of LED2
      if (S2 == SW_PRESSED)
      {
         LED2 = LED_OFF;
      }
      else
      {
         LED2 = LED_ON;
      }
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// To configure a pin as a digital input, the pin is configured as digital
// and open-drain and the port latch should be set to a '1'. The weak-pullups
// are used to pull the pins high. Pressing the switch pulls the pins low.
//
// To configure a pin as a digital output, the pin is configured as digital
// and push-pull.
//
// An output pin can also be configured to be an open-drain output if the
// system requires it. For example, if the pin is an output on a multi-device
// bus, it will probably be configured as an open-drain output instead of a
// push-pull output. For the purposes of this example, the output pins are
// configured as push-pull outputs because the pins need to drive LEDs.
//
// P1.0   digital   push-pull     LED1
// P1.1   digital   push-pull     LED2
//
// P1.7   digital   open-drain    S1 (Switch 1)
//
// P2.1   digital   open-drain    S2 (Switch 2)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDIN |= 0x83;                     // P1.0, P1.1, and P1.7 are digital

   P1MDOUT &= ~0x80;                   // P1.7 is open-drain
   P2MDOUT &= ~0x02;                   // P2.1 is open-drain
   P1MDOUT |= 0x03;                    // P1.0 and P1.1 are push-pull

   P1 |= 0x80;                         // Set the P1.7 latch to '1'
   P2 |= 0x02;                         // Set the P2.1 latch to '1'

   XBR2 = 0x40;                        // Enable crossbar and enable
                                       // weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
