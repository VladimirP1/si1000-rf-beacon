//-----------------------------------------------------------------------------
// c8051f3xx.h
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
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//

#ifndef _c8051f3xx_H_
#define _c8051f3xx_H_

//-----------------------------------------------------------------------------
// Target Definitions
//-----------------------------------------------------------------------------

#define MCU_F320     0
#define MCU_F326     1
#define MCU_F340     2
#define MCU_F380     3
#define MCU_T620     4
#define MCU_T622     5

//-----------------------------------------------------------------------------
// Target Selection
//-----------------------------------------------------------------------------

#define TARGET_MCU   MCU_F326

//-----------------------------------------------------------------------------
// Target Includes
//-----------------------------------------------------------------------------

#if (TARGET_MCU == MCU_F320)
#include <compiler_defs.h>
#include <C8051F320_defs.h>
#endif // (TARGET_MCU == MCU_F320)

#if (TARGET_MCU == MCU_F326)
#include <compiler_defs.h>
#include <C8051F326_defs.h>
#endif // (TARGET_MCU == MCU_F326)

#if (TARGET_MCU == MCU_F340)
#include <compiler_defs.h>
#include <C8051F340_defs.h>
#endif // (TARGET_MCU == MCU_F340)

#if (TARGET_MCU == MCU_F380)
#include <compiler_defs.h>
#include <C8051F380_defs.h>
#endif // (TARGET_MCU == MCU_F380)

#if (TARGET_MCU == MCU_T620)
#include <compiler_defs.h>
#include <C8051T620_defs.h>
#endif // (TARGET_MCU == MCU_T620)

#if (TARGET_MCU == MCU_T622)
#include <compiler_defs.h>
#include <C8051T622_defs.h>
#endif // (TARGET_MCU == MCU_T622)

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------

// Return the LSB given a U16
#define LOBYTE(w)       ((U8)(w))

// Return the MSB given a U16
#define HIBYTE(w)       ((U8)(((U16)(w) >> 8) & 0xFF))

// Define a little-endian multi-byte array initialization given a U16
#define LE_ARRAY(w)     LOBYTE(w), HIBYTE(w)

// Define a big-endian multi-byte array initialization given a U16
#define BE_ARRAY(w)     HIBYTE(w), LOBYTE(w)

#endif // _c8051f3xx_H_
