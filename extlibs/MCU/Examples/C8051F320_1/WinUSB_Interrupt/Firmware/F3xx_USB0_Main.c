//-----------------------------------------------------------------------------
// F3xx_USB_Main.c
//-----------------------------------------------------------------------------
// Copyright 2008-2012 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Stub file for Firmware Template.
//
//
// How To Test:    See Readme.txt
//
//
// Target:         C8051F320/1,
//                 C8051F326/7,
//                 C8051F34x,
//                 C8051F38x,
//                 C8051T620/1/6/7, C8051T320/3,
//                 C8051T622/3, C8051T326/7
//
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.6
// Command Line:   See Readme.txt
// Project Name:   USB0_Interrupt
//
// Release 2.0
//    -Rewrite (CM)
//    -02 NOV 2012
//
// Release 1.1
//    -Changed "Usb_Init" to "USB_Init" (TP)
//    -07 OCT 2010
//
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//
//-----------------------------------------------------------------------------
// Header Files
//-----------------------------------------------------------------------------

#include "c8051f3xx.h"
#include "F3xx_USB0_Register.h"
#include "F3xx_USB0_InterruptServiceRoutine.h"
#include "F3xx_USB0_Descriptor.h"
#include "F3xx_USB0_Main.h"

//-----------------------------------------------------------------------------
// Interrupt Prototypes
//-----------------------------------------------------------------------------

INTERRUPT_PROTO (Timer2_ISR, INTERRUPT_TIMER2);
INTERRUPT_PROTO (ADC0_ConvComplete_ISR, INTERRUPT_ADC0_EOC);
INTERRUPT_PROTO (Usb_ISR, INTERRUPT_USB0);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// IN_PACKET and OUT_PACKET buffer bytes immediately before and
// after they are transferred across USB inside the report handlers

SEGMENT_VARIABLE(In_Packet[IN_EP1_PACKET_SIZE], U8, SEG_XDATA);
SEGMENT_VARIABLE(Out_Packet[OUT_EP1_PACKET_SIZE], U8, SEG_XDATA);

//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main(void)
{
   U16 i;

   System_Init ();                     // Initialize Sysclk, Port IO, Timer2, ADC0
   USB0_Init ();                       // Initialize USB0

   // Initialize packets to 0
   for (i = 0; i < sizeof (In_Packet); i++)
      In_Packet[i] = 0;
   for (i = 0; i < sizeof (Out_Packet); i++)
      Out_Packet[i] = 0;

   EA = 1;                             // Enable global interrupts

   while (1)
   {
      // It is possible that the contents of the following packets can change
      // while being updated.  This doesn't cause a problem in the sample
      // application because the bytes are all independent.  If data is NOT
      // independent, packet update routines should be moved to an interrupt
      // service routine, or interrupts should be disabled during data updates.

      // Update LED states
      if (Out_Packet[0] == 1) SetLed(1, 1);
      else                    SetLed(1, 0);
      if (Out_Packet[1] == 1) SetLed(2, 1);
      else                    SetLed(2, 0);

      // Set Port 0 [4-7] pins
      P0 &= ~0xF0;
      P0 |= (Out_Packet[2] & 0xF0);

      In_Packet[0] = Switch1State;        // Send status of switch 1
      In_Packet[1] = Switch2State;        // and switch 2 to host
      In_Packet[2] = (P0 & 0x0F);         // Port 0 [0-3]
      In_Packet[3] = Potentiometer;       // Potentiometer value
      In_Packet[4] = Temperature;         // Temperature sensor value
   }
}
