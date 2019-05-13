//-----------------------------------------------------------------------------
// Si106x_SleepMode_smaRTClockWake.c
//-----------------------------------------------------------------------------
// Copyright (C) 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// The smaRTClock is configured to generate a wake-up event every 2 seconds. 
// The device will toggle between Sleep/Suspend mode and Normal mode. In 
// the low power state, the LED will be turned on. 
//
// If a smaRTClock oscillator failure occurs, both LEDs will remain on
// and the device will be in a high power state.
//
// How To Test:
//
// 1) Download code to the Si1060 development board 
// 2) Run the program. The LED will turn on when the device is asleep.
//
//
// Target:         Si106x
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (FB)
//    - 29 JUL 2013
//
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <compiler_defs.h>             // compiler declarations
#include <Si1060_defs.h>            // SFR declarations
#include <Si1060_lib.h>             // library declarations
                       
//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SBIT (LED, SFR_P0, 0);             // Used to show active/sleep state



//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
bit Blink_LED = 0;

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
                                               
#define LED_ON           0
#define LED_OFF          1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void);
void PORT_Init (void);


//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
                                       // enable)

   PORT_Init ();                       // Initialize Port I/O

   LED = LED_OFF;                      // Initilize the state of the led
             

   OSCILLATOR_Init ();                 // Initialize Oscillator
 
   RTC_Init ();                        // Initialize SmaRTClock
   RTC_WriteAlarm(WAKE_INTERVAL_TICKS);// Set the Alarm Value
   RTC0CN_SetBits(RTC0TR+RTC0AEN+ALRM);// Enable Counter, Alarm, and Auto-Reset

   LPM_Init();                         // Initialize Power Management
   LPM_Enable_Wakeup(RTC);

   EA = 1;
                                       
   
   RTC_Alarm = 1;                      // Set the RTC Alarm Flag on startup

   //----------------------------------
   // Main Application Loop
   //----------------------------------
   while (1)
   {
      
      //-----------------------------------------------------------------------
      // Task #1 - Handle RTC Failure
      //-----------------------------------------------------------------------
      if(RTC_Failure)
      {
         RTC_Failure = 0;              // Reset RTC Failure Flag to indicate
                                       // that we have detected an RTC failure
                                       // and are handling the event

         // Do something...RTC Has Stopped Oscillating
         while(1);                     // <Insert Handler Code Here>
      }


      //-----------------------------------------------------------------------
      // Task #2 - Handle RTC Alarm
      //-----------------------------------------------------------------------
      if(RTC_Alarm)
      {

         RTC_Alarm = 0;                // Reset RTC Alarm Flag to indicate 
                                       // that we have detected an alarm
                                       // and are handling the alarm event

         Blink_LED = !Blink_LED;       // Change blinking state

         if(Blink_LED)
         {
            // Adjust LED state for active mode
            LED = LED_OFF;           
         }
         else
         {
            // Adjust LED state for sleep mode
            LED = LED_ON;
         }

      }

      //-----------------------------------------------------------------------
      // Task #3 - Sleep for the remainder of the interval
      //-----------------------------------------------------------------------
      // If blinking, stay awake, otherwise sleep      

      if(Blink_LED)
      {
         // Wait for next alarm or clock failure, then clear flags
         while((PMU0CF & RTCAWK) == 0);
         if(PMU0CF & RTCAWK) RTC_Alarm = 1;
         if(PMU0CF & RTCFWK) RTC_Failure = 1;
         PMU0CF = CLEAR;
              
      }
      else
      {
         // Place the device in Sleep Mode
         LPM(SLEEP);                      // Enter Sleep Until Next Alarm
      }      
      
   }
}
//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal precision
// oscillator.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{


   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x24;                      // Select low power internal osc.
                                       // divided by 4 as the system clock

   FLSCL |= 0x40;                      // Set the BYPASS bit for operation > 10MHz

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
// P1.5   digital   push-pull     Red LED
// P1.6   digital   push-pull     Yellow LED

//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0MDIN |= 0x0C;                     // P0.2, P0.3 are digital
   P1MDIN |= 0x60;                     // P1.5, P1.6 are digital

   P0MDOUT &= ~0x0C;                   // P0.2, P0.3 are open-drain
   P1MDOUT |= 0x60;                    // P1.5, P1.6 are push-pull

   P0     |= 0x0C;                     // Set P0.2, P0.3 latches to '1'

   XBR2    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
}




              