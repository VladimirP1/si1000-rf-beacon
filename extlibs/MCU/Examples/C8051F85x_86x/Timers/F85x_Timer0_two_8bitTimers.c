//-----------------------------------------------------------------------------
// F85x_Timer0_two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program presents an example of use of the Timer 0 of the C8051F85x/86x
// in two 8-bit reload counter/timer mode. It sets the two timers to interrupt
// every millisecond.
//
// When the timer low interrupts, a software counter (low_counter) increments
// and toggles LED1 when it reaches the value defined by LED1_TOGGLE_RATE.
//
// When the timer high interrupts, a software counter (high_counter) increments
// and toggles LED2 when it reaches the value defined by LED2_TOGGLE_RATE.
//
// Pinout:
//
//    P1.0 -> LED1 (toggled by TL0)
//    P1.1 -> LED2 (toggled by TH0)
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
#include <C8051F850_defs.h>            // SFR declarations

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000/8  // SYSCLK in Hz


#define TIMER_PRESCALER            48  // Based on Timer CKCON settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

// Note: TIMER_TICKS_PER_MS should not exceed 255 (0xFF) for the 8-bit timers

#define LED1_TOGGLE_RATE          100  // LED1 toggle rate in milliseconds
                                       // If LED1_TOGGLE_RATE = 1, LED1 will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

#define LED2_TOGGLE_RATE           30  // LED2 toggle rate in milliseconds
                                       // if LED2_TOGGLE_RATE = 1, LED2 will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SBIT(LED1, SFR_P1, 0);                 // DS5 P1.0 LED
SBIT(LED2, SFR_P1, 1);                 // DS6 P1.1 LED

#define LED_ON                      0
#define LED_OFF                     1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Port_Init(void);                  // Port initialization routine
void Timer0_Init(void);                // Timer0 initialization routine

INTERRUPT_PROTO(TIMER0_ISR, INTERRUPT_TIMER0);
INTERRUPT_PROTO(TIMER1_ISR, INTERRUPT_TIMER1);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   Port_Init ();                       // Init Ports
   Timer0_Init ();                     // Initialize the Timer0

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
// This function configures the crossbar and GPIO ports.
//
// P1.0   digital   push-pull   LED1 (toggled by TL0)
// P1.1   digital   push-pull   LED2 (toggled by TH0)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDOUT |= 0x03;                    // Set LED's to push-pull

   XBR2 = 0x40;                        // Enable crossbar
}

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Timer 0 as two 8-bit timers, interrupts
// enabled, using the HFOSC at 24.5 MHz / 8 with a divider of 48.
//
// Note: The Timer0 uses a 1:48 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.  This prescaler setting
// affects both Timer 0 and Timer 1.
//
//-----------------------------------------------------------------------------
void Timer0_Init(void)
{
   TH0 = -TIMER_TICKS_PER_MS;          // Init Timer0 High register
   TL0 = -TIMER_TICKS_PER_MS;          // Init Timer0 Low register

   TMOD = 0x03;                        // Timer 0 in two 8-bit mode

   CKCON = 0x02;                       // Timer 0 uses a 1:48 prescaler

   ET0 = 1;                            // Timer 0 interrupt enabled
   ET1 = 1;                            // Timer 1 interrupt enabled

   // Start TL0 and TH0
   TR0 = 1;
   TR1 = 1;
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer0 interrupt and toggle the Yellow LED
//
//-----------------------------------------------------------------------------
INTERRUPT (TIMER0_ISR, INTERRUPT_TIMER0)
{
   static U16 low_counter = 0;

   TL0 = -TIMER_TICKS_PER_MS;          // Reset Timer0 Low register

   low_counter++;

   if(low_counter == LED1_TOGGLE_RATE)
   {
      LED1 = !LED1;                    // Toggle the LED

      low_counter = 0;                 // Reset interrupt counter
   }
}

//-----------------------------------------------------------------------------
// Timer1_ISR
//-----------------------------------------------------------------------------
//
// Note: In this mode the Timer0 is effectively using the Timer1 interrupt
// vector. For more info, check the Timers chapter of the datasheet.
//
//-----------------------------------------------------------------------------
INTERRUPT (TIMER1_ISR, INTERRUPT_TIMER1)
{
   static U16 high_counter = 0;

   TH0 = -TIMER_TICKS_PER_MS;          // Reset Timer0 High register

   high_counter++;

   if(high_counter == LED2_TOGGLE_RATE)
   {
      LED2 = !LED2;                    // Toggle the LED

      high_counter = 0;                // Reset interrupt counter
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
