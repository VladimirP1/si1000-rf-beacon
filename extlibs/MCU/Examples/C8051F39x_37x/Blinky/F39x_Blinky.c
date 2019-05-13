//-----------------------------------------------------------------------------
// F39x_Blinky.c
//-----------------------------------------------------------------------------
// Copyright (C) 2012 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program flashes the green LED on the C8051F390 UDP MCU board about
// five times a second using the interrupt handler for Timer2.
//
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J18: P1.1_LED - P1.1
//    - J12
//    - J13
// 2) Download code to the 'F39x UDP MCU board
// 3) Run the program.  If the green LED flashes, the program is working
//
//
// Target:         C8051F390
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (ST)
//    - 16 JAN 2012
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <compiler_defs.h>             // compiler declarations
#include <C8051F390_defs.h>            // SFR declarations

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000/8        // SYSCLK frequency in Hz

SBIT(LED, SFR_P1, 1);                  // LED='1' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void PORT_Init (void);
void Timer2_Init (int counts);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
                                       // enable)
   PORT_Init ();

   Timer2_Init (SYSCLK / 12 / 10);     // Init Timer2 to generate interrupts
                                       // at a 10 Hz rate.

   EA = 1;                             // Enable global interrupts

   while (1) {}                        // Spin forever
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Configure the Crossbar and GPIO ports.
// P1.3 - LED (push-pull output)
//
void PORT_Init (void)
{
   P1SKIP   = 0x02;                       // skip the LED pin from crossbar
   P1MDOUT |= 0x02;                       // enable LED as a push-pull output
   XBR0     = 0x00;                       // no digital peripherals selected
   XBR1     = 0x40;                       // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/48 as its time base.
//
void Timer2_Init (int counts)
{
   TMR2CN  = 0x00;                        // Stop Timer2; Clear TF2;
                                          // use SYSCLK/12 as timebase
   CKCON  &= ~0x60;                       // Timer2 clocked based on T2XCLK;

   TMR2RL  = -counts;                     // Init reload values
   TMR2    = 0xffff;                      // set to reload immediately
   ET2     = 1;                           // enable Timer2 interrupts
   TR2     = 1;                           // start Timer2
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
// This routine changes the state of the LED whenever Timer2 overflows.
//
INTERRUPT(Timer2_ISR, INTERRUPT_TIMER2)
{
   TF2H = 0;                              // clear Timer2 interrupt flag
   LED = !LED;                            // change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------