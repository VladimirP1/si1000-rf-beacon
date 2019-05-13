//-----------------------------------------------------------------------------
// F85x_Flash.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program tests all the functions available in F85x_FlashPrimitives.c
// and F85x_FlashUtils.c.  It uses a test data array to mimic the changes
// made to the flash and test for correctness.  While the test is running, the
// LED is on.  If the test fails, the LED will turn off.  If the test passes,
// the LED will blink.
//
// Pinout:
//
//    P1.0 -> LED
//
//    all other port pins unused
//
// How To Test:
//
// Option 1:
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Load the F85x_Flash.c, F85x_FlashPrimitives.c, and F85x_FlashUtils.c
//    files in the Silicon Labs IDE.
// 4) Compile and download code to the 'F85x UDP MCU board.
// 5) Run the code.
// 6) Check that the DS5 P1.0 LED is blinking, indicating a passing test.
//
// Option 2:
// 1) Ensure shorting blocks are place on the following:
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Load the F85x_Flash.c, F85x_FlashPrimitives.c, and F85x_FlashUtils.c
//    files in the Silicon Labs IDE.
// 4) Compile and download code to the 'F85x UDP MCU board.
// 5) Run the code.
// 6) Open the Code window in the IDE and verify the following data at
//    address <start_address>:
//
//        48 49 4A 44 45 46
//        47 00 48 49 4A 44
//        45 46 47 00 5A 5A
//        5A 5A 5A 5A 5A 5A
//
// Note: C8051F85x/86x devices with less than 8 kB of memory will need to
// use a different START_ADDRESS.
//
// Target:         C8051F85x/86x
// Tool chain:     Keil C51 8.00
// Command Line:   None
//
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - Moved the start_address from 0x0800 to 0x1600
//    - Declared the array as a variable to prevent the linker from placing
//      code there.
//    - 24 JUL 2013
//
// Release 1.0
//    -Initial Revision (TP)
//    -19 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "compiler_defs.h"
#include <C8051F850_defs.h>                 // SFR declarations
#include "F85x_FlashPrimitives.h"
#include "F85x_FlashUtils.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000/8  // SYSCLK in Hz
#define START_ADDRESS          0x1600


SBIT (LED, SFR_P1, 0);                 // DS5 P1.0 LED

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void VDDMon_Init (void);               // VDD Monitor initialization routine
void Port_Init (void);                 // Port initialization routine

void Timer0_Delay_ms (unsigned int ms);

