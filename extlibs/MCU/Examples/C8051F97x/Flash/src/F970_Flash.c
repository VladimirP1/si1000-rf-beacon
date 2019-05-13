//-----------------------------------------------------------------------------
// F970_Flash.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program tests all the functions available in F970_FlashPrimitives.c
// and F970_FlashUtils.c. It uses a test data array to mimic the changes
// made to the Flash and test for correctness. While the test is running, the
// LED is on. If the test fails, the LED will turn off. If the test passes,
// the LED will blink.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Open the Memory window/tab in the IDE.
// 5) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 6) Verify that the memory starting at <start_address> matches the following:
//
//      48 49 4A 44 45 46
//      47 00 48 49 4A 44
//      45 46 47 00 5A 5A
//      5A 5A 5A 5A 5A 5A
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 5 MAY 2014

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "F970_FlashPrimitives.h"
#include "F970_FlashUtils.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SBIT (LED, P0, 5);                 // LED == 0 means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void VDDMon_Init (void);
void PORT_Init (void);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

// This is the test address.  It is a 16-bit address that treats the 32K
// or 16K of code space as a linear space.
FLADDR start_address = 0x4000;

SEGMENT_VARIABLE (test_write_buff[8],   U8, SEG_XDATA) = "ABCDEFG";
SEGMENT_VARIABLE (test_write_buff2[3],  U8, SEG_XDATA) = "HIJ";
SEGMENT_VARIABLE (test_read_buff[8],    U8, SEG_XDATA) = {0};
SEGMENT_VARIABLE (test_compare_buff[8], U8, SEG_XDATA) = "ABCDEFG";

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   U8 temp_byte = 0x00;
   U8 i, j;

   bit error_flag = 0;
   bit SFLE = 0;                       // All writes target Flash and not
                                       // scratchpad

   SFRPAGE = PCA0_PAGE;                // Disable the watchdog timer
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;

   if ((RSTSRC & 0x02) != 0x02)
   {
      if ((RSTSRC & 0x40) == 0x40)
      {
         LED = 1;
         while (1);                    // Last reset was caused by a Flash
                                       // Error Device Reset
                                       // LED is off and loop forever to
                                       // indicate error
      }
   }

   SYSCLK_Init ();                     // Initialize the internal oscillator
   PORT_Init ();                       // Initialize crossbar and GPIO
   VDDMon_Init ();                     // Initialize the VDD monitor

   LED = 0;


   // Initially erase the test pages of Flash
   FLASH_PageErase (start_address, SFLE);
   FLASH_PageErase (start_address + 2, SFLE);

   //BEGIN TEST================================================================

   // Check if able to Write and Read the Flash--------------------------------
   FLASH_ByteWrite (start_address, 0xA5, SFLE);

   temp_byte = FLASH_ByteRead (start_address, SFLE);

   if (temp_byte != 0xA5)
   {
      error_flag = 1;
   }
   //--------------------------------------------------------------------------


   // Check if able to Erase a page of the Flash-------------------------------
   FLASH_PageErase (start_address, SFLE);

   temp_byte = FLASH_ByteRead (start_address, SFLE);

   if (temp_byte != 0xFF)
   {
      error_flag = 1;
   }
   //--------------------------------------------------------------------------

   // Check if able to write and read a series of bytes------------------------
   FLASH_Write (start_address, test_write_buff, sizeof(test_write_buff), SFLE);

   FLASH_Read (test_read_buff, start_address, sizeof(test_write_buff), SFLE);

   for (i = 0; i < sizeof(test_write_buff); i++)
   {
      if (test_read_buff[i] != test_write_buff[i])
      {
         error_flag = 1;
      }
   }
   //--------------------------------------------------------------------------

   // Check if able to Erase a few bytes---------------------------------------
   FLASH_Clear (start_address, 2, SFLE);

   FLASH_Read (test_read_buff, start_address, sizeof(test_write_buff), SFLE);

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
   FLASH_Update (start_address, test_write_buff2, 3, SFLE);

   FLASH_Read (test_read_buff, start_address, sizeof(test_write_buff), SFLE);

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

   // Check if able to copy data in the Flash----------------------------------
   FLASH_Copy (start_address + sizeof(test_write_buff), SFLE, start_address,
               SFLE, sizeof(test_write_buff));

   FLASH_Read (test_read_buff, start_address + sizeof(test_write_buff),
              sizeof(test_read_buff), SFLE);

   for (i = 0; i < sizeof(test_write_buff); i++)
   {
      if (test_read_buff[i] != test_compare_buff[i])
      {
         error_flag = 1;
      }
   }
   //--------------------------------------------------------------------------

   // FLASH test routines------------------------------------------------------
   FLASH_Fill (start_address + sizeof(test_write_buff) * 2,
               sizeof(test_write_buff), 0x5A, SFLE);

   FLASH_Read(test_read_buff, start_address + sizeof(test_write_buff) * 2,
              sizeof(test_write_buff), SFLE);

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

         // Simple delay loop
         for (i = 65000; i > 0; i--)
         {
            for (j = 65000; j > 0; j--);
         }
      }
      else
      {
         LED = 1;
      }
   }
}


//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// This routine initializes the system clock to use the internal 20 MHz
// oscillator as its clock source divided by 8.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = LEGACY_PAGE;

   CLKSEL = CLKSEL_CLKSL__LPOSC |         // Use Low Power Int Osc
            CLKSEL_CLKDIV__SYSCLK_DIV_8;  // 20MHz/8 = 2.5MHz

   SFRPAGE = SFRPAGE_save;             // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// VDDMon_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the VDD Monitor and enables it as a reset source.
// The VDD Monitor is enabled following a power-on reset, but this routine
// explicitly enables the VDD Monitor.
//
// Note: If the VDD Monitor is not enabled or not enabled as a reset source,
// any attempt to write or erase Flash will cause a Flash Error Device Reset.
//-----------------------------------------------------------------------------
void VDDMon_Init(void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   // Step 1. Enable the VDD Monitor and set it to the high threshold
   VDM0CN = 0xA0;

   // Step 2. Wait for the VDD Monitor to stabilize (approximately 5 µs).
   // Note: This delay should be omitted if software contains routines which
   // erase or write Flash memory.
   // Omitted in this code

   // Step 3. Select the VDD Monitor as a reset source (PORSF bit in
   // RSTSRC = ‘1’).

   RSTSRC = 0x02;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P0.5   digital   push-pull     LED05
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = CONFIG_PAGE;

   P0MDIN |= 0x02;                     // P1.3 is digital
   P0MDOUT |= 0x02;                    // P1.3 is push-pull

   XBR1    = XBR1_XBARE__ENABLED |     // Enable crossbar
             XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups

   SFRPAGE = SFRPAGE_save;             // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
