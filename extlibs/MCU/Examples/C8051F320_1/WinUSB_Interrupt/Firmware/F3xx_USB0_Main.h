//-----------------------------------------------------------------------------
// F3xx_USB0_Main.h
//-----------------------------------------------------------------------------
// Copyright 2008-2012 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Stub file for Firmware Template.
//
//
// How To Test:    See Readme.txt
//
//
// Target:         C8051F320/1,
//                 C8051F326/7,
//                 C8051F34x,
//                 C8051F38x,
//                 C8051T620/1/6/7, C8051T320/3,
//                 C8051T622/3, C8051T326/7
//
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.6
// Command Line:   See Readme.txt
// Project Name:   USB0_Interrupt
//
// Release 2.0
//    -Rewrite (CM)
//    -02 NOV 2012
//
// Release 1.1
//    -Changed "Usb_Init" to "USB_Init" (TP)
//    -07 OCT 2010
//
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//

#ifndef _USBTOUART_H_
#define _USBTOUART_H_

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------

#define SYSCLOCK           24000000

//-----------------------------------------------------------------------------
// External Global Variables
//-----------------------------------------------------------------------------

extern U8 Switch1State;    // Indicate status of switch
extern U8 Switch2State;    // starting at 0 == unpressed

extern U8 Potentiometer;   // Last read potentiometer value
extern U8 Temperature;     // Last read temperature sensor value

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void System_Init (void);
void USB0_Init (void);
void SetLed (U8 led, U8 on);

#endif // _USB_DESC_H_
