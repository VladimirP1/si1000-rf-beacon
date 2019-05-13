//-----------------------------------------------------------------------------
// F85x_Timer0_8bitReloadTimer.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program presents an example of use of the Timer0 in 8-bit counter/timer
// with reload mode. It uses the Timer0 to create an interrupt at a certain
// rate and toggles the LED when the interrupt counter reaches the selected
// value.
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
// Note: All time estimates are for SYSCLK freq ~ 24.5/8 MHz
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>            // SFR declarations

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000/8  // SYSCLK in Hz (24.5 MHz internal
                                       // HFOSC / 8)

#define TIMER_PRESCALER            48  // Based on Timer CKCON settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

// Note: TIMER_TICKS_PER_MS should not exceed 255 (0xFF) for the 8-bit timer

#define LED_TOGGLE_RATE           100  // LED toggle rate in milliseconds
                                       // if LED_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

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
void Timer0_Init (void);               // Timer0 initialization routine

INTERRUPT_PROTO (TIMER0_ISR, INTERRUPT_TIMER0);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   Port_Init();                        // Init Ports
   Timer0_Init();                      // Initialize Timer 0

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
// P1.0   digital  push-pull  LED1
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDOUT |= 0x01;                    // LED1 pin is push-pull

   XBR2 = 0x40;                        // Enable the crossbar
}

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Timer0 as a 8-bit timer with auto-reload and
// interrupts enabled.
//
// Note: The Timer 0 uses a 1:48 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.  This prescaler setting
// affects both Timer 0 and Timer 1.
//
//-----------------------------------------------------------------------------
void Timer0_Init(void)
{
   TH0 = -TIMER_TICKS_PER_MS;          // Initialize the Timer 0 reload
                                       // register
   TL0 = TH0;                          // Set the intial Timer 0 value

   TMOD = 0x02;                        // Timer 0 in 8-bit auto-reload mode
   CKCON = 0x02;                       // Timer 0 uses a 1:48 prescaler

   ET0 = 1;                            // Timer 0 interrupts enabled
   TCON = 0x10;                        // Start Timer 0
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer0 interrupt and toggle the LED when appropriate
//
//-----------------------------------------------------------------------------
INTERRUPT (TIMER0_ISR, INTERRUPT_TIMER0)
{
   static U16 counter = 0;

   counter++;

   if(counter == LED_TOGGLE_RATE)
   {
      LED1 = !LED1;                    // Toggle the LED
      counter = 0;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
