//-----------------------------------------------------------------------------
// F85x_FlashUtils.h
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

#ifndef _F85X_FLASHUTILS_H_
#define _F85X_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "F85x_FlashPrimitives.h"

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------

// Flash read/write/erase routines
void FLASH_Write (FLADDR dest, char *src, unsigned int numbytes);
char * FLASH_Read (char *dest, FLADDR src, unsigned int numbytes);
void FLASH_Clear (FLADDR addr, unsigned int numbytes);

// Flash update/copy routines
void FLASH_Update (FLADDR dest, char *src, unsigned int numbytes);
void FLASH_Copy (FLADDR dest, FLADDR src, unsigned numbytes);

// Flash test routines
void FLASH_Fill (FLADDR addr, ULONG length, UCHAR fill);

//-----------------------------------------------------------------------------
// Close Header #define
//-----------------------------------------------------------------------------

#endif    // _F85x_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------