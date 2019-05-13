//-----------------------------------------------------------------------------
// F97x_CRC.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This firmware calculates a CRC on data with a software-implemented CRC and
// the C8051F970's hardware-implemented CRC.  It measures the cycles spent
// performing both tasks and outputs this information through the UART.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP1: VCP RX <> P0.1/UART0.TX
//    - JP2: VCP TX <> P0.2/UART0.RX
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
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
// 7) When LED05 is solid, the program has completed.
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
#define BAUDRATE       115200          // Baud rate of UART in bps

SBIT (LED, SFR_P0, 5);                 // LED=0 means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void TIMER2_Init (void);
void UART0_Init (void);

U16 UpdateCRC(U16 CRC_acc, U8 CRC_input);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   U16 CRC_Val;                         // Stores software-implemented CRC
   U16 SoftwareCRC_NumCycles;
   U16 HardwareCRC_NumCycles;

   UU16 CRCHW;                          // Saves hardware-generated CRC


   SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;

   SYSCLK_Init ();                      // Initialize system clock
   PORT_Init ();                        // Initialize crossbar and GPIO
   TIMER2_Init ();                      // Init Timer2
   UART0_Init ();                       // Initialize UART0 for printf's

   LED = 0;                             // Turn off LED


   SFRPAGE = TIMER2_PAGE;
   TMR2CN_TR2 = 1;                      // Enable timer 2

   // First, generate a 16-bit CRC in software, using a sequence
   // of characters as inputs, and save the number of cycles used
   // to perform the calculations
   CRC_Val = 0xFFFF;

   CRC_Val = UpdateCRC(CRC_Val, '1');
   CRC_Val = UpdateCRC(CRC_Val, '2');
   CRC_Val = UpdateCRC(CRC_Val, '3');
   CRC_Val = UpdateCRC(CRC_Val, '4');
   CRC_Val = UpdateCRC(CRC_Val, '5');
   CRC_Val = UpdateCRC(CRC_Val, '6');
   CRC_Val = UpdateCRC(CRC_Val, '7');
   CRC_Val = UpdateCRC(CRC_Val, '8');
   CRC_Val = UpdateCRC(CRC_Val, '9');

   SFRPAGE = TIMER2_PAGE;
   TMR2CN_TR2 = 0;                      // Disable timer 2

   SoftwareCRC_NumCycles = TMR2;        // Save number of cycles

   TMR2 = 0;                            // Clear timer

   TMR2CN_TR2 = 1;                      // Enable timer 2


   // Now use the C8051F750's CRC to generate the same CRC and save
   // the number of cycles used to perform the calculations
   SFRPAGE = CRC0_PAGE;

   //CRC0CN |= 0x00;                     // 32-bit CRC
   CRC0CN |= CRC0CN_CRCVAL__SET_ONES;    // Initializes CRC to 0xFFFF
   CRC0CN |= CRC0CN_CRCINIT__INIT;       // Initializes CRC result

   CRC0IN = '1';                         // Write values into the CRC
   CRC0IN = '2';
   CRC0IN = '3';
   CRC0IN = '4';
   CRC0IN = '5';
   CRC0IN = '6';
   CRC0IN = '7';
   CRC0IN = '8';
   CRC0IN = '9';

   CRC0CN = CRC0CN_CRCPNT__ACCESS_UPPER; // Return bits 15-8 of the 16-bit CRC result
   CRCHW.U8[0] = CRC0DAT;                // Read CRC value from hardware

   CRC0CN = CRC0CN_CRCPNT__ACCESS_LOWER; // Return bits 7-0 of the 16-bit CRC result
   CRCHW.U8[1] = CRC0DAT;                // Read CRC value from hardware


   SFRPAGE = TIMER2_PAGE;
   TMR2CN_TR2 = 0;                       // Disable timer 2


   HardwareCRC_NumCycles = TMR2;         // Save number of cycles

   // Output results
   printf("\nCycles used for software-generated CRC: %d",SoftwareCRC_NumCycles);
   printf("\nSoftware-generated CRC: %x", CRC_Val);
   printf("\nCycles used for hardware-generated CRC: %d",HardwareCRC_NumCycles);
   printf("\nHardware-generated CRC: %x\n", CRCHW.U16);

   LED = 0;                              // Turn on LED

   while (1);                            // Spin forever
}


U16 UpdateCRC (U16 CRC_acc, U8 CRC_input)
{
   U8 i;

   #define POLY 0x1021      // bit-reversed version of the poly 0x1021

   // Create the CRC "dividend" for polynomial arithmetic (binary arithmetic
   // with no carries)
   CRC_acc = CRC_acc ^ (CRC_input << 8);

   // "Divide" the poly into the dividend using CRC XOR subtraction
   // CRC_acc holds the "remainder" of each divide
   //
   // Only complete this division for 8 bits since input is 1 byte
   for (i = 0; i < 8; i++)
   {
      // Check if the MSB is set (if MSB is 1, then the POLY can "divide"
      // into the "dividend")
      if ((CRC_acc & 0x8000) == 0x8000)
      {
         // if so, shift the CRC value, and XOR "subtract" the poly
         CRC_acc = CRC_acc << 1;
         CRC_acc ^= POLY;
      }
      else
      {
         // if not, just shift the CRC value
         CRC_acc = CRC_acc << 1;
      }
   }

   // Return the final remainder (CRC value)
   return CRC_acc;
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
// P0.5 - digital  push-pull   LED
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0SKIP |= 0x01;                        // Skip P0.0

   P0MDOUT |= 0x02;                       // Set TX pin to push-pull
   P0MDOUT |= 0x20;                       // Enable LED as a push-pull


   XBR0 = XBR0_URT0E__ENABLED;            // Enable UART0

   XBR1 = XBR1_XBARE__ENABLED |           // Enable crossbar
          XBR1_WEAKPUD__PULL_UPS_ENABLED; //  with weak pullups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// TIMER2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure Timer2 to 16-bit auto-reload, but do not start
//
//-----------------------------------------------------------------------------
void TIMER2_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = TIMER2_PAGE;

   TMR2CN = TMR2CN_T2XCLK__SYSCLK_DIV_12_CAP_RTC | // Clock based on SYSCLK/12
            TMR2CN_TR2__STOP |                     // Stop Timer2
            TMR2CN_TF2CEN__DISABLED |              // Disable capture mode
            TMR2CN_TF2LEN__DISABLED;               // No Interrupt

   CKCON  |= CKCON_T2ML__SYSCLK;             // Timer2 clocked based on SYSCLK

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 for <BAUDRATE> and 8-N-1.
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
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


   TL1 = TH1;                          // Init Timer1 reload values

   TMOD &= ~(TMOD_T1M__FMASK |         // Clear Timer1 bits
             TMOD_CT1__BMASK |
             TMOD_GATE1__BMASK);
   TMOD |=  TMOD_T1M__MODE2 |          // 8-bit Counter/Timer with auto-reload
            TMOD_CT1__TIMER |          // Timer that increments based on CKCON
            TMOD_GATE1__DISABLED;      // Ignore ext INT0 line

   TCON_TR1 = TCON_TR1__RUN;           // Start Timer1


   SFRPAGE = UART0_PAGE;
   SCON0_TI = SCON0_TI__SET;           // Indicate TX0 ready

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
