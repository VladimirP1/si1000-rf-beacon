//-----------------------------------------------------------------------------
// F3xx_Flash.c
//-----------------------------------------------------------------------------
// Copyright 2012 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Flash utilities for writing/erasing flash pages
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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "c8051f3xx.h"
#include "F3xx_Flash.h"

//-----------------------------------------------------------------------------
// Static Global Variables
//-----------------------------------------------------------------------------

static U8 FlashKey[2];

//-----------------------------------------------------------------------------
// Global Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SetFlashKey
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//                1) U8 key[2] : Sets the flash key code used to write/erase
//                               flash
//
// Sets the flash key code for flash writes/erases. The MCU will be more
// resistant to flash corruption if the key codes are not stored on chip.
// For example, the flash key can be set from a USB packet prior to programming
// and then cleared once finished to ensure that the flash key is not available.
//
// Enable flash writes:  key[2] = { 0xA5, 0xF1 }
// Disable flash writes: key[2] = { 0x00, 0x00 }
//
//-----------------------------------------------------------------------------
void SetFlashKey(U8 key[2])
{
   FlashKey[0] = key[0];
   FlashKey[1] = key[1];
}

//-----------------------------------------------------------------------------
// EraseFlashPage
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//                1) U16 pageAddress : the address of the flash page to erase
//
// Erases the specified flash page
//
//-----------------------------------------------------------------------------
void EraseFlashPage(U16 pageAddress)
{
   U8 EA_Save = EA;
   VARIABLE_SEGMENT_POINTER (pAddr, U8, SEG_XDATA) = (U8 SEG_XDATA *) pageAddress;

   // Disable interrupts
   EA = 0;

   // Write flash key codes
   FLKEY = FlashKey[0];
   FLKEY = FlashKey[1];

   // Enable program erase
   PSCTL = 0x03;

   // Erase page by writing to a byte within the page
   *pAddr = 0x00;

   // Disable program erase
   PSCTL = 0x00;

   // Restore interrupts
   EA = EA_Save;
}

//-----------------------------------------------------------------------------
// WriteFlashPage
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//                1) U16 address : the address of the flash page to write
//                2) U8 buffer[] : a buffer to write to the flash page starting
//                                 at the specified page address
//                3) U16 size : the number of bytes in the buffer to write
//
// Write the specified number of bytes in the buffer to the specified address.
//
//-----------------------------------------------------------------------------
void WriteFlashPage(U16 address, U8* buffer, U16 size)
{
   U8 EA_Save = EA;
   VARIABLE_SEGMENT_POINTER (pAddr, U8, SEG_XDATA) = (U8 SEG_XDATA *) address;
   U16 i;

   // Disable interrupts
   EA = 0;

   // Enable program writes
   PSCTL = 0x01;

#if FLASH_GROUP_WRITE_EN
   // Enable two-byte flash writes
   PFE0CN |= 0x01;
#endif // FLASH_GROUP_WRITE_EN

   for (i = 0; i < size; i++)
   {
      // Write flash key codes
      FLKEY = FlashKey[0];
      FLKEY = FlashKey[1];

      // Write a single byte to the page
      pAddr[i] = buffer[i];
   }

   // Disable program writes
   PSCTL = 0x00;

   // Restore interrupts
   EA = EA_Save;
}

//-----------------------------------------------------------------------------
// ReadFlashPage
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//                1) U16 address : the address of the flash page to read
//                2) U8 buffer[] : a buffer to read from the flash page starting
//                                 at the specified page address
//                3) U16 size : the number of bytes to read into the buffer
//
// Read the specified number of bytes from flash and store in the buffer.
//
//-----------------------------------------------------------------------------
void ReadFlashPage(U16 address, U8* buffer, U16 size)
{
   VARIABLE_SEGMENT_POINTER (pAddr, U8, SEG_CODE) = (U8 SEG_CODE *) address;
   U16 i;

   for (i = 0; i < size; i++)
   {
      buffer[i] = pAddr[i];
   }
}
