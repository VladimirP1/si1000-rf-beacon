//-----------------------------------------------------------------------------
// F12x_Init.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// 
// Contains Initialization Routines for the F120.
//
// FID:            
// Target:         C8051F12x
// Tool chain:     Keil C51 7.20 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.72 
// Command Line:   See Readme.txt
// Project Name:   CP220x_Ethernet_Routines
//
// 
//
// Release 1.1
//    - Configures C8051F120 SYSCLK to 98 MHz
//
// Release 1.0
//    -Initial Release (FB)
//    -30 MAY 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "global.h"

#if(MCU == F120)

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F12x
//-----------------------------------------------------------------------------

sfr16 DP       = 0x82;                 // data pointer
sfr16 ADC0     = 0xbe;                 // ADC0 data
sfr16 ADC0GT   = 0xc4;                 // ADC0 greater than window
sfr16 ADC0LT   = 0xc6;                 // ADC0 less than window
sfr16 RCAP2    = 0xca;                 // Timer2 capture/reload
sfr16 RCAP3    = 0xca;                 // Timer3 capture/reload
sfr16 RCAP4    = 0xca;                 // Timer4 capture/reload
sfr16 TMR2     = 0xcc;                 // Timer2
sfr16 TMR3     = 0xcc;                 // Timer3
sfr16 TMR4     = 0xcc;                 // Timer4
sfr16 DAC0     = 0xd2;                 // DAC0 data
sfr16 DAC1     = 0xd2;                 // DAC1 data
sfr16 PCA0CP5  = 0xe1;                 // PCA0 Module 5 capture
sfr16 PCA0CP2  = 0xe9;                 // PCA0 Module 2 capture
sfr16 PCA0CP3  = 0xeb;                 // PCA0 Module 3 capture
sfr16 PCA0CP4  = 0xed;                 // PCA0 Module 4 capture
sfr16 PCA0     = 0xf9;                 // PCA0 counter
sfr16 PCA0CP0  = 0xfb;                 // PCA0 Module 0 capture
sfr16 PCA0CP1  = 0xfd;                 // PCA0 Module 1 capture

//-----------------------------------------------------------------------------
// Local Constants
//-----------------------------------------------------------------------------
#define SYSCLK  73500000               // System Clock in Hz


//-----------------------------------------------------------------------------
// Local Global Variables 
//-----------------------------------------------------------------------------

static unsigned int timeout;

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------
void Reset_Init(void);

void wait_ms(int ms);
void clear_ms_timer_flag(void);
char get_ms_timer_flag(void);
void start_ms_timer();

void CP220x_RST_Low(void);
void CP220x_RST_High(void);
unsigned char AB4_RST_State(void);

#if(UART_ENABLED)
char _getkey ();
char putchar (char c);
#endif

//-----------------------------------------------------------------------------
// Local Function Prototypes
//-----------------------------------------------------------------------------

void PORT_Init (void);
void SYSCLK_Init (void);
void EMIF_Init (void);

#if(UART_ENABLED)
void UART1_Init (void);
#endif

//-----------------------------------------------------------------------------
// Exported Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Reset_Init
//-----------------------------------------------------------------------------
void Reset_Init(void)
{
   // Enable the VDD Monitor as a reset source
   // Do not use read-modify write instruction on this register
   RSTSRC = 0x02;

   // Disable Watchdog Timer
   WDTCN = 0xde;
   WDTCN = 0xad;

   // Initialize the MCU
   PORT_Init();
   SYSCLK_Init();
   EMIF_Init();

   EX0 = 1;                            // Enable External Interrupt 0   

   #if(UART_ENABLED)
   UART1_Init();
   puts("\n*** Reset ***\nC8051F12x MCU Initialized\n");
   #endif

}

//-----------------------------------------------------------------------------
// External Interrupt 0
//-----------------------------------------------------------------------------
//
// This interrupt service routine is routed to the ethernet interrupt
//
void INT0_ISR(void) interrupt 0
{
   Ethernet_ISR();
}


//-----------------------------------------------------------------------------
// wait_ms
//-----------------------------------------------------------------------------
//
// This routine inserts a delay of <ms> milliseconds.
//
void wait_ms(int ms)
{
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page

   SFRPAGE = TMR2_PAGE;

   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
   TMR2CF = 0x00;                      // use SYSCLK/12 as timebase

   RCAP2 = -(SYSCLK/1000/12);          // Timer 2 overflows at 1 kHz
   TMR2 = RCAP2;

   ET2 = 0;                            // Disable Timer 2 interrupts

   TR2 = 1;                            // Start Timer 2

   while(ms){
      TF2 = 0;
      while(!TF2);                     // wait until timer overflows
      ms--;                            // decrement ms
   }

   TR2 = 0;                            // Stop Timer 2

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFRPAGE
}

//-----------------------------------------------------------------------------
// reset_timout
//-----------------------------------------------------------------------------
//
// This routine resets the global timeout.
//
void reset_timeout(unsigned int ms)
{
   timeout = ms;
   start_ms_timer();
      
}


//-----------------------------------------------------------------------------
// timeout_expired
//-----------------------------------------------------------------------------
//
// This routine manages the timeout and returns TRUE if timeout expired.
//
unsigned char timeout_expired(void)
{
   if(get_ms_timer_flag()){
      clear_ms_timer_flag();
      if(timeout > 0){
         timeout--;
      }
   }
   
   return (timeout == 0);
     
   
}
//-----------------------------------------------------------------------------
// start_ms_timer
//-----------------------------------------------------------------------------
//
// This routine starts a timer with a 1kHz overflow rate (1ms period).
//
void start_ms_timer()
{
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page

   SFRPAGE = TMR2_PAGE;

   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
   TMR2CF = 0x00;                      // use SYSCLK/12 as timebase

   RCAP2 = -(SYSCLK/1000/12);          // Timer 2 overflows at 1 kHz
   TMR2 = RCAP2;

   ET2 = 0;                            // Disable Timer 2 interrupts

   TR2 = 1;                            // Start Timer 2

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFRPAGE
}

//-----------------------------------------------------------------------------
// get_ms_timer_flag()
//-----------------------------------------------------------------------------
//
// This routine returns the state of the ms_timer overflow flag.
//
char get_ms_timer_flag(void)
{
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page

   SFRPAGE = TMR2_PAGE;

   return TF2;

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFRPAGE
}

//-----------------------------------------------------------------------------
// clear_ms_timer_flag()
//-----------------------------------------------------------------------------
//
// This routine returns the state of the ms_timer overflow flag.
//
void clear_ms_timer_flag(void)
{
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page

   SFRPAGE = TMR2_PAGE;

   TF2 = 0;

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFRPAGE
}


//-----------------------------------------------------------------------------
// CP220x_RST_Low
//-----------------------------------------------------------------------------
//
// Drives the CP220x's Reset Pin Low.
//
void CP220x_RST_Low(void)
{
   char SFRPAGE_SAVE = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;
   P4 &= ~0x20;                        // Set P4.5 Low
   SFRPAGE = SFRPAGE_SAVE;
}

//-----------------------------------------------------------------------------
// CP220x_RST_High
//-----------------------------------------------------------------------------
//
// Drives the CP220x's Reset Pin High.
//
void CP220x_RST_High(void)
{
   char SFRPAGE_SAVE = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;
   P4 |= 0x20;                        // Set P4.5 High
   SFRPAGE = SFRPAGE_SAVE;
}

//-----------------------------------------------------------------------------
// AB4_RST_State
//-----------------------------------------------------------------------------
//
// Returns the state of the AB4's reset pin.
//
unsigned char AB4_RST_State(void)
{
   char rst_pin_state;
   char SFRPAGE_SAVE = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;
   rst_pin_state = P4 & 0x20;          // Get P4.5 State
   SFRPAGE = SFRPAGE_SAVE;
   return rst_pin_state;
}

//-----------------------------------------------------------------------------
// Local Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Configure UART1, Interrupts, Crossbar and GPIO ports
//
void PORT_Init (void)
{
   char SFRPAGE_SAVE = SFRPAGE;     // Save Current SFR page

   SFRPAGE = CONFIG_PAGE;           // set SFR page
   P0MDOUT |= 0x01;                 // set TX1 to push-pull
   P1MDOUT |= 0x40;                 // Set P1.6(TB_LED) to push-pull
   P2MDOUT |= 0x0C;                 // Set P2.2(AB4_LED1) and P2.3(AB4_LED2)
                                    // to push-pull
   // all pins used by the external memory interface are in push-pull mode
   P4MDOUT =  0xC0;
   P5MDOUT =  0xFF;
   P6MDOUT =  0xFF;
   P7MDOUT =  0xFF;
   P4 = 0xDF;                       // /WR, /RD, are high, RESET is low
   P5 = 0xFF;
   P6 = 0xFF;                       // P5, P6 contain the address lines
   P7 = 0xFF;                       // P7 contains the data lines

   TCON &= ~0x01;                   // Make /INT0 level triggered

   // Enable UART0, CP0, and /INT0. This puts /INT0 on P0.3.
   XBR0 = 0x80;
   XBR1 = 0x04;
   XBR2 = 0x44;
   SFRPAGE = SFRPAGE_SAVE;       // Restore SFR page

}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock.
//
void SYSCLK_Init (void)
{
     int i;                           // software timer

   char SFRPAGE_SAVE = SFRPAGE;     // Save Current SFR page

   SFRPAGE = CONFIG_PAGE;           // set SFR page

   OSCICN = 0x83;                   // set internal oscillator to run
                                    // at its maximum frequency

   CLKSEL = 0x00;                   // Select the internal osc. as
                                    // the SYSCLK source
   //Turn on the PLL and increase the system clock by a factor of M/N
   PLL0CN  = 0x00;                  // Set internal osc. as PLL source
   SFRPAGE = LEGACY_PAGE;
   FLSCL   = 0x30;                  // Set FLASH read time for 100 MHz clk
   SFRPAGE = CONFIG_PAGE;

   PLL0CN |= 0x01;                  // Enable Power to PLL

   PLL0DIV = 0x01;                  // Set Pre-divide value to N (N = 1)
   PLL0MUL = 0x04;                  // Multiply SYSCLK by M (M=4)
   PLL0FLT = 0x01;                  // Set the PLL filter register for
                                    // a reference clock from 12.2 - 19.5 MHz
                                    // and an output clock from 65 - 100 MHz
   for (i=0; i < 256; i++) ;        // Wait at least 5us
   PLL0CN  |= 0x02;                 // Enable the PLL
   while(!(PLL0CN & 0x10));         // Wait until PLL frequency is locked
   CLKSEL  = 0x02;                  // Select PLL as SYSCLK source

   SFRPAGE = SFRPAGE_SAVE;          // Restore SFR page
}

