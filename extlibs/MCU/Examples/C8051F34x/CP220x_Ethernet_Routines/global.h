//-----------------------------------------------------------------------------
// global.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// 
// #includes all header files to make global functions available to the entire
// project.
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
// Global Includes
//-----------------------------------------------------------------------------

#include "typedefs.h"                  // Include Type Definitions
#include "config.h"                    // Include Hardware Configuration File
#include "const.h"                     // Global Constants

#include "MCU_Init.h"                  // Include MCU Initialization Files
#include "main.h"                      // Application Level Symbols

#include "CP220x_REG.h"                // Include CP220x Registers
#include "CP220x_CORE.h"               // Include CP220x Core Routines
#include "CP220x_ETH.h"                // Include CP220x Ethernet Routines

#include <string.h>                    // Standard 'C' Libraries
#include <intrins.h>

#if(UART_ENABLED)
   #include <stdio.h>                  // Include Standard I/O Routines
   #include <ctype.h>
#endif

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------