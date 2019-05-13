//-----------------------------------------------------------------------------
// F750_Ports_PortMatch.c
//-----------------------------------------------------------------------------
// Copyright 2010 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Wake up MCU from IDLE when Switches are pressed 
//
// This program demonstrates how to use the Port Match feature.  The firmware
// enables the port match interrupt and puts the device in idle mode.  Pressing
// a switch wakes the device up from idle mode and turns on an LED in the Port
// Match ISR.  Releasing the switch turns off the LED and the re-enters idle
// mode.
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
// 5) Push the button BTN03 and see that the corresponding
//    LED05 turns on.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 02 MAY 2014

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       20000000/8    // SYSCLK frequency in Hz

SBIT (LED05,   SFR_P0, 5);         // '0' means ON, '1' means OFF
SBIT (BTN03,   SFR_P0, 3);         // '0' means ON, '1' means OFF

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void PORT_Match_Init (void);

INTERRUPT_PROTO(PORT_Match_ISR, PMATCH_IRQn);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = PCA0_PAGE;                // Disable the watchdog timer
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;

   SYSCLK_Init ();                     // Initialize system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   PORT_Match_Init ();                 // Enable the Port Match interrupt

   IE_EA = 1;                          // Enable global interrupts

   while (1)
   {
      LED05 = 1;                       // Turn off LED

      PCON |= PCON_IDLE__IDLE;         // Enter Idle mode
      NOP();                           // Safety dummy instruction

      // Pressing the switch pulls P0.3 low which triggers the Port Match
      // interrupt.  When the Port Match interrupt occurs, the device wakes up
      // from Idle mode and vectors to the Port_Match_ISR

      // Port Match is a level sensitive interrupt, so the firmware waits for
      // the switch to be released in the ISR before proceeding.

      // Once the ISR is finished, code execution resumes after the instruction
      // that put the device in idle mode
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
   CLKSEL = CLKSEL_CLKSL__LPOSC |         // Use Low Power Int Osc
            CLKSEL_CLKDIV__SYSCLK_DIV_8;  // 20MHz/8 = 2.5MHz
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P0.3   digital   open-drain    BTN03
// P0.5   digital   push-pull     LED05
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x10;                          // P0.5 is push-pull

   P0 |= 0x08;                               // Set P0.3 latch to '1'


   XBR1    = XBR1_XBARE__ENABLED |           // Enable crossbar
             XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PORT_Match_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Enable the Port Match interrupt for P1.1.  Whenever P1.1 is logic low, the
// Port Match interrupt is active.
//
//-----------------------------------------------------------------------------

void PORT_Match_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0MASK = P0MASK_B3__COMPARED;       // Set P0.3 as the pin to monitor


   SFRPAGE = LEGACY_PAGE;
   P0MAT = P0MAT_B3__LOW;              // Interrupt when P0.3 is low


   EIE2 |= EIE2_EMAT__ENABLED;         // Enable Port Match Interrupts

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Interrupt Service Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Match_ISR
//-----------------------------------------------------------------------------
//
// Whenever the Port_Match ISR is active, turn on the LED.  Since the Port
// Match interrupt is level sensitive and not edge sensitive, the ISR
// waits for the switch to be released  and the interrupt is cleared before
// exiting Otherwise, the Port_Match ISR would constantly be triggered until
// the switch was released.
//
//-----------------------------------------------------------------------------

INTERRUPT(PORT_Match_ISR, PMATCH_IRQn)
{
   LED05 = 0;                          // Turn on LED

   while (BTN03 == 0);                 // Wait for switch release
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
