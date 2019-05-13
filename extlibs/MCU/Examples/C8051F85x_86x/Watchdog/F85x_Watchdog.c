//-----------------------------------------------------------------------------
// F85x_Watchdog.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program uses the Watchdog Timer to generate resets if the times between
// writes to the WDT update register (WDTCN) exceed a specified limit. The WDT
// can be disabled and enabled in the software as needed. This program resets
// the MCU when the S1 switch is held down. Upon a Watchdog reset, the LED
// blinks four times faster compared to before. The reset is caused by an
// overflow of the WDT and can be confirmed by checking the value of the
// RSTSRC register.
//
// How to Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to the 'F85x UDP MCU board.
// 4) Run the code:
//         a) The test will blink the DS5 P1.0 LED at a rate of 10 Hz until the
//            switch S1 (P1.7) is pressed.
//         b) Once the the switch is pressed and held for a long enough time,
//            the WDT will cause a reset.
//         c) The initialization code checks for a WDT reset and blinks the LED
//            four times faster than before.
//
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 20 FEB 2013
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>            // SFR declarations

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK            24500000 / 8 // SYSCLK frequency in Hz

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SBIT(LED1, SFR_P1, 0);

#define LED_ON                       0
#define LED_OFF                      1

SBIT(S1, SFR_P1, 7);

#define SW_PRESSED                   0
#define SW_NOT_PRESSED               1

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void Port_Init (void);
void WDT_Init (void);
void Timer2_Init (int counts);

INTERRUPT_PROTO (TIMER2_ISR, INTERRUPT_TIMER2);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   Port_Init();                        // Initialize crossbar and GPIO

   if ((RSTSRC & 0x02) == 0x00)        // First check the PORSF bit. If PORSF
   {                                   // is set, all other RSTSRC flags are
                                       // indeterminate.
      // Check if the last reset was due to the Watchdog Timer
      if (RSTSRC == 0x08)
      {
         Timer2_Init (SYSCLK / 12 / 20); // Make LED blink at 20 Hz
         EA = 1;                         // Enable global interrupts

         while(1);                       // wait forever
      }
      else
      {
         // Init Timer2 to generate interrupts at a 5 Hz rate.
         Timer2_Init (SYSCLK / 12 / 5);
      }

   }

   WDT_Init();                         // Intialize the Watchdog Timer

   EA = 1;

   while (1)
   {
       while(!S1);                     // Force the MCU to not kick the WDT as
                                       // long as S1 is pressed.  If S1 is
                                       // held for long enough, a WDT reset
                                       // will occur.

       WDTCN = 0xA5;                   // Reset the WDT
    }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Crossbar and GPIO ports.
//
// P1.0   digital   push-pull     LED1
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDOUT |= 0x01;                    // LED1 is a push-pull output

   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1)  int counts - calculated Timer overflow rate
//                    range is positive range of integer: 0 to 32767
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/48 as its time base.
//
//-----------------------------------------------------------------------------
void Timer2_Init (int counts)
{
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON  &= ~0x30;                    // Timer2 clocked based on T2XCLK;
   TMR2RL  = -counts;                  // Init reload values
   TMR2    = 0xffff;                   // set to reload immediately
   ET2     = 1;                        // enable Timer2 interrupts
   TR2     = 1;                        // start Timer2
}

//-----------------------------------------------------------------------------
// WDT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Initialize the WDT to the desired interval.
//
//-----------------------------------------------------------------------------
void WDT_Init()
{
   // Even though the LFOSC is disabled after a reset and the register reads
   // disabled, the oscillator is automatically enabled whenever it's used by
   // the WDT. This means we do not need to enable it here.

   // WDT interval given by:
   //
   // 4 ^ (3 + WDTCN[2:0]) * TLFOSC
   //
   // With the LFOSC running at 80 kHz, let's set the timeout interval to 5 s:
   //
   // 5 s = 4 ^ (3 + WDTCN[2:0]) * 12.5 us
   // 400000 = 4 ^ (3 + WDTCN[2:0])
   // log4(400000) = 3 + WDTCN[2:0]
   // 9.3048 = 3 + WDTCN[2:0]
   // WDTCN[2:0] = ~6
   //
   // This gives us an actual WDT interval of:
   //
   // 4 ^ (3 + 6) * 12.5 us = 3.28 s
   //

   // WDTCN.7 must be cleared to 0 to write to the timeout interval
   WDTCN = 0x06;

   // Enable and reload the WDT
   WDTCN = 0xA5;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED whenever Timer2 overflows.
//
//-----------------------------------------------------------------------------
INTERRUPT(TIMER2_ISR, INTERRUPT_TIMER2)
{
   TF2H = 0;                           // Clear the Timer 2 interrupt flag

   LED1 = !LED1;                       // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
