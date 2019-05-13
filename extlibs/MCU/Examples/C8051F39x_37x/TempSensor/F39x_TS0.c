//-----------------------------------------------------------------------------
// F39x_TS0.c
//-----------------------------------------------------------------------------
// Copyright 2012 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program continuously displays the current die temperature in degrees  
// Celsius obtained by the precision temperature sensor.
//
// The temperature sensor output is a 16-bit, two's complement number
// (1 sign bit, 8 integer bits, and 7 decimals bits) stored in
// TS0DATH:TS0DATL. 
//
// Note: only 4 of the 7 decimal bits are used in this code example
//
// 
// How To Test:
//
// 1) Ensure that jumpers are placed on J14 of the C8051F390/370 UDP MCU
//    board that connect P0.4 to the MCU_TX, and P0.5 to the MCU_RX.
// 2) Connect USB cable from the development board to a PC
// 3) Specify the target baudrate in the constant <BAUDRATE>.
// 4) Open Hyperterminal, or a similar program, and connect to the target
//    board's USB port (virtual com port).
// 5) Download and execute code to the 'F390/370 UDP MCU board.
//
//
// Target:         C8051F390
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.2
//    - Using 4 of the 7 decimal bits of the TS0 output (ST)
//    - 15 JUL 2012
//
// Release 1.1
//    - Added comments and formatting (ST)
//    - 15 JUN 2012
//
// Release 1.0
//    - Initial Revision (BD)
//

//-----------------------------------------------------------------------------
// Includes and Device-Specific Parameters
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F390_defs.h>            // SFR declarations
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK    49000000/2           // SYSCLK frequency in Hz
#define BAUDRATE      115200           // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART0_Init (void);
void PORT_Init (void);
void TS0_Init (void);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
   UU32 temp_reading;                  
   U16 hexval;                         // Holds raw temperature sensor output      
   U8 temp_int;                        // Holds integer portion of temperature
                                       //   in decimal (degrees C)
   U16 temp_frac;                      // Holds decimal portion of temperature 
                                       //   in decimal (degrees C)
   U8 sign_char;                       // '-' if the temperature is negative

   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
                                       // enable)

   PORT_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART0_Init();
   TS0_Init();

   while(1)
   {
      //-----------------------
      // 1. Obtain temperature
      //-----------------------      
      SFRPAGE = TS0CN_PAGE;

      TS0CN |= 0x80;    
      TS0CN &= ~0x80;                  // Start temp sensor measurement

      while ((TS0CN & 0x40)!= 0x40);   // Wait conversion to complete
      TS0CN &= ~0x40;

      SFRPAGE = TS0DAT_PAGE;

      hexval = TS0DAT;                 // Store raw temperature sensor output

      //--------------------------------------
      // 2. Format temperture for UART output
      //--------------------------------------
      temp_reading.U32 = hexval;  

      temp_frac = temp_reading.U8[b0]>>3;    // Store only 4 of 7 decimal bits
      temp_reading.U32 = temp_reading.U32 << 1;

      // Convert raw temperature sensor value to decimal for UART output
      if (temp_reading.U8[b2])
      {
         sign_char = '-';                    // Temperature is negative
         temp_int = -(temp_reading.S8[b1]);  // Decimal value of integer
         temp_frac = 0x0F & (-temp_frac);    
         temp_frac = 625 * (temp_frac);      // Decimal value of fraction
         if (temp_frac != 0)
         {
            temp_int--;
         }
      }
      else
      {
         sign_char = ' ';                    // Temperature is postive
         temp_int = temp_reading.U8[b1];     // Decimal value of integer
         temp_frac = 0x0F & temp_frac;
         temp_frac = 625 * (temp_frac);      // Decimal value of fraction 
      }

      //--------------------------------------------------------
      // 3. Print temperature in Celsius and raw hex value to UART
      //--------------------------------------------------------
      printf("%c%d.%04u\t0x%04X\n", sign_char, (U16)temp_int, temp_frac, hexval);
   }
}


//-----------------------------------------------------------------------------
// TS0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the precision temperature sensor
//
//-----------------------------------------------------------------------------

void TS0_Init (void)
{
   unsigned char SFRPAGE_save = SFRPAGE;
   SFRPAGE = TS0CN_PAGE;

   TS0CN = 0x07;                       // TS length = max

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// PORT_Init
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
void PORT_Init (void)
{
   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output
   XBR0    = 0x01;                     // Enable UART on P0.4(TX) and P0.5(RX)
   XBR1    = 0x40;                     // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 49 MHz
// oscillator divided by 2 as its clock source.  Also enables missing clock 
// detector reset.
//
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   OSCICN = 0x83;                      // Set system clock to 24.5 MHz
   RSTSRC = 0x04;                      // Enable missing clock detector
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
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TR1 = 1;                            // START Timer1
   TI0 = 1;                            // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
