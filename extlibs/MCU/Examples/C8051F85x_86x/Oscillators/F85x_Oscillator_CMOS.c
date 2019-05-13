//-----------------------------------------------------------------------------
// F85x_Oscillator_CMOS.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates how to configure the oscillator for use with an
// external CMOS clock.
//
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J27: S1 - P1.7
//    - JP2
// 2) Ensure that a CMOS clock is connected to the EXTCLK pin (P0.3).
// 3) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 4) Compile and download code to the 'F85x UDP MCU board.
// 5) Run the code.
// 6) Press S1 to switch to the external oscillator. The divided precision
//    oscillator (24.5 MHz / 8) will be output on P0.0 until S1 is pressed.
// 7) Measure the frequency output on P0.0.
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
// Global VARIABLES
//-----------------------------------------------------------------------------

SBIT(S1, SFR_P1, 7);

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

   // Wait until S1 is pressed before switching to the external oscillator
   while (S1 == 1);

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
// This function initializes the system clock to use the external oscillator
// in CMOS clock mode.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor reset enabled

   CLKSEL = 0x01;                      // Select external oscillator as system
                                       // clock source
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
// P0.3   digital    open-drain/High-Z     EXTCLK
//
// P1.7   digital    open-drain/High-Z    S1
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // EXTCLK configuration
   P0MDIN |=  0x08;                    // P0.3 is digital
   P0SKIP |=  0x08;                    // P0.3 skipped in the Crossbar
   P0MDOUT &= ~0x08;                   // P0.3 output mode is open-drain
   P0 |=  0x08;                        // P0.3 output drivers are OFF

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0 = 0x80;                        // Route /SYSCLK to first available pin
   XBR2 = 0x40;                        // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
