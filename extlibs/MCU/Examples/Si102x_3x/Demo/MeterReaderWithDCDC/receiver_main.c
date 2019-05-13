//=============================================================================
// receiver_main.c
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
//    This is the receive side of the AMR demo. The EZRadioPRO is programmed
//    to continuously receive. When a packet is received, the 4-byte count
//    value is displayed on the LCD.
//
//    To demonstrate the impact of the dc-dc converter and clock source on
//    current consumption, one of four operational modes may be selected by
//    pressing SW1 through SW4:
//        * SW1 - dc-dc disabled, clock source is precision oscillator
//        * SW2 - dc-dc enabled, clock source is precision oscillator
//        * SW3 - dc-dc disabled, clock source is low-power oscillator
//        * SW4 - dc-dc enabled, clock source is low-power oscillator
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

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
bit PC0_Comparator0Flag;
SEGMENT_VARIABLE (RxBuffer[64], U8, BUFFER_MSPACE);
LOCATED_VARIABLE_NO_INIT (reserved, U8, SEG_XDATA, 0x0000);
U8 button;
U8 last_button;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void SYSCLK_Init (void);
void Port_Init (void);
void SPI1_Init (void);
void LCD_Init_VIM878 (void);
void InitPrecisionBuckOff (void);
void InitPrecisionBuckOn (void);
void InitLowPowerBuckOff (void);
void InitLowPowerBuckOn (void);

//-----------------------------------------------------------------------------
// SDCC needs ISR Prototype in main module
// SDCC needs putchar() prototype and function
//-----------------------------------------------------------------------------
#ifdef SDCC
INTERRUPT_PROTO(Receiver_ISR, INTERRUPT_INT0);
INTERRUPT_PROTO(T0_ISR, INTERRUPT_TIMER0);
#endif

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
   U8 status;
   UU32 count;

   SFRPAGE  = LEGACY_PAGE;
   PCA0MD  &= ~0x40;                   // disable watchdog timer

   VDM0CN   = 0x00;                    // disable both VDD monitors

   SFRPAGE  =  DPPE_PAGE;
   DC0MD    = 0x48;                    // disable DCDC and force bypass

   SYSCLK_Init();
   SPI1_Init();
   Port_Init();
   RTC_Init();

   // configure LCD
   LCD_Init_VIM878();
   lcdPutString("READY");

   // configure EZRadioPRO
   status   = rtPhyInit();

   rtPhySet (TRX_FREQUENCY, 915000000);
   rtPhySet (TRX_CHANNEL_SPACING, 100000L);
   rtPhySet (TRX_DEVIATION, 20000L);
   rtPhySet (TRX_DATA_RATE, 40000L);
   rtPhySet (RX_BAND_WIDTH, 80000L);

   status   = rtPhyInitRadio();

   // configure port match to interrupt on SW1 - SW4
   SFRPAGE  = LEGACY_PAGE;
   P0MASK   = 0x0F;                    // Mask out all pins except P0.0-P0.3
   P0MAT    = 0x0F;                    // P0.0-0.3 normally high

   EIE2    |= 0x02;                    // Enable Port Match interrupt

   EA       = 1;

   status   = rtPhyRxOn();

   //----------------------------------
   // Main Application Loop
   //----------------------------------
   while (1)
   {
      if(RxPacketReceived)
      {
         rtPhyGetRxPacket(&RxPacketLength, RxBuffer);

         if(RxPacketLength == 4)
         {
            count.U8[b0] = RxBuffer[0];
            count.U8[b1] = RxBuffer[1];
            count.U8[b2] = RxBuffer[2];
            count.U8[b3] = RxBuffer[3];
            lcdPrintf("%08ld", count.U32);
         }
         else
         {
            lcdPutString("ERROR_01");
         }
      }
      if(button != last_button)
      {
         status = rtPhyShutDown();

         switch (button)
         {
            case 0x01:                 // SW1 is pressed
               InitPrecisionBuckOff();
               break;

            case 0x02:                 // SW2 is pressed
               InitPrecisionBuckOn();
               break;

            case 0x04:                 // SW3 is pressed
               InitLowPowerBuckOff();
               break;

            case 0x08:                 // SW4 is pressed
               InitLowPowerBuckOn();
               break;

            default:
               break;
         }

         status = rtPhyReStart();
         status = rtPhyInitRadio();
         status = rtPhyRxOn();

         last_button = button;
      }
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

   // initialize port match
   P0MASK   = 0x0F;
   P0MAT    = 0x0F;

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
INTERRUPT(PortMatch_ISR, INTERRUPT_PORT_MATCH)
{
   SFRPAGE = LEGACY_PAGE;

   button = ~P0 & 0x0F;                // Pressing one of the 4 switches
                                       // grounds that port pin. The polarity
                                       // is reversed before storing in the
                                       // global variable.

   // return from interrupt restores SFRPAGE
}

//-----------------------------------------------------------------------------
// InitPrecisionBuckOff()
//-----------------------------------------------------------------------------
void InitPrecisionBuckOff (void)
{
   U8 restore;

   restore = SFRPAGE;

   SFRPAGE = LEGACY_PAGE;

   // If VBAT switch enabled, disable it and disable DCDC.
   if((PCON & 0x04) == 0x04)
   {
      PCON &= ~0x04;                   // Disable VBAT switch
      SFRPAGE  =  DPPE_PAGE;
      DC0MD = 0x48;                    // Disable DCDC and enable force bypass
      LCD0MSCN &= ~0x20;               // Disable DCBIAS in LCD0
   }

   // If Precision osc not enabled, enable it.
   SFRPAGE = LEGACY_PAGE;
   if((OSCICN & 0x80) == 0x00)
   {
      OSCICN |= 0x80;                  // Enable precision oscillator
      CLKSEL  = 0x00;                  // Switch SYSCLK to precision oscillator
   }
   SFRPAGE   = restore;
}

//-----------------------------------------------------------------------------
// InitPrecisionBuckOn()
//-----------------------------------------------------------------------------
void InitPrecisionBuckOn (void)
{
   U8 restore;

   restore = SFRPAGE;

   SFRPAGE = LEGACY_PAGE;

   // If precision osc not enabled, enable it.
   if((OSCICN & 0x80) == 0x00)
   {
      OSCICN |= 0x80;                  // Enable precision oscillator
      CLKSEL  = 0x00;                  // Switch SYSCLK to precision oscillator
   }

   // If VBAT switch is disabled, enable DCDC and VBAT switch.
   if((PCON & 0x04) == 0x00)
   {
      SFRPAGE   =  LCD0_PAGE;
      LCD0MSCN |= 0x20;                // Enable DCDC bias
      LCD0PWR  &= ~0x08;               // Clear LCD mode bit when usign DCDC

      SFRPAGE   =  DPPE_PAGE;
      DC0CN     = 0x03;                // 40 ns pulse
      DC0CF     = 0x0B;                // 1.9 V max switch
      DC0MD     = 0x43;                // enable DCDC

      // wait on DCDC below high threshold and above low threshold.
      //while((DC0RDY & 0xC0)!=0x40);

      PCON     |= 0x04;                // enable VBAT switch
   }
   SFRPAGE      = restore;
}

//-----------------------------------------------------------------------------
// InitLowPowerBuckOff()
//-----------------------------------------------------------------------------
void InitLowPowerBuckOff (void)
{
   U8 restore;

   restore = SFRPAGE;

   SFRPAGE = LEGACY_PAGE;

   // If VBAT switch enabled, disable it and disable DCDC.
   if((PCON & 0x04) == 0x04)
   {
      PCON     &= ~0x04;               // Disable VBAT switch
      SFRPAGE   = DPPE_PAGE;
      DC0MD     = 0x48;                // Disable DCDC and enable force bypass
      LCD0MSCN &= ~0x20;               // Disable DCBIAS in LCD0
   }

   // If Precision osc enabled, switch to LP osc.
   SFRPAGE = LEGACY_PAGE;
   if((OSCICN & 0x80) == 0x80)
   {
      CLKSEL    = 0x04;                // Switch SYSCLK to low-power oscillator
      OSCICN   &= ~0x80;               // Disable Precision oscillator
      REG0CN    = 0x00;                // Disable oscillator Bias
   }
   SFRPAGE = restore;
}

//-----------------------------------------------------------------------------
// InitPrecisionBuckOn()
//-----------------------------------------------------------------------------
void InitLowPowerBuckOn (void)
{
   U8 restore;

   restore = SFRPAGE;

   SFRPAGE = LEGACY_PAGE;

   // If Precision osc enabled, switch to LP osc.
   if((OSCICN & 0x80) == 0x80)
   {
      CLKSEL  = 0x04;                  // Switch SYSCLK to low-power oscillator
      OSCICN &= ~0x80;                 // Disable Precision oscillator
      REG0CN  = 0x00;                  // Disable oscillator Bias
   }

   // If VBAT switch is disabled, enable DCDC and VBAT switch.
   if((PCON & 0x04) == 0x00)
   {
      SFRPAGE   =  LCD0_PAGE;
      LCD0MSCN |= 0x20;                // enable DCDC bias
      LCD0PWR  &= ~0x08;               // force bandgap on continuously in active mode

      SFRPAGE   =  DPPE_PAGE;
      DC0CN     = 0x03;                // 40 ns pulse
      DC0CF     = 0x0B;                // 1.9 V max switch
      DC0MD     = 0x43;                // enable DCDC

      // wait on DCDC below high threshold and above low threshold.
      //while((DC0RDY & 0xC0)!=0x40);

      PCON     |= 0x04;                // enable VBAT switch
   }
   SFRPAGE = restore;
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------