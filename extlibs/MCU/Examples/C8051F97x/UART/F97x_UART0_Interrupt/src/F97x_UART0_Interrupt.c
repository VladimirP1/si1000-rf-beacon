//-----------------------------------------------------------------------------
// F97x_UART0_Interrupt.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to configure the C8051F97x to write to and read
// from the UART interface. The program reads a word using the UART interrupts
// and outputs that word to the screen, with all characters in uppercase
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP1: VCP RX <> P0.1/UART0.TX
//    - JP2: VCP TX <> P0.2/UART0.RX
//    - JP3:    VDD <> VMCU
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Connect a USB cable from J9 of the development board to a PC,
// 5) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the virtual COM port at <BAUDRATE> (115200) and 8-N-1.
// 6) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 7) Type up to 64 characters into the Terminal and press Enter.  The MCU
//    will then print back the characters that were typed
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
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // SYSCLK frequency in Hz
#define BAUDRATE     115200            // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
LOCATED_VARIABLE_NO_INIT (reserved, U8, SEG_XDATA, 0x0000);


//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void UART0_Init (void);

INTERRUPT_PROTO (UART0_ISR, INTERRUPT_UART0);


//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

#define UART_BUFFERSIZE 64
U8 UART_Buffer[UART_BUFFERSIZE];

U8 UART_Buffer_Size = 0;
U8 UART_Input_First = 0;
U8 UART_Output_First = 0;
U8 TX_Ready = 1;
static U8 Byte;


//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = PCA0_PAGE;                // Disable the watchdog timer
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;

   SYSCLK_Init ();                     // Configure system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   UART0_Init ();                      // Initialize UART0

   IE_EA = 1;                          // Enable global interrupts

   while(1)
   {
      // If the complete word has been entered via the terminal followed by
      // carriage return

      if(TX_Ready == 1 && UART_Buffer_Size != 0 && Byte == 13)
      {
         TX_Ready = 0;                 // Set the flag to zero

         SFRPAGE = UART0_PAGE;
         SCON0 |= SCON0_TI__SET;       // Indicate TX0 ready
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
// Return Value:  None
// Parameters:    None
//
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator as its clock source.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   OSCICN |= OSCICN_IOSCEN__ENABLED;      // Enable High Presicion Int Osc

   // Wait for HFOSC to ready
   while ((OSCICN & OSCICN_IFRDY__BMASK) == OSCICN_IFRDY__NOT_SET);

   // Switch to HFOSC
   CLKSEL = CLKSEL_CLKSL__HFOSC |
            CLKSEL_CLKDIV__SYSCLK_DIV_1;

   SFRPAGE = SFRPAGE_save;
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
// P0.1 - digital  push-pull   UART TX
// P0.2 - digital  open-drain  UART RX
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;


   P0SKIP |= 0x01;                           // Skip P0.0

   P0MDOUT |= 0x02;                          // Set TX pin to push-pull


   XBR0    = XBR0_URT0E__ENABLED;            // Enable UART0

   XBR1    = XBR1_XBARE__ENABLED |           // Enable crossbar
             XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups

   SFRPAGE = SFRPAGE_save;                   // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = UART0_PAGE;

   SCON0 = SCON0_SMODE__8_BIT |
           SCON0_MCE__MULTI_DISABLED |
           SCON0_REN__RECEIVE_ENABLED |
           SCON0_TB8__CLEARED_TO_0 |
           SCON0_RB8__CLEARED_TO_0 |
           SCON0_TI__NOT_SET |
           SCON0_RI__NOT_SET;


   SFRPAGE = TIMER01_PAGE;
   CKCON &= ~(CKCON_T1M__BMASK |
              CKCON_SCA__FMASK);
   #if (SYSCLK/BAUDRATE/2/256 < 1)
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON |= CKCON_T1M__SYSCLK;
   #elif (SYSCLK/BAUDRATE/2/256 < 4)
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON |= CKCON_SCA__SYSCLK_DIV_4;
   #elif (SYSCLK/BAUDRATE/2/256 < 12)
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON |= CKCON_SCA__SYSCLK_DIV_12;
   #else
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON |= CKCON_SCA__SYSCLK_DIV_48;
   #endif

   TL1 = TH1;                          // Init Timer1

   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;

   TCON_TR1 = 1;                       // START Timer1


   IP |= IP_PS0__HIGH;                 // Make UART high priority
   IE |= IE_ES0__ENABLED;              // Enable UART0 interrupts

   TX_Ready = 1;                       // Flag showing that UART can transmit

   SFRPAGE = SFRPAGE_save;             // Restore saved SFRPAGE
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// UART0_Interrupt
//-----------------------------------------------------------------------------
//
// This routine is invoked whenever a character is entered or displayed on the
// Hyperterminal.
//
//-----------------------------------------------------------------------------

INTERRUPT(UART0_ISR, UART0_IRQn)
{
   U8 SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = UART0_PAGE;

   if ((SCON0 & SCON0_RI__BMASK) == SCON0_RI__SET)
   {

      if( UART_Buffer_Size == 0)
      {  // If new word is entered
         UART_Input_First = 0;
      }

      SCON0 &= ~SCON0_RI__BMASK;         // Clear interrupt flag

      Byte = SBUF0;                      // Read a character from UART

      if (UART_Buffer_Size < UART_BUFFERSIZE)
      {
         UART_Buffer[UART_Input_First] = Byte; // Store in array

         UART_Buffer_Size++;                   // Update array's size

         UART_Input_First++;                   // Update counter
      }
   }

   if ((SCON0 & SCON0_TI__BMASK) == SCON0_TI__SET) // Check if transmit flag is set
   {

      SCON0 &= ~SCON0_TI__BMASK;                   // Clear interrupt flag

      if (UART_Buffer_Size != 1)                   // If buffer not empty
      {
         // If a new word is being output
         if ( UART_Buffer_Size == UART_Input_First )
         {
              UART_Output_First = 0;
         }

         // Store a character in the variable byte
         Byte = UART_Buffer[UART_Output_First];

         if ((Byte >= 0x61) && (Byte <= 0x7A))
         {  // If upper case letter
            Byte -= 32;
         }

         SBUF0 = Byte;                   // Transmit to Hyperterminal

         UART_Output_First++;            // Update counter

         UART_Buffer_Size--;             // Decrease array size

      }
      else
      {
         UART_Buffer_Size = 0;           // Set the array size to 0
         TX_Ready = 1;                   // Indicate transmission complete
      }
   }

   SFRPAGE = SFRPAGE_save;               // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
