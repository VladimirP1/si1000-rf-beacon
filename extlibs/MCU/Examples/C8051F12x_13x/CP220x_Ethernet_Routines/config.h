//-----------------------------------------------------------------------------
// config.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// 
// Contains global compile-time configuration options.
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
// MCU Definition
//-----------------------------------------------------------------------------
//
// The global <MCU> definition can be one of the following values:
// 
//    F340 -- Selects the 'F340 Family
//    
//    Note: Additional MCU Families may be added in the future.
//
#define F340 1
#define F120 2

#define MCU F120


//-----------------------------------------------------------------------------
// Hardware (Board) Definition
//-----------------------------------------------------------------------------
//
// The global <BOARD> definition can be one of the following values:
// 
//    F340_TB -- Selects the 'F340 Target Board with an AB4 Ethernet
//    Development Board
//    
//    CP2201EB -- Selects the CP2201 Evaluation Board
//
//    Note: Additional hardware may be added in the future.
//

#define F340_TB   1
#define CP2201EB  2
#define F120_TB   3

#define BOARD F120_TB


//-----------------------------------------------------------------------------
// MCU Configuration Options
//-----------------------------------------------------------------------------
//
// Please enable or disable the following options:
//
#define UART_ENABLED   1
 
#if(UART_ENABLED)
   #define  BAUDRATE   115200
#endif

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