//-----------------------------------------------------------------------------
// EMIF_Init
//-----------------------------------------------------------------------------
//
// Configure the External Memory Interface for both on and off-chip access.
//
void EMIF_Init (void)
{

   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page

   SFRPAGE = LEGACY_PAGE;
   EMI0CF = 0xFB;                      // Split-mode (banked), non-multiplexed
                                       // on P4 - P7
   EMI0TC = 0xFF;	 	                  // This constant may be modified
                                       // according to SYSCLK to meet the
                                       // timing requirements for the CP2200


   EMI0CN = 0x20;			               // Page of XRAM accessed by EMIF

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
}

//===============
// CONDITIONAL
//===============
#if(UART_ENABLED)

//-----------------------------------------------------------------------------
// UART1_Init
//-----------------------------------------------------------------------------
//
// Configure the UART1 using Timer1, for <baudrate> and 8-N-1.
//
void UART1_Init (void)
{
   char SFRPAGE_SAVE = SFRPAGE;     // Save Current SFR page

   SFRPAGE = UART1_PAGE;
   SCON1   = 0x10;                  // SCON1: mode 0, 8-bit UART, enable RX

   SFRPAGE = TIMER01_PAGE;
   TMOD   &= ~0xF0;
   TMOD   |=  0x20;                 // TMOD: timer 1, mode 2, 8-bit reload


   if (SYSCLK/BAUDRATE/2/256 < 1) {
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON |= 0x10;                // T1M = 1; SCA1:0 = xx
   } else if (SYSCLK/BAUDRATE/2/256 < 4) {
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x13;               // Clear all T1 related bits
      CKCON |=  0x01;               // T1M = 0; SCA1:0 = 01
   } else if (SYSCLK/BAUDRATE/2/256 < 12) {
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x13;               // T1M = 0; SCA1:0 = 00
   } else {
      // Adjust for truncation in special case
      // Note: Additional cases may be required if the system clock is changed.
      #if ((BAUDRATE == 115200) && (SYSCLK == 98000000))
         TH1 = -((SYSCLK/BAUDRATE/2/48)+1);
      #else
         TH1 = -(SYSCLK/BAUDRATE/2/48);
      #endif
      CKCON &= ~0x13;               // Clear all T1 related bits
      CKCON |=  0x02;               // T1M = 0; SCA1:0 = 10
   }

   TL1 = TH1;                       // initialize Timer1
   TR1 = 1;                         // start Timer1

   SFRPAGE = UART1_PAGE;
   TI1 = 1;                         // Indicate TX1 ready

   SFRPAGE = SFRPAGE_SAVE;          // Restore SFR page

}

//-----------------------------------------------------------------------------
// _getkey
//-----------------------------------------------------------------------------
//
// SFR Paged version of _getkey
//
char _getkey ()  {

   char SFRPAGE_SAVE = SFRPAGE;     // Save Current SFR page
   char c;

   SFRPAGE = UART1_PAGE;
   while (!RI1);
   c = SBUF1;

   RI1 = 0;
   SFRPAGE = SFRPAGE_SAVE;

   return (c);
}

//-----------------------------------------------------------------------------
// putchar
//-----------------------------------------------------------------------------
//
// SFR Paged version of putchar
//
char putchar (char c)  {

   char SFRPAGE_SAVE = SFRPAGE;     // Save Current SFR page
   SFRPAGE = UART1_PAGE;

   // output CR
   if (c == '\n')  {
      while (!TI1);
      TI1 = 0;
      SBUF1 = 0x0d;
   }

   // output character
   while (!TI1);
   TI1 = 0;
   SBUF1 = c;

   SFRPAGE = SFRPAGE_SAVE;

   return (c);
}

#endif // UART_ENABLED

#endif // MCU == F120

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------