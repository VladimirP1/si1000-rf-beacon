//-----------------------------------------------------------------------------
// F85x_Clock_Stop.c
//-----------------------------------------------------------------------------
// Copyright (c) 2013 by Silicon Laboratories. 
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v10/License_Agreement_v10.htm
// Original content and implementation provided by Silicon Laboratories.
//
// Refer to F85x_Clock_Stop_Readme.txt for more information regarding this
// code example's functionality.
//
//
// Important User Warning:
//   If an 'F85x MCU is programmed to enter stop mode immediately post-reset,
//   the debugger will no longer be able to reprogram or interact with the
//   board, rendering it inoperable. This example code features a switch
//   which acts as a safeguard to prevent the MCU from entering stop mode
//   until after the switch is pressed. If the user chooses to remove this 
//   safeguard, they do so at their own risk.
//   
//
// How to Measure (Table 1.2 - Stop Mode Current):
// -----------------------------------------------
// 1) Remove the JP2 (Imeasure) shorting block and attach an ammeter between 
//    the JP2 (Imeasure) headers. 
// 2) Ensure that the P1.7-S1 (J27) shorting block is attached, and remove
//    all other shorting blocks.
// 3) Within the IDE, change the constant value <LDO_ENABLE> to toggle the Low
//    Dropout Regulator on or off. Choices are "ON" or "OFF". 
// 4) Compile and download the code to your C8051F85x_86x board.
// 5) Disconnect the evaluation board from the IDE, then run the code.
// 6) To activate stop mode, push S1 (P1.7) on the evaluation board.
//
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
//    - 2 AUG 2013
//
//
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>            // SFR declarations

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SBIT(S1, SFR_P1, 7);

//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

#define SW_PRESSED           0
#define SW_NOT_PRESSED       1

#define ON 1
#define OFF 0

//-----------------------------------------------------------------------------
// LDO_ENABLE
//-----------------------------------------------------------------------------
//
// This setting determines whether the processor will keep the Low Dropout
// Regulator (LDO) on or off while in stop mode. Possible settings include:
//
// #define LDO_ENABLE ON --> Low Dropout Regulator is ON
// #define LDO_ENABLE FF --> Low Dropout Regulator is OFF
//
//-----------------------------------------------------------------------------

//*** USER CONFIGURABLE VALUE ***

#define LDO_ENABLE ON

//*******************************

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Port_Init(void);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
  
   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   // Initializes S1 operation.
   Port_Init();

   // Wait until user pushes S1 to enter stop mode
   while( S1 == SW_NOT_PRESSED ){}

   // Enable/Disable LDO according to user settings   
   if( LDO_ENABLE )
   {
	   // Case A: LDO is Enabled
      REG0CN &= ~0x08;
   }  
   else
   {
	   // Case B: LDO is Disabled
      REG0CN |= 0x08;      
   }
   
   // Place the processor in stop mode
   PCON |= 0x02;

   while (1){} 
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
// P1.7   digital   open-drain   input   S1 (Switch 1)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // S1 (P1.7) Input - Stop Mode Enable Switch
   P1MDIN |= 0x80;                     // P1.7 is digital
   P1MDOUT &= ~0x80;                   // P1.7 is open-drain
   P1 |= 0x80;                         // Set the P1.7 latch to '1'

   XBR2 = 0x40;                        // Enable Crossbar and weak pull-ups
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
