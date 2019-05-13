//-----------------------------------------------------------------------------
// F97x_SmaRTClock_Single_Alarm.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to use F97x RTC timer without auto reset
// function. LED15 will be turned on when RTC Alarm is matched.
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
// 5) LED15 will be turn on when RTC Alarm (10 secondes) is matched.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 09 MAY 2014
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums

#include "F97x_SmaRTClock.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       20000000/8          // SYSCLK frequency in Hz

#define RTC_ALARM_DELAY    10000         // RTC Alarm delay in milliseconds

#define RTC_ALARM_VALUE    RTC_ALARM_DELAY * 0x8000 / 1000

SBIT (LED15, P1, 5);                     // LED=1 means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void TIMER2_Init (void);

INTERRUPT_PROTO(RTC_Alarm_ISR, RTC0ALARM_IRQn);


//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = PCA0_PAGE;                  // Disable the watchdog timer
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;

   SYSCLK_Init ();                       // Configure system clock
   PORT_Init ();                         // Initialize crossbar and GPIO
   RTC_Init (RTC_USE_LFO);               // Initialize SmaRTClock


   // Clear RTC capture value
   RTC_SetTimer(0x00000000);             // Clear RTC timer

   // Make sure RTC0AEN is disabled before setting alarm
   RTC_Write(RTC0CN, RTC0CN_RTC0EN__ENABLED | RTC0CN_RTC0AEN__DISABLED);

   // Set delay value
   RTC_WriteAlarm((U32) RTC_ALARM_VALUE);

   // Enable RTC Alarm and auto-reset
   RTC_Write(RTC0CN, RTC0CN_RTC0EN__ENABLED |   // Keep RTC enabled
                     RTC0CN_RTC0TR__RUN |       // Start RTC Timer
                     RTC0CN_RTC0AEN__ENABLED);  // Enable Alarm

   // Enable interrupts
   EIE1 |= EIE1_ERTC0A__ENABLED;
   IE_EA = 1;

   // Alarm LED turned off
   LED15 = 1;

   while(1);
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
            CLKSEL_CLKDIV__SYSCLK_DIV_8; // 20 MHz / 8 = 2.5 MHz

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
// P1.5 - digital  push-pull   LED
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;                // Save the current SFRPAGE
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x20;                          // Enable LED05 as a push-pull

   P1MDOUT |= 0x20;                          // Enable LED15 as a push-pull

   XBR1    = XBR1_XBARE__ENABLED |           // Enable crossbar
             XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups

   SFRPAGE = SFRPAGE_save;                   // Restore saved SFRPAGE
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// RTC_Alarm_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED when the RTC Alarm is matched.
//
//-----------------------------------------------------------------------------

INTERRUPT(RTC_Alarm_ISR, RTC0ALARM_IRQn)
{
   // Clear flag so that ISR is not called multiple times
   RTC_Write(RTC0CN, RTC0CN_RTC0EN__ENABLED |   // Keep RTC enabled
                     RTC0CN_RTC0TR__RUN |       // Start RTC Timer
                     RTC0CN_RTC0AEN__DISABLED); // Clear flag and disable Alarm

   // Turn on Alarm LED
   LED15 = 0;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
