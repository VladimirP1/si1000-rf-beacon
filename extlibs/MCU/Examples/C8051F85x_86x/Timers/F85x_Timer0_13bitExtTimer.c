//-----------------------------------------------------------------------------
// F85x_Timer0_13bitExtTimer.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program presents an example of use of the Timer0 in 13-bit
// counter/timer mode from an external pin. It uses two I/O pins; one to create
// the input pulses (SIGNAL) to be counted and another one to enable the
// counting process (GATE).
//
// Pinout:
//
//    P0.0 -> T0 (Timer0 External Input)
//    P0.1 -> /INT0
//
//    P1.0 -> LED (digital, push-pull)
//
//    P1.4 -> SIGNAL (digital, push-pull)
//    P1.5 -> GATE (digital, push-pull)
//
//    P1.7 -> S1 (switch)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - J27: DS6 P1.1 - P1.1/ADC0.9
//    - J27: S1 - P1.7
//    - J27: S2 - P2.1
//    - JP2
// 2) Externally connect the following pins:
//
//                      P0.0 (T0) <--> P1.4 (SIGNAL)
//                   P0.1 (/INT0) <--> P1.5 (GATE)
//
// 3) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 4) Modify PULSES_PER_TOGGLE to change the number of input pulses/interrupt.
// 5) Modify SOFTWARE_DELAY to change the speed of the SIGNAL waveform.
// 6) Compile and download code to the 'F85x UDP MCU board.
// 7) Run the code.
// 8) To enable the counting, press and hold S1 (switch). Firmare will set or
//    clear the state of the GATE signal based on the switch value.
// 9) The DS5 P1.0 LED will blink and SIGNAL can be observed on an
//    oscilloscope.
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

#define PULSES_PER_TOGGLE          100 // Arbitrary number of pulses in the
                                       // input pin necessary to create an
                                       // interrupt.
                                       // Limited to 0x1FFF or 8191d for a
                                       // 13-bit timer

#define SOFTWARE_DELAY (SYSCLK/100000L)// Software timer to generate the
                                       // SIGNAL output
                                       // Generate a signal in the kHz range

// These are defined here so the compiler does the work, not the MCU core

#define AUX0 0x1FFF - PULSES_PER_TOGGLE + 1
#define AUX1             AUX0 & 0x001F // 5 LSBs of timer value in TL0[4:0]
#define AUX2    ((AUX0 & 0x1FFF) >> 5) // High 8 bits of timer in TH0

#define TIMER0_RELOAD_HIGH        AUX2 // Reload value for Timer0 high byte
#define TIMER0_RELOAD_LOW         AUX1 // Reload value for Timer0 5 LSBs

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SBIT(LED1, SFR_P1, 0);                 // DS5 P1.0 LED

#define LED_ON                       0
#define LED_OFF                      1

SBIT(SIGNAL, SFR_P1, 4);               // SIGNAL is used to input pulses into
                                       // T0 pin
SBIT(GATE, SFR_P1, 5);                 // Gate control signal for Timer0

SBIT(S1, SFR_P1, 7);                   // Switch that enables counting

#define SW_PRESSED                   0
#define SW_NOT_PRESSED               1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Port_Init(void);                  // Port initialization routine
void Timer0_Init(void);                // Timer0 initialization routine

INTERRUPT_PROTO (TIMER0_ISR, INTERRUPT_TIMER0);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   volatile U16 counter = 0;

   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   Timer0_Init ();                     // Initialize the Timer0
   Port_Init ();                       // Init Ports

   LED1 = LED_OFF;

   EA = 1;                             // Enable global interrupts

   while (1)
   {
      if (S1 == SW_PRESSED)
      {
         GATE = 1;                     // Enable counting
      }
      else
      {
         GATE = 0;                     // Disable counting
      }

      // Wait a certain time before toggling signal
      for(counter = 0; counter < SOFTWARE_DELAY; counter++);

      SIGNAL = !SIGNAL;                // Toggle the SIGNAL pin
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
// This function configures the crossbar and GPIO ports.
//
// P0.0   digital   open-drain   T0 (Timer0 External Input)
// P0.1   digital   open-drain   /INT0
//
// P1.0   digital   push-pull    LED1
//
// P1.4   digital   push-pull    SIGNAL
// P1.5   digital   push-pull    GATE
//
// P1.7   digital   open-drain   S1 (Switch 1)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDOUT |= 0x31;                    // Set P1.0, P1.4, and P1.5 to push-pull

   IT01CF = 0x01;                      // /INT0 available at P0.1

   XBR1 = 0x08;                        // T0 enabled on the crossbar
   XBR2 = 0x40;                        // Enable crossbar
}

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Timer0 as a 13-bit timer, interrupts enabled,
// using an external signal T0 as a clock source.
//
// Note: The GATE0 gate control is used in this example.
//
//-----------------------------------------------------------------------------
void Timer0_Init(void)
{
   TMOD = 0x0C;                        // Timer 0 in 13-bit mode ext. counter
                                       // gated counting T0 input

   TH0 = TIMER0_RELOAD_HIGH;           // Reinit Timer0 High register
   TL0 = TIMER0_RELOAD_LOW;            // Reinit Timer0 Low register

   TCON = 0x11;                        // Timer 0 ON with INT0 edge active

   ET0 = 1;                            // Timer 0 interrupt enabled
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
INTERRUPT (TIMER0_ISR, INTERRUPT_TIMER0)
{
   LED1 = !LED1;                       // Toggle the LED

   TH0 = TIMER0_RELOAD_HIGH;           // Reinit Timer0 High register
   TL0 = TIMER0_RELOAD_LOW;            // Reinit Timer0 Low register
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
