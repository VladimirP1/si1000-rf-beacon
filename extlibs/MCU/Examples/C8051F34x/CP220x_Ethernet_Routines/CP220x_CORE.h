//-----------------------------------------------------------------------------
// CP220x_CORE.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// 
// Header file for core functions used when accessing the CP220x.
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

// Duplex Modes
#define AUTO_NEG    0
#define HALF_DUPLEX 1
#define FULL_DUPLEX 2

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

extern bit full_duplex;

//-----------------------------------------------------------------------------
// Exported Function Definitions
//-----------------------------------------------------------------------------

// Initialization routines
extern unsigned char CP220x_HW_Reset(void);
extern unsigned char PHY_Init();
extern void MAC_Init();

// MAC read/write routines
extern unsigned int MAC_Read(unsigned char mac_reg_offset);
extern void MAC_Write(unsigned char mac_reg_offset, unsigned int mac_reg_data);
extern void MAC_GetAddress(MACADDRESS* pMAC);
extern void MAC_SetAddress(MACADDRESS* pMAC);

// FLASH read/write/erase routines
extern unsigned char CPFLASH_ByteRead (unsigned int addr);
extern unsigned char CPFLASH_ByteWrite (unsigned int addr, char byte);
extern unsigned char CPFLASH_PageErase (unsigned int addr);

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------