//-----------------------------------------------------------------------------
// F85x_FlashPrimitives.h
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// flash memory.
//
// Target:         C8051F85x/86x
// Tool chain:     Keil C51 8.00
// Command Line:   None
//
//
// Release 1.0
//    -Initial Revision (TP)
//    -19 FEB 2013
//

//-----------------------------------------------------------------------------
// Open Header #define
//-----------------------------------------------------------------------------

#ifndef _F85X_FLASHPRIMITIVES_H_
#define _F85X_FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// Structures, Unions, Enumerations, and Type Definitions
//-----------------------------------------------------------------------------

typedef unsigned long ULONG;
typedef unsigned int  UINT;
typedef unsigned char UCHAR;
typedef UINT FLADDR;

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#ifndef FLASH_PAGESIZE
#define FLASH_PAGESIZE 512
#endif

#ifndef FLASH_TEMP
#define FLASH_TEMP 0x1C00L             // For 8 kB Flash devices
//#define FLASH_TEMP 0x0C00L           // For 4 kB Flash devices
//#define FLASH_TEMP 0x0400L           // For 2 kB Flash devices
#endif

#ifndef FLASH_LAST
#define FLASH_LAST 0x1E00L             // For 8 kB Flash devices
//#define FLASH_TEMP 0x0E00L           // For 4 kB Flash devices
//#define FLASH_TEMP 0x0600L           // For 2 kB Flash devices
#endif

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------

void          FLASH_ByteWrite (FLADDR addr, char byte);
unsigned char FLASH_ByteRead  (FLADDR addr);
void          FLASH_PageErase (FLADDR addr);

//-----------------------------------------------------------------------------
// Close Header #define
//-----------------------------------------------------------------------------

#endif    // _F85X_FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------