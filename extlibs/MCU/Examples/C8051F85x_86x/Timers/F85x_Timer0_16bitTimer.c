//-----------------------------------------------------------------------------
// F85x_Timer0_16bitTimer.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program presents an example of use of the Timer 0 of the C8051F85x in
// 16-bit counter/timer mode. In this example, the LED toggles at a rate in ms
// defined by the LED_TOGGLE_RATE constant.
//
// The timer is set with the calculated overflow values. The overflow interrupt
// reloads the timer values and toggles the LED.
//
// Pinout:
//
//    P1.0 -> LED1
//
//    all other port pins unused
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) If another LED blink rate is required, change LED_TOGGLE_RATE (in ms).
// 4) Compile and download code to the 'F85x UDP MCU board.
// 5) Run the code. Check that the DS5 P1.0 LED is blinking.
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

#define SYSCLK             24500000L/8 // SYSCLK in Hz

#define TIMER_PRESCALER             48 // Based on Timer CKCON settings

#define LED_TOGGLE_RATE             50 // LED toggle interval in milliseconds
                                       // if LED_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

// Note: LED_TOGGLE_RATE*TIMER_TICKS_PER_MS should not exceed 65535 (0xFFFF)
// for the 16-bit timer

// These are defined here so the compiler does the work, not the MCU core

#define AUX1     TIMER_TICKS_PER_MS*LED_TOGGLE_RATE
#define AUX2     -AUX1
#define AUX3     AUX2 & 0x00FF
#define AUX4     ((AUX2 & 0xFF00) >> 8)

#define TIMER0_RELOAD_HIGH       AUX4  // Reload value for Timer0 high byte
#define TIMER0_RELOAD_LOW        AUX3  // Reload value for Timer0 low byte

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SBIT(LED1, SFR_P1, 0);                 // DS5 P1.0 LED

#define LED_ON                      0
#define LED_OFF                     1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Port_Init(void);                 // Port initialization routine
void Timer0_Init(void);               // Timer0 initialization routine

INTERRUPT_PROTO(TIMER0_ISR, INTERRUPT_TIMER0);

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
// P1.0   digital   push-pull  LED1
//
//-----------------------------------------------------------------------------
void Port_Init(void)
{
   P1MDOUT |= 0x01;                    // Set LED1 to push-pull

   XBR2 = 0x40;                        // Enable crossbar
}

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Timer0 as a 16-bit timer with interrupts
// enabled.
//
// Note: The Timer0 uses a 1:48 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.  This prescaler setting
// affects both Timer 0 and Timer 1.
//
//-----------------------------------------------------------------------------
void Timer0_Init(void)
{
   TH0 = TIMER0_RELOAD_HIGH;           // Init Timer0 High register
   TL0 = TIMER0_RELOAD_LOW;            // Init Timer0 Low register

   TMOD = 0x01;                        // Timer 0 in 16-bit mode

   CKCON = 0x02;                       // Timer 0 uses a 1:48 prescaler

   ET0 = 1;                            // Timer 0 interrupt enabled

   TR0 = 1;                            // Start Timer 0
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer0 interrupt and toggle the LED
//
//-----------------------------------------------------------------------------
INTERRUPT(TIMER0_ISR, INTERRUPT_TIMER0)
{
   LED1 = !LED1;                       // Toggle the LED

   TR0 = 0;                            // Stop Timer 0

   TH0 = TIMER0_RELOAD_HIGH;           // Reset Timer0 High register
   TL0 = TIMER0_RELOAD_LOW;            // Reset Timer0 Low register

   TR0 = 1;                            // Start Timer 0
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
