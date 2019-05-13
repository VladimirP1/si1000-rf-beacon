//-----------------------------------------------------------------------------
// F85x_Timer2_16bitReloadTimer.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program presents an example of use of the Timer2 in 16-bit reload
// counter/timer mode.
//
// The timer counts from the reload value up to FFFFh. When it overflows, an
// interrupt is generated and the timer is automatically reloaded by hardware.
// In this interrupt, firmware toggles the LED.
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

#define TIMER_PRESCALER            12  // Based on Timer2 CKCON and TMR2CN
                                       // settings

#define LED_TOGGLE_RATE            50  // LED toggle rate in milliseconds
                                       // if LED_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

// These are defined here so the compiler does the work, not the MCU core

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

// Note: LED_TOGGLE_RATE*TIMER_TICKS_PER_MS should not exceed 65535 (0xFFFF)
// for the 16-bit timer

#define TIMER2_RELOAD       -(TIMER_TICKS_PER_MS*LED_TOGGLE_RATE)

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SBIT(LED1, SFR_P1, 0);                 // DS5 P1.0 LED

#define LED_ON                      0
#define LED_OFF                     1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Port_Init (void);                 // Port initialization routine
void Timer2_Init (void);               // Timer2 initialization routine

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
// This function configures the crossbar and GPIO ports.
//
// P1.0  digital  push-pull  LED1
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDOUT |= 0x01;                    // Set LED1 to push-pull

   XBR2 = 0x40;                        // Enable crossbar
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures Timer2 as a 16-bit reload timer, interrupts
// enabled, using the SYSCLK at 24.5 MHz / 8 with a divider of 12.
//
// Note: The Timer2 uses a 1:12 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//
//-----------------------------------------------------------------------------
void Timer2_Init(void)
{
   CKCON &= ~0x60;                     // Timer2 uses SYSCLK/12
   TMR2CN &= ~0x01;

   TMR2RL = TIMER2_RELOAD;             // Reload value to be used in Timer2
   TMR2 = TMR2RL;                      // Init the Timer 2 counter

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
// Here we process the Timer2 interrupt and toggle the LED.
//
//-----------------------------------------------------------------------------
INTERRUPT (TIMER2_ISR, INTERRUPT_TIMER2)
{
   TF2H = 0;                           // Clear the interrupt flag

   LED1 = !LED1;                       // Toggle the LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
