//-----------------------------------------------------------------------------
// F85x_PortMatch.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This software shows the necessary configuration to use Port Match as an
// interrupt source.  If the S1 button is pressed, the Port Match Interrupt
// toggles LED1. If the S2 button is pressed, the Port Match Interrupt
// toggles LED2.
//
// Pinout:
//
// P1.0 - LED1
// P1.1 - LED2
// P1.2 - Port Match (for Switch 2)
//
// P1.7 - S1 (Switch 1) and Port Match
//
// P2.1 - S2 (Switch 2)
//
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - J27: DS6 P1.1 - P1.1/ADC0.9
//    - J27: S1 - P1.7
//    - JP2
// 2) Externally connect S2 to P1.2.
// 3) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 4) Compile and download code to the 'F85x UDP MCU board.
// 5) Run the code.
// 6) Press one of the switches. The corresponding LED will toggle.
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
//    -Initial Revision (TP)
//    -20 FEB 2013
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK        24500000         // Clock speed in Hz

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SBIT(LED1, SFR_P1, 0);                 // DS5 P1.0 LED
SBIT(LED2, SFR_P1, 1);                 // DS6 P1.1 LED

#define LED_ON               0
#define LED_OFF              1

SBIT(S1, SFR_P1, 7);
SBIT(S2, SFR_P2, 1);
SBIT(S2_PM, SFR_P1, 2);

#define SW_PRESSED           0
#define SW_NOT_PRESSED       1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Port_Init (void);

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

   P1MAT  |= 0x84;                     // Set up the port-match values on P1.2
   P1MASK |= 0x84;                     // and P1.7.
                                       // An event will occur if
                                       // (P1 & P1MASK) != (P1MAT & P1MASK)
                                       // The switch pins are 1 if the switch
                                       // isn't pressed.

   EIE1 |= 0x02;                       // Enable Port Match interrupts

   EA = 1;                             // Enable global interrupts

   while(1)
   {
      // Wait for both switches to be released before re-enabling port match
      // interrupts
      if ((S1 == 1) && (S2_PM == 1))
      {
         EIE1 |= 0x02;
      }
   }
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
// This routine initializes the system clock to use the precision internal
// oscillator divided by 1 as its clock source.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
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
// This function configures the crossbar and GPIO ports.
//
// Pinout:
//
// P1.0 - digital   push-pull   LED1
// P1.1 - digital   push-pull   LED2
//
// P1.3 - digital   open-drain  Port Match input for Switch 2
//
// P1.7 - digital   open-drain  S1 (Switch 1)
//
// P2.1 - digital   open-drain  S2 (Switch 2)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDOUT |= 0x03;                    // LED1 and LED2 are push-pull outputs

   XBR2 = 0x40;                        // Enable crossbar and weak pullups
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port Match ISR
//-----------------------------------------------------------------------------
//
// Whenever S1 (P1.7) is pressed, toggle LED1.  If S2 is pressed (connected to
// P1.2), toggle LED2.
//
// The Port Match interrupt occurs whenever (Pn & PnMASK) != (PnMAT & PnMASK).
// We must disable the interrupt and wait until the switches are released in
// main, or else the interrupt will trigger multiple times.  It is generally
// not good practice to sit in an ISR for long periods of time.
//
//-----------------------------------------------------------------------------
INTERRUPT(PM_ISR, INTERRUPT_PORT_MATCH)
{
   // If the Port Match event occured on S1, toggle LED1.
   if(S1 == 0)
   {
      LED1 = !LED1;
   }

   // If the Port Match event occured on S2 (connected to P1.2), toggle LED2
   if(S2_PM == 0)
   {
      LED2 = !LED2;
   }

   EIE1 &= ~0x02;                      // Disable Port Match interrupts to
                                       // prevent multiple interrupts from
                                       // occurring while the switches are
                                       // pressed
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
