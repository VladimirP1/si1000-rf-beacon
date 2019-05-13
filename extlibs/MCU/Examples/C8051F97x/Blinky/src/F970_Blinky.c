//-----------------------------------------------------------------------------
// F97x_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Typical blinking LED
//
// This program flashes LED05 on the C8051F970 target board two times
// a second using the interrupt handler for Timer2.
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

#define SYSCLK       20000000/8          // SYSCLK frequency in Hz

SBIT (LED, P0, 5);                       // LED=1 means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void TIMER2_Init (void);

INTERRUPT_PROTO (Timer2_ISR, TIMER2_IRQn);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = PCA0_PAGE;                  // Disable the watchdog timer
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;

   SYSCLK_Init ();                       // Configure system clock
   PORT_Init ();                         // Initialize crossbar and GPIO
   TIMER2_Init ();                       // Init Timer2 for ADC conversion clock

   IE_EA = 1;                            // Enable global interrupts

   while (1);                            // Spin forever
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
   U8 SFRPAGE_save = SFRPAGE;            // Save the current SFRPAGE
   SFRPAGE = LEGACY_PAGE;

   CLKSEL = CLKSEL_CLKSL__LPOSC |        // Use Low Power Int Osc
            CLKSEL_CLKDIV__SYSCLK_DIV_8; // 20MHz/8 = 2.5MHz

   SFRPAGE = SFRPAGE_save;               // Restore saved SFRPAGE
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
   U8 SFRPAGE_save = SFRPAGE;                // Save the current SFRPAGE
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x20;                          // Enable LED as a push-pull

   XBR1    = XBR1_XBARE__ENABLED |           // Enable crossbar
             XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups

   SFRPAGE = SFRPAGE_save;                   // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// TIMER2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// 4 Hz using SYSCLK/12 as its time base.
//
//-----------------------------------------------------------------------------

void TIMER2_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = TIMER2_PAGE;

   TMR2CN_TR2 = TMR2CN_TR2__STOP;      // Stop Timer2
   TMR2CN &= ~TMR2CN_T2XCLK__SYSCLK_DIV_12; // Use SYSCLK/12 as timebase

   CKCON &= ~(CKCON_T2MH__EXTERNAL_CLOCK | // Timer2 clocked based on T2XCLK
              CKCON_T2ML__EXTERNAL_CLOCK);

   TMR2RL = -(SYSCLK / 12 / 4);        // Init reload values
   TMR2 = 0xFFFF;                      // Set to reload immediately

   IE_ET2 = 1;                         // Enable Timer2 interrupts
   TMR2CN_TR2 = 1;                     // Start Timer2

   SFRPAGE = SFRPAGE_save;             // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED whenever Timer2 overflows.
//
//-----------------------------------------------------------------------------

INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                    // Clear Timer2 interrupt flag
   LED = !LED;                         // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
