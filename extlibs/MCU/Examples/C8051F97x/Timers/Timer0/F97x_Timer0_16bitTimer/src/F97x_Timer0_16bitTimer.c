//-----------------------------------------------------------------------------
// F97x_Timer0_16bitTimer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use Timer0 as a single 16-bit timer
//
// This program presents an example of use of the Timer0 of the C8051F97x in
// 16-bit counter/timer mode. It uses the 'F970 TB as the HW platform.
// In this example the LED is toggled at a rate defined by the
// LED05_TOGGLE_RATE constant.(in milliseconds)
//
// The timer is set with this values and counts until an overflow when it
// generates an interrupt. This interrupt reloads the timer values and toggles
// the LED.
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
// 5) LED05 will blink.
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

#define LED05_PERIOD_MS           200

#define TIMER_PRESCALER            48  // Based on Timer0 CKCON settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

 // Reload value for Timer0
#define TIMER0_RELOAD      -(TIMER_TICKS_PER_MS * LED05_PERIOD_MS)

#if (TIMER_TICKS_PER_MS * LED05_PERIOD_MS > 65535)
  #error "TIMER_TICKS_PER_MS * LED05_PERIOD_MS is over 65535"
#endif

#define TIMER0_RELOAD_LOW   (TIMER0_RELOAD & 0x00FF)  // Reload value for Timer0 low byte
#define TIMER0_RELOAD_HIGH  (TIMER0_RELOAD & 0xFF00) >> 8  // Reload value for Timer0 high byte


SBIT (LED05, P0, 5);                     // LED='0' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void TIMER0_Init (void);

INTERRUPT_PROTO (TIMER0_ISR, TIMER0_IRQn);

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
// P0.5 - digital  push-pull   LED
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x20;                    // Enable LED as a push-pull output

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
// This function configures the Timer0 as a 16-bit timer, interrupt enabled.
// Using the internal osc. at 2.5MHz with a prescaler of 1:8 and reloading the
// T0 registers with TIMER0_RELOAD_HIGH/LOW it will interrupt and then toggle
// the LED.
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

   TMOD = TMOD_T0M__MODE1 |            // Timer0 in 16-bit mode
          TMOD_CT0__TIMER;

   TH0 = TIMER0_RELOAD_HIGH;           // Timer0 reload values
   TL0 = TIMER0_RELOAD_LOW;

   TCON |= TCON_TR0__RUN;              // Enable Timer0

   IE |= IE_ET0__ENABLED;              // Timer0 interrupts enabled

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
   TH0 = TIMER0_RELOAD_HIGH;           // Reinitialize Timer0 reload values
   TL0 = TIMER0_RELOAD_LOW;

   LED05 = !LED05;                     // Toggle LED05
 }

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
