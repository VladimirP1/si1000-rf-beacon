//-----------------------------------------------------------------------------
// F85x_Oscillator_LFOSC_Idle.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// The program demonstrates the Low Frequency Oscillator (LFOSC) and power
// savings mode operation on the F85x/86x. The MCU will sit in low power
// idle mode with only the low frequency oscillator and timer T0 running.
// When the MCU wakes up, it will switch to a 24.5 MHz clock, turn on an
// LED, and wait 100 ms.
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to the 'F85x UDP MCU board.
// 4) Run the code.
// 5) The P1.0 LED should blink once a second.
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
//    - 19 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>            // SFR declarations

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK  24500000               // SYSCLK in Hz

#define SLEEP_TIME 10000               // Sleep 1 second

#define WAIT_TIME    100               // Wait 100 ms

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init(void);
void Port_Init(void);

void Enter_Idle(U16);
void wait_ms(U16);

INTERRUPT_PROTO(T0_ISR, INTERRUPT_TIMER0);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

SBIT (LED1, SFR_P1, 0);                 // DS5 P1.0 LED

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   Oscillator_Init();
   Port_Init();

   EA = 1;                             // Set global interrupt enable

   while (1)
   {
      Enter_Idle(SLEEP_TIME);

      LED1 = !LED1;                    // Turn off the LED

      wait_ms(WAIT_TIME);
   }
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P1.0   push-pull   output   LED1
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDOUT |= 0x01;                    // Set LED1 to push-pull

   XBR2 |= 0x40;                       // Enable the crossbar
}

//-----------------------------------------------------------------------------
// Oscillator_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator as its clock source.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   CLKSEL = 0x00;                      // Set system clock to 24.5 MHz
}

//-----------------------------------------------------------------------------
// Enter_Idle ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : time - sleep time (10000 = 1 second)
//
// This function will put the MCU into idle mode using the T0 interrupt to wake
// up the MCU. In idle mode, the MCU will be running off the internal low
// frequency oscillator (LFOSC) running at 10 kHz (80 kHz / 8).
//
//-----------------------------------------------------------------------------
void Enter_Idle(unsigned int time)
{
   U8 IE_save;
   U8 EIE1_save;

   EA = 0;                             // Clear global interrupt enable

   // Save interrupt state
   IE_save = IE;                       // Preserve IE register
   EIE1_save = EIE1;                   // Preserve EIE1 register

   // Disable all interrupts
   IE = 0x00;                          // Disable interrupts
   EIE1 = 0x00;                        // Disable extended interrupts

   // Initialize T0 to wake up the MCU

   TCON &= ~0x30;                      // Stop T0 & clear flag
   TMOD &= ~0x0F;                      // Clear T0 nibble of TMOD
   TMOD |= 0x01;                       // T0 in 16-bit counter mode
   CKCON |= 0x04;                      // T0 uses SYSCLK

   // Set T0 using time
   time = -time;
   TL0 = (unsigned char)(time & 0x00FF);
   TH0 = (unsigned char)(time >> 8);

   // Comment out these lines to measure steady state idle current
   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   ET0 = 1;                            // Enable T0 interrupts
   EA = 1;                             // Enable global interrupts
   //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

   // Switch to the LFOSC
   OSCLCN &= ~0x03;                    // Set the LFOSC divider to 8
   OSCLCN |= 0x80;                     // Enable the LFOSC
   while ((OSCLCN & 0x40) != 0x40);    // Poll until the LFOSC is ready
   CLKSEL = 0x02;                      // Switch to LFOSC

   TR0 = 1;                            // Start Timer 0

   // Go into idle mode
   PCON |= 0x01;                       // Idle mode
   PCON = PCON;                        // Follow the instruction to enter idle
                                       // with a 3 cycle dummy instruction

   //--wake--//

   EIE1 = EIE1_save;                   // Restore ext interrupts
   IE = IE_save;                       // Restore interrupts
   EA = 1;                             // Set global interrupt enable
}

//-----------------------------------------------------------------------------
// wait_ms ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : time - delay time in ms
//
// This function will implement a delay using T0.  This function assumes
// SYSCLK is running from the HFOSC at 24.5 MHz.
//
//-----------------------------------------------------------------------------
void wait_ms (U16 ms)
{
   TCON &= ~0x30;                      // Stop Timer0; Clear TF0
   TMOD &= ~0x0f;                      // 16-bit free run mode
   TMOD |=  0x01;

   CKCON |= 0x04;                      // Timer0 counts SYSCLKs

   while (ms) {
      TR0 = 0;                         // Stop Timer0
      TH0 = -(SYSCLK/1000 >> 8);       // Overflow in 1ms
      TL0 = -(SYSCLK/1000);
      TF0 = 0;                         // Clear overflow indicator
      TR0 = 1;                         // Start Timer0
      while (!TF0);                    // Wait for overflow
      ms--;                            // Update ms counter
   }

   TR0 = 0;                            // Stop Timer0
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer 0 ISR
//-----------------------------------------------------------------------------

INTERRUPT(T0_ISR, INTERRUPT_TIMER0)
{
   CLKSEL = 0x00;                      // Switch back to the HFOSC

   TR0 = 0;                            // Stop Timer 0
   ET0 = 0;                            // Disable the Timer 0 interrupt
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