LOCATED_VARIABLE_NO_INIT(flash_write_array[512], U8, SEG_CODE, START_ADDRESS);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   unsigned char temp_byte = 0x00;

   char test_write_buff[8] = "ABCDEFG";
   char test_write_buff2[3] = "HIJ";
   char test_read_buff[8] = {0};
   char test_compare_buff[8] = "ABCDEFG";

   unsigned char i;

   bit error_flag = 0;

   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   VDDMon_Init();

   Port_Init();

   LED = 1;

   // Initially erase the test page of flash
   FLASH_PageErase(START_ADDRESS);

   //BEGIN TEST================================================================

   // Check if able to Write and Read the flash--------------------------------
   FLASH_ByteWrite(START_ADDRESS, 0xA5);

   temp_byte = FLASH_ByteRead(START_ADDRESS);

   if (temp_byte != 0xA5)
   {
      error_flag = 1;
   }
   //--------------------------------------------------------------------------


   // Check if able to Erase a page of the flash-------------------------------
   FLASH_PageErase(START_ADDRESS);

   temp_byte = FLASH_ByteRead(START_ADDRESS);

   if (temp_byte != 0xFF)
   {
      error_flag = 1;
   }
   //--------------------------------------------------------------------------

   // Check if able to write and read a series of bytes------------------------
   FLASH_Write(START_ADDRESS, test_write_buff, sizeof(test_write_buff));

   FLASH_Read(test_read_buff, START_ADDRESS, sizeof(test_write_buff));

   for (i = 0; i < sizeof(test_write_buff); i++)
   {
      if (test_read_buff[i] != test_write_buff[i])
      {
         error_flag = 1;
      }
   }
   //--------------------------------------------------------------------------

   // Check if able to Erase a few bytes---------------------------------------
   FLASH_Clear(START_ADDRESS, 2);

   FLASH_Read(test_read_buff, START_ADDRESS, sizeof(test_write_buff));

   // Simulate the same changes to a data array for comparison
   test_compare_buff[0] = 0xFF;
   test_compare_buff[1] = 0xFF;

   for (i = 0; i < sizeof(test_compare_buff); i++)
   {
      if (test_read_buff[i] != test_compare_buff[i])
      {
         error_flag = 1;
      }
   }
   //--------------------------------------------------------------------------

   // Check if able to "update" (erase then re-write) a few bytes--------------
   FLASH_Update (START_ADDRESS, test_write_buff2, 3);

   FLASH_Read(test_read_buff, START_ADDRESS, sizeof(test_write_buff));

   // Simulate the same changes to a data array for comparison
   test_compare_buff[0] = test_write_buff2[0];
   test_compare_buff[1] = test_write_buff2[1];
   test_compare_buff[2] = test_write_buff2[2];

   for (i = 0; i < sizeof(test_compare_buff); i++)
   {
      if (test_read_buff[i] != test_compare_buff[i])
      {
         error_flag = 1;
      }
   }
   //--------------------------------------------------------------------------

   // Check if able to copy data in the flash----------------------------------
   FLASH_Copy (START_ADDRESS+sizeof(test_write_buff), START_ADDRESS,
               sizeof(test_write_buff));

   FLASH_Read(test_read_buff, START_ADDRESS+sizeof(test_write_buff),
              sizeof(test_read_buff));

   for (i = 0; i < sizeof(test_write_buff); i++)
   {
      if (test_read_buff[i] != test_compare_buff[i])
      {
         error_flag = 1;
      }
   }
   //--------------------------------------------------------------------------

   // Flash test routines------------------------------------------------------
   FLASH_Fill (START_ADDRESS+sizeof(test_write_buff)*2, sizeof(test_write_buff),
               0x5A);

   FLASH_Read(test_read_buff, START_ADDRESS+sizeof(test_write_buff)*2,
              sizeof(test_write_buff));

   for (i = 0; i < sizeof(test_write_buff); i++)
   {
      if (test_read_buff[i] != 0x5A)
      {
         error_flag = 1;
      }
   }
   //--------------------------------------------------------------------------

   //END OF TEST===============================================================

   while (1)                           // Loop forever
   {
      // Blink LED to indicate success
      if (error_flag == 0)
      {
         LED = !LED;

         Timer0_Delay_ms (100);
      }
      else
      {
         LED = 0;
      }
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// VDDMon_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function enables the VDD Monitor and enables it as a reset source.
// The VDD Monitor is enabled following a power-on reset, but subsequent resets
// can accidentally disable the monitor.
//
//-----------------------------------------------------------------------------
void VDDMon_Init(void)
{
   VDM0CN |= 0x80;                     // Ensure the VDD Monitor is enabled

   // Select the VDD Monitor as a reset source (PORSF bit in RSTSRC = ‘1’)
   RSTSRC = 0x02;
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// Pinout:
//
//    P1.0 -> LED
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDOUT |= 0x01;                    // Set LED as push-pull

   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_Delay_ms
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) unsigned int ms - number of milliseconds of delay
//                        range is full range of integer: 0 to 65335
//
// Configure Timer0 to delay <ms> milliseconds before returning.
//
// Note: This routine modifies the SCA0-1 bits in CKCON, which will affect
// the timing of Timer1.
//
//-----------------------------------------------------------------------------
void Timer0_Delay_ms (unsigned int ms)
{
   unsigned i;                         // Millisecond counter

   TCON  &= ~0x30;                     // STOP Timer0 and clear overflow flag
   TMOD  &= ~0x0F;                     // Configure Timer0 to 16-bit mode
   TMOD  |=  0x01;

   CKCON &=  ~0x04;                    // Timer0 counts SYSCLK / 48
   CKCON |= 0x02;

   for (i = 0; i < ms; i++)            // Count milliseconds
   {
      TCON &= ~0x10;                   // STOP Timer0
      TH0 = (-SYSCLK/48/1000) >> 8;    // Set Timer0 to overflow in 1ms
      TL0 = -SYSCLK/48/1000;
      TCON |= 0x10;                    // START Timer0
      while (TF0 == 0);                // Wait for overflow
      TCON &= ~0x20;                   // Clear overflow indicator
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------