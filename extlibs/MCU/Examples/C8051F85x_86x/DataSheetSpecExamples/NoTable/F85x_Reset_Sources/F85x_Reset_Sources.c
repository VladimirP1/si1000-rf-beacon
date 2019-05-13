//-----------------------------------------------------------------------------
// F85x_Reset_Sources.c
//-----------------------------------------------------------------------------
// Copyright (c) 2013 by Silicon Laboratories. 
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v10/License_Agreement_v10.htm
// Original content and implementation provided by Silicon Laboratories.
//
// Refer to F85x_Reset_Sources_Readme.txt for more information regarding this
// code example's functionality.
//
// ** BEFORE FOLLOWING ANY OF THESE STEPS, THE USER MUST SELECT THE RESET
//    MODE IN THE "GLOBAL DECLARATIONS" SECTION BELOW **
// 
// How to Test (Supply Monitor):
// -----------------------------
//    1) Ensure that the  J27-LED1 (P1.0) and J27-LED2 (P1.1) headers
//       have shorting blocks attached to them.
//    2) Download code to the 'F85x UDP MCU board
//    3) Disconnect the debug adapter and power adapter from the board.
//    4) Attach a configurable DC power supply to VDD (TP5) and GND (TP8).
//    5) Set VDD to 3.3V. The code should be running, with LED1 (DS5) lit up.
//    6) Lower VDD until it drops below the supply threshold, or until LED1
//       (DS5) is no longer lit.
//    7) Increase VDD back up to 3.3V. DS6 (LED2) should now be lit up instead,
//       signaling that a reset has occurred and the supply monitor was the
//       reset source.
//
// How to Test (Missing Clock Detector):
// -------------------------------------
//    1) Download code to the 'F85x UDP MCU board.
//    2) Run the program.  After a moment passes, halt the program.
//    3) Look at the current execution position. The RSTSRC register
//       should have detected a missing clock reset.
//
//    NOTE: The missing clock detector becomes activated by this firmware
//          through use of the low power oscillator, which becomes set
//          to a very low frequency. This low setting is slow enough that
//          the missing clock detector detects it as a missing clock trigger.
// 
// How to Test (Watchdog Timer):
// -----------------------------
//    1) Download code to the 'F85x UDP MCU board.
//    2) Run the program.  Wait approximately 12-15 seconds for the watchdog
//       timer to count up and overflow. Afterwards, halt code execution.
//    3) Look at the current execution position. The RSTSRC register
//       should have detected a watchdog timer reset.
//
// How to Test (Software Reset):
// -----------------------------
//    1) Download code to the 'F85x UDP MCU board.
//    2) Run the program.  After a moment passes, halt the program.
//    3) Look at the current execution position. The RSTSRC register
//       should have detected a software reset.
//
// How to Test (Comparator0 Reset):
// --------------------------------
//    1) Attach the variable potentiometer header (J27-POT) to the Comparator0 
//       negative input, CP0N.0 (for all F85x packages this should be P0.0).
//       Ensure that the potentiometer setting is at a low voltage.
//    2) Download code to the 'F85x UDP MCU board.
//    3) Run the program.  After a moment passes, halt the program.
//    4) The program should currently be waiting on a reset source to
//       activate. Continue program execution, then adjust the potentiometer
//       such that it crosses above the voltage of LDO.
//    5) Halt the program again. The RSTSRC register should now have
//       detected a Comparator0 reset. 
//
// Target:         C8051F85x/86x
// Target Board:   UPMU-F850 MCU-100
// Tool chain:     Generic
// Command Line:   None
//
//
// Revision History:
// -----------------
// Release 1.0
//    - Initial Revision (PMD)
//    - 2 JUL 2013
//
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>             // SFR declarations

//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

// ** USER MUST ENTER THEIR CHOICE FOR THIS DEFINE FIELD **

#define RESET_MODE RSTMD_WATCHDOG

// ********************************************************

#define RSTMD_SUPPLYMONITOR 0          // Choice A: Supply Monitor
#define RSTMD_CLOCKDETECT   1          // Choice B: Missing Clock Detector
#define RSTMD_WATCHDOG      2          // Choice C: Watchdog Timer
#define RSTMD_SOFTWARERST   3          // Choice D: Software Reset
#define RSTMD_COMPARATOR0   4          // Choice E: Comparator0 Reset

SBIT(DS5, SFR_P1, 0);                  // DS5='0' means ON
SBIT(DS6, SFR_P1, 1);                  // DS6='0' means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Reset_Init(void);
void Port_Init(void);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   Port_Init();

   // Possible Reset Sources are listed below. The current location of the
   // program after the debugger halts code execution will determine the reset
   // source.

   if( (RSTSRC & 0x20) == 0x20 )
   {
      // Reset Source was Comparator0 (C0RSEF)
      while(1){}                       
   }

   if( (RSTSRC & 0x10) == 0x10 )
   {
      // Reset Source was Software Reset (SWRSF)
      while(1){}                       
   }

   if( (RSTSRC & 0x08) == 0x08 )
   {
      // Reset Source was Watchdog Timer (WDTRSF)
      while(1){}                       
   }

   if( (RSTSRC & 0x04) == 0x04 )
   {
      // Reset Source was Missing Clock Detector (MCDRSF)
      while(1){}                       
   }

   if( (RSTSRC & 0x02) == 0x02 )
   {
      // Reset Source was Supply Monitor (PORSF)
	  DS6 = !DS6;
      while(1){}                       
   }

   if( (RSTSRC & 0x01) == 0x01 )
   {
      while(1)                         // Reset Source was Hardware (PINRSF)
	  {
	     Reset_Init();
		  DS5 = !DS5;                    // If LED1 On, device is awaiting reset.
		  while(1){}                     // Wait for reset source to trigger.
	  }
   }
   
   while(1){}
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Reset_Init()
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures the MCU to reset on an event according to the user's chosen mode.
// The trigger event is arranged to trigger, if possible.
//
//-----------------------------------------------------------------------------
void Reset_Init (void)
{

   int cycleDelay = 0;

   switch( RESET_MODE )
   {
   
      case RSTMD_SUPPLYMONITOR:
	        RSTSRC |= 0x02;             // Enables Supply Monitor reset source
	        break;

      case RSTMD_CLOCKDETECT:
	        RSTSRC |= 0x04;             // Enables Missing Clock reset source
		     CLKSEL = 0x72;              // Sets system clock to 80 kHz/128 (slow)
		     break;

      case RSTMD_WATCHDOG:
	        OSCLCN |= 0x83;             // Set the LFOSC divider to 1
	        WDTCN = 0xA5;               // Enable Watchdog, then do nothing
		     break;
   
      case RSTMD_SOFTWARERST:
	        RSTSRC |= 0x10;             // Forces a software system reset
		     break;
   
      case RSTMD_COMPARATOR0:
	        CPT0CN = 0x80;              // Enable Comparator0 peripheral
           CPT0MX = 0x08;              // Set Positive Input as LDO (1.8V), set
		                                 // Negative Input as External pin CP0N.0 
	        RSTSRC |= 0x20;             // Enable Comparator0 reset source
		     break;


   }

   return;
   
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Configure the Crossbar and GPIO ports.
//
// P1.0 - LED (push-pull output)
// P1.1 - LED (push-pull output)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDOUT |= 0x03;                    // Enable LEDs as a push-pull output
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
