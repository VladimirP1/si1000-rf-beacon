//-----------------------------------------------------------------------------
// Si102x_PCA0_Capture_Input.c
//-----------------------------------------------------------------------------
// Copyright 2011 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program uses the PCA's capture mode to time events on an input pin.
// The difference between subsequent captures is calculated and stored in
// memory.
//
// In this example, PCA Module 0 is used to capture rising edges of an
// incoming digital waveform, and the crossbar is configured to receive the
// CEX0 pin on on P0.0.  The PCA is configured to clock once every 10 us.
//
// Timer2 overflows are used to toggle pin P0.1 at an overflow rate specified
// by the definition T2_RELOAD_CLOCKS. This signal at P0.1 is provided simply
// as a test method for the code.
//
// How To Test:
//
// 1) Download code to a target device with P0.0 and P0.1 tied together.
// 2) Run the program for a few seconds, and then halt.
// 3) Verify that the variable "capture_period" is equal to 50 (=500us).
//
// Alternately:
// 1) Run a separate signal generator into the P0.0 pin.
// 2) Run the program for a few seconds, and then halt.
// 3) Verify that the variable "capture_period" matches the expected time
//    between rising edges (in units of 10us).
//
//
// Target:         Si102x/3x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (MRF)
//    - 27 OCTOBER 2011
//


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <Si1020_defs.h>               // SFR declarations

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // Internal oscillator frequency in Hz

#define T0_CLOCKS 245                  // Use 245 clocks per T0 Overflow
                                       //    (245 = 10 us)
#define T2_RELOAD_CLOCKS 6125          // SYSCLKs per 1/2 cycle square wave
                                       //    (245 = 10 us)

SBIT (TEST_OUT, SFR_P0, 1);            // Output for testing purposes

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
LOCATED_VARIABLE_NO_INIT (reserved, U8, SEG_XDATA, 0x0000);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void PCA0_Init (void);
void TestTimerInit (void);
INTERRUPT_PROTO (PCA0_ISR, INTERRUPT_PCA0);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   PCA0MD = 0x00;                      // Disable watchdog timer

   PORT_Init ();                       // Initialize crossbar and GPIO
   OSCILLATOR_Init ();                 // Initialize oscillator
   PCA0_Init ();                       // Initialize PCA0

   TestTimerInit ();                   // Configure Timer2 for test signal

   EA = 1;

   SFRPAGE = LEGACY_PAGE;

   while (1)
   {
      // check to see if Timer2 overflowed
      if (TF2H)
      {
         TEST_OUT = !TEST_OUT;         // Toggle test pin
         TF2H = 0;                     // Clear T2 overflow
      }

   };
}


//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal precision
// oscillator.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   SFRPAGE = CONFIG_PAGE;
   FLSCL     = 0x40;                   // Set BYPASS bit for >12.5 MHz.

   SFRPAGE = LEGACY_PAGE;
   REG0CN |= 0x10;                     // Enable the precision osc. bias
   OSCICN |= 0x80;                     // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P0.0   digital   open-drain     PCA0 CEX0
// P0.1   digital   push-pull
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   SFRPAGE = LEGACY_PAGE;
   P0MDOUT |= 0x02;                    // Set TEST_OUT (P0.1) to push-pull

   XBR0    = 0x00;
   XBR1    = 0x01;                     // Route CEX0 to P0.0,
   XBR2    = 0x40;                     // Enable crossbar and weak pull-ups

}

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base, and sets up capture mode for
// Module 0 (CEX0 pin).
//
// The PCA will capture rising edges on the CEX0 pin, and interrupt the
// processor.  Software subtracts the previous capture value from the most
// recent caputre value to set the "captured_clocks" variable.
//
// The PCA time base in this example is configured to use Timer 0, overflowing
// every 10 us. The PCA clock rate will determine the maximum time between
// edges that can be reliably detected (this is equal to 65535 * the PCA clock).
//
// The time range that can be measured using this example is 10us (MIN) to
// 655.350 ms (MAX) when the clocks are set up as they are in this example.
// The precision is limited to the PCA clock rate (in this example, 10 us).
// Using different PCA clock sources or a different processor clock will allow
// for different time capture ranges or levels of precision.
//
//    -------------------------------------------------------------------------
//    How "Capture Mode" Works:
//
//    The PCA's Capture Mode works by making a copy of the current PCA timer
//    value (PCA0) into the capture/compare register for the module (PCA0CPn).
//    The module can be configured to capture rising edges, falling edges, or
//    both rising and falling edges.  When a capture occurs, the CCFn flag is
//    set, and an interrupt is (optionally) generated by the PCA module.
//
//    To detect an edge (and not reject it as noise) the new logic level must
//    be present at the pin for at least two SYSCLK cycles.
//
//    With a constantly-running PCA timer, the time between two edges on a
//    signal can be accurately measured to within 1 PCA clock cycle.
//    Multiple PCA channels can be used in capture mode to measure timing
//    between different signals.
//    -------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   SFRPAGE = LEGACY_PAGE;

   // Configure Timer 0 for 8-bit auto-reload mode, using SYSCLK as time base
   TMOD &= 0xF0;                       // Clear all T0 control bits
   TMOD |= 0x02;                       // 8-bit auto-reload timer
   CKCON |= 0x04;                      // T0 uses SYSCLK
   TH0 = (unsigned char) -T0_CLOCKS;   // Set up reload value
   TL0 = (unsigned char) -T0_CLOCKS;   // Set up initial value

   // Configure PCA time base; overflow interrupt disabled
   PCA0CN = 0x00;                      // Stop counter; clear all flags
   PCA0MD = 0x04;                      // Use Timer 0 as time base


   PCA0CPM0 = 0x21;                    // Module 0 = Rising Edge Capture Mode
                                       // enable CCF flag.


   EIE1 |= 0x10;                       // Enable PCA interrupts

   // Start PCA counter
   CR = 1;
   TR0 = 1;                            // Start Timer 0
}


//-----------------------------------------------------------------------------
// TestTimerInit
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine simply sets up Timer 2 in 16-bit auto-reload mode, to be used
// as a rudimentary source for testing
//
// The T2_RELOAD_CLOCKS value can be set up to generate multiples of 10us for
// testing, by using the formula:
//
// T2_RELOAD_CLOCKS = 245 * (Time_between_rising_edges / 10us)/2
//
//-----------------------------------------------------------------------------
void TestTimerInit (void)
{
   SFRPAGE = LEGACY_PAGE;

   CKCON |= 0x10;                      // Use SYSCLK to clock Timer2

   TMR2RL = -(int)T2_RELOAD_CLOCKS;    // Set up Timer 2 reload rate
   TMR2 = -(int)T2_RELOAD_CLOCKS;      // Preload timer

   TMR2CN = 0x04;                      // Timer 2 run in 16b auto-reload

}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This is the ISR for the PCA.  It handles the case when a capture occurs on
// channel 0, and updates the variables to reflect the new capture information.
//
//-----------------------------------------------------------------------------
INTERRUPT(PCA0_ISR, INTERRUPT_PCA0)
{
   static unsigned int current_capture_value, previous_capture_value;
   static unsigned int capture_period;

   SFRPAGE = LEGACY_PAGE;

   if (CCF0)                           // If Module 0 caused the interrupt
   {
      CCF0 = 0;                        // Clear module 0 interrupt flag.

      // Store most recent capture value
      current_capture_value = PCA0CP0;

      // Calculate capture period from last two values.
      capture_period = current_capture_value - previous_capture_value;

      // Update previous capture value with most recent info.
      previous_capture_value = current_capture_value;

   }
   else                                // Interrupt was caused by other bits.
   {
      PCA0CN &= ~0x86;                 // Clear other interrupt flags for PCA
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------