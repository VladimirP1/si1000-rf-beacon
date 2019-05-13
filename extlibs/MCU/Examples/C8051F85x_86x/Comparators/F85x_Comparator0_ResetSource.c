//-----------------------------------------------------------------------------
// F85x_Comparator0_ResetSource.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Silicon Laboratories MCUs can use different mechanisms to reset the device.
//
// This software shows the necessary configuration to use Comparator0 as
// a reset source.  The code executes the initialization routines and then
// spins in an infinite while() loop.  If the S1 button (on the board) is
// pressed, then the comparators postive input (CP0+) input will drop below
// the comparator's negative input (CP0-).  When this happens, Comparator0
// will cause the device to be reset.
//
// When the code is waiting for the button to be pressed, the LED is on.
//
// Upon every device reset, the code checks the RSTSRC register to determine
// if the last reset was due to a comparator reset.  If it was a
// comparator reset, the code will blink the LED constantly.
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J27: S1 - P1.7
//    - J27: DS5 P1.0 - P1.0
//    - JP2
// 2) On the board, connect P0.0 to P1.7 using an external wire.
// 3) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 4) Compile and download code to the 'F85x UDP MCU board.
// 5) Run the code and verify that the DS5 P1.0 LED is on.
// 6) Press the S1 switch (this causes a comparator reset).
// 7) Verify that the LED is blinking.
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
//    - 16 JAN 2013
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>            // SFR declarations

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // Clock speed in Hz
#define COMPARATOR_SETTLE  10          // Settle time in us

SBIT (LED, SFR_P1, 0);                 // DS5 P1.0 LED
SBIT (S1, SFR_P1, 7);                  // S1==0 means switch depressed

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);               // Configure the system clock
void Port_Init (void);                 // Configure the Crossbar and GPIO
void Comparator0_Init (void);          // Configure Comparator0
void ResetSRC_Init(void);              // Configure Reset Sources
void Timer0_Delay (unsigned int us);   // Used as a delay

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   SYSCLK_Init();                      // Initialize the system clock
   Port_Init ();                       // Initialize crossbar and GPIO
   Comparator0_Init();                 // Initialize Comparator0

   // When checking the RSTSRC register for the source of the last reset,
   // you must first also check that the PORSF bit is also '0'.  If the PORSF
   // bit is set ('1'), then all other bits in the RSTSRC register are invalid.

   // If last reset was due to comparator reset
   if (((RSTSRC & 0x02) == 0x00) && ((RSTSRC & 0x20) == 0x20))
   {
      while (1)
      {
         Timer0_Delay (30000);         // Delay
         LED = !LED;                   // Flip LED state
      }
   }
   // Last reset was due to a power-on reset or something else
   else
   {
      ResetSRC_Init();                 // Init comparators as reset source
      LED = 0;                         // Turn LED on

      while(1){}                       // Infinite while loop waiting for
                                       // comparator reset
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator divided by 1 as its clock source.
//
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   CLKSEL = 0x00;                      // Set system clock to 24.5 MHz
}

//-----------------------------------------------------------------------------
// Port_Init
//
// Return Value : None
// Parameters   : None
//
//  This function configures the crossbar and GPIO ports.
//
// P0.0   digital   push-pull     Comparator + Input
// P0.1   digital   open-drain    Comparator - Input
// P1.0   digital   open-drain    Switch
// P1.1   digital   push-pull     LED
//-----------------------------------------------------------------------------

void Port_Init (void)
{
   P0SKIP = 0x03;                      // Skip P0.0 and P0.1 in
                                       // crossbar assignment

   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups
   P0MDIN &= ~0x03;                    // Comparator 0 inputs (P0.0 and P0.1)
                                       // are analog inputs.
   P1MDOUT |= 0x01;                    // LED is push-pull output
}

//-----------------------------------------------------------------------------
// Comparator0_Init
//
// Return Value : None
// Parameters   : None
//
//  This function configures the comparator to pins P0.0 and P0.1
//-----------------------------------------------------------------------------

void Comparator0_Init (void)
{
   CPT0CN = 0x80;                      // Comparator enabled
   CPT0MX = 0x10;                      // P0.1 = Negative Input
                                       // P0.0 = Positive Input

   Timer0_Delay (COMPARATOR_SETTLE);   // Allow CP0 output to settle for 10 us
}

//-----------------------------------------------------------------------------
// ResetSRC_Init
//
// Return Value : None
// Parameters   : None
//
// This function configures the comparator as a reset source
//-----------------------------------------------------------------------------

void ResetSRC_Init (void)
{
   RSTSRC = 0x22;                      // Comparator 0 -> reset source
}

//-----------------------------------------------------------------------------
// Timer0_Delay
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) unsigned int us - number of microseconds of delay
//                        range is full range of integer: 0 to 65335
//
// Configure Timer0 to delay <us> microseconds before returning.
//-----------------------------------------------------------------------------

void Timer0_Delay (unsigned int us)
{
   unsigned i;                         // Millisecond counter

   TCON  &= ~0x30;                     // STOP Timer0 and clear overflow flag
   TMOD  &= ~0x0f;                     // Configure Timer0 to 16-bit mode
   TMOD  |=  0x01;
   CKCON |=  0x04;                     // Timer0 counts SYSCLKs

   for (i = 0; i < us; i++)            // Count microseconds
   {
      TCON &= ~0x10;                   // STOP Timer0
      TH0 = (-SYSCLK/1000000) >> 8;    // Set Timer0 to overflow in 1us
      TL0 = -SYSCLK/1000000;
      TCON |= 0x10;                    // START Timer0
      while (TF0 == 0);                // Wait for overflow
      TCON &= ~0x20;                   // Clear overflow indicator
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
