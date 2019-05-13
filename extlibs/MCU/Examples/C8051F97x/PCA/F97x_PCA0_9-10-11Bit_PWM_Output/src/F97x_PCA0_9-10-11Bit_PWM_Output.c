//-----------------------------------------------------------------------------
// F97x_PCA0_9-10-11Bit_PWM_Output.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use the PCA to generate a 9/10/11-bit PWM waveform
//
// This program sends a PWM waveform out of an I/O pin, using the PCA's
// 9/10/11-bit PWM Output Mode.  The duty cycle of the waveform is modified
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
// 3) Change the BIT_MODE variable to desired PWM resolution
// 4) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 5) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 6) Observe the triangle wave on LED05.
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

#define SYSCLK       20000000/8        // SYSCLK frequency in Hz

// Defines the number of bits used in Pulse Width Modulation.
//  9 = 9 bit, 10 = 10 bit, 11 = 11 bit
#define BIT_MODE 9

#if(BIT_MODE == 9)
   #define  PCA0PWM_CLSEL  PCA0PWM_CLSEL__9_BITS  // 9-bit PWM
   #define  BIT_VALUE 512
 #elif(BIT_MODE == 10)
   #define  PCA0PWM_CLSEL  PCA0PWM_CLSEL__10_BITS // 10-bit PWM
   #define  BIT_VALUE 1024
 #elif(BIT_MODE == 11)
   #define  PCA0PWM_CLSEL  PCA0PWM_CLSEL__11_BITS // 11-bit PWM
   #define  BIT_VALUE 2048
 #else
   #error "Invalid number for BIT_MODE."
#endif

SBIT (LED, P0, 5);                     // LED='1' means ON

//-----------------------------------------------------------------------------
// Function Prototypes
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
      for (delay_count = 100; delay_count > 0; delay_count--);


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
         if (CEX0_Compare_Value == (BIT_VALUE - 1))
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
// This function configures the PCA time base, and sets up 9/10/11-bit PWM
// output mode for Module 0 (CEX0 pin).
//
// The frequency of the PWM signal generated at the CEX0 pin is equal to the
// PCA main timebase frequency divided by 512/1024/2048 respectively.
//
// The PCA time base in this example is configured to use SYSCLK, and SYSCLK
// is set up to use the internal oscillator running at 20 MHz.  Therefore,
// the frequency of the PWM signal will be:

// 9  bits => 20 MHz / 512  = 47.851 KHz
// 10 bits => 20 MHz / 1024 = 23.926 KHz
// 11 bits => 20 MHz / 2048 = 11.963 KHz

// Using different PCA clock sources or a different processor clock will
// result in a different frequency for the PWM signal.
//
//    -------------------------------------------------------------------------
//    How "9/10/11-Bit PWM Mode" Works:
//
//       The PCA's 9/10/11-bit PWM Mode works by setting an output pin low
//    every time the main n-bits in the PCA counter (PCA0H) overflows, and
//    then setting the pin high whenever a specific match condition is met.
//
//    Upon a PCA0 nth-bit overflow (PCA0 incrementing from 0x01FF to 0x0000),
//    the CEXn pin will be set low.
//
//    When the PCA0 register increments and matches the PCA0CPn register for
//    the selected module, the CEXn pin will be set high, except when the
//    ECOMn bit in PCA0CPMn is cleared to '0'.  By varying the value of the
//    PCA0CPn register, the duty cycle of the waveform can also be varied.
//
//    When ECOMn = '1', the duty cycle of the PWM waveform is:
//
//       9-bit PWM Duty Cycle = ( 512 - PCA0CPn) / 512
//      10-bit PWM Duty Cycle = (1024 - PCA0CPn) / 1024
//      11-bit PWM Duty Cycle = (2048 - PCA0CPn) / 2048
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
// The duty cycle of the PWM output signal in 9/10/11-bit PWM mode should be
// varied by writing to an "Auto-Reload" Register, which is dual-mapped into
// the PCA0CPHn and PCA0CPLn register locations. The data written to define
// the duty cycle should be right-justified in the registers. The auto-reload
// registers are accessed (read or written) when the bit ARSEL in PCA0PWM is
// set to ‘1’. The capture/compare registers are accessed when ARSEL is set
// to ‘0’.

// When adjusting the PWM duty cycle, the low byte of the PCA0CPn register
// (PCA0CPLn) should be written first, followed by the high byte (PCA0CPHn).
// Writing the low byte clears the ECOMn bit, and writing the high byte will
// restore it.  This ensures that a match does not occur until the full
// n-bit value has been written to the compare register.  Writing the high
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
            PCA0CN_CCF1__NOT_SET |     //  clear flags
            PCA0CN_CCF2__NOT_SET |
            PCA0CN_CR__STOP |
            PCA0CN_CF__NOT_SET;

   PCA0MD &= ~PCA0MD_CPS__FMASK;       // Timebase = SYSCLK
   PCA0MD |= PCA0MD_CPS__SYSCLK;

   PCA0CPM0 = PCA0CPM0_ECCF__ENABLED  | // 9/10/11-bit PWM mode and
              PCA0CPM0_PWM__ENABLED   | // enable ECOM0 match and
              PCA0CPM0_TOG__DISABLED  | // interrupt flags
              PCA0CPM0_MAT__ENABLED   |
              PCA0CPM0_CAPP__DISABLED |
              PCA0CPM0_ECOM__ENABLED  |
              PCA0CPM0_PWM16__8_BIT;

   PCA0PWM = PCA0PWM_CLSEL |            // Sets PWM mode according to BIT_MODE
             PCA0PWM_ARSEL__AUTORELOAD; // Update Auto-Reload Register


   // Configure initial PWM duty cycle = 50%
   CEX0_Compare_Value = BIT_VALUE - (BIT_VALUE * 0.5);
   PCA0CPL0 = (CEX0_Compare_Value & 0x00FF);
   PCA0CPH0 = (CEX0_Compare_Value & 0xFF00) >> 8;

   PCA0PWM &= ~PCA0PWM_ARSEL__BMASK;   // Update Capture/Compare Register

   PCA0CN_CR = 1;                      // Enable PCA0

   EIE1 |= EIE1_EPCA0__BMASK;          // Enable PCA0 interrupts

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
   U8 SFRPAGE_save = SFRPAGE;                // Save the current SFRPAGE
   SFRPAGE = PCA0_PAGE;

   PCA0CN &= ~PCA0CN_CCF0__BMASK;            // Clear module 0 interrupt flag.

   PCA0PWM |= PCA0PWM_ARSEL__AUTORELOAD;     // Update Auto-Reload Register

   PCA0CPL0 = (CEX0_Compare_Value & 0x00FF); // Set duty cycle
   PCA0CPH0 = (CEX0_Compare_Value & 0xFF00) >> 8;

   PCA0PWM &= ~PCA0PWM_ARSEL__BMASK;         // Update Capture/Compare Register

   SFRPAGE = SFRPAGE_save;                   // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
