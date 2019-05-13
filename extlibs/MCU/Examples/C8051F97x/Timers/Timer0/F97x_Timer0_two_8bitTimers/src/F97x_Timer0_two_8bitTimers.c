//-----------------------------------------------------------------------------
// F97x_Timer0_two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use Timer0 as two 8-bit timers
//
// This program presents an example of use of the Timer0 of the C8051F97x in
// two 8-bit reload counter/timer mode. It uses the 'F970 TB as HW platform.
//
// It sets the two timers to interrupt every millisecond.
//
// When the timer low interrupts, a counter (low_counter) increments and upon
// reaching the value defined by LED15_TOGGLE_RATE toggles LED15
// When the timer high interrupts a counter (high_counter) increments and upon
// reaching the value defined by LED05_PERIOD_MS toggles LED05
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

#define TIMER_PRESCALER            48  // Based on Timer0 CKCON settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

#if (TIMER_TICKS_PER_MS > 255)
  #error "TIMER_TICKS_PER_MS is over 255"
#endif

#define TIMER0_RELOAD      -TIMER_TICKS_PER_MS // Reload value for Timer2

#define TIMER0_RELOAD_LOW   TIMER0_RELOAD  // Reload value for Timer0 low byte
#define TIMER0_RELOAD_HIGH  TIMER0_RELOAD  // Reload value for Timer0 high byte

#define LED15_PERIOD_MS            200
#define LED05_PERIOD_MS            500


SBIT (LED15, P1, 5);                     // LED='0' means ON
SBIT (LED05, P0, 5);                     // LED='0' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void TIMER0_Init (void);

INTERRUPT_PROTO (TIMER0_ISR, TIMER0_IRQn);
INTERRUPT_PROTO (TIMER1_ISR, TIMER1_IRQn);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = PCA0_PAGE;                // Disable the watchdog timer
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;

   SYSCLK_Init ();                     // Configure system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   TIMER0_Init ();                     // Initialize TIMER0

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
//
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
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Timer0 as two 8-bit timers, interrupt enabled.
// Using the internal osc. at 12MHz with a prescaler of 1:8 and reloading the
// TH0 register with TIMER0_RELOAD_HIGH and TL0 with TIMER0_RELOAD_HIGH.
//
// Note: The Timer0 uses a 1:48 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void TIMER0_Init(void)
{
   U8 SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = TIMER01_PAGE;

   CKCON &= ~(CKCON_SCA__FMASK |       // Timer0 clock = SYSCLK/48
              CKCON_T0M__BMASK);
   CKCON |= CKCON_SCA__SYSCLK_DIV_48 |
            CKCON_T0M__PRESCALE;

   TMOD = TMOD_T0M__MODE3 |            // Timer0 in dual 8-bit mode
          TMOD_CT0__TIMER;

   TL0 = TIMER0_RELOAD_LOW;            // Timer0/1 reload values
   TH0 = TIMER0_RELOAD_HIGH;

   TCON |= TCON_TR0__RUN |             // Enable Timer0/1
           TCON_TR1__RUN;

   IE |= IE_ET0__ENABLED |             // Timer0/1 interrupts enabled
         IE_ET1__ENABLED;

   SFRPAGE = SFRPAGE_save;             // Restore saved SFRPAGE
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
INTERRUPT (TIMER0_ISR, TIMER0_IRQn)
{
   static unsigned int low_ctr = 0;  // Define counter variables

   TL0 = TIMER0_RELOAD_LOW;          // Reinitialize Timer0 Reload value

   if((low_ctr++) == LED15_PERIOD_MS)
   {
      low_ctr = 0;                   // Reset interrupt counter

      LED15 = !LED15;                // Toggle LED15
   }
}

//-----------------------------------------------------------------------------
// Timer1_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer0 interrupt and toggle the LED
//
//-----------------------------------------------------------------------------
INTERRUPT (TIMER1_ISR, TIMER1_IRQn)
{
   static unsigned int high_ctr = 0; // Define counter variables

   TH0 = TIMER0_RELOAD_HIGH;         // Reinitialize Timer1 Reload value

   if((high_ctr++) == LED05_PERIOD_MS)
   {
      high_ctr = 0;                 // Reset interrupt counter

      LED05 = !LED05;               // Toggle LED05
   }
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
