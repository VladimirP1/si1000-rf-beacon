//-----------------------------------------------------------------------------
// F85x_ADC0_ExternalInput.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// --------------------
//
// This example code takes and averages 2048 analog measurements from input
// P1.2 using ADC0, then prints the results to a terminal window via the UART.
//
// The system is clocked by the internal 24.5 MHz oscillator divided by 1.
// Timer 2 triggers a conversion on ADC0 on each overflow.  The completion of
// this conversionin turn triggers an interrupt service routine (ISR).  The ISR
// averages 2048 measurements, then prints the value to the terminal via printf
// before starting another average cycle.
//
// The analog multiplexer selects P1.2 (ADC0.10) as the positive ADC0 input.
// This pin is configured as an analog input in the port initialization
// routine. The negative ADC0 input is connected via mux to ground, which
// provides for a single-ended ADC input.
//
// A 100 kohm potentiometer may be connected (J27) as a voltage divider between
// VDD (3.30 V) and AGND on the terminal strip as shown below:
//
// ---------
//          |
//         o| VDD  ----|
//         o| GND   ---|<-|
//         o| P1.2     |  |
//         o|    |        |
//         o|     --------
//         o|
//          |
//----------
// C8051F850 UDP MCU Card
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
// In this application, assume a 100 kohm potentiometer as the voltage divider.
// The expression evaluates to:
//
//                   | 2^10 |
//    Tsettle =   ln | ---- | * 105e3 * 5e-12 = 4.4 us
//                   | 0.25 |
//
// In addition, one must allow at least 1.5uS after changing analog mux
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
// F850 Resources:
// ---------------
// Timer1: clocks UART
// Timer2: overflow initiates ADC conversion
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J27: POT - P1.2/ADC0.10
//    - J7: all
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to the 'F85x UDP MCU board.
// 4) Connect a USB cable from the development board to a PC,
// 5) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the virtual COM port at <BAUDRATE> (115200) and 8-N-1.
// 6) Run the code.
// 7) The terminal program will print the voltage measured by the device.
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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>            // SFR declarations
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // SYSCLK frequency in Hz
#define BAUDRATE       115200          // Baud rate of UART in bps

SBIT (LED, SFR_P1, 0);                 // DS5 P1.0 LED

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void Port_Init (void);
void Timer2_Init(void);
void ADC0_Init(void);
void UART0_Init (void);

INTERRUPT_PROTO (ADC_ISR, INTERRUPT_ADC0_EOC);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   SYSCLK_Init ();                     // Initialize system clock to 24.5 MHz
   Port_Init ();                       // Initialize crossbar and GPIO
   Timer2_Init();                      // Init Timer2 to generate
                                       // Overflows to trigger ADC
   UART0_Init();                       // Initialize UART0 for printf's
   ADC0_Init();                        // Initialize ADC0

   EA = 1;                             // Enable global interrupts
   while (1) {}
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
// Return Value:  None
// Parameters:    None
//
// Configure the Crossbar and GPIO ports.
//
// P0.4 - UART TX (push-pull)
// P0.5 - UART RX
//
// P1.2 - ADC0 analog input (ADC0.10)
// P1.3 - LED (push-pull)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P0MDOUT |= 0x10;                    // Set TX pin to push-pull
   P1MDOUT |= 0x01;                    // Enable LED as a push-pull output
   P1MDIN &= ~0x04;                    // Set P1.2 as an analog input

   XBR0 = 0x01;                        // Enable UART0
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 100 us
// intervals.  Timer 2 overflow automatically triggers ADC0 conversion.
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                       // Use SYSCLK as timebase, 16-bit
                                       // auto-reload
   CKCON |= 0x10;                      // Select SYSCLK for timer 2 source
   TMR2RL = 65535 - (SYSCLK / 10000);  // Init reload value for 10 us
   TMR2 = 0xFFFF;                      // Set to reload immediately
   TR2 = 1;                            // Start Timer2
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures ADC0 to make single-ended analog measurements on the ADC0.10
// input (P1.2).
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   ADC0CN0 = 0x02;                     // ADC0 disabled, conversions triggered
                                       // on Timer2 overflows

   ADC0CF = (((SYSCLK)/12250000)-1)<<3; // Set SAR clock to 12.25 MHz

   ADC0CF |= 0x01;                     // 1x gain, normal tracking mode,
                                       // 10-bit mode

   ADC0AC = 0x00;                      // 12-bit mode disabled, accumulation
                                       // disabled, right-justified, repeat
                                       // count set to 1

   ADC0PWR = 0x00;                     // Low power mode disabled

   ADC0MX = 0x0A;                      // ADC0 positive input = ADC0.10 or
                                       // P1.2

   REF0CN = 0x08;                      // Ground reference is the GND pin,
                                       // voltage reference is the VDD pin
                                       // (3.3 V), temperature sensor disabled

   EIE1 |= 0x08;                       // Enable ADC0 conversion complete
                                       // interrupt

   ADEN = 1;                           // Enable ADC0
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
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
      CKCON |=  0x08;                  // T1M = 1; SCA1:0 = xx
   } else if (SYSCLK/BAUDRATE/2/256 < 4) {
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
   } else if (SYSCLK/BAUDRATE/2/256 < 12) {
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } else if (SYSCLK/BAUDRATE/2/256 < 48) {
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   } else {
      while (1);                       // Error.  Unsupported baud rate
   }

   TL1 = TH1;                          // init Timer1
   TMOD &= ~0xF0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TR1 = 1;                            // START Timer1
   TI = 1;                             // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ADC0_ISR
//-----------------------------------------------------------------------------
//
// This ISR averages 2048 samples then prints the result to the terminal.  The
// ISR is called after each ADC conversion which is triggered by Timer2.
//
//-----------------------------------------------------------------------------
INTERRUPT (ADC_ISR, INTERRUPT_ADC0_EOC)
{
   static unsigned long accumulator = 0;     // Accumulator for averaging
   static unsigned int measurements = 2048;  // Measurement counter
   unsigned long result = 0;
   unsigned long mV;                         // Measured voltage in mV

   ADINT = 0;                                // Clear ADC0 conv. complete flag

   accumulator += ADC0;
   measurements--;

   if(measurements == 0)
   {
      measurements = 2048;
      result = accumulator / 2048;
      accumulator = 0;

      // The 10-bit ADC value is averaged across 2048 measurements.
      // The measured voltage applied to P0.7 is then:
      //
      //                           Vref (mV)
      //   measurement (mV) =   --------------- * result (bits)
      //                       (2^10)-1 (bits)

      mV =  (result * 3300) / 1023;
      printf("P1.2 voltage: %ld mV\n",mV);
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
