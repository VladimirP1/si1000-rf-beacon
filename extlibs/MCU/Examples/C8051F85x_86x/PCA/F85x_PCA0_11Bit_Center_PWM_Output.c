//-----------------------------------------------------------------------------
// F85x_PCA0_11Bit_Center_PWM_Output.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program sends one center-aligned PWM waveform out of one I/O pin
// using the PCA's 11-bit Center-Aligned PWM Output Mode.  The duty cycle of
// the waveform is modified periodically within the main loop.
//
// In this example, PCA Module 0 is used to generate the waveform, and
// the crossbar is configured to send the CEX0 signal out on P0.0.
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - JP2
// 2) Place an oscilloscope probe on P0.0.
// 3) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 4) Compile and download code to the 'F85x UDP MCU board.
// 5) Run the code - the waveform should be visible on the oscilloscope.
// 6) Verify that the duty cycle of the waveforms vary smoothly between
//    minimum and maximum values.
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 15 JAN 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>            // SFR declaration

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // Internal oscillator frequency in Hz

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void Port_Init (void);
void PCA0_Init (void);

INTERRUPT_PROTO (PCA0_ISR, INTERRUPT_PCA0);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

unsigned int CEX0_Compare_Value;       // Holds current PCA compare value

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   unsigned int delay_count;           // Used to implement a delay
   bit duty_direction = 0;             // 0 = Decrease; 1 = Increase

   // Disable watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   Port_Init ();                       // Initialize crossbar and GPIO
   SYSCLK_Init ();                     // Initialize oscillator
   PCA0_Init ();                       // Initialize PCA0

   EA = 1;

   while (1)
   {
      // Wait a little while
      for (delay_count = 1200; delay_count > 0; delay_count--);

      if (duty_direction == 1)         // Direction = Increase
      {
         // First, check the ECOM0 bit
         if ((PCA0CPM0 & 0x40) == 0x00)
         {
            PCA0CPM0 |= 0x40;          // Set ECOM0 if it is '0'
         }
         else                          // Increase duty cycle otherwise
         {
            CEX0_Compare_Value--;      // Increase duty cycle

            if (CEX0_Compare_Value == 0x0000)
            {
               duty_direction = 0;     // Change direction for next time
            }
         }
      }
      else                             // Direction = Decrease
      {
         if (CEX0_Compare_Value == 0x07FF)
         {
            PCA0CPM0 &= ~0x40;         // Clear ECOM0
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
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator divided by 1 as its clock source.  Also enables missing clock
// detector reset.
//
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   CLKSEL = 0x00;                      // Set system clock to 24.5 MHz

   RSTSRC = 0x06;                      // Enable missing clock detector
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
// P0.0   digital   push-pull     PCA0 CEX0
// P0.1   digital   push-pull     PCA0 CEX1
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   XBR0 = 0x00;
   XBR1 = 0x01;                        // Route CEX0 to P0.0
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups

   P0MDOUT |= 0x01;                    // Set CEX0 (P0.0) to push-pull
}

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base, and sets up 8-bit center-aligned
// PWM output mode for Module 0 (CEX0 pin).
//
// The frequency of the PWM signal generated at the CEX0 pin is equal to the
// PCA main timebase frequency divided by 256.
//
// The PCA time base in this example is configured to use SYSCLK, and SYSCLK
// is set up to use the internal oscillator running at 24.5 MHz.  Therefore,
// the frequency of the PWM signal will be 24.5 MHz / 256 = 95.7 kHz.
// Using different PCA clock sources or a different processor clock will
// result in a different frequency for the PWM signal.
//
//    -------------------------------------------------------------------------
//    How "11-Bit PWM Mode" Works:
//
//    In 9 to 15-bit PWM mode, the duty cycle is determined by the value of the
//    least significant N bits of the PCA0CPn register, where N is the selected
//    PWM resolution (in this case, 11).
//
//    To adjust the duty cycle, PCA0CPn should not normally be written
//    directly. Instead, it is recommended to adjust the duty cycle by writing
//    to an Auto-Reload register, which is dual-mapped into the PCA0CPHn and
//    PCA0CPLn register locations. The data written to define the duty cycle
//    should be right-justified in the registers. The auto-reload registers are
//    accessed (read or written) when the bit ARSEL in PCA0PWM is set to 1.
//    The capture/compare registers are accessed when ARSEL is set to 0. This
//    allows seamless updating of the PWM waveform, as the PCA0CPn register is
//    reloaded automatically with the value stored in the auto-reload registers
//    during the overflow edge (in edge-aligned mode) or the up edge (in
//    center-aligned mode).
//
//    If the MATn bit is set to 1, the CCFn flag for the module will be
//    set each time a match edge or up edge occurs. The COVF flag in PCA0PWM
//    can be used to detect the overflow or down edge.
//
//    The 9 to 15-bit PWM mode is selected by setting the ECOMn and PWMn bits
//    in the PCA0CPMn register, and setting the CLSEL bits in register PCA0PWM
//    to the desired cycle length (other than 8-bits). If the MATn bit is set
//    to 1, the CCFn flag for the module will be set each time a match edge or
//    up edge occurs. The COVF flag in PCA0PWM can be used to detect the
//    overflow or down edge.
//
//    Important Note About PCA0CPHn and PCA0CPLn Registers: When writing a
//    16-bit value to the PCA0CPn registers, the low byte should always be
//    written first. Writing to PCA0CPLn clears the ECOMn bit to 0; writing to
//    PCA0CPHn sets ECOMn to 1.
//    -------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   // Configure PCA time base; overflow interrupt disabled
   PCA0CN = 0x00;                      // Stop counter; clear all flags
   PCA0MD = 0x08;                      // Use SYSCLK as time base

   PCA0PWM = 0x43;                     // Target PCA0CPH/L registers,
                                       // enable cycle overflows, select
                                       // 11-bit PWM mode

   PCA0CLR = 0x00;                     // Comparator clears disabled

   PCA0CENT = 0x01;                    // Module 0 center-aligned

   PCA0CPM0 = 0x42;                    // Module 0 = 11-bit PWM mode

   // Configure initial PWM duty cycle = 50%
   CEX0_Compare_Value = 2048 - (2048 * 0.5);

   PCA0PWM |= 0x80;                    // Target Auto-reload registers

   PCA0CPL0 = (CEX0_Compare_Value & 0x00FF);
   PCA0CPH0 = (CEX0_Compare_Value & 0xFF00)>>8;

   PCA0PWM &= ~0x80;                   // Target PCA0CPH/L registers

   EIE1 |= 0x10;                       // Enable PCA interrupts

   // Start PCA counter
   CR = 1;
}

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
INTERRUPT(PCA0_ISR, INTERRUPT_PCA0)
{
   PCA0PWM &= ~0x20;                   // Clear cycle overflow flag

   PCA0PWM |= 0x80;                    // Target Auto-reload registers

   PCA0CPL0 = (CEX0_Compare_Value & 0x00FF);
   PCA0CPH0 = (CEX0_Compare_Value & 0xFF00)>>8;

   PCA0PWM &= ~0x80;                   // Target PCA0CPH/L registers
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
