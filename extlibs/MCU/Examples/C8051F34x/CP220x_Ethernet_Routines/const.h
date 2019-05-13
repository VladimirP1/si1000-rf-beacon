//-----------------------------------------------------------------------------
// const.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// 
// Contains global constants.
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
// Global Constants
//-----------------------------------------------------------------------------

#define OSC_ERROR   0x01               // Error Return Codes
#define CAL_ERROR   0x02
#define MEM_ERROR   0x04
#define FLASH_ERROR 0x08
#define MAC_ERROR   0x10
#define LINK_ERROR  0x20
#define DATA_ERROR  0x40

#define DEFAULT_TIMEOUT 100            // Default Timeout in ms  
#define ONE_SECOND      1000           // Timout equal to 1 second

#define TRUE  1
#define FALSE 0

#define NULL_PTR 0x000000              // Definition of Null Pointer


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------