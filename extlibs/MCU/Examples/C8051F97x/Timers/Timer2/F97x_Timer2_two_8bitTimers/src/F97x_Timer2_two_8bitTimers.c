//-----------------------------------------------------------------------------
// F97x_Timer2_two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use Timer2 as two 8-bit timers
//
// This program presents an example of use of the Timer2 of the C8051F97x's in
// two 8-bit reload counter/timer mode. It uses the 'F970 TB as HW platform.
//
// It sets Timer2 to be two 8-bit timers that interrupt every millisecond.
//
// When the Timer2 low interrupts a counter (low_counter) increments and upon
// reaching the value defined by LED_TOGGLE_RATE toggles the LED
// When the Timer2 high interrupts a counter (high_counter) increments and upon
// reaching the value defined by SIGNAL_TOGGLE_RATE toggles the SIGNAL pin
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) LED05 and LED15 will blink.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 2 MAY 2014

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK             20000000/8  // SYSCLK frequency in Hz

#define TIMER_PRESCALER            12  // Based on Timer2 CKCON and TMR2CN
                                       // settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

#define TIMER2_RELOAD      -TIMER_TICKS_PER_MS // Reload value for Timer2

#define TIMER2_RELOAD_HIGH  TIMER2_RELOAD  // Reload value for Timer2 high byte
#define TIMER2_RELOAD_LOW   TIMER2_RELOAD  // Reload value for Timer2 low byte

#define LED15_PERIOD_MS            200
#define LED05_PERIOD_MS            333


SBIT (LED15, P1, 5);                     // LED='0' means ON
SBIT (LED05, P0, 5);                     // LED='0' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void TIMER2_Init (void);

INTERRUPT_PROTO (TIMER2_ISR, TIMER2_IRQn);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = PCA0_PAGE;                // Disable the watchdog timer
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;

   SYSCLK_Init ();                     // Configure system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   TIMER2_Init ();                     // Initialize TIMER2

   IE_EA = 1;                          // Enable global interrupts

   while (1);
}


//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// This routine initializes the system clock to use the internal 20 MHz
// oscillator as its clock source divided by 8.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;             // Save the current SFRPAGE
   SFRPAGE = LEGACY_PAGE;

   CLKSEL = CLKSEL_CLKSL__LPOSC |         // Use Low Power Int Osc
            CLKSEL_CLKDIV__SYSCLK_DIV_8;  // 20MHz/8 = 2.5MHz

   SFRPAGE = SFRPAGE_save;                // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the Crossbar and GPIO ports.
//
// P0.5 - digital  push-pull   LED05
// P1.5 - digital  push-pull   LED15
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x20;                    // Enable LED as a push-pull output

   P1MDOUT |= 0x20;                    // Enable LED as a push-pull output

   XBR1  = XBR1_XBARE__ENABLED |       // Enable crossbar
           XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups

   SFRPAGE = SFRPAGE_save;             // Restore saved SFRPAGE
}


//-----------------------------------------------------------------------------
// TIMER2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures Timer2 as two 8-bit reload timers with interrupt.
// Using the internal osc. at 20MHz/8 with a timer prescaler of 1:12
// reloading the TMR2H register with TIMER2_RELOAD_HIGH and TMR2L with
// TIMER2_RELOAD_HIGH.
//
// Note: The Timer2 uses a 1:12 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void TIMER2_Init(void)
{
   U8 SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = TIMER2_PAGE;

   CKCON = CKCON_T2ML__EXTERNAL_CLOCK | // Timer2 uses T2XCLK
           CKCON_T2MH__EXTERNAL_CLOCK;

   TMR2CN = TMR2CN_T2XCLK__SYSCLK_DIV_12_CAP_CMP | // Use SYSCLK/12
            TMR2CN_TR2__STOP |                     // and low byte interupt
            TMR2CN_T2SPLIT__8_BIT_RELOAD |
            TMR2CN_TF2CEN__DISABLED |
            TMR2CN_TF2LEN__ENABLED |
            TMR2CN_TF2L__NOT_SET |
            TMR2CN_TF2H__NOT_SET;

   TMR2RLL = TIMER2_RELOAD_LOW;        // Reload value to be used in Timer2
   TMR2RLH = TIMER2_RELOAD_HIGH;
   TMR2L = TIMER2_RELOAD_LOW;
   TMR2H = TIMER2_RELOAD_HIGH;

   TMR2CN |= TMR2CN_TR2__RUN;          // Enable Timer2 in auto-reload mode
   IE_ET2 = 1;                         // Timer2 interrupt enabled

   SFRPAGE = SFRPAGE_save;             // Restore saved SFRPAGE
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer2 interrupt and toggle the LED
//
//-----------------------------------------------------------------------------
INTERRUPT (TIMER2_ISR, TIMER2_IRQn)
{
   static unsigned int low_ctr = 0;  // Define counter variables
   static unsigned int high_ctr = 0;

   if(TMR2CN & TMR2CN_TF2H__BMASK)
   {
      if((low_ctr++) == LED15_PERIOD_MS)
      {
         low_ctr = 0;                 // Reset interrupt counter

         LED15 = !LED15;              // Toggle LED15
      }

      TMR2CN &= ~TMR2CN_TF2L__BMASK;
   }

   if(TMR2CN & TMR2CN_TF2H__BMASK)
   {
      if((high_ctr++) == LED05_PERIOD_MS)
      {
         high_ctr = 0;                // Reset interrupt counter

         LED05 = !LED05;              // Toggle LED05
      }

      TMR2CN &= ~TMR2CN_TF2H__BMASK;
   }
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
