//-----------------------------------------------------------------------------
// CP220x_ETH.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// 
// This file contains basic send and receive functions for the CP220x.
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
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "global.h"

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void CP220x_Send( MACADDRESS* pDestAddr, unsigned char* buffer, 
                  unsigned int buffer_length, unsigned int packet_type);

unsigned int CP220x_Receive(unsigned char* buffer, unsigned int buffer_length);

//-----------------------------------------------------------------------------
// CP220x_Send
//-----------------------------------------------------------------------------
// 
// Return Value : None
// Parameters   : 
//   1)  MACADDRESS* pDestAddr - destination MAC address. 
//   2)  unsigned char* buffer - address of payload. 
//   3)  unsigned int buffer_length - length of payload. 
//   4)  unsigned int packet_type - contents of Ethertype field.
//
// This function sends an IEEE 802.3 Ethernet packet using the CP220x.
// Upon entry, there should be valid data in array <buffer>.
//
//        (8 bytes)  48-bit  48-bit    16-bit   0-1500 bytes   
//  ----------------------------------------------------------------------
// | Preamble| SFD | Dest |Source| Type/Length |Data Field | Pad |  FCS  |
// |         |     | Addr | Addr |    Field    |           |     | (CRC) |
//  ----------------------------------------------------------------------
//    supplied by  |         supplied by the MCU                 | supplied
//      CP220x     |          (minimum 64 bytes)                 | by CP220x  
//
//
//-----------------------------------------------------------------------------
void CP220x_Send( MACADDRESS* pDestAddr, unsigned char* buffer, 
                  unsigned int buffer_length, unsigned int packet_type)
{

   int i; 
   unsigned int ramaddr;

   // Define Macro to increment the RAM address Pointer
   #define INC_RAMADDR  ramaddr++; \
                        RAMADDRH = (ramaddr >> 8);\
                        RAMADDRL = (ramaddr & 0x00FF);


   // Step 1: Poll TXBUSY until it becomes 0x00
   while(TXBUSY);

   // Step 2: Set the TXSTARTH:TXSTARTL address to 0x0000
   TXSTARTH = 0x00;
   TXSTARTL = 0x00;


   // Step 3: Load data into transmit buffer
   // When the random access method is used, we do not need to check for
   // aborted packets. This method will be slightly slower than the Autowrite
   // method, however, it reduces code space requirements.
  
      // Setup RAM Address Pointer To 0x0000	
      RAMADDRH = 0x00;
      RAMADDRL = 0x00;
      ramaddr = 0x0000;

      // Step 3a: Load the destination address
      for(i = 0; i < 6; i++){
         
         RAMTXDATA = pDestAddr->Char[i];
         INC_RAMADDR

      }

      // Step 3b: Load the source address
      for(i = 0; i < 6; i++){
         RAMTXDATA = MYMAC.Char[i];
         INC_RAMADDR
      }
      
      // Step 3c: Load the Type/Length Field
      RAMTXDATA = (packet_type >> 8);
      INC_RAMADDR

      RAMTXDATA = (packet_type & 0xFF);
      INC_RAMADDR


      // Step 3d: Load the packet payload
      for(i = 0; i < buffer_length; i++){
         RAMTXDATA = buffer[i];
         INC_RAMADDR
      }
      
      // Step 3e: Pad short packets
      while(ramaddr < 64){
         RAMTXDATA = 0;
         INC_RAMADDR
      }
      
      // Set the TXENDH:TXENDL address to <ramaddr - 1>
      ramaddr--;
      TXENDH = (ramaddr >> 8);
      TXENDL = (ramaddr & 0x00FF);


   // Step 4: Set the TXSTARTH:TXSTARTL address back to 0x0000
   TXSTARTH = 0x00;
   TXSTARTL = 0x00;
   
   // Step 5: Write '1' to TXGO to begin transmission
   TXCN = 0x01;
 
}


//-----------------------------------------------------------------------------
// CP220x_Receive
//-----------------------------------------------------------------------------
// 
// This function reads the current packet from the CP220x receive buffer and 
// copies it to the passed buffer. The data copied to the buffer includes the
// 14-byte Ethernet Header and the Data Field. 
//
// Returns the number of bytes added to the buffer.
//
//        (8 bytes)  48-bit  48-bit    16-bit       0-1500 bytes   
//  --------------------------------------------------------------------------
// | Preamble | SFD | Dest | Source | Type/Length |  Data Field | Pad |  FCS  |
// |          |     | Addr |  Addr  |    Field    |             |     | (CRC) |
//  --------------------------------------------------------------------------
//     supplied by  |           supplied by the MCU             | supplied by 
//       CP220x     |                                           |    CP220x  
//-----------------------------------------------------------------------------
unsigned int CP220x_Receive(unsigned char* buffer, unsigned int buffer_length)
{
   bit rx_ok;
   bit skip = 0;
   UINT cplen;   
   unsigned int i;

   // Step 1: Check the RXOK bit to see if packet was received correctly
   rx_ok = (CPINFOL & RXOK) && (CPINFOH & RXVALID);
   
   // Step 2: If packet received correctly, read the length, otherwise, skip packet.
   if(rx_ok){
   
      // Read the packet length
      cplen.Char[0] = CPLENH;
      cplen.Char[1] = CPLENL;
        
   } else {
     
      // Set packet length to zero
      cplen.Int = 0;
      
      // Skip packet
      skip = 1;      
   }   

   // Step 3: Read the entire packet from the buffer 
   
   // If packet will fit in the buffer
   if(buffer_length >= cplen.Int){
      
      // Copy entire packet
      for(i = 0; i < cplen.Int; i++){
         *buffer++ = RXAUTORD;
      }

   } else {

      // Set packet length to zero
      cplen.Int = 0;
      
      // Skip packet
      skip = 1; 
   }
      
   // Step 4: Skip the packet, or clear the valid bit if the entire packet
   // has been unloaded from the buffer.
   
   if(skip)
   {
      RXCN |= 0x02;                    // Skip the packet
   } 

   else 
   {
      RXCN |= 0x04;                    // Clear the valid bit only
   }

   // If there are no more packets in the receive buffer, enable reception
   if(TLBVALID == 0x00)
   {
      RXCN = 0x00;   
   }
   
   // Return the number of bytes added to the buffer
   return cplen.Int;
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------