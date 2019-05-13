//-----------------------------------------------------------------------------
// F97x_PCA0_Capture_Input.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use the PCA to time external IO events
//
// This program uses the PCA's capture mode to time events on an input pin.
// The difference between subsequent captures is calculated and stored in
// memory.
//
// In this example, PCA Module 0 is used to capture rising edges of an
// incoming digital waveform, and the crossbar is configured to receive the
// CEX0 pin on on P0.3.  The PCA is configured to clock once every 10 us.
//
// Timer2 overflows are used to toggle pin P0.1 at an overflow rate specified
// by the definition T2_RELOAD_CLOCKS. This signal at P0.1 is provided simply
// as a test method for the code.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
// 2) SELF TEST:
//     - Add a jumper from P0.1 to P0.3 (T2 Output <> PCA0 CEX0)
//    EXTERNAL SOURCE:
//     - Connect an external signal to P0.3 (PCA0 CEX0)
// 3) Place the VDD SELECT switch in the VREG position and power the board.
// 4) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 5) Add "capture_period" to the Watch List
// 6) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 7) Wait a few seconds and then pause the execution of the program.
// 8) SELF TEST:
//     - Verify that "capture_period" == 50 (500us)
//    EXTERNAL SOURCE:
//     - Verify that "capture_period" is equal to the source's period
//       in units of 10us (IE: 50 = 500us)
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 30 APR 2014


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       20000000          // SYSCLK frequency in Hz

#define T0_CLOCKS 200                  // Use 200 clocks per T0 Overflow
                                       // (200 = 10 us)
#define T2_RELOAD_CLOCKS 5000          // SYSCLKs per 1/2 cycle square wave
                                       // (200 = 10 us)

SBIT (TEST_OUT, SFR_P0, 1);            // Output for testing purposes

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void PCA0_Init (void);

void TestTimerInit (void);

INTERRUPT_PROTO (PCA0_ISR, PCA0_IRQn);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = PCA0_PAGE;
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable watchdog timer

   SYSCLK_Init ();                     // Initialize system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   PCA0_Init ();                       // Initialize PCA0

   TestTimerInit ();                   // Configure Timer2 for test signal

   IE_EA = 1;

   while (1)
   {
      // check to see if Timer2 overflowed
      if ((TMR2CN & TMR2CN_TF2H__BMASK) == TMR2CN_TF2H__SET)
      {
         TMR2CN &= ~TMR2CN_TF2H__BMASK;// Clear T2 overflow
         TEST_OUT = !TEST_OUT;         // Toggle test pin
      }
   }
}


//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure system clock to low power internal oscillator / 8
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;             // Save the current SFRPAGE
   SFRPAGE = LEGACY_PAGE;

   CLKSEL = CLKSEL_CLKSL__LPOSC |         // Use Low Power Int Osc
            CLKSEL_CLKDIV__SYSCLK_DIV_1;  // 20MHz/1 = 20MHz

   SFRPAGE = SFRPAGE_save;                // Restore saved SFRPAGE
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
// P0.1   digital   push-pull      T2 Output
// P0.3   digital   open-drain     PCA0 CEX0
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0SKIP |= 0x07;                        // Skip P0.0-2

   P0MDOUT |= 0x02;                       // P0.1 is push-pull
   P0MDOUT &= ~0x08;                      // P0.3 is open-drain


   XBR0 = XBR0_PCA0ME__CEX0;              // PCA0 CEX0 to P0.3

   XBR1 = XBR1_XBARE__ENABLED |           // Enable crossbar
          XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups

   SFRPAGE = SFRPAGE_save;
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
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = TIMER2_PAGE;

   // Configure Timer 0 for 8-bit auto-reload mode, using SYSCLK as time base
   TMOD |= TMOD_T0M__MODE2;            // 8-bit auto-reload timer
   CKCON |= CKCON_T0M__SYSCLK;         // T0 uses SYSCLK

   TH0 = (U8) -T0_CLOCKS;              // Set up reload value
   TL0 = (U8) -T0_CLOCKS;              // Set up initial value


   SFRPAGE = PCA0_PAGE;

   // Configure PCA time base; overflow interrupt disabled
   PCA0CN = PCA0CN_CR__STOP;           // Stop counter; Clear all flags
   PCA0MD = PCA0MD_CPS__T0_OVERFLOW;   // Use Timer 0 as time base

   PCA0CPM0 = PCA0CPM0_CAPP__ENABLED | // Module 0 = Rising Edge Capture Mode
              PCA0CPM0_ECCF__ENABLED;  //  enable CCF flag.


   PCA0CN_CR = 1;                      // Enable PCA0

   EIE1 |= EIE1_EPCA0__ENABLED;        // Enable PCA interrupts

   TCON_TR0 = 1;                       // Enable Timer0

   SFRPAGE = SFRPAGE_save;
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
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   CKCON |= CKCON_T2ML__SYSCLK;        // Use SYSCLK to clock Timer2

   TMR2RL = -(int)T2_RELOAD_CLOCKS;    // Set up Timer 2 reload rate
   TMR2 = -(int)T2_RELOAD_CLOCKS;      // Preload timer

   TMR2CN = TMR2CN_TR2__RUN;           // Timer 2 run in 16b auto-reload

   SFRPAGE = SFRPAGE_save;
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
INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
   static unsigned int current_capture_value, previous_capture_value;
   static unsigned int capture_period;

   if (PCA0CN_CCF0 != 0) // If Module 0 caused the interrupt
   {
      PCA0CN_CCF0 = 0;   // Clear module 0 interrupt flag.

      // Store most recent capture value
      current_capture_value = PCA0CP0;

      // Calculate capture period from last two values.
      capture_period = current_capture_value - previous_capture_value;

      // Update previous capture value with most recent info.
      previous_capture_value = current_capture_value;
   }
   else // Interrupt was caused by other bits.
   {
      PCA0CN &= ~(PCA0CN_CF__BMASK   |   // Clear other interrupt flags for PCA
                  PCA0CN_CCF2__BMASK |
                  PCA0CN_CCF1__BMASK);
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
