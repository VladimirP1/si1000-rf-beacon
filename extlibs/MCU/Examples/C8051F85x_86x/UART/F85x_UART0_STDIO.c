//-----------------------------------------------------------------------------
// F85x_UART0_STDIO.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates how to configure the C8051F850 to use routines
// in STDIO.h to write to and read from the UART interface.  The program
// reads a character using the STDIO routine getkey(), outputs that character
// to the screen, and then outputs the ASCII hex value of that character.
//
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J7: all
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to the 'F85x UDP MCU board.
// 4) Connect a USB cable from the J5 of the MCU card to a PC.
// 5) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual COM port) at <BAUDRATE> (115200) and 8-N-1.
// 6) Run the code and follow the prompts in the terminal program.
//
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 16 JAN 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>            // SFR declarations
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK      24500000           // SYSCLK frequency in Hz
#define BAUDRATE      115200           // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART0_Init (void);
void Port_Init (void);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
   unsigned char inputcharacter;       // Used to store character from UART

   // Disable watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   Port_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART0_Init();

   while (1)
   {
      printf ("\nEnter character: ");
      inputcharacter = getchar();
      printf ("\nCharacter entered : %c",inputcharacter);
      printf ("\n      Value in Hex: %bx",inputcharacter);
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
// P0.4   digital   push-pull    UART TX
// P0.5   digital   open-drain   UART RX
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output

   XBR0 = 0x01;                        // Enable UART on P0.4(TX) and P0.5(RX)
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups
}

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
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits
   if (SYSCLK/BAUDRATE/2/256 < 1) {
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   } else if (SYSCLK/BAUDRATE/2/256 < 4) {
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
   } else if (SYSCLK/BAUDRATE/2/256 < 12) {
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } else {
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   }

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xF0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TR1 = 1;                            // START Timer1
   TI = 1;                             // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
