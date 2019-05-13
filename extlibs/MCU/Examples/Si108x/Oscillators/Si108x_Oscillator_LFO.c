//-----------------------------------------------------------------------------
// Si108x_Oscillator_LFO.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates how to use the LFO mode of the 
// smaRTClock oscillator as a low frequency system clock.
//
//
// How To Test:
//
// 1) Download code to an Si108x development board.
// 2) Measure the frequency output on P0.0.
//
//
// Target:         Si108x
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.0
//    -Initial Revision (FB)
//    -22 OCT 2013

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <Si1080_defs.h>            // SFR declarations

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void PORT_Init (void);
void RTC_Init (void);
void OSCILLATOR_Init (void);

unsigned char RTC_Read (unsigned char);
void RTC_Write (unsigned char, unsigned char);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
                                       // enable)

   PORT_Init();                        // Initialize Port I/O
   RTC_Init ();                        // Initialize RTC
   OSCILLATOR_Init ();                 // Initialize Oscillator
   

   while (1);                          // Infinite Loop

}                                      

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
//
// P0.0   digital    push-pull      /SYSCLK
//-----------------------------------------------------------------------------
void PORT_Init (void)
{

   // Buffered System Clock Output
   P0MDOUT |= 0x03;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0    = 0x08;                     // Route /SYSCLK to first available pin
   XBR2    = 0x40;                     // Enable Crossbar and weak pull-ups
   
}

//-----------------------------------------------------------------------------
// RTC_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will initialize the RTC. 
//
//-----------------------------------------------------------------------------
void RTC_Init (void)
{

   RTC0KEY = 0xA5;                     // Unlock the smaRTClock interface
   RTC0KEY = 0xF1;

    
   RTC_Write (RTC0XCN, 0x08);          // Enable the LFO

   RTC_Write (RTC0CN, 0x80);           // Enable smaRTClock oscillator
      

   //----------------------------------
   // smaRTClock has been started
   //----------------------------------
 

   RTC_Write (RTC0CN, 0xC0);           // Enable missing smaRTClock detector
                                       // and leave smaRTClock oscillator
                                       // enabled.

   

      
}

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the smaRTClock oscillator.
// 
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{

      
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.
   
   CLKSEL &= ~0x70;                    // Specify a clock divide value of 1
   
   while(!(CLKSEL & 0x80));            // Wait for CLKRDY to ensure the 
                                       // divide by 1 has been applied

   CLKSEL = 0x03;                      // Select smaRTClock oscillator 
                                       // as the system clock

}


//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// RTC_Read ()
//-----------------------------------------------------------------------------
//
// Return Value : RTC0DAT
// Parameters   : 1) unsigned char reg - address of RTC register to read
//
// This function will read one byte from the specified RTC register.
// Using a register number greater that 0x0F is not permited.
//
//-----------------------------------------------------------------------------

unsigned char RTC_Read (unsigned char reg)
{
   reg &= 0x0F;                        // mask low nibble
   RTC0ADR  = reg;                     // pick register
   RTC0ADR |= 0x80;                    // set BUSY bit to read
   while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
   return RTC0DAT;                     // return value
}

//-----------------------------------------------------------------------------
// RTC_Write ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) unsigned char reg - address of RTC register to write
//                2) unsigned char value - the value to write to <reg>
//
// This function will Write one byte to the specified RTC register.
// Using a register number greater that 0x0F is not permited.
//-----------------------------------------------------------------------------
void RTC_Write (unsigned char reg, unsigned char value)
{
   reg &= 0x0F;                        // mask low nibble
   RTC0ADR  = reg;                     // pick register
   RTC0DAT = value;                    // write data
   while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------