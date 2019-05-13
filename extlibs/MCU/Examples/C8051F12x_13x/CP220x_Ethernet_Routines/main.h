//-----------------------------------------------------------------------------
// main.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// 
// Header file for CP220x Send and Receive Example.
// 
// FID:            
// Target:         Multiple
// Tool chain:     Keil C51 7.20 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.72 
// Command Line:   See Readme.txt
// Project Name:   CP220x_Ethernet_Routines
//
// 
//
// Release 1.1
//    - Configures C8051F120 SYSCLK to 98 MHz
//
// Release 1.0
//    -Initial Release (FB)
//    -30 MAY 2006
//

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------
extern IPADDRESS  xdata MYIP;
extern MACADDRESS xdata MYMAC; 
extern MACADDRESS xdata BROADCASTMAC;

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
void Ethernet_ISR(void);

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------