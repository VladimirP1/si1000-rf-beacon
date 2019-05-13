//-----------------------------------------------------------------------------
// F12x_CP220x_Start.c
//-----------------------------------------------------------------------------
// Copyright (C) 2005 Silicon Laboratories, Inc.
//
// Date: 12/14/05 
// Target: C8051F12x 
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
#include <c8051F120.h>                 // Device-specific SFR Definitions

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
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK         73500000        // System Clock Frequency in Hz  

#define BASE_ADDRESS   0x20            // Starting address of off-chip XRAM. 
                                       // The C8051F120 has 8K of on-chip XRAM.
                                       // Off-chip access begins at 0x2000.
                                       // This value is loaded into EMI0CN.

#define EMIF_TIMING	  0xEF            // EMIF timing adjusted for a 75MHz
                                       // system clock. Assumes no back-to-back
                                       // EMIF operations with the CP220x
                                       // This value is loaded into EMI0TC.

sbit TB_LED = P1^6;                    // Green Target Board LED
sbit TB_SW2 = P3^7;                    // SW2 on Target Board

sbit AB4_SW1 = P2^0;                   // SW1 on AB4 Board
sbit AB4_SW2 = P2^1;                    // SW2 on AB4 Board
sbit AB4_LED1 = P2^2;                  // Green AB4 Board LED
sbit AB4_LED2 = P2^3;                  // Red AB4 Board LED

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

   // Disable watchdog timer
   WDTCN = 0xde;
   WDTCN = 0xad;

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
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page

   SFRPAGE = CONFIG_PAGE;              // set SFR page
   P1MDOUT |= 0x40;                    // Set P1.6(TB_LED) to push-pull
   P2MDOUT |= 0x0C;                    // Set P2.2(AB4_LED1) and P2.3 (AB4_LED2)
  
   // all pins used by the external memory interface are in push-pull mode
   // the reset pin (P4.5) is open-drain
   P4MDOUT =  0xDF;
   P5MDOUT =  0xFF;
   P6MDOUT =  0xFF;
   P7MDOUT =  0xFF;
   P4 = 0xC0;                          // /WR, /RD, are high, /RST is low
   P5 = 0xFF;
   P6 = 0xFF;                          // P5, P6 contain the address lines
   P7 = 0xFF;                          // P7 contains the data lines

   TCON &= ~0x01;                      // Make /INT0 level triggered

   // Enable UART0, CP0, and /INT0. This puts /INT0 on P0.3.
   XBR0 = 0x84;
   XBR1 = 0x04;
   XBR2 = 0x40;
   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page

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

   EMI0CF = 0xFB;                      // Split-mode, non-multiplexed on P0 - P3

   EMI0TC = EMIF_TIMING;               // This constant may be modified
                                       // according to SYSCLK to meet the
                                       // timing requirements for the CP2200

   EMI0CN = BASE_ADDRESS;              // Page of XRAM accessed by EMIF
   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page

}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock.
//
void SYSCLK_Init (void)
{
   int i;                              // software timer

   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page

   SFRPAGE = CONFIG_PAGE;              // set SFR page

   OSCICN = 0x83;                      // set internal oscillator to run
                                       // at its maximum frequency

   CLKSEL = 0x00;                      // Select the internal osc. as
                                       // the SYSCLK source
   //Turn on the PLL and increase the system clock by a factor of M/N
   PLL0CN  = 0x00;                     // Set internal osc. as PLL source
   SFRPAGE = LEGACY_PAGE;
   FLSCL   = 0x30;                     // Set FLASH read time for 100 MHz clk
   SFRPAGE = CONFIG_PAGE;

   PLL0CN |= 0x01;                     // Enable Power to PLL

   PLL0DIV = 0x01;                     // Set Pre-divide value to N (N = 1)
   PLL0MUL = 0x04;                     // Multiply SYSCLK by M (M=4)
   PLL0FLT = 0x01;                     // Set the PLL filter register for
                                       // a reference clock from 12.2 - 19.5 MHz
                                       // and an output clock from 65 - 100 MHz
   for (i=0; i < 256; i++) ;           // Wait at least 5us
   PLL0CN  |= 0x02;                    // Enable the PLL
   while(!(PLL0CN & 0x10));            // Wait until PLL frequency is locked
   CLKSEL  = 0x02;                     // Select PLL as SYSCLK source

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page

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
// ether_reset_low
//-----------------------------------------------------------------------------
//
// This routine drives the reset pin of the ethernet controller low.
//
void ether_reset_low()
{

   unsigned char SFRPAGE_Save;

   SFRPAGE_Save = SFRPAGE;             // Save Current SFR page
   SFRPAGE = CONFIG_PAGE;              // Switch to ports SFR page

   P4 &= ~0x20;                        // Pull reset low

   SFRPAGE = SFRPAGE_Save;             // Restore SFR page

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

   unsigned char SFRPAGE_Save;

   SFRPAGE_Save = SFRPAGE;             // Save Current SFR page
   SFRPAGE = CONFIG_PAGE;              // Switch to ports SFR page

   P4 |= 0x20;                         // Allow reset to rise

   SFRPAGE = SFRPAGE_Save;             // Restore SFR page

}

//-----------------------------------------------------------------------------
// ether_reset_state
//-----------------------------------------------------------------------------
//
// This routine returns the state of reset pin 
//
char ether_reset_state()
{

   unsigned char SFRPAGE_Save;
   char retval;   

   SFRPAGE_Save = SFRPAGE;             // Save Current SFR page
   SFRPAGE = CONFIG_PAGE;              // Switch to ports SFR page

   retval = P4 & 0x20;                 // Read state of /RST

   SFRPAGE = SFRPAGE_Save;             // Restore SFR page
   
   return retval;
}

