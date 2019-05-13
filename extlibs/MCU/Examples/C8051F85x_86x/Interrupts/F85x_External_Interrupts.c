//-----------------------------------------------------------------------------
// F85x_External_Interrupts.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This software shows the necessary configuration to use External Interrupt 0
// (/INT0) or External Interrupt 1 (/INT1) as an interrupt source.  The code
// executes the initialization routines and then spins in an infinite while()
// loop.  If the S1 button is pressed, then the edge-triggered /INT0 input on
// P0.2 will cause an interrupt and toggle LED1. If the S2 button is pressed,
// then the edge-triggered /INT1 input on P0.3 will cause an interrupt and
// toggle the LED2.
//
// Pinout:
//
// P0.2 - /INT0 input (S1 connected externally)
// P0.3 - /INT1 input (S2 connected externally)
//
// P1.0 - LED1
// P1.1 - LED2
//
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - J27: DS6 P1.1 - P1.1/ADC0.9
//    - JP2
// 2) Externally connect S1 to P0.2 and S2 to P0.3 by using J27.
// 3) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 4) Compile and download code to the 'F85x UDP MCU board.
// 5) Run the code.
// 6) Press the switches.  Every time a switch is pressed, one of
//    the LEDs should toggle.
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
// Include Files
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>            // SFR declarations

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SBIT (LED1, SFR_P1, 0);                // DS5 P1.0 LED
SBIT (LED2, SFR_P1, 1);                // DS6 P1.1 LED

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000    // Clock speed in Hz

#define LED_ON  0
#define LED_OFF 1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);           // Configure the system clock
void Port_Init (void);                 // Configure the Crossbar and GPIO
void Ext_Interrupt_Init (void);        // Configure External Interrupts (/INT0
                                       // and /INT1)

INTERRUPT_PROTO (INT0_ISR, INTERRUPT_INT0);
INTERRUPT_PROTO (INT1_ISR, INTERRUPT_INT1);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   Oscillator_Init();                  // Initialize the system clock
   Port_Init ();                       // Initialize crossbar and GPIO
   Ext_Interrupt_Init();               // Initialize External Interrupts

   EA = 1;

   while(1);                           // Infinite while loop waiting for
                                       // an interrupt from /INT0 or /INT1
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
// This function initializes the system clock to use the internal high
// frequency oscillator at its maximum frequency.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   CLKSEL = 0x00;                      // Set system clock to 24.5 MHz

   RSTSRC = 0x06;                      // Enable missing clock detector
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
// P0.2   digital   open-drain    /INT0 (S1 connected externally)
// P0.3   digital   open-drain    /INT1 (S2 connected externally)
//
// P1.0   digital   push-pull     LED1
// P1.1   digital   push-pull     LED2
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P0MDIN |= 0x0C;                     // P0.2, P0.3 are digital
   P1MDIN |= 0x03;                     // P1.0, P1.1 are digital

   P0MDOUT &= ~0x0C;                   // P0.2, P0.3 are open-drain
   P1MDOUT |= 0x03;                    // P1.0, P1.1 are push-pull

   P0 |= 0x0C;                         // Set P0.2, P0.3 latches to '1'

   XBR2 = 0x40;                        // Enable crossbar and enable
                                       // weak pull-ups
}

//-----------------------------------------------------------------------------
// Ext_Interrupt_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures and enables /INT0 and /INT1 (External Interrupts)
// as negative edge-triggered.
//
//-----------------------------------------------------------------------------
void Ext_Interrupt_Init (void)
{
   TCON = 0x05;                        // /INT 0 and /INT 1 are edge triggered

   IT01CF = 0x32;                      // /INT0 active low; /INT0 on P0.2;
                                       // /INT1 active low; /INT1 on P0.3

   EX0 = 1;                            // Enable /INT0 interrupts
   EX1 = 1;                            // Enable /INT1 interrupts
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// /INT0 ISR
//-----------------------------------------------------------------------------
//
// Whenever a negative edge appears on P0.2, toggle LED1.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
INTERRUPT(INT0_ISR, INTERRUPT_INT0)
{
   LED1 = !LED1;
}

//-----------------------------------------------------------------------------
// /INT1 ISR
//-----------------------------------------------------------------------------
//
// Whenever a negative edge appears on P0.3, toggle LED2.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
INTERRUPT(INT1_ISR, INTERRUPT_INT1)
{
   LED2 = !LED2;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
