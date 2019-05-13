//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// 
// This file contains the main routine for the CP220x Send and Receive example.
// 
// FID:            
// Target:         Multiple
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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "global.h"

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void main(void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

IPADDRESS  xdata MYIP;
MACADDRESS xdata MYMAC; 
MACADDRESS xdata BROADCASTMAC;
MACADDRESS xdata DESTMAC;

unsigned char xdata RX_BUFF[1500];
unsigned char xdata TX_BUFF [256];

//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main(void)
{	
   unsigned char error_code;
   unsigned int i;
   unsigned int num_bytes;

   //----------------------------------
   // MCU Initialization
   //----------------------------------
   
   // Initialize the MCU for Ethernet
   Reset_Init();
   
   // Turn off yellow LED.   
   YELLOW_LED = 0;

   //----------------------------------
   // Initialize Global Variables
   //----------------------------------

   // Initialize broadcast MAC address 
   BROADCASTMAC.Int[0] = 0xFFFF;
   BROADCASTMAC.Int[1] = 0xFFFF;
   BROADCASTMAC.Int[2] = 0xFFFF;
   
   // Initialize destination MAC address 
   DESTMAC.Int[0] = 0x0012;
   DESTMAC.Int[1] = 0x3F51;
   DESTMAC.Int[2] = 0x3589;

   // Initialize MYIP address 
   MYIP.Char[0] = 0;
   MYIP.Char[1] = 0;
   MYIP.Char[2] = 0;
   MYIP.Char[3] = 1;
   
   // Initialize Transmit Buffer
   for(i = 0; i < sizeof(TX_BUFF); i++)
   {
      TX_BUFF[i] = 0;
   }

   //----------------------------------
   // Infinite Execution Loop
   // This loop allows the CP220x
   // to be re-initialized each time
   // it is unplugged from a network.
   //----------------------------------
   
   do{ 
      
      //-------------------------------
      // Initialize the CP220x
      //-------------------------------
      
      //-----------------------------------------------------------------------
      // Part A: Hardware Reset and Reset Initialization
      //
      // Perform the hardware reset and recommended reset initialization 
      // procedure listed in the CP220x datasheet.
      //
      // After completion, check the return code for any of the following
      // possible errors:
      //    OSC_ERROR - oscillator error 
      //    CAL_ERROR - self-initialization error
      //    MEM_ERROR - external memory bus error
      //-----------------------------------------------------------------------
      
      EA = 0;                          // Disable Interrupts
      
      error_code = CP220x_HW_Reset();  // Reset and Initialize the CP220x
   
      // Check for the three possible error codes  
      if(error_code & (OSC_ERROR | CAL_ERROR | MEM_ERROR))
      {
         // Print an Error Message
         #if(UART_ENABLED)
         printf("\n\n *** Please Reset Device ***\n");
         #endif 
         
         // Place device in an infinite loop if an error occurs   
         while(1);
      }
      
      //-----------------------------------------------------------------------
      // Part B: Physical Layer Initialization
      //
      // Initialize the physical layer (PHY) for Autonegotiation
      //
      // After completion, check the return code for any of the following
      // possible errors:
      //    LINK_ERROR - The device is not connected to a network or 
      //                 autonegotiation has failed. 
      //
      // If an error occurs, keep re-trying until the device is able to 
      // complete a successful auto-negotiation or is plugged into a network.    
      //-----------------------------------------------------------------------

      while(1){
            
         error_code = PHY_Init();      // Initialize the physical layer
         
         if(error_code & LINK_ERROR)   // Check for the possible error codes
         {                             // On Failure:
            YELLOW_LED = !YELLOW_LED;  // Toggle indicator
         } 
         
         else                          
         {                             // On Success:   
            YELLOW_LED = 1;            // Turn on the indicator
            break;                     // Exit initialization loop 
         }

      }

      //-----------------------------------------------------------------------
      // Part C: MAC Initialization
      //
      // Initialize the media acccess controller (MAC) 
      //
      // Currently, the MAC initialization routine does not return any errors. 
      //-----------------------------------------------------------------------
      
      MAC_Init();                      // Initialize the MAC
                             
      
      //-----------------------------
      // Main Application Loop
      //-----------------------------   
      
      // Initialize timer to time out in one second
      reset_timeout(ONE_SECOND);

      // Reset receive buffer and enable interrupts
      RXCN = RXCLEAR;
      EA  =  1;
      
      while(1){
         
         //--------------------------------------------------------------------
         // Task #1 
         // Exit application loop if device is removed from network
         //--------------------------------------------------------------------
         if(!(PHYCN & LINKSTA))        // Check the link status bit
         {                             // On detection of bad link:
            YELLOW_LED = 0;            // Turn off the indicator
            break;                     // Exit application loop
         }
         
         //--------------------------------------------------------------------
         // Task #2
         // Check if a packet has been received
         //--------------------------------------------------------------------
         if(CPINFOH & RXVALID)         // Check if the current packet is valid
         {                             // On detection of a valid packet:
      
            // Unload packet from the receive buffer and store in <RX_BUFF>   
            num_bytes = CP220x_Receive(RX_BUFF, sizeof(RX_BUFF));
            
            //-----------------------------------------------------------------
            // Application-Specific Code:
            // If UART is enabled, print packet statistics to the terminal.
            //-----------------------------------------------------------------
            #if(UART_ENABLED)
            
               // Print the number of bytes received.      
               printf("Received Packet (%i bytes)\n", num_bytes);
           
               // Print each byte to the UART terminal
               for(i = 0; i < num_bytes; i++)
               {
   	            // Create a new row every 16 bytes
                  if((i & 0x000F) == 0x0000)
                  { 
                     // Print the row address in the left column
                     printf("\n%04X   ", i);
                  }
               
                  // Print the byte
                  printf("%02bX ", RX_BUFF[i]);
               }
            
               // Print two new-line characters for clarity (whitespace).
               printf("\n\n");
      	   
            #endif
      
         } 

         //--------------------------------------------------------------------
         // Task #3
         // Check if timer has expired. Send one packet every second.
         //--------------------------------------------------------------------
         if(timeout_expired()){
            
            // Reset timer to time out again after another second.
            reset_timeout(ONE_SECOND);
            
            // Send one packet.
            CP220x_Send(&DESTMAC, TX_BUFF, sizeof(TX_BUFF), IP_PACKET);
         }      
      
      //-----------------------------
      // End Main Application Loop
      //----------------------------- 
      }          

   

   // Main do-while initialization loop -- code execution will now restart 
   } while(1);
}

//-----------------------------------------------------------------------------
// Ethernet_ISR
//-----------------------------------------------------------------------------
//
// This Interrupt Service Routine checks the number of packets in the receive
// buffer after each packet is received. If the buffer has 7 or more packets,
// then packet reception is disabled. Packet reception is re-enabled in the 
// CP220x_Receive() routine after all packets have been unloaded.
//
// Allowed Latency: This interrupt service routine should be serviced within
//                  51.2 us of the interrupt flag. This is the amount of time
//                  it takes to receive a minimum-sized 64-byte packet. To 
//                  allow for greater latency (51.2us/packet), the maximum 
//                  number of packets allowed in the receive buffer can be 
//                  decreased.
//-----------------------------------------------------------------------------
void Ethernet_ISR(void)
{
   unsigned char interrupt_read;
   unsigned char valid_bits;
   unsigned char num_packets;

   // Clear interrupt flags.
   interrupt_read = INT1;
   interrupt_read = INT0;
   
   // Check for packet received interrupt
   if( interrupt_read & RXINT)
   {   
      // Count the number of packets in the receive buffer         
      // This is equal to the number of bits set to 1 in TLBVALID
      valid_bits = TLBVALID;     
                           
      // Loop accumulates the number of bits set in Valid_Bits and 
      // stores this value in i. Uses the Brian Kernighan method 
      // for counting bits
      for(num_packets = 0; valid_bits; num_packets++)      
      {                                 
         valid_bits &= valid_bits - 1; 
      }
   
      // If the receive buffer has 7 packets, then disable reception.
      if( num_packets >= 7) 
      {
         RXCN = RXINH;           // Inhibit New Packet Reception
      }
   } 
   
   // Check for the FIFO Full Interrupt
   else if (interrupt_read & RXFINT)
   {
      // This interrupt could mean a few large packets were received,
      // or a large number of small packets were received.      
   } 
   
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------