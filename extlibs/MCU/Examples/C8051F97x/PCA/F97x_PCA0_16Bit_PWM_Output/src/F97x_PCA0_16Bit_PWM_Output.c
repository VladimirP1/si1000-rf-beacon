//-----------------------------------------------------------------------------
// F97x_PCA0_8Bit_PWM_Output.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use the PCA to generate a 16-bit PWM waveform
//
// This program sends a PWM waveform out of an I/O pin, using the PCA's
// 16-bit PWM Output Mode.  The duty cycle of the waveform is modified
// periodically within the main loop.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P0.5.
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
//      or pressing F8..
// 5) Observe the triangle wave on LED05.
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

#define SYSCLK       20000000          // SYSCLK frequency in Hz

SBIT (LED, SFR_P0, 5);                 // LED=0 means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void PCA0_Init (void);

INTERRUPT_PROTO (PCA0_ISR, PCA0_IRQn);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

U16 CEX0_Compare_Value;                // Holds current PCA compare value

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   U16 delay_count;                    // Used to implement a delay
   bit duty_direction = 0;             // 0 = Decrease; 1 = Increase

   SFRPAGE = PCA0_PAGE;                // Disable the watchdog timer
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;

   SYSCLK_Init ();                     // Configure system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   PCA0_Init ();                       // Initialize PCA0

   IE_EA = 1;                          // Enable global interrupts

   while (1)
   {
      // Wait a little while
      for (delay_count = 10; delay_count > 0; delay_count--);


      SFRPAGE = PCA0_PAGE;
      if (duty_direction == 1)         // Increase Duty Cycle
      {
         // First, check the ECOM0 bit
         if ((PCA0CPM0 & PCA0CPM0_ECOM__BMASK) == PCA0CPM0_ECOM__DISABLED)
         {
            PCA0CPM0 |= PCA0CPM0_ECOM__ENABLED; // Set ECOM0
         }
         else                          // Increase duty cycle otherwise
         {

            CEX0_Compare_Value--;      // Increase duty cycle

            if (CEX0_Compare_Value == 0x00)
            {
               duty_direction = 0;     // Change direction for next time
            }
         }
      }
      else                             // Decrease Duty Cycle
      {
         if (CEX0_Compare_Value == (65536 - 1))
         {
            PCA0CPM0 &= ~PCA0CPM0_ECOM__BMASK; // Clear ECOM0
            duty_direction = 1;        // Change direction for next time
         }
         else
         {
            CEX0_Compare_Value++;      // Decrease duty cycle
         }
      }
   }
}


//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure system clock to low power internal oscillator / 8
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;             // Save the current SFRPAGE
   SFRPAGE = LEGACY_PAGE;

   CLKSEL = CLKSEL_CLKSL__LPOSC |         // Use Low Power Int Osc
            CLKSEL_CLKDIV__SYSCLK_DIV_1;  // 20MHz/1 = 20MHz

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


   P0SKIP  |= 0x1F;                    // Put PWM on P0.5
   P0MDOUT |= 0x20;                    // Enable LED as a push-pull output


   XBR0 |= XBR0_PCA0ME__CEX0;          // Enable PCA0 CEX0 output

   XBR1  = XBR1_XBARE__ENABLED |       // Enable crossbar
           XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups

   SFRPAGE = SFRPAGE_save;             // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base, and sets up 16-bit PWM output
// mode for Module 0 (CEX0 pin).
//
// The frequency of the PWM signal generated at the CEX0 pin is equal to the
// PCA main timebase frequency divided by 65536.
//
// The PCA time base in this example is configured to use SYSCLK, and SYSCLK
// is set up to use the internal oscillator running at 24.5 MHz.  Therefore,
// the frequency of the PWM signal will be 24.5 MHz / 65536 = 373.8 Hz.
// Using different PCA clock sources or a different processor clock will
// result in a different frequency for the PWM signal.
//
//    -------------------------------------------------------------------------
//    How "16-Bit PWM Mode" Works:
//
//       The PCA's 16-bit PWM Mode works by setting an output pin low every
//    time the main 16-bit PCA counter (PCA0H) overflows, and then setting
//    the pin high whenever a specific match condition is met.
//
//    Upon a PCA0 overflow (PCA0 incrementing from 0xFFFF to 0x0000), the
//    CEXn pin will be set low.
//
//    When the PCA0 register increments and matches the PCA0CPn register for
//    the selected module, the CEXn pin will be set high, except when the
//    ECOMn bit in PCA0CPMn is cleared to '0'.  By varying the value of the
//    PCA0CPn register, the duty cycle of the waveform can also be varied.
//
//    When ECOMn = '1', the duty cycle of the PWM waveform is:
//
//       16-bit PWM Duty Cycle = (65536 - PCA0CPn) / 65536
//
//    To set the duty cycle to 100%, a value of 0x0000 should be loaded into
//    the PCA0CPn register for the module being used (with ECOMn set to '1').
//    When the value of PCA0CPn is equal to 0x0000, the pin will never be
//    set low.
//
//    To set the duty cycle to 0%, the ECOMn bit in the PCA0CPMn register
//    should be cleared to 0.  This prevents the PCA0CPn match from occuring,
//    which results in the pin never being set high.
//
// When adjusting the PWM duty cycle, the low byte of the PCA0CPn register
// (PCA0CPLn) should be written first, followed by the high byte (PCA0CPHn).
// Writing the low byte clears the ECOMn bit, and writing the high byte will
// restore it.  This ensures that a match does not occur until the full
// 16-bit value has been written to the compare register.  Writing the high
// byte first will result in the ECOMn bit being set to '0' after the 16-bit
// write, and the duty cycle will also go to 0%.
//
// It is also advisable to wait until just after a match occurs before
// updating the PWM duty cycle.  This will help ensure that the ECOMn
// bit is not cleared (by writing PCA0CPLn) at the time when a match was
// supposed to happen.  This code implements the compare register update in
// the PCA ISR upon a match interrupt.
//    -------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = PCA0_PAGE;

   PCA0CN = PCA0CN_CCF0__NOT_SET |     // Disable PCA0 and
            PCA0CN_CCF1__NOT_SET |     // clear flags
            PCA0CN_CCF2__NOT_SET |
            PCA0CN_CR__STOP |
            PCA0CN_CF__NOT_SET;

   PCA0MD &= ~PCA0MD_CPS__FMASK;       // Timebase = SYSCLK
   PCA0MD |= PCA0MD_CPS__SYSCLK;

   PCA0CPM0 = PCA0CPM0_ECCF__ENABLED | // 16-bit PWM mode and
              PCA0CPM0_PWM__ENABLED |  // enable ECOM0 match and
              PCA0CPM0_TOG__DISABLED | // interrupt flags
              PCA0CPM0_MAT__ENABLED |
              PCA0CPM0_CAPN__DISABLED |
              PCA0CPM0_CAPP__DISABLED |
              PCA0CPM0_ECOM__ENABLED |
              PCA0CPM0_PWM16__16_BIT;

   // Configure initial PWM duty cycle = 50%
   CEX0_Compare_Value = 65536 - (65536 * 0.5);
   PCA0CPL0 = (CEX0_Compare_Value & 0x00FF);
   PCA0CPH0 = (CEX0_Compare_Value & 0xFF00)>>8;

   EIE1 |= EIE1_EPCA0__BMASK;          // Enable PCA0 interrupts

   PCA0CN |= PCA0CN_CR__RUN;           // Enable PCA0 counter

   SFRPAGE = SFRPAGE_save;             // Restore saved SFRPAGE
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This is the ISR for the PCA.  It handles the case when a match occurs on
// channel 0, and updates the PCA0CPn compare register with the value held in
// the global variable "CEX0_Compare_Value".
//
//-----------------------------------------------------------------------------
INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
   SFRPAGE = PCA0_PAGE;

   PCA0CN &= ~PCA0CN_CCF0__BMASK;      // Clear module 0 interrupt flag.

   // Update Capture/Compare Register
   PCA0CPL0 = (CEX0_Compare_Value & 0x00FF);
   PCA0CPH0 = (CEX0_Compare_Value & 0xFF00) >> 8;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
