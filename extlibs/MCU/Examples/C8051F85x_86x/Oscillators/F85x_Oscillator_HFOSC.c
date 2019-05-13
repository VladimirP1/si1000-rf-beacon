//-----------------------------------------------------------------------------
// F85x_Oscillator_Precision.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates how to configure the system clock to use the
// precision internal oscillator at the maximum frequency.
//
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to the 'F85x UDP MCU board.
// 4) Run the code.
// 5) Measure the frequency output on P0.0.
//
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
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
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
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
// This function initializes the system clock to use the internal precision
// oscillator at the maximum frequency.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock
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
// P0.0   digital    push-pull             /SYSCLK
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0 = 0x80;                        // Route /SYSCLK to first available pin
   XBR2 = 0x40;                        // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
