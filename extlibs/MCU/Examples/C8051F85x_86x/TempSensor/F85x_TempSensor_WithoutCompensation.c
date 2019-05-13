//-----------------------------------------------------------------------------
// F85x_TemperatureSensor_WithoutCompensation.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program shows the use of the temperature sensor along with the ADC.
// Without calibration, the temperature accuracy is about +/- 10 degrees C.
//
// The ADC is set to left-justified mode. When in this mode, the ADC uses bits
// 6 through 15 to output the ADC readings with the decimal point being to the
// left of bit 15 instead of to the right of bit 0 in right-justified mode.
// The ADC0 reading lookings like:
//
//    Left-justified:     0.xxxxxxxxxx000000
//    Right-justified:    000000xxxxxxxxxx.0
//
//    where x's represent the readings from the ADC, and 0's are always 0
//
// This mode's output is fractional since the decimal point is on the left
// side. Since the ADC output is fractional, we can directly multiply the ADC
// reading with the VREF voltage and find the voltage of the temp sensor
// reading.
//
// The only caveat of this is that we have to keep track of where the decimal
// point is after multiplication.
//
// This program uses the following equation from the datasheet:
//
//    Vtemp = 2.85T(C) + 757mV
//
// Moving T(C) to the left-hand side:
//
//    T(C) = (1 / 2.85)Vtemp - (757 mV / 2.85)
//
// In this instance, the equivalent linear equation is given by:
//
//    T(C) = 0.351Vtemp - 265.61 (V in millivolts)
//
// Converting from V to an ADC reading:
//
//    T(C) = (0.351*VREF*ADC) - 265.61;
//
// Sampling 256 times:
//
//    T(C) = (0.351*VREF)*(ADC_sum/256) - 265.61;
//
// Assuming a VREF = 1650 mV:
//
//    T(C) = 2.261*ADC_sum - 265.61;
//
// To allow for precise calculations using fixed point math these
// factors are going to be scaled up 1000x
//
// Remarks:
//
//    SLOPE - coefficient of the line that multiplies the ADC_sum value
//    OFFSET - the coefficient to be added or subtracted to the line
//    T(C) - Temperature in Celcius
//    ADC_sum - Sum of the ADC conversions (Oversampled)
//    OVER_ROUND - This variable keeps track of where the decimal place is.
//       We get right-justified by shifting right OVER_ROUND times.
//    Oversampling - For more information on oversampling to improve ADC
//       resolution, please refer to Application Note AN118.
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
//    to the USB port (virtual COM port) at <BAUDRATE> (9600) and 8-N-1.
// 6) Run the code. The format of the output is as follows:
//
//    T = 24.3(C)
//
//    where T is the temperature in Celcius
//
// 7) The temperature will change by simply touching the IC with any warm
//    object (including a finger) or any cold object, like freeze spray (be
//    careful to avoid ESD issues!).
//
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
// Revision History:
//
// Release 1.2 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.1
//    - Added slope/offset for Revision B devices (TP)
//    - 5 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 25 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK                24500000 // SYSCLK frequency in Hz
#define BAUDRATE                  9600 // Baud rate of UART in bps

#define SCALE                    1000L // Scale for temp calculations
#define SAMPLING_NUMBER            256 // Number of samples per calculation
#define OVER_ROUND                  10 // Number of shifts (>>N) to reach the
                                       // correct number of bits of precision

#define REV_B                     0x01 // Value of the REVID register for
                                       // revision B
#define SLOPE_REV_B               2414 // Slope of the temp transfer function
#define OFFSET_REV_B           267041L // Offset for the temp transfer function
                                       // These values are for revision B
                                       // devices with the equation
                                       //     Vtemp = 2.67T(C) + 713mV

#define REV_C                     0x02 // Value of the REVID register for
                                       // revision C
#define SLOPE_REV_C               2261 // Slope of the temp transfer function
#define OFFSET_REV_C           265610L // Offset for the temp transfer function

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

U32 ADC_SUM = 0;                       // Accumulates the ADC samples
bit CONV_COMPLETE = 0;                 // ADC accumulated result ready flag

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init(void);
void Port_Init(void);
void Timer2_Init(void);
void UART0_Init(void);
void ADC0_Init(void);

INTERRUPT_PROTO (ADC0_ISR, INTERRUPT_ADC0_EOC);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   S32 temp_scaled;                    // Stores scaled temperature value
   S32 temp_whole;                     // Stores unscaled whole number portion
                                       // of temperature for output.
   S16 temp_frac;                      // Stores unscaled decimal portion of
                                       // temperature for output.

   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   Oscillator_Init ();                 // Initialize Oscillator
   ADC0_Init ();                       // Initialize ADC0
   Timer2_Init();                      // Init Timer2 to generate
   Port_Init ();                       // Initialize crossbar and GPIO
   UART0_Init ();                      // Initialize UART

   EA = 1;                             // Enable all interrupts

   printf("\f");                       // Clear the screen before outputs

   while (1)                           // Spin forever
   {
      if(CONV_COMPLETE)
      {
         temp_scaled = ADC_SUM;        // Apply our derived equation to ADC

         if (REVID == REV_B)
         {
            temp_scaled *= SLOPE_REV_B;

            // With a left-justified ADC, we have to shift the decimal place
            // of temp_scaled to the right so we can match the format of
            // OFFSET. Once the formats are matched, we can subtract OFFSET.
            temp_scaled = temp_scaled >> OVER_ROUND;

            temp_scaled -= OFFSET_REV_B;
         }
         else
         {
            temp_scaled *= SLOPE_REV_C;

            // With a left-justified ADC, we have to shift the decimal place
            // of temp_scaled to the right so we can match the format of
            // OFFSET. Once the formats are matched, we can subtract OFFSET.
            temp_scaled = temp_scaled >> OVER_ROUND;

            temp_scaled -= OFFSET_REV_C;
         }

         // Calculate the temp's whole number portion by unscaling
         temp_whole = temp_scaled/SCALE;

         // The temp_frac value is the unscaled decimal portion of temperature
         temp_frac = (S16)((temp_scaled - temp_whole*SCALE) / (SCALE/10));

         if(temp_frac < 0)
         {
            temp_frac *= -1;           // If negative, remove negative from
                                       // temp_frac portion for output.
         }

         printf(" T = %ld.%d(C) \r", temp_whole, temp_frac);

         CONV_COMPLETE = 0;
      }
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal oscillator
// at its maximum frequency 24.5 MHz. Also enables the Missing Clock Detector.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock

   RSTSRC = 0x06;                      // Enable missing clock detector
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initialized the GPIO and the Crossbar
//
// P0.4   digital   push-pull    UART TX
// P0.5   digital   open-drain   UART RX
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P0MDOUT |= 0x10;                    // UART-TX push-pull

   XBR0 = 0x01;                        // Enable UART I/O
   XBR2 = 0x40;                        // Enable crossbar
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 100uS
// intervals.  Timer 2 overflows automatically trigger ADC0 conversions.
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK as timebase, 16-bit
                                       // auto-reload
   CKCON |= 0x10;                      // Select SYSCLK for timer 2 source
   TMR2RL = 65535 - (SYSCLK / 10000);  // Init reload value for 10 us
   TMR2 = 0xffff;                      // Set to reload immediately
   TR2 = 1;                            // Start Timer2
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
   TI = 1;                             // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Initialize the ADC to use the temperature sensor.
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   REF0CN = 0x1C;                      // Temp. Sensor on, VREF = 1.65 V

   ADC0MX = 0x10;                      // Select Temp. Sensor

   ADC0AC = 0x20;                      // 10-bit mode, left justified
   ADC0CF |= 0x01;                     // 10-bit mode, normal tracking mode,
                                       // PGA gain = 1
   ADC0CN0 = 0x02;                     // ADC conversion starts on TMR2
                                       // overflow

   ADC0PWR = 0xC0;                     // Select bias current mode 3

   ADC0CN0 |= 0x80;                    // ADC enabled
   EIE1 |= 0x08;                       // Enable ADC0 conversion complete int.
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ADC0_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This ISR averages 256 samples then copies the result to ADC_SUM.
//
//-----------------------------------------------------------------------------

INTERRUPT (ADC0_ISR, INTERRUPT_ADC0_EOC)
{

   static U32 accumulator = 0;         // Accumulator for averaging
   static U16 measurements = SAMPLING_NUMBER; // Measurement counter

   ADINT = 0;                          // Clear ADC0 conv. complete flag

   // Checks if obtained the necessary number of samples
   if(measurements == 0)
   {

      ADC_SUM = accumulator;           // Copy total into ADC_SUM
      measurements = SAMPLING_NUMBER;  // Reset counter
      accumulator = 0;                 // Reset accumulator
      CONV_COMPLETE = 1;               // Set result ready flag
   }
   else
   {
      accumulator += ADC0 >> 6;        // If not, keep adding while shifting
                                       // out unused bits in ADC0
      measurements--;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
