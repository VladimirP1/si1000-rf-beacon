//-----------------------------------------------------------------------------
// F85x_Voltage_Monitor.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v10/License_Agreement_v10.htm
// Original content and implementation provided by Silicon Laboratories.
//
//
// Refer to F85x_Voltage_Monitor_Readme.txt for more information regarding the
// functionality of this code example.
//
//
// How To Measure (Table 1.2 - Voltage Supply Monitor Current):
// ------------------------------------------------------------
// 1) Ensure a shorting block is placed across Imeasure (JP2).
// 2) Place VDD Select (SW1) in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Download and run the code on the target board.
// 4) Remove all of the shorting blocks on the board except for the block(s)
//    across the following pins:
//    a) S1 and P1.7 on J27
// 5) Measure the current across Imeasure. This reading is the current with the
//    supply monitor enabled.
// 6) Use switch at P1.7 (S1) to disable the supply monitor.
// 7) Record the current reading and subtract the value from the previously
//     recorded value. The difference should be within the range of the values
//     listed on the datasheet for Voltage Supply Monitor under Table 1.2.
//
//
// How To Measure (Table 1.3 - Voltage Supply Monitor Threshold):
// --------------------------------------------------------------
// 1) Ensure a shorting block is placed across Imeasure (JP2).
// 2) Place VDD Select (SW1) in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Download and run the code on the target board.
// 4) Using the IDE, stop the program and disconnect the board.
// 5) Physically disconnect the debugger from the board and remove all power
//    sources.
// 6) Remove all of the shorting blocks on the board except for the block(s)
//    across the following pins:
//    a) Imeasure on JP2
// 7) Connect a power supply to VDD (TP5) and set to 3.3V.
// 8) Use an oscilloscope to monitor RESET (TP3).
// 9) Reduce the supply voltage until RESET is pulled low. Verify the voltage
//    at which RESET is pulled low is within the values given for VDD Supply
//    Monitor Threshold in Table 1.3.
//
//
// Target:         C8051F85x/86x
// Target Board:   UPMU-F850 MCU-100
// Tool Chain:     Generic
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (BGD)
//    - 17 JUL 2013
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>            // SFR declarations

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK          24500000       // SYSCLK frequency in Hz

#define SW_PRESSED      0
#define SW_NOT_PRESSED  1

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SBIT(S1, SFR_P1, 7);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void Port_Init (void);

INTERRUPT_PROTO (PM_ISR, INTERRUPT_PORT_MATCH);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{

   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   SYSCLK_Init ();                     // Initialize system clock to 24.5 MHz
   Port_Init ();                       // Initialize crossbar and GPIO

   VDM0CN |= 0x80;                     // Enable VDD Monitor

   EA = 1;                             // Enable global interrupts

   while (1) {}                        // Loop forever
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
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator divided by 1 as its clock source.  Also enables missing clock
// detector reset.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // set VDD Monitor as reset source

   CLKSEL = 0x00;                      // Set system clock to 24.5 MHz
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the Crossbar and GPIO ports.
//
// P1.7  digital  open-drain  S1 (Switch 1)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{

   // Port 1 initialization
   P1MDIN |= 0x80;                     // Set P1.7 as digital
   P1MDOUT &= ~0x80;                   // Set P1.7 as open-drain
   P1 |= 0x80;                         // Set the P1.7 latch to '1'

   P1MAT |= 0x80;                      // Set up the port-match value on P1.7
   P1MASK |= 0x80;                     // An event will occur if
                                       // (P1 & P1MASK) != (P1MAT & P1MASK)
                                       // The switch pins are 1 if the switch
                                       // isn't pressed.

   EIE1 |= 0x02;                       // Enable Port Match interrupts

   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port Match ISR
//-----------------------------------------------------------------------------
//
// Whenever S1 (P1.7) is pressed, the VDD Monitor enable is toggled. When the
// VDD Monitor is enabled, the VDD Monitor is selected as a reset source.
//
// The Port Match interrupt occurs whenever (Pn & PnMASK) != (PnMAT & PnMASK).
// We must disable the interrupt and wait until the switches are released in
// main, or else the interrupt will trigger multiple times.  It is generally
// not good practice to sit in an ISR for long periods of time.
//
//-----------------------------------------------------------------------------
INTERRUPT (PM_ISR, INTERRUPT_PORT_MATCH)
{
   if (S1 == SW_PRESSED)
   {
      // If the Voltage Supply Monitor is enabled
      if((VDM0CN & 0x80) == 0x80)
      {
         VDM0CN &= ~0x80;              // Disable VDD Monitor
         RSTSRC &= ~0x02;              // Clear VDD Monitor as reset source
      }
      else
      {
         VDM0CN |= 0x80;               // Disable VDD Monitor
         RSTSRC |= 0x02;               // Set VDD Monitor as reset source
      }

      P1MAT &= ~0x80;                  // Set port match to trigger at switch
                                       // rising edge to prevent multiple
                                       // interrupts from occurring while the
                                       // switch is pressed
   }
   else
   {
      P1MAT |= 0x80;                   // Reset port match to trigger at switch
                                       // falling edge
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
