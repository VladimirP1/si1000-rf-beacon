//-----------------------------------------------------------------------------
// F85x_HF_Oscillator.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates the consistency of the high frequency oscillator
// with the VDD ranging from 2.2 V to 3.6 V (the full supply range).
//
// How to Use:
//
// 1) Download code to an 'F85x target board.
// 2) Run the program, and then click 'Disconnect'.
// 3) Connect a DC Power Supply to TP5 (VDD) and GND.
// 4) Connect an oscilloscope probe to P0.0 and GND.
// 5) Disconnect the Debug Adapter from the board.
// 6) Disconnect the DC Adapter.
// 7) Measure the frequency output on P0.0 while changing the output from
//    2.2 V to 3.6 V.
//
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (AJ)
//    - 11 JULY 2013
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
