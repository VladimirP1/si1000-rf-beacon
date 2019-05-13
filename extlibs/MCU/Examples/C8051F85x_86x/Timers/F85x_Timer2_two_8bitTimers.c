//-----------------------------------------------------------------------------
// F85x_Timer2_two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program presents an example of use of the Timer2 in two 8-bit reload
// counter/timer mode. It sets Timer 2 to be two 8-bit timers that interrupt
// every millisecond.
//
// When the Timer2 low interrupts, a counter (low_counter) increments and
// toggles LED1 after reaching the value defined by LED1_TOGGLE_RATE.
// When the Timer2 high interrupts, a counter (high_counter) increments and
// toggles LED2 after reaching the value defined by LED2_TOGGLE_RATE.
//
// Pinout:
//
//    P1.0 -> LED1 (toggled by TMR2L)
//    P1.1 -> LED2 (toggled by TMR2H)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - J27: DS6 P1.1 - P1.1/ADC0.9
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) If other LED blink rates are required, change LED1_TOGGLE_RATE and
//    LED2_TOGGLE_RATE (in ms).
// 4) Compile and download code to the 'F85x UDP MCU board.
// 5) Run the code. Verify that both LEDs are blinking.
//
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
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
#include <C8051F850_defs.h>            // SFR declaration

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK          (24500000L/8L) // SYSCLK in Hz

#define TIMER_PRESCALER            12L // Based on Timer2 CKCON and TMR2CN
                                       // settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  (SYSCLK/TIMER_PRESCALER/1000L)

// Note: TIMER_TICKS_PER_MS should not exceed 255 (0xFF) for the 8-bit timers

#define LED1_TOGGLE_RATE           100 // LED1 toggle rate in milliseconds
                                       // if LED1_TOGGLE_RATE = 1, LED1 will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

#define LED2_TOGGLE_RATE            30 // LED2 toggle rate in milliseconds
                                       // if LED2_TOGGLE_RATE = 1, LED2 will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SBIT(LED1, SFR_P1, 0);                 // DS5 P1.0 LED
SBIT(LED2, SFR_P1, 1);                 // DS6 P1.1 LED

#define LED_ON                       0
#define LED_OFF                      1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Port_Init(void);                  // Port initialization routine
void Timer2_Init(void);                // Timer2 initialization routine

INTERRUPT_PROTO (TIMER2_ISR, INTERRUPT_TIMER2);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   Port_Init ();                       // Init Ports
   Timer2_Init ();                     // Initialize the Timer2

   EA = 1;                             // Enable global interrupts

   while (1);                          // Loop forever
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
// P1.0   digital   push-pull   LED1 (toggled by TMR2L)
// P1.1   digital   push-pull   LED2 (toggled by TMR2H)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDOUT |= 0x03;                    // Set LEDs to be push-pull

   XBR2 = 0x40;                        // Enable crossbar
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures Timer2 as two 8-bit reload timers with interrupts
// using the HFOSC at 24.5 MHz / 8 with a timer prescaler of 1:12.
//
// Note: The Timer2 uses a 1:12 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//
//-----------------------------------------------------------------------------
void Timer2_Init(void)
{
   CKCON &= ~0x30;                     // Timer2 uses SYSCLK/12
   TMR2CN &= ~0x01;

   TMR2RLH = -TIMER_TICKS_PER_MS;      // Init Timer2 Reload High register
   TMR2RLL = -TIMER_TICKS_PER_MS;      // Init Timer2 Reload Low register

   TMR2H = -TIMER_TICKS_PER_MS;        // Init Timer2 High register
   TMR2L = -TIMER_TICKS_PER_MS;        // Init Timer2 Low register

   TMR2CN = 0x28;                      // Low byte interrupt and split-mode
                                       // enabled

   TR2 = 1;                            // Start Timer 2

   ET2 = 1;                            // Timer2 interrupt enabled
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This function processes the Timer2 interrupt. It checks both counters for
// matches and, upon a match, toggles the LEDs.
//
//-----------------------------------------------------------------------------
INTERRUPT (TIMER2_ISR, INTERRUPT_TIMER2)
{
   static U16 low_counter = 0;
   static U16 high_counter = 0;

   if(TF2L == 1)
   {
      low_counter++;

      if(low_counter == LED1_TOGGLE_RATE)
      {
         LED1 = !LED1;                 // Toggle the yellow LED

         low_counter = 0;              // Reset interrupt counter
      }

      TF2L = 0;
   }
   if(TF2H == 1)
   {
      high_counter++;

      if(high_counter == LED2_TOGGLE_RATE)
      {
         LED2 = !LED2;                 // Toggle the Red LED

         high_counter = 0;             // Reset interrupt counter
      }

      TF2H = 0;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
