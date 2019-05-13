//=============================================================================
// transmitter_main.c
//=============================================================================
// Copyright 2011 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// C File Description:
//
//
// Target:
//    Si1020 with "UDP F960 MCU MUX LCD" board
//
// IDE:
//    Silicon Laboratories IDE
//
// Tool Chains:
//    Keil
//    SDCC
//    Raisonance
//
// Project Name:
//    AMR (automatic meter reader) example
//
// Description:
//    This is the transmit side of the AMR demo. The EZRadioPRO is programmed
//    to wake up periodically and read the pulse counter. The 4-byte count
//    value is displayed on the LCD and transmitted to the receiver.
//
// This software must be used in accordance with the End User License Agreement.
//
//=============================================================================
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <compiler_defs.h>
#include <Si1020_defs.h>
#include "hardware_defs.h"
#include "SmaRTClockSi1020.h"
#include "lcdPutString.h"
#include "lcdPrintf.h"
#include "lcdConfig.h"
#include "rtPhy.h"
#include "rtPhy_const.h"
#include "rtPhy_defs.h"
#include "PulseCounter.h"
#include "PowerManagementSi1020.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
bit PC0_Comparator0Flag;
SEGMENT_VARIABLE (TxBuffer[64], U8, BUFFER_MSPACE);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void SYSCLK_Init (void);
void Port_Init (void);
void SPI1_Init (void);
void LCD_Init_VIM878 (void);

//-----------------------------------------------------------------------------
// Disable watchdog before SDCC runs startup code
//-----------------------------------------------------------------------------
#if defined SDCC
void _sdcc_external_startup (void)
{
   PCA0MD &= ~0x40;                    // Disable Watchdog timer
}
#endif

//-----------------------------------------------------------------------------
// main()
//-----------------------------------------------------------------------------
void main (void)
{
   U8 value;
   U8 status;
   UU32 count;
   U32 current;

   SFRPAGE = LEGACY_PAGE;
   PCA0MD  &= ~0x40;                   // disable watchdog timer

   SFRPAGE  =  LCD0_PAGE;
   DC0MD = 0x4A;                       // dc-dc disabled
   LCD0MSCN &= ~0x20;                  // disable DCDC bias
   SFRPAGE = LEGACY_PAGE;

   SYSCLK_Init();
   SPI1_Init();
   Port_Init();
   RTC_Init();

   // configure LCD
   LCD_Init_VIM878();
   lcdPutString("READY");

   // configure pulse counter
   PC0_Init (PC0_SINGLE_MODE, PC0_SAMPLING_RATE_2MS, 0);

   value = PC0_SetDebounceTimeHigh (4000); // set debounce time to 4 ms
   value = PC0_SetDebounceTimeLow (4000);

   current = PC0_SetPullUpCurrent(1000);

   SFRPAGE = DPPE_PAGE;
   PC0INT0 = PC0_OVRF;                 // Enable overflow interrupts

   PC0_Comparator0Flag = 0;            // Clear software flags

   // configure EZRadioPRO
   status = rtPhyInit();

   rtPhySet(TRX_FREQUENCY, 915000000);
   rtPhySet(TRX_CHANNEL_SPACING, 100000L);
   rtPhySet(TRX_DEVIATION, 20000L);
   rtPhySet(TRX_DATA_RATE, 40000L);
   rtPhySet(RX_BAND_WIDTH, 80000L);

   status = rtPhyInitRadio();

   status = rtPhyShutDown();

   // configure RTC
   RTC_WriteAlarm(0, 160000L);         // Set the Alarm Value for ~10 seconds

   RTC_Write(RTC0CF, 0x09);            // Enable Alarm0 and Auto-Reload

   RTC_Write(RTC0CN, (RTC_Read(RTC0CN)|0x10)); // run timer

   LPM_Init();                         // Initialize Power Management
   LPM_Enable_Wakeup(RTC);

   EA = 1;                             // Enable global interrupts

   LPM(SLEEP);                         // Enter Sleep Until Next Alarm

   //----------------------------------
   // Main Application Loop
   //----------------------------------
   while (1)
   {
      //-----------------------------------------------------------------------
      // Task #1 - Handle RTC Failure
      //-----------------------------------------------------------------------
      if(RTC_ClockFailWakeUp)
      {
         RTC_ClockFailWakeUp = 0;      // Reset RTC Failure Flag to indicate
                                       // that we have detected an RTC failure
                                       // and are handling the event

         // Do something...RTC Has Stopped Oscillating
         while(1);                     // <Insert Handler Code Here>
      }

      //-----------------------------------------------------------------------
      // Task #2 - Handle RTC Alarm
      //-----------------------------------------------------------------------
      if(RTC_Alarm0_WakeUp)
      {
         RTC_Alarm0_WakeUp = 0;        // Reset RTC Alarm Flag to indicate
                                       // that we have detected an alarm
                                       // and are handling the alarm event
         RTC_Write(RTC0CF, 0x09);      // clear flag

         LED2 = EXTINGUISH;
         LED3 = ILLUMINATE;

         count.U32 = PC0_ReadCounter0();

         TxBuffer[0] = count.U8[b0];
         TxBuffer[1] = count.U8[b1];
         TxBuffer[2] = count.U8[b2];
         TxBuffer[3] = count.U8[b3];

         lcdPrintf("%08ld", count.U32);

         status = rtPhyReStart();
         status = rtPhyInitRadio();
         status = rtPhyTx(4,TxBuffer);

         while((phyRead(EZRADIOPRO_DEVICE_STATUS) & 0x03) != 0x00)
         {
            LED2 = ILLUMINATE;
         }

         status = rtPhyShutDown();

         if(status != PHY_STATUS_SUCCESS)
         {
            LED2 = ILLUMINATE;
         }

         LED3 = EXTINGUISH;
      }

      //-----------------------------------------------------------------------
      // Task #3 - Sleep for the remainder of the interval
      //-----------------------------------------------------------------------
      LPM(SLEEP);                      // Enter Sleep Until Next Alarm
   }
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   U8 save_page;

   SFRPAGE = LEGACY_PAGE;
   CLKSEL  = 0x04;                     // Use 20 MHz LPOSC
   FLSCL  |= 0x40;
   
   SFRPAGE  = save_page;
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   U8 save_page;

   SFRPAGE  = LEGACY_PAGE;

   P1MDIN   = 0xDE;                    // enable PC0 and P1.5 as analog input
                                       //  to disable weak pull-up, P1.5 is
                                       //  looped back to GPIO_1 on radio
   P2MDIN   = 0xDF;                    // disable weak pull-up on P2.5 which
                                       //  is looped back to GPIO_0 on radio
   SFRPAGE  = CONFIG_PAGE;
   P4MDIN   = 0x3F;                    // disable weak pull-up on P4.6/P4.7
                                       //  which are looped back to GPIO_3/2
                                       //  on radio
   SFRPAGE = LEGACY_PAGE;

   // Init SPI
   XBR1    |= 0x40;                    // Enable SPI1 (3 wire mode)
   P2MDOUT |= 0x0D;                    // SCK1, MOSI1, & NSS1,push-pull
   SFRPAGE  = CONFIG_PAGE;
   P2DRV    = 0x0D;                    // MOSI1, SCK1, NSS1, high-drive mode

   // Initialize SDN
   SFRPAGE  = LEGACY_PAGE;
   P1MDOUT |= 0x80;                    // SDN P1.7 push-pull
   SFRPAGE  = CONFIG_PAGE;
   P1DRV   |= 0x80;                    // SDN P1.7 high-drive mode
   SDN      = 1;

   SFRPAGE  = LEGACY_PAGE;

   XBR2    |= 0x40;                    // enable Crossbar

   IT01CF   = 0x06;                    // INT0 is assigned to P1.6
   IE0      = 0;
   IT0      = 0;

   SFRPAGE  = save_page;
}

//-----------------------------------------------------------------------------
// SPI1_Init
//-----------------------------------------------------------------------------
void SPI1_Init (void)
{
   U8 save_page;

   save_page = SFRPAGE;

   SFRPAGE   = SPI1_PAGE;

   SPI1CFG  |= 0x40;                   // master mode
   SPI1CN    = 0x00;                   // 3 wire master mode
   SPI1CKR   = 0x00;                   // initialize SPI prescaler
   SPI1CN   |= 0x01;                   // enable SPI
   NSS1      = 1;                      // set NSS high

   SFRPAGE   = save_page;
}

//-----------------------------------------------------------------------------
// LCD_Init_VIM878
//
// Description:
// This function will configure the LCD for a 128-segment 4-mux display.
//
// The refresh rate assumes a 16 kHz RTC clock using the LFO.
//
// This function uses the lcdConfig module to configure the LCD regsiters.
//
//-----------------------------------------------------------------------------
void LCD_Init_VIM878 (void)
{
   //          +- 4-mux
   //          |  +- 32 segments
   //          |  |   +- Mode 3 - Constant Contrast mode
   //          |  |   |  +- Contrast 2.9 volts
   //          |  |   |  |     +- Refresh 60 Hz
   //          |  |   |  |     |   +- Toggle 1 Hz
   LCD0_Config(4, 32, 3, 2900, 60, 1);
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------