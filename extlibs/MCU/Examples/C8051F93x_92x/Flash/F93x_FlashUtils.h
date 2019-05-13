//-----------------------------------------------------------------------------
// F93x_FlashUtils.h
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
// Target:         C8051F93x/92x
// Tool chain:     Keil C51 8.00
// Command Line:   None
//
//
// Release 1.0
//    -Initial Revision (TP)
//    -14 JAN 2013
//

//-----------------------------------------------------------------------------
// Open Header #define
//-----------------------------------------------------------------------------

#ifndef _F93X_FLASHUTILS_H_
#define _F93X_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "F93x_FlashPrimitives.h"

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------

// FLASH read/write/erase routines
void FLASH_Write (FLADDR dest, char *src, unsigned int numbytes, bit SFLE);
char * FLASH_Read (char *dest, FLADDR src, unsigned int numbytes, bit SFLE);
void FLASH_Clear (FLADDR addr, unsigned int numbytes, bit SFLE);

// FLASH update/copy routines
void FLASH_Update (FLADDR dest, char *src, unsigned int numbytes, bit SFLE);
void FLASH_Copy (FLADDR dest, bit destSFLE, FLADDR src, bit srcSFLE,
                 unsigned numbytes);

// FLASH test routines
void FLASH_Fill (FLADDR addr, ULONG length, UCHAR fill, bit SFLE);

//-----------------------------------------------------------------------------
// Close Header #define
//-----------------------------------------------------------------------------

#endif    // _F410_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------