//-----------------------------------------------------------------------------
// F97x_External_Interupts.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Interrupt driven IO using Buttons and LEDs
//
// This software shows the necessary configuration to use External Interrupts 0
// and 1 as interrupt sources. If the BTN03 is pressed, then the edge-triggered
// /INT0 will cause an interrupt and toggle LED05. If BTN04 is pressed, then the
// /level-triggered /INT1 will continuously toggle LED15 until it is released.
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
// 5) Press a button. When BTN03 is pressed, LED05 will toggle once. When BTN04
//    is pressed, LEd15 will continuously toggle until released.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 30 APR 2014
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK             20000000      // SYSCLK frequency in Hz


SBIT (LED05,   SFR_P0, 5);               // '0' means ON, '1' means OFF
SBIT (LED15,   SFR_P1, 5);               // '0' means ON, '1' means OFF

SBIT (BTN03,   SFR_P0, 3);               // '0' means ON, '1' means OFF
SBIT (BTN04,   SFR_P0, 4);               // '0' means ON, '1' means OFF

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);               // Configure the system clock
void PORT_Init (void);                 // Configure the Crossbar and GPIO
void EXTINT_Init (void);               // Configure External Interrupts (/INT0
                                       // and /INT1)

INTERRUPT_PROTO(ExtInt0_ISR, INT0_IRQn);
INTERRUPT_PROTO(ExtInt1_ISR, INT1_IRQn);
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = PCA0_PAGE;                // Disable the watchdog timer
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;

   SYSCLK_Init ();                     // Initialize system clock
   PORT_Init ();                       // Initialize crossbar and GPIO

   EXTINT_Init();                      // Initialize External Interrupts

   IE_EA = 1;                          // Enable Interrupts

   while(1);                           // Infinite while loop waiting for
                                       // an interrupt from /INT0 or /INT1
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
// oscillator as its clock source divided by 1.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   CLKSEL = CLKSEL_CLKSL__LPOSC |         // Use Low Power Int Osc
            CLKSEL_CLKDIV__SYSCLK_DIV_1;  // 20MHz/1 = 20MHz
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
// Pinout:
//
// P0.3 - digital   open-drain  /INT0
// P0.4 - digital   open-drain  /INT1
// P0.5 - digital   push-pull   LED05
//
// P1.5 - digital   push-pull   LED15
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Save the current SFRPAGE
   U8 SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x20;                       // Enable LED05 as a push-pull
   P1MDOUT |= 0x20;                       // Enable LED15 as a push-pull


   XBR1 = XBR1_XBARE__ENABLED |           // Enable crossbar
          XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups

   SFRPAGE = SFRPAGE_save;

}

//-----------------------------------------------------------------------------
// Ext_Interrupt_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures and enables /INT0 (External Interrupt)
// and /INT1 (External Interrupt) as active low level-triggered.
//
//-----------------------------------------------------------------------------
void EXTINT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = TIMER01_PAGE;

   TCON = TCON_IT0__EDGE |             // /INT0 is edge triggered
          TCON_IT1__LEVEL;             // /INT1 is level triggered


   SFRPAGE = CONFIG_PAGE;

   IT01CF = IT01CF_IN0PL__ACTIVE_LOW | // /INT0 is Active Low
            IT01CF_IN0SL__P0_3 |       //  and triggered by P0.3 falling edge
            IT01CF_IN1PL__ACTIVE_LOW | // /INT1 is Active Low
            IT01CF_IN1SL__P0_4;        //  and triggered by P0.4


   IE_EX0 = 1;                         // Enable /INT0 interrupt
   IE_EX1 = 1;                         // Enable /INT1 interrupt

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// /Ext_Interrupt0_ISR
//-----------------------------------------------------------------------------
//
// Whenever a falling edge appears on P0.3, LED is toggled.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
INTERRUPT(ExtInt0_ISR, INT0_IRQn)
{
   LED05 = !LED05; // Toggle LED
}

//-----------------------------------------------------------------------------
// /Ext_Interrupt1_ISR
//-----------------------------------------------------------------------------
//
// Whenever an active low level appears on P0.4, LED is toggled.
// The interrupt pending flag is not cleared. If the interrupt request is
// not deactivated, another interrupt request will occur as long as the level
// matches the INT1 configuration
//
//-----------------------------------------------------------------------------
INTERRUPT(ExtInt1_ISR, INT1_IRQn)
{
   LED15 = !LED15; // Toggle LED
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
