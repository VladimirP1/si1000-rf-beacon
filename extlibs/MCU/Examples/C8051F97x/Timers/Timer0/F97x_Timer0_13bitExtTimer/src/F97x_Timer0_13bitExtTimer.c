//-----------------------------------------------------------------------------
// F97x_Timer0_13bitExtTimer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use Timer0 as a single 13-bit timer with an external clock source
//
// This program presents an example of use of the Timer0 of the C8051F97x in
// 13-bit counter/timer in counter mode from an external pin. It uses two I/O
// pins; one to create the input pulses (SIGNAL) to be counted and another one
// to enable the counting process (GTE).
//
// This code uses the 'F970 TB as HW platform.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
//    -        P0.0 <> P0.1 (T0 with SIGNAL)
//    -        P0.2 <> P0.3 (/INT0 with BTN03)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) To enable counting, press and hold BTN03. LEd05 will blink.
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
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             20000000/8

#define PULSES_PER_TOGGLE       100    // Arbitrary number of pulses in the
                                       // input pin necessary to create an
                                       // interrupt.
                                       // Limited to 0x1FFF or 8191d for a
                                       // 13-bit timer

#define SOFTWARE_DELAY  SYSCLK/100000L // Software timer to generate the
                                       // SIGNAL output
                                       // Generate a signal in the kHz range

#define AUX0 0x1FFF-PULSES_PER_TOGGLE+1
#define AUX1 AUX0&0x001F               // 5 LSBs of timer value in TL0[4:0]
#define AUX2 ((AUX0&0x1FFF)>>5)        // High 8 bits of timer in TH0

#define TIMER0_RELOAD_HIGH      AUX2   // Reload value for Timer0 high byte
#define TIMER0_RELOAD_LOW       AUX1   // Reload value for Timer0 5 LSBs


SBIT (LED05, SFR_P0, 5);               // '0' means ON, '1' means OFF

SBIT (BTN03, SFR_P0, 3);               // Gate control signal for Timer0

SBIT (SIGNAL, SFR_P0, 1);              // Sample Toggle Signals


//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void TIMER0_Init (void);

INTERRUPT_PROTO (TIMER0_ISR, TIMER0_IRQn);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   unsigned int counter;

   SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;

   SYSCLK_Init ();                      // Initialize system clock
   PORT_Init ();                        // Initialize crossbar and GPIO
   TIMER0_Init ();                      // Init Timer2 for ADC conversion clock


   LED05 = 1;                           // Turn off LED

   IE_EA = 1;                           // Enable global interrupts

   while (1)
   {
      // Wait a certain time before toggling signal
      for(counter = 0; counter < SOFTWARE_DELAY; counter++);

      SIGNAL = !SIGNAL;                // Toggle the SIGNAL pin
   }
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
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P0.0   digital   open-drain   T0 (Timer0 External Input)
// P0.1   digital   push-pull    SIGNAL
// P0.2   digital   open-drain   /INT0
// P0.3   digital   push-pull    BTN03
// P0.5   digital   push-pull    LED05
//
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT &= ~0x01;                   // P0.0 is open-drain
   P0MDOUT |= 0x02;                    // P0.1 is push-pull
   P0MDOUT &= ~0x04;                   // P0.2 is open-drain
   P0MDOUT |= 0x08;                    // P0.3 is push-pull
   P0MDOUT |= 0x20;                    // P0.5 is push-pull


   IT01CF = IT01CF_IN0SL__P0_2 |       // /INT0 available at P0.2
            IT01CF_IN0PL__ACTIVE_HIGH; //  invert button logic

   XBR1 = XBR1_XBARE__ENABLED |        // Enable crossbar
          XBR1_WEAKPUD__PULL_UPS_ENABLED | // with weak pullups
          XBR1_T0E__ENABLED;           //  Timer0 ExtIn on P1.2

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Timer0 as a 13-bit timer, interrupt enabled.
// Using an external signal as clock source 1:8 and reloading the
// TH0 and TL0 registers it will interrupt and then toggle the LED upon roll-
// over of the timer.
//
// Note: In this example the GATE0 gate control is used.
//-----------------------------------------------------------------------------
void TIMER0_Init(void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = TIMER01_PAGE;

   TMOD = TMOD_T0M__MODE0 |            // Timer0 in 13-bit mode
          TMOD_GATE0__ENABLED |        //  gate using /INT0
          TMOD_CT0__COUNTER;           //  counter mode (uses T0)

   IE_ET0 = 1;                         // Timer0 interrupt enabled

   TCON = TCON_TR0__RUN |              // Timer0 ON with INT0 edge active
          TCON_IT0__EDGE;

   TH0 = TIMER0_RELOAD_HIGH;           // Reinit Timer0 High register
   TL0 = TIMER0_RELOAD_LOW;            // Reinit Timer0 Low register

   SFRPAGE = SFRPAGE_save;
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
   TH0 = TIMER0_RELOAD_HIGH;           // Reinit Timer0 High register
   TL0 = TIMER0_RELOAD_LOW;            // Reinit Timer0 Low register

   LED05 = !LED05;                     // Toggle LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
