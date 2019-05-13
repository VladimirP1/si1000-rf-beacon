//-----------------------------------------------------------------------------
// F02x_CP220x_Start.c
//-----------------------------------------------------------------------------
// Copyright (C) 2005 Silicon Laboratories, Inc.
//
// Date: 12/14/05 
// Target: C8051F02x 
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
#include <c8051F020.h>                 // Device-specific SFR Definitions

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F02x
//-----------------------------------------------------------------------------

sfr16 DP       = 0x82;                 // data pointer
sfr16 TMR3RL   = 0x92;                 // Timer3 reload value
sfr16 TMR3     = 0x94;                 // Timer3 counter
sfr16 ADC0     = 0xbe;                 // ADC0 data
sfr16 ADC0GT   = 0xc4;                 // ADC0 greater than window
sfr16 ADC0LT   = 0xc6;                 // ADC0 less than window
sfr16 RCAP2    = 0xca;                 // Timer2 capture/reload
sfr16 TMR2     = 0xcc;                 // Timer2
sfr16 RCAP4    = 0xe4;                 // Timer4 capture/reload
sfr16 TMR4     = 0xf4;                 // Timer4
sfr16 DAC0     = 0xd2;                 // DAC0 data
sfr16 DAC1     = 0xd5;                 // DAC1 data

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK         22118400        // System Clock Frequency in Hz  

#define BASE_ADDRESS   0x10            // Starting address of off-chip XRAM. 
                                       // The C8051F020 has 4K of on-chip XRAM.
                                       // Off-chip access begins at 0x1000.
                                       // This value is loaded into EMI0CN.

#define EMIF_TIMING	  0x4D            // EMIF timing adjusted for a 25MHz
                                       // system clock. Assumes no back-to-back
                                       // EMIF operations with the CP220x
                                       // This value is loaded into EMI0TC.

sbit TB_LED = P1^6;                    // Green Target Board LED
sbit TB_SW2 = P3^7;                    // SW2 on Target Board

sbit AB4_SW1 = P2^0;                   // SW1 on AB4 Board
sbit AB4_SW2 = P2^1;                   // SW2 on AB4 Board
sbit AB4_LED1 = P2^2;                  // Green AB4 Board LED

// Note: On the 'F02x, the AB4_LED2 must be controlled from P4.4 the P2.3 must
// be configured to High-Z. The upper nibble of P4 contains /RD and /WR and 
// must be placed in push-pull mode. This forces P4.4 into push-pull mode. 
// The functions AB4_LED2_Off() and AB4_LED2_On() have been provided to 
// control LED2 from Port 4.

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
void AB4_LED2_Off(void);
void AB4_LED2_On(void);

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
// Note: The CP220x reset pin is configured to push-pull. We recommend that 
// the pin used for /RST be configured in open-drain mode. This pin assignment
// was chosen for compatibiity across multiple development kit platforms.
// Designs using only an F020 should select a different pin for /RST.
//
void PORT_Init (void)
{
   P1MDOUT |= 0x40;                 // Set P1.6(TB_LED) to push-pull
   P2MDOUT |= 0x0C;                 // Set P2.2(AB4_LED1) and P2.3 (AB4_LED2)

   P74OUT = 0xFF;                   // External memory interface pins are 
                                    // push-pull

   P4 = 0xC0;                       // /WR, /RD, are high, RESET is low
   P5 = 0xFF;
   P6 = 0xFF;                       // P5, P6 contain the address lines
   P7 = 0xFF;                       // P7 contains the data lines

   TCON &= ~0x01;                   // Make /INT0 level triggered

   // Enable UART0, CP0, and /INT0. This puts /INT0 on P0.3.
   XBR0 = 0x84;
   XBR1 = 0x04;
   XBR2 = 0x40;

}
//-----------------------------------------------------------------------------
// EMIF_Init
//-----------------------------------------------------------------------------
//
// Configure the External Memory Interface for both on and off-chip access.
//
void EMIF_Init (void)
{
   EMI0CF = 0xFB;             // Split-mode, non-multiplexed on P0 - P3

   EMI0TC = EMIF_TIMING;      // This constant may be modified
                              // according to SYSCLK to meet the
                              // timing requirements for the CP2200

   EMI0CN = BASE_ADDRESS;     // Page of XRAM accessed by EMIF

}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock.
//
void SYSCLK_Init (void)
{
   int i;                              // delay counter

   OSCXCN = 0x67;                      // start external oscillator with
                                       // 22.1184MHz crystal

   for (i=0; i < 256; i++) ;           // wait for oscillator to start

   while (!(OSCXCN & 0x80)) ;          // Wait for crystal osc. to settle

   OSCICN = 0x88;                      // select external oscillator as SYSCLK
                                       // source and enable missing clock
                                       // detector
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

   T2CON = 0x00;                       // Stop Timer2; Clear TF2;
   CKCON &= ~0x20;                     // use SYSCLK/12 as timebase

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

}

//-----------------------------------------------------------------------------
// ether_reset_low
//-----------------------------------------------------------------------------
//
// This routine drives the reset pin of the ethernet controller low.
//
void ether_reset_low()
{

   P4 &= ~0x20;                        // Pull reset low

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

   P4 |= 0x20;                         // Allow /RST to rise 

}

//-----------------------------------------------------------------------------
// ether_reset_state
//-----------------------------------------------------------------------------
//
// This routine returns the state of reset pin 
//
char ether_reset_state()
{
   char retval;   

   retval = P4 & 0x20;                 // Read state of /RST

   return retval;
}


//-----------------------------------------------------------------------------
// AB4_LED2_Off
//-----------------------------------------------------------------------------
//
// Drives the AB4's LED2 low.
//
void AB4_LED2_Off(void)
{
   P4 &= ~0x10;                        // Set P4.4 Low
}

//-----------------------------------------------------------------------------
// AB4_LED2_On
//-----------------------------------------------------------------------------
//
// Drives the AB4's LED2 high.
//
void AB4_LED2_On(void)
{
   P4 |= 0x10;                        // Set P4.4 High
}