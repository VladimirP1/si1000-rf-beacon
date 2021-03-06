//-----------------------------------------------------------------------------
// T622_EPROM_Utils.h
//-----------------------------------------------------------------------------
// Copyright 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// EPROM memory.
//
// Target:         C8051T622/3, 'T326/7
// Tool chain:     Keil / Raisonance
// Command Line:   None
//
//
// Release 1.0
//    -Initial Revision (GP / TP)
//    -02 SEP 2008
//

//-----------------------------------------------------------------------------
// Open Header #define
//-----------------------------------------------------------------------------

#ifndef _T622_EPROMUTILS_H_
#define _T622_EPROMUTILS_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "compiler_defs.h"
#include "T622_EPROM_Primitives.h"

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------

// EPROM read/write routines
void EPROM_Write (EPROMADDR dest, char *src, unsigned int numbytes);
char * EPROM_Read (char *dest, EPROMADDR src, unsigned int numbytes);

// EPROM copy routines
void EPROM_Copy (EPROMADDR dest, EPROMADDR src, unsigned int numbytes);

// EPROM test routines
void EPROM_Fill (EPROMADDR addr, U32 length, U8 fill);

//-----------------------------------------------------------------------------
// Close Header #define
//-----------------------------------------------------------------------------

#endif    // _T622_EPROMUTILS_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------