//-----------------------------------------------------------------------------
// F39x_Oscillator_Capacitor.c
//-----------------------------------------------------------------------------
// Copyright 2012 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates how to configure the oscillator for use with an
// external capacitor.
//
//
// How To Test:
//
// 1) Ensure that a capacitor (C13) is installed.
// 2) Ensure shorting blocks are not placed on J6 and J7.
// 3) Download code to an 'F390 UDP MCU board.
// 4) Measure the frequency output on P0.0.
//
// Choosing Component Values:
//
//       frequency (Hz) is proportional to [ KF(XFCN) / (C * VDD) ]
//
//       where KF(XFCN) is a factor based on XFCN.
//
// The actual frequency is best determined by measurement. In this example,
// a 33pF capacitor was used with VDD = 3.3V. Component values are best
// chosen by trial and error.
//
// The resulting frequencies achievable using 33pF and 3.3V VDD are:
//
//          SYSCLK   |   SYSCLK/2   |   XFCN
//      --------------------------------------
//           8 kHz   |      4 kHz   |    0
//          24 kHz   |     12 kHz   |    1
//          72 kHz   |     36 kHz   |    2
//         206 kHz   |    103 kHz   |    3
//         602 kHz   |    301 kHz   |    4
//         1.7 kHz   |    868 kHz   |    5
//         6.8 MHz   |    3.4 kHz   |    6
//        20.4 MHz   |   10.2 kHz   |    7
//
// Note: In C mode, the frequency measured at P0.0 will be SYSCLK/2.
//
// Target:         C8051F390, C8051F370
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (ST)
//    - 30 JUL 2012
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F390_defs.h>            // SFR declarations

//-----------------------------------------------------------------------------
// External Oscillator RC Frequency Selection
//-----------------------------------------------------------------------------

// XFCN Setting Macro
#define XFCN             0             // XFCN/KF setting.

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
                                       // enable)

   PORT_Init();                        // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator

   while (1) {};                       // loop forever

}                                      // end of main()

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
// This function initializes the system clock to use the external oscillator
// in C mode.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   int i;                              // delay counter

   OSCXCN = (0x50 | XFCN);             // start external oscillator with
                                       // the appropriate XFCN setting
                                       // in C Mode

   for (i = 0; i < 256; i++);          // Wait for C osc. to start

   RSTSRC = 0x02;                      // Enable missing VDD Monitor as a 
                                       // reset source

   CLKSEL = 0x01;                      // Select external oscillator as system
                                       // clock source

   OSCICN = 0x00;                      // Disable the internal oscillator.
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
// The oscillator pins should be configured as analog inputs when using the
// external oscillator in crystal mode.
//
// P0.0   digital    push-pull     /SYSCLK
// P0.3   analog     don't care     XTAL2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Oscillator Pins
   P0MDIN &= ~0x08;                    // P0.3 is analog
   P0SKIP |= 0x08;                     // P0.3 skipped in the Crossbar

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0    = 0x08;                     // Route /SYSCLK to first available pin
   XBR1    = 0x40;                     // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------