//-----------------------------------------------------------------------------
// MCU_Init.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// 
// This file contains generic host controller initializaion function prototypes. 
// It may be used to add support for new microcontroller families. To add support
// for a new family, create a "Fxxx_Init.c" file and provide definitions for the 
// functions in this header file.
// 
// FID:            
// Target:         Multiple Device Families
// Tool chain:     Keil C51 7.20 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.72 
// Command Line:   See Readme.txt
// Project Name:   CP220x_Ethernet_Routines
//
// 
//
// Release 1.1
//    - Configures C8051F120 SYSCLK to 98 MHz
//
// Release 1.0
//    -Initial Release (FB)
//    -30 MAY 2006
//

//-----------------------------------------------------------------------------
// MCU Register Definition
//-----------------------------------------------------------------------------

#if(MCU == F340)
   #include "C8051F340.h"
#elif(MCU == F120)
   #include "C8051F120.h"
#endif

//-----------------------------------------------------------------------------
// Bit Addressable Pin Definitions
//-----------------------------------------------------------------------------

#if(BOARD == CP2201EB)

   sbit INT_PIN = P0^7;
   sbit YELLOW_LED = P2^7;
   sbit CP2201_RST = P0^6;
   sbit RX = P0^5;

#elif(BOARD == F340_TB)

   sbit INT_PIN  = P0^3;
   sbit YELLOW_LED = P1^3;
   sbit AB4_SW1  = P1^1;
   sbit AB4_SW2  = P1^2;
   sbit AB4_LED1 = P1^3;
   sbit AB4_LED2 = P1^4;
   sbit RX = P0^5;

#elif(BOARD == F120_TB)

   sbit INT_PIN  = P0^3;
   sbit YELLOW_LED = P2^2;
   sbit AB4_SW1  = P2^0;
   sbit AB4_SW2  = P2^1;
   sbit AB4_LED1 = P2^2;
   sbit AB4_LED2 = P2^3;


#endif   

//-----------------------------------------------------------------------------
// Required Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Reset_Init
//-----------------------------------------------------------------------------
// Must initialize the Port I/O, and EMIF. Also must disable the watchdog 
// timer and enable the VDD Monitor.
//
extern void Reset_Init(void);

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
// Must initialize the system clock.
//
extern void SYSCLK_Init (void);

//-----------------------------------------------------------------------------
// wait_ms
//-----------------------------------------------------------------------------
// Must insert a 1 ms delay using an on-chip timer.
// 
extern void wait_ms(int ms);              

//-----------------------------------------------------------------------------
// Other support routines
//-----------------------------------------------------------------------------
// Provide a method of implementing timeouts. See 'F34x_Init.c for an example.
// 
extern void clear_ms_timer_flag(void);
extern char get_ms_timer_flag(void);
extern void start_ms_timer();
extern void reset_timeout(unsigned int ms);
extern unsigned char timeout_expired(void);

//-----------------------------------------------------------------------------
// CP220x_RST_Low, CP220x_RST_High
//-----------------------------------------------------------------------------
// Sets the CP220x Reset Pin High or Low
// 
// Note: These functions automatically handles SFR Paging (if necessary)
// 
extern void CP220x_RST_Low(void);
extern void CP220x_RST_High(void);

//-----------------------------------------------------------------------------
// AB4_RST_State
//-----------------------------------------------------------------------------
// Returns the state of the reset pin.
// 
// Note: These functions automatically handle SFR Paging (if necessary)
// 
extern unsigned char AB4_RST_State(void);

//-----------------------------------------------------------------------------
// Overloaded UART routines
//-----------------------------------------------------------------------------
// Standard UART routines overloaded to address SFR paging.
// 
#if(UART_ENABLED)                         
extern char _getkey ();                   
extern char putchar (char c);             
#endif 
                                   
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------                                          
                                          
                                       
                                                                                    
                                          

                                         
