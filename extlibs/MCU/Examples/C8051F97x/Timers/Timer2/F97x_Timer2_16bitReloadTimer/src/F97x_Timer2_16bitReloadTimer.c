//-----------------------------------------------------------------------------
// F97x_Timer2_16bitReloadTimer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use Timer2 as a single 16-bit timer
//
// This program presents an example of use of the Timer2 of the C8051F97x in
// 16-bit reload counter/timer mode. It uses the 'F970 TB as HW platform.
//
// The timer reload registers are initialized with a certain value so that
// the timer counts from that value up to FFFFh and when it overflows an
// interrupt is generated and the timer is automatically reloaded.
// In this interrupt the ISR toggles the LED.
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

#define TIMER_PRESCALER            12  // Based on Timer2 CKCON and TMR2CN
                                       // settings

#define LED_TOGGLE_RATE           250  // LED toggle rate in milliseconds
                                       // if LED_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000


#if (TIMER_TICKS_PER_MS * LED_TOGGLE_RATE > 65535)
  #error "Reload value is larger than 16 bits."
#endif


#define AUX1     TIMER_TICKS_PER_MS * LED_TOGGLE_RATE

#define TIMER2_RELOAD            -AUX1 // Reload value for Timer2

SBIT (LED, P0, 5);                     // LED='0' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void TIMER2_Init (void);

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
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x20;                    // Enable LED as a push-pull output

   XBR1  = XBR1_XBARE__ENABLED |       // Enable crossbar
           XBR1_WEAKPUD__PULL_UPS_ENABLED;      // with weak pullups

   SFRPAGE = SFRPAGE_save;             // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// TIMER2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures Timer2 as a 16-bit reload timer, interrupt enabled.
// Using the SYSCLK at 20MHz/8 with a 1:12 prescaler.
//
// Note: The Timer2 uses a 1:12 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void TIMER2_Init(void)
{
   U8 SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = TIMER2_PAGE;

   CKCON  = CKCON_T2ML__EXTERNAL_CLOCK |           // Timer2 uses T2XCLK
            CKCON_T2MH__EXTERNAL_CLOCK;

   TMR2CN = TMR2CN_T2XCLK__SYSCLK_DIV_12_CAP_CMP | // Use SYSCLK/12
            TMR2CN_TR2__STOP |                     // and interrupt on reload
            TMR2CN_T2SPLIT__16_BIT_RELOAD |
            TMR2CN_TF2CEN__DISABLED |
            TMR2CN_TF2LEN__DISABLED |
            TMR2CN_TF2L__NOT_SET |
            TMR2CN_TF2H__SET;

   TMR2RLL = (TIMER2_RELOAD & 0x00FF); // Reload value to be used in Timer2
   TMR2RLH = (TIMER2_RELOAD & 0xFF00) >> 8;
   TMR2L = TMR2RLL;
   TMR2H = TMR2RLH;

   TMR2CN |= TMR2CN_TR2__RUN;          // Enable Timer2 in auto-reload mode
   IE |= IE_ET2__ENABLED;              // Timer2 interrupt enabled

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
   TMR2CN &= ~TMR2CN_TF2H__BMASK;      // Reset Interrupt

   LED = !LED;                         // Toggle the LED
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
