//-----------------------------------------------------------------------------
// F34x_Init.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// 
// Contains Initialization Routines for the F340.
//
// FID:            
// Target:         C8051F34x
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

#if(MCU == F340)

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F34x
//-----------------------------------------------------------------------------
sfr16 TMR2RL   = 0xca;                    // Timer2 reload value
sfr16 TMR2     = 0xcc;                    // Timer2 counter
sfr16 TMR3RL   = 0x92;                    // Timer3 reload value
sfr16 TMR3     = 0x94;                    // Timer3 counter

//-----------------------------------------------------------------------------
// Local Constants
//-----------------------------------------------------------------------------
#define SYSCLK  48000000


#if(BOARD == CP2201EB)
sbit AB4_RST = P0^6;
#else
sbit AB4_RST = P1^0;
#endif

//-----------------------------------------------------------------------------
// Local Global Variables 
//-----------------------------------------------------------------------------

static unsigned int timeout;

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------
void Reset_Init(void);
void SYSCLK_Init (void);

void wait_ms(int ms);  

void CP220x_RST_Low(void);
void CP220x_RST_High(void);

#if(UART_ENABLED)
char _getkey ();
char putchar (char c);
#endif

//-----------------------------------------------------------------------------
// Local Function Prototypes
//-----------------------------------------------------------------------------

void PORT_Init (void);

void EMIF_Init (void);

#if(UART_ENABLED)
void UART0_Init (void);
#endif

//-----------------------------------------------------------------------------
// Exported Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Reset_Init
//-----------------------------------------------------------------------------
void Reset_Init(void)
{
   
   // Disable Watchdog timer
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
                                       // enable)
   // Initialize the MCU
   PORT_Init();
   SYSCLK_Init();   
   EMIF_Init();

   EX0 = 1;                            // Enable External Interrupt 0   

   #if(UART_ENABLED)
   UART0_Init();
   puts("\n*** Reset ***\nC8051F34x MCU Initialized\n");
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
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON  &= ~0x30;                    // Timer2 clocked based on T2XCLK;

   TMR2RL = -(SYSCLK/1000/12);         // Timer 2 overflows at 1 kHz
   TMR2 = TMR2RL;

   ET2 = 0;                            // Disable Timer 2 interrupts

   TR2 = 1;                            // Start Timer 2

   while(ms){
      TF2H = 0;
      while(!TF2H);                    // wait until timer overflows
      ms--;                            // decrement ms
   }

   TR2 = 0;                            // Stop Timer 2

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

   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON  &= ~0x30;                    // Timer2 clocked based on T2XCLK;
   
   TMR2RL = -(SYSCLK/1000/12);         // Timer 2 overflows at 1 kHz
   TMR2 = TMR2RL;

   ET2 = 0;                            // Disable Timer 2 interrupts

   TR2 = 1;                            // Start Timer 2

}

//-----------------------------------------------------------------------------
// get_ms_timer_flag()
//-----------------------------------------------------------------------------
//
// This routine returns the state of the ms_timer overflow flag.
//
char get_ms_timer_flag(void)
{
   return TF2H;
}

//-----------------------------------------------------------------------------
// clear_ms_timer_flag()
//-----------------------------------------------------------------------------
//
// This routine returns the state of the ms_timer overflow flag.
//
void clear_ms_timer_flag(void)
{
   TF2H = 0;
}

//-----------------------------------------------------------------------------
// CP220x_RST_Low
//-----------------------------------------------------------------------------
//
// Drives the CP220x's Reset Pin Low.
//
void CP220x_RST_Low(void)
{
   AB4_RST = 0;                        // Set P1.0 Low
}

//-----------------------------------------------------------------------------
// CP220x_RST_High
//-----------------------------------------------------------------------------
//
// Allows the CP220x's Reset Pin to be pulled up.
//
// Waits until the reset pin rises.
//
void CP220x_RST_High(void)
{
   AB4_RST = 1;                        // Set P1.0 High

   while(!AB4_RST);
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
   
   rst_pin_state = AB4_RST;          // Get P4.5 State
   
   return rst_pin_state;
}
//-----------------------------------------------------------------------------
// Local Initialization Routines
//-----------------------------------------------------------------------------

#if(BOARD == CP2201EB)

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Configure the Crossbar and GPIO ports for the CP2201EB
//
// P0.2 - 
// P0.3 - 
//
// P0.4 - UART TX
// P0.5 - UART RX
// P0.6 - /RST
// P0.7 - /INT 
//
// P1.3 - ALE
//
// P1.5 - VREF (analog input)
// P1.6 - /RD
// P1.7 - /WR
//
// P2.2 - LIGHT_SENSE (analog input)
// P2.4 - LED
// P2.5 - LED
// P2.6 - /LS_EN
// P2.7 - YELLOW_LED
//
// P3.7 - /A15_CS
// P4 - Address/Data Bus
//

void PORT_Init (void)
{
   IT01CF = 0x67;                      // Enable Interrupt 0 on P0.7
                                       // Enable Interrupt 1 on P0.6

   XBR0    = 0x01;                     // Enable UART on P0.4(RX) and P0.5(TX)
   XBR1    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
   
   P0MDOUT |= 0x10;                    // Enable TX0 as push-pull output

   P1MDOUT |= 0xC8;                    // /WR and /RD and ALE are push-pull
   
   P2MDOUT |= 0xF0;                    // Yellow LED and /LS_EN are push-pull
                                       // P2.4 and P2.5 are push-pull
   P2      &= ~0x80;                   // LED initially off
   P2      |=  0x40;                   // LS initially off
     

   P0MDOUT &= ~0x40;                   // Reset is open-drain
   P0      &= ~0x40;                   // Reset initially zero

   P1MDIN &= ~0x20;                    // P1.5 Vref is analog input
   P2MDIN &= ~0x04;                    // P2.2 is an analog input   

   // Set EMIF pins to push pull
   P3MDOUT |= 0x80;                    // ALE is push-pull
   P4MDOUT |= 0xFF;                    // Data Bus

}

#else
//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Configure the Crossbar and GPIO ports.
// 
// P0.3 - /INT
// P0.4 - UART TX
// P0.5 - UART RX
// 
// P1.0 - AB4-/RST 
// P1.1 - AB4-SW1
// P1.2 - AB4-SW2
// P1.3 - AB4-LED1
// P1.4 - AB4-LED2
// P1.5 -
// P1.6 - /RD
// P1.7 - /WR
//
// P2.2 - LED - shared with address line
//
// P2 - Address High
// P3 - Address Low
// P4 - Data Bus
//

void PORT_Init (void)
{

   IT01CF = 0x03;                      // Enable Interrupt 0 on P0.3
   TCON &= ~0x01;                      // Make /INT0 level triggered

   XBR0    = 0x01;                     // Enable UART on P0.4(TX) and P0.5(RX)
   XBR1    = 0x40;                     // Enable Crossbar and weak pull-ups

   P0MDOUT |= 0x10;                    // Enable TX0 as push-pull output
   P1MDOUT |= 0xC0;                    // /WR and /RD are push-pull
   P1MDOUT |= 0x18;                    // AB4 LEDs are push-pull
   P1MDOUT &= ~0x01;                   // /RST is open-drain
      
   // Configure External Memory Interface Pins to push-pull except for
   // P2.0 and P2.1 which are used for target board switches.
   P2MDOUT |= 0xFC;                    // ADDR[15:8]
   P3MDOUT |= 0xFF;                    // ADDR[7:0]
   P4MDOUT |= 0xFF;                    // DATA[7:0]
   
   // Set initial values of External Memory Interface Pins
   P1 = 0xFE;                          // /WR, /RD, are high, RESET is low
   P1 &= ~0x18;                        // LEDs off
   P2 = 0xFF;                          // ADDR[15:8] initially high 
   P3 = 0xFF;                          // ADDR[7:0] initially high
   P4 = 0xFF;                          // DATA[7:0] intially high
}
#endif


//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock to use the internal oscillator
// multiplied by 4 as the system clock. The resulting frequency is 48 MHz.
// Also enables the Missing Clock Detector and VDD monitor.
//

void SYSCLK_Init (void)
{
   int i;

   OSCICN |= 0x03;                     // Configure internal oscillator for
                                       // its maximum frequency
  
   CLKMUL = 0x00;                      // Reset Clock Multiplier and select
                                       // internal oscillator as input source

   CLKMUL |= 0x80;                     // Enable the Clock Multiplier

   for(i = 0; i < 256; i++);           // Delay at least 5us
   
   CLKMUL |= 0xC0;                     // Initialize the Clock Multiplier
   
   while(!(CLKMUL & 0x20));            // Wait for MULRDY => 1
   
   RSTSRC = 0x06;                      // Enable missing clock detector
                                       // and VDD monitor
   
   FLSCL |= 0x10;                      // Set Flash Scale for 48MHz
   
   CLKSEL |= 0x03;                     // Select output of clock multiplier
                                       // as the system clock.

}

//-----------------------------------------------------------------------------
// EMIF_Init
//-----------------------------------------------------------------------------
//
// Configure the external memory interface to use upper port pins in 
// non-multiplexed mode to a mixed on-chip/off-chip configuration without 
// Bank Select.
//
void EMIF_Init (void)
{
#if(BOARD == CP2201EB)
   EMI0CF = 0x09;                      // muxed mode; split mode 
                                       // with bank select
#else
   EMI0CF = 0x1B;                      // non-muxed mode; split mode 
                                       // with bank select
#endif

   EMI0TC = 0xFF;                      // slowest timing (4-cycle MOVX)

   EMI0CN = 0x20;                      // Off-chip
}

//===============
// CONDITIONAL
//===============
#if(UART_ENABLED)

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//
void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits
   if (SYSCLK/BAUDRATE/2/256 < 1) {
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   } else if (SYSCLK/BAUDRATE/2/256 < 4) {
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x09;
   } else if (SYSCLK/BAUDRATE/2/256 < 12) {
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } else {
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   }

   TL1 = TH1;                          // init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TR1 = 1;                            // START Timer1
   TI0 = 1;                            // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// _getkey
//-----------------------------------------------------------------------------
//
// SFR Paged version of _getkey
//
char _getkey ()  {

   char c;
  
   while (!RI0);
   c = SBUF0;
   
   RI0 = 0;
   return (c);
}

//-----------------------------------------------------------------------------
// putchar
//-----------------------------------------------------------------------------
//
// SFR Paged version of putchar
//
char putchar (char c)  {

   // output CR  
   if (c == '\n')  {
      while (!TI0);
      TI0 = 0;
      SBUF0 = 0x0d;                         
   }
   
   // output character
   while (!TI0);
   TI0 = 0;
   SBUF0 = c;   
   
   return (c);
}

#endif // UART_ENABLED

#endif // MCU == F340

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------