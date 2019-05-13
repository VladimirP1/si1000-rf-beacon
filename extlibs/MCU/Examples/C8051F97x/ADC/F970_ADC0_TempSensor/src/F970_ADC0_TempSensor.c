//-----------------------------------------------------------------------------
// F970_ADC0_TempSensor.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This example code takes and averages 256 analog measurements from on-chip
// Temperature Sensor using ADC0, then prints the results to a terminal window
// via the UART.
//
// The system is clocked by the internal 24.5 MHz oscillator divided by 1.
// Timer 2 triggers a conversion on ADC0 on each overflow.  The completion of
// this conversion in turn triggers an interrupt service routine (ISR).  The ISR
// averages 256 measurements, then prints the value to the terminal via printf
// before starting another average cycle.
//
// The analog multiplexer selects Temperature Sensor as the positive ADC0 input.
// The negative ADC0 input is connected to ground, which provides
// for a single-ended ADC input.
//
// Terminal output is done via printf, which directs the characters to
// UART0.  A UART initialization routine is therefore necessary.
//
// ADC Settling Time Requirements, Sampling Rate:
// ----------------------------------------------
//
// The total sample time per input is comprised of an input setting time
// (Tsettle), followed by a conversion time (Tconvert):
//
// Tsample  = Tsettle + Tconvert
//
// |--------Settling-------|==Conversion==|----Settling--- . . .
// Timer 2 overflow        ^
// ADC0 ISR                               ^
//
// The ADC input voltage must be allowed adequate time to settle before the
// conversion is made.  This settling depends on the external source
// impedance, internal mux impedance, and internal capacitance.
// Settling time is given by:
//
//                   | 2^n |
//    Tsettle =   ln | --- | * Rtotal * Csample
//                   | SA  |
//
// In this application, assume a 10 kohm potentiometer as the voltage divider.
// The expression evaluates to:
//
//                   | 2^10 |
//    Tsettle =   ln | ---- | * 105e3 * 5e-12 = 4.4 us
//                   | 0.25 |
//
// In addition, one must allow at least 1.5 us after changing analog mux
// inputs or PGA settings.  The settling time in this example, then, is
// dictated by the large external source resistance.
//
// The conversion is given in the datasheet (10-bit mode, SAR clock 12.25 MHz)
// as 1 us.
//
// Tsample, minimum  = Tsettle + Tconvert
//                   = 4.4 us + 1 us
//                   = 5.4 u
//
// Timer2 is set to start a conversion every 100 us, which is far longer
// than the minimum required.
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
// 7) The terminal program will print the temperature measured by the device.
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

#define SYSCLK       24500000            // SYSCLK frequency in Hz
#define BAUDRATE       115200            // Baud rate of UART in bps
#define TEMP_OFFSET      1025            // Temperature Offset @25 degree C
#define TEMP_SLOPE         34            // Temperature slope 3.4 * 10

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void TIMER2_Init (void);
void UART0_Init (void);
void ADC0_Init (void);

INTERRUPT_PROTO (ADC_ISR, ADC0EOC_IRQn);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;

   SYSCLK_Init ();                      // Initialize system clock
   PORT_Init ();                        // Initialize crossbar and GPIO
   TIMER2_Init ();                      // Init Timer2 for ADC conversion clock
   UART0_Init ();                       // Initialize UART0 for printf's
   ADC0_Init ();                        // Initialize ADC0

   IE_EA = 1;                           // Enable global interrupts

   while (1);                           // Spin forever
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
// P0.0 - digital  push-pull   POT VDD ('F970-TB Errata)
// P0.1 - digital  push-pull   UART TX
// P0.2 - digital  open-drain  UART RX
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0SKIP |= 0x01;                        // Skip P0.0

   P0MDOUT |= 0x02;                       // Set TX pin to push-pull


   XBR0 = XBR0_URT0E__ENABLED;            // Enable UART0

   XBR1 = XBR1_XBARE__ENABLED |           // Enable crossbar
          XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// TIMER2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 100 us
// intervals. Timer 2 overflow automatically triggers ADC0 conversion.
//
//-----------------------------------------------------------------------------
void TIMER2_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = TIMER2_PAGE;

   TMR2CN = TMR2CN_T2XCLK__SYSCLK_DIV_12_CAP_CMP | // Clock based on SYSCLK/12
            TMR2CN_TR2__STOP |                     // Stop Timer2
            TMR2CN_TF2CEN__DISABLED |              // Disable capture mode
            TMR2CN_TF2LEN__DISABLED;               // No Interrupt

   CKCON  |= CKCON_T2ML__EXTERNAL_CLOCK;           // Timer2 uses T2XCLK

   TMR2RL = 65535 - (SYSCLK / 12 / 10000);         // Init reload value for 100 us
   TMR2 = 0xFFFF;                                  // Set to reload immediately
   TMR2CN_TR2 = TMR2CN_TR2__RUN;                   // Start Timer2

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
   SCON0_TI = 1;                       // Indicate TX0 ready

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures ADC0 to make single-ended analog measurements on pin P1.2
// Also enabled the gain and the voltage reference
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;

   SFRPAGE = ADC0_PAGE;

   REF0CN = REF0CN_TEMPE__TEMP_ENABLED |  // Enable Internal Temp Sensor
            REF0CN_REFSL__VDD_PIN;        // VDD is VREF

   ADC0CN_ADBMEN = ADC0CN_ADBMEN__BURST_DISABLED; // Set SAR clock to SYSCLK

   ADC0CF = (0 << ADC0CF_ADSC__SHIFT) |
            ADC0CF_AD8BE__NORMAL |      // ADC is in 10 bit mode
            ADC0CF_ADTM__TRACK_NORMAL | // Normal tracking
            ADC0CF_ADGN__GAIN_1;        // Unity gain

   ADC0CN &= ~ADC0CN_ADCM__FMASK;       // Use Timer2 to trigger sampling
   ADC0CN |= ADC0CN_ADCM__TIMER2;

   ADC0MX = ADC0MX_ADC0MX__TEMP;        // Select Temperature Sensor as input

   EIE1 |= EIE1_EADC0__ENABLED;         // Enable ADC0 conversion complete interrupt

   ADC0CN_ADEN = 1;                     // Enable ADC0


   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// ADC0_ISR
//-----------------------------------------------------------------------------
//
// This ISR averages 256 samples then prints the result to the terminal.  The
// ISR is called after each ADC conversion which is triggered by Timer2.
//
//-----------------------------------------------------------------------------
INTERRUPT (ADC_ISR, ADC0EOC_IRQn)
{

   static U32 accumulator = 0;         // Accumulator for averaging
   static U16 measurements = 256;      // Measurement counter
   U32 result = 0;
   S16 mV;                             // Measured voltage in mV
   S16 Temp;

   ADC0CN_ADINT = 0;                   // Clear ADC0 conv. complete flag

   accumulator += ADC0;                // Add latest sample
   measurements--;                     // Subtract counter

   if (measurements == 0)
   {
      measurements = 256;
      result = accumulator / 256;
      accumulator = 0;

      // Convert result to fixed point
      mV = result * 3300 / 1024;
      Temp = 25 + ((mV - TEMP_OFFSET) * 10 / TEMP_SLOPE);

      // SFR page is already correct for printf (SBUF0 page)
      printf("\nTemperature value: %i\n", Temp);
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
