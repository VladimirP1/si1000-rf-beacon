//-----------------------------------------------------------------------------
// F85x_PCA0_8Bit_Center_PWM_Output.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program sends two center-aligned PWM waveforms out of two I/O pins
// using the PCA's 8-bit Center-Aligned PWM Output Mode.  The duty cycles of
// the waveforms are modified periodically within the main loop, and the
// waveforms are increasing/decreasing opposite of each other.
//
// In this example, PCA Modules 0 and 1 are used to generate the waveforms, and
// the crossbar is configured to send the CEX0 signal out on P0.0 and the CEX1
// signal out on P0.1.
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - JP2
// 2) Place oscilloscope probes on P0.0 and P0.1.
// 3) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 4) Compile and download code to the 'F85x UDP MCU board.
// 5) Run the code - the waveforms should be visible on the oscilloscope.
// 6) Verify that the duty cycle of the waveforms vary smoothly between
//    minimum and maximum values.
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
//    - 15 JAN 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>            // SFR declaration

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // Internal oscillator frequency in Hz

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void Port_Init (void);
void PCA0_Init (void);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   unsigned int delay_count;           // Used to implement a delay
   bit duty_direction0 = 0;            // Module 0: 0 = Decrease; 1 = Increase
   bit duty_direction1 = 0;            // Module 1: 0 = Increase; 1 = Decrease

   // Disable watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   Port_Init ();                       // Initialize crossbar and GPIO
   SYSCLK_Init ();                     // Initialize oscillator
   PCA0_Init ();                       // Initialize PCA0

   while (1)
   {
      // Wait a little while
      for (delay_count = 30000; delay_count > 0; delay_count--);

      // Module 0
      if (duty_direction0 == 1)        // Direction = Increase
      {
         // First, check the ECOM0 bit
         if ((PCA0CPM0 & 0x40) == 0x00)
         {
            PCA0CPM0 |= 0x40;          // Set ECOM0 if it is '0'
         }
         else                          // Increase duty cycle otherwise
         {
            PCA0CPH0--;                // Increase duty cycle

            if (PCA0CPH0 == 0x00)
            {
               duty_direction0 = 0;    // Change direction for next time
            }
         }
      }
      else                             // Direction = Decrease
      {
         if (PCA0CPH0 == 0xFF)
         {
            PCA0CPM0 &= ~0x40;         // Clear ECOM0
            duty_direction0 = 1;       // Change direction for next time
         }
         else
         {
            PCA0CPH0++;                // Decrease duty cycle
         }
      }

      // Module 1
      if (duty_direction1 == 1)        // Direction = Decrease
      {
         // First, check the ECOM1 bit
         if ((PCA0CPM1 & 0x40) == 0x00)
         {
            PCA0CPM1 |= 0x40;          // Set ECOM1 if it is '0'
         }
         else                          // Increase duty cycle otherwise
         {
            PCA0CPH1++;                // Decrease duty cycle

            if (PCA0CPH1 == 0xFF)
            {
               duty_direction1 = 0;    // Change direction for next time
            }
         }
      }
      else                             // Direction = Increase
      {
         if (PCA0CPH1 == 0x00)
         {
            PCA0CPM1 &= ~0x40;         // Clear ECOM1
            duty_direction1 = 1;       // Change direction for next time
         }
         else
         {
            PCA0CPH1--;                // Decrease duty cycle
         }
      }
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
// oscillator divided by 1 as its clock source.  Also enables missing clock
// detector reset.
//
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   CLKSEL = 0x00;                      // Set system clock to 24.5 MHz

   RSTSRC = 0x06;                      // Enable missing clock detector
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
// P0.0   digital   push-pull     PCA0 CEX0
// P0.1   digital   push-pull     PCA0 CEX1
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   XBR0 = 0x00;
   XBR1 = 0x02;                        // Route CEX0 to P0.0 and CEX1 to P0.1
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups

   P0MDOUT |= 0x03;                    // Set CEX0/CEX1 (P0.0/P0.1) to
                                       // push-pull
}

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base, and sets up 8-bit center-aligned
// PWM output mode for Module 0 (CEX0 signal) and Module 1 (CEX1 signal).
//
// The frequency of the PWM signal generated at the CEX0/CEX1 pins is equal to
// the PCA main timebase frequency divided by 256.
//
// The PCA time base in this example is configured to use SYSCLK, and SYSCLK
// is set up to use the internal oscillator running at 24.5 MHz.  Therefore,
// the frequency of the PWM signal will be 24.5 MHz / 256 = 95.7 kHz.
// Using different PCA clock sources or a different processor clock will
// result in a different frequency for the PWM signal.
//
//    -------------------------------------------------------------------------
//    How "8-Bit PWM Mode" Works:
//
//    The PCA's 8-bit PWM Mode works by setting an output pin low every
//    time the main PCA counter low byte (PCA0L) overflows, and then setting
//    the pin high whenever a specific match condition is met.
//
//    Upon a PCA0L overflow (PCA0L incrementing from 0xFF to 0x00), two things
//    happen:
//
//    1) The CEXn pin will be set low.
//    2) The contents of the PCA0CPHn register for the module are copied into
//       the PCA0CPLn register for the module.
//
//    When the PCA0L register increments and matches the PCA0CPLn register for
//    the selected module, the CEXn pin will be set high, except when the
//    ECOMn bit in PCA0CPMn is cleared to '0'.  By varying the value of the
//    PCA0CPHn register, the duty cycle of the waveform can also be varied.
//
//    When ECOMn = '1', the duty cycle of the PWM waveform is:
//
//       8-bit PWM Duty Cycle = (256 - PCA0CPLn) / 256
//
//    To set the duty cycle to 100%, a value of 0x00 should be loaded into the
//    PCA0CPHn register for the module being used (with ECOMn set to '1').
//    When the value of PCA0CPLn is equal to 0x00, the pin will never be
//    set low.
//
//    To set the duty cycle to 0%, the ECOMn bit in the PCA0CPMn register
//    should be cleared to 0.  This prevents the PCA0CPLn match from occuring,
//    which results in the pin never being set high.
//    -------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   // Configure PCA time base; overflow interrupt disabled
   PCA0CN = 0x00;                      // Stop counter; clear all flags
   PCA0MD = 0x08;                      // Use SYSCLK as time base

   PCA0PWM = 0x00;                     // Target PCA0CPH/L registers,
                                       // disable cycle overflows, select
                                       // 8-bit PWM mode

   PCA0CLR = 0x00;                     // Comparator clears disabled

   PCA0CENT = 0x03;                    // Modules 0 and 1 center-aligned

   PCA0CPM0 = 0x42;                    // Module 0 = 8-bit PWM mode
   PCA0CPM1 = 0x42;                    // Module 1 = 8-bit PWM mode

   // Configure initial PWM duty cycle = 50%
   PCA0CPH0 = 256 - (256 * 0.5);
   PCA0CPH1 = 256 - (256 * 0.5);

   // Start PCA counter
   CR = 1;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------

