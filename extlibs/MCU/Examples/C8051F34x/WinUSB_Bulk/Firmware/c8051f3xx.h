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
//                 C8051F38x
//
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.6
// Command Line:   See Readme.txt
// Project Name:   USB0_Bulk
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

//-----------------------------------------------------------------------------
// Target Selection
//-----------------------------------------------------------------------------

#define TARGET_MCU   MCU_F340

//-----------------------------------------------------------------------------
// Target Includes
//-----------------------------------------------------------------------------

#if (TARGET_MCU == MCU_F320)

#include <compiler_defs.h>
#include <C8051F320_defs.h>

#define FLASH_PAGE_SIZE       512      // Size of flash page in bytes
#define FLASH_START           0x1200   // Address of first writeable page
#define FLASH_NUM_PAGES       21       // Number of writeable pages
#define FLASH_GROUP_WRITE_EN  0        // Doesn't support 2-byte flash writes

#endif // (TARGET_MCU == MCU_F320)


#if (TARGET_MCU == MCU_F326)

#include <compiler_defs.h>
#include <C8051F326_defs.h>

#define FLASH_PAGE_SIZE       512      // Size of flash page in bytes
#define FLASH_START           0x1200   // Address of first writeable page
#define FLASH_NUM_PAGES       21       // Number of writeable pages
#define FLASH_GROUP_WRITE_EN  0        // Doesn't support 2-byte flash writes

#endif // (TARGET_MCU == MCU_F326)


#if (TARGET_MCU == MCU_F340)

#include <compiler_defs.h>
#include <C8051F340_defs.h>

#define FLASH_PAGE_SIZE       512      // Size of flash page in bytes
#define FLASH_START           0x1200   // Address of first writeable page
#define FLASH_NUM_PAGES       54       // Number of writeable pages
#define FLASH_GROUP_WRITE_EN  1        // Allow 2-byte flash writes

#endif // (TARGET_MCU == MCU_F340)


#if (TARGET_MCU == MCU_F380)

#include <compiler_defs.h>
#include <C8051F380_defs.h>

#define FLASH_PAGE_SIZE       512      // Size of flash page in bytes
#define FLASH_START           0x1200   // Address of first writeable page
#define FLASH_NUM_PAGES       54       // Number of writeable pages
#define FLASH_GROUP_WRITE_EN  1        // Allow 2-byte flash writes

#endif // (TARGET_MCU == MCU_F380)

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
