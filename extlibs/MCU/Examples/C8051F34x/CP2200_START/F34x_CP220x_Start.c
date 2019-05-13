//-----------------------------------------------------------------------------
// F34x_CP220x_Start.c
//-----------------------------------------------------------------------------
// Copyright (C) 2005 Silicon Laboratories, Inc.
//
// Date: 12/14/05 
// Target: C8051F34x 
//
// Description:
//    This file contains the initializations required to use the CP220x Register
//    Windows in the Silicon Laboratories IDE with the AB4 Ethernet Development
//    Board. The PORT_Init() and EMIF_Init() routines should be changed to use
//    this software with custom hardware.
//
//    To view the contents of CP220x registers:
//    1) Run this code until the while(1) is reached.
//    2) Click View->Debug Windows->CP220x.
//    3) Select an INI file.  
//    4) A register window should open displaying the register contents.
//
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <c8051F340.h>                 // Device-specific SFR Definitions

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F34x
//-----------------------------------------------------------------------------
sfr16 TMR2RL   = 0xca;                 // Timer2 reload value
sfr16 TMR2     = 0xcc;                 // Timer2 counter

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK         48000000        // System Clock Frequency in Hz  

#define BASE_ADDRESS   0x10            // Starting address of off-chip XRAM. 
                                       // The C8051F120 has 8K of on-chip XRAM.
                                       // Off-chip access begins at 0x2000.
                                       // This value is loaded into EMI0CN.

#define EMIF_TIMING	  0x9E            // EMIF timing adjusted for a 50MHz
                                       // system clock. Assumes no back-to-back
                                       // EMIF operations with the CP220x
                                       // This value is loaded into EMI0TC.

sbit TB_SW1 = P2^0;                    // SW1 on Target Board
sbit TB_SW2 = P2^1;                    // SW2 on Target Board
sbit TB_LED1 = P2^2;                   // LED1 Target Board LED
sbit TB_LED2 = P2^3;                   // LED2 Target Board LED

sbit AB4_RST = P1^0;                   // /RST on AB4 Board
sbit AB4_SW1 = P1^1;                   // SW1 on AB4 Board
sbit AB4_SW2 = P1^2;                   // SW2 on AB4 Board
sbit AB4_LED1 = P1^3;                  // Green AB4 Board LED
sbit AB4_LED2 = P1^4;                  // Red AB4 Board LED

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

// Initialization Routines
void PORT_Init (void);
void SYSCLK_Init (void);
void EMIF_Init(void);

// Support Routines
void wait_ms(int ms);
void ether_reset_low(void);
void ether_reset_high(void);
char ether_reset_state(void);

//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main(void)
{

   // Disable Watchdog timer
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
                                       // enable)

   // Initialize the MCU
   PORT_Init();
   SYSCLK_Init();
   EMIF_Init();
   
   // Reset the CP220x
   ether_reset_low();
   wait_ms(5);
   ether_reset_high();
   while(!ether_reset_state());

   while(1);
}


//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Configure the Interrupts, Crossbar and GPIO ports
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
   P2 = 0xFF;                          // ADDR[15:8] initially high 
   P3 = 0xFF;                          // ADDR[7:0] initially high
   P4 = 0xFF;                          // DATA[7:0] intially high

}
//-----------------------------------------------------------------------------
// EMIF_Init
//-----------------------------------------------------------------------------
//
// Configure the External Memory Interface for both on and off-chip access.
//
void EMIF_Init (void)
{
   
   EMI0CF = 0x1B;             // non-muxed mode; split mode
                              // with bank select

   EMI0TC = EMIF_TIMING;      // This constant may be modified
                              // according to SYSCLK to meet the
                              // timing requirements for the CP2200

   EMI0CN = BASE_ADDRESS;     // Page of XRAM accessed by EMIF

}

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
// Support Routines
//-----------------------------------------------------------------------------

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
   CKCON  &= ~0x60;                    // Timer2 clocked based on T2XCLK;

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
// ether_reset_low
//-----------------------------------------------------------------------------
//
// This routine drives the reset pin of the ethernet controller low.
//
void ether_reset_low()
{

   AB4_RST = 0;                        // Pull /RST low

}

//-----------------------------------------------------------------------------
// ether_reset_high
//-----------------------------------------------------------------------------
//
// This routine places the reset pin in High-Z allowing it to be pulled up 
// using the external pull-up resistor.
//
void ether_reset_high()
{

   AB4_RST = 1;                        // Allow /RST to rise

}

//-----------------------------------------------------------------------------
// ether_reset_state
//-----------------------------------------------------------------------------
//
// This routine returns the state of reset pin 
//
char ether_reset_state()
{

   return AB4_RST;                     // Return state of /RST

}

