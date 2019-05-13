//-----------------------------------------------------------------------------
// CP220x_CORE.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// 
// This file contains core functions used when accessing the CP220x.
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
// Global Variables
//-----------------------------------------------------------------------------
bit full_duplex = 0;

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------

// Initialization routines
unsigned char CP220x_HW_Reset(void);
unsigned char PHY_Init(void);
void MAC_Init(void);

// MAC read/write routines
void MAC_Write(unsigned char mac_reg_offset, unsigned int mac_reg_data);
void MAC_GetAddress(MACADDRESS* pMAC);
void MAC_SetAddress(MACADDRESS* pMAC);

// FLASH read/write/erase routines
unsigned char poll_flash_busy (void);
unsigned char CPFLASH_ByteRead (unsigned int addr);
unsigned char CPFLASH_ByteWrite (unsigned int addr, char byte);
unsigned char CPFLASH_PageErase (unsigned int addr);

//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CP220x_HW_Reset
//-----------------------------------------------------------------------------
//
// Return Value : 
//   unsigned char - '0' on success, or one of the following error codes:
//       MEM_ERROR
//       OSC_ERROR
//       CAL_ERROR
//
// Parameters   : None
//
// Performs Steps 1 - 5 of Reset Initialization (See Section 6.2 of the CP220x
// Datasheet for more details)
//-----------------------------------------------------------------------------
unsigned char CP220x_HW_Reset(void)
{  
   unsigned char temp_char;

   // Reset the CP2200 by holding the /RST pin low for at least 15 us
   CP220x_RST_Low();
   wait_ms(20);
   
   //--------------------------------------------------------------------------
   // Step 1: Wait for the reset pin to rise.
   //--------------------------------------------------------------------------
   CP220x_RST_High();
   
   //--------------------------------------------------------------------------
   // Step 2 + 3: Wait for oscillator and self initializaion to complete
   //--------------------------------------------------------------------------
   
   // Start a one second timeout
   reset_timeout(ONE_SECOND*5);

   // Wait for the interrupt pin to go low
   // Loop will exit successfully if interrupt detected
   // The function will return error if the reset pin goes low,
   // or the one second timeout expires
   while(INT_PIN){                        
   
      // Check the state of the reset pin     
      if(!AB4_RST_State()){
         return OSC_ERROR;
      }
      
      // Check the state of the one second timeout
      if(timeout_expired()){
         return OSC_ERROR;
      }
   }

   // Start a new one second timeout
   reset_timeout(ONE_SECOND);
   
   // Wait for Oscillator Initialization and Self Initialization to complete
   // Verify that both SELFINTR and OSCINTR are set and that 
   // INT0 is not reading 0xFF;
   do {
      temp_char = INT0RD;
      
      if(timeout_expired()){
         return CAL_ERROR;
      }
      
   } while(((temp_char & 0x30) != 0x30) || (temp_char == 0xFF)) ;        
   
   //--------------------------------------------------------------------------
   // Additional Step: Verify Communication
   //--------------------------------------------------------------------------

   // Verify communication
   if(RXHASHH != 0x04){
      #if(UART_ENABLED)
      puts("EMIF Failure. Check EMI0CN.");
      #endif
      return MEM_ERROR;
   }   
   
   // Read and write the RAM at 0x7FF in the transmit buffer
   RAMADDRH = 0x07;
   RAMADDRL = 0xFF;

   RAMTXDATA = 0x00;
   if(RAMTXDATA != 0x00){
      #if(UART_ENABLED)
      puts("EMIF Failure. Cannot set data bus to 0x00.");
      #endif
      return MEM_ERROR;
   } 
   
   RAMTXDATA = 0xFF;
   if(RAMTXDATA != 0xFF){
      #if(UART_ENABLED)
      puts("EMIF Failure. Cannot set data bus to 0xFF. Please Reset Device.");
      #endif
      return MEM_ERROR;
   } 
      

   #if(UART_ENABLED)
      puts("Oscillator and Self Initialization Complete\n");
   #endif
 
   //--------------------------------------------------------------------------
   // Step 4: Disable Interrupts For Events that will not be monitored
   //--------------------------------------------------------------------------   
   
   // Disable All Interrupts except for the packet received interrupt
   INT0EN = 0x03;                         
   INT1EN = 0x00;
   
   // Clear all Interrupt Flags by reading the self-clearing status registers                          
   temp_char = INT0;                      
   temp_char = INT1;                      
   
   return 0;
}


//-----------------------------------------------------------------------------
// PHY_Init
//-----------------------------------------------------------------------------
//
// Return Value : 
//   unsigned char - '0' on success, or the following error code:
//       LINK_ERROR
//
// Parameters   : None
//
// Initializes the PHY using Autonegotiation
//-----------------------------------------------------------------------------
unsigned char PHY_Init()
{
   unsigned char temp_char;
   unsigned char retval = 0;
   
   //--------------------------------------------------------------------------
   // Auto-Negotiation Synchronization (Section 15.2 of CP220x Datasheet)
   //--------------------------------------------------------------------------

   // Step 1: Disable the PHY
   PHYCN = 0x00;  
   
   // Step 2: Enable the PHY with link integrity test and auto-negotiation 
   // turned off
      
      // A. Disable the Transmitter Power Save Option and Configure Options
      TXPWR = 0x80;
      PHYCF = ( SMSQ | JABBER | ADPAUSE | AUTOPOL );

      // B. Enable the Physical Layer
      PHYCN = PHYEN;

      // C. Wait for the physical layer to power up
      wait_ms(10);

      // D. Enable the Transmitter and Receiver
      PHYCN = ( PHYEN | TXEN | RXEN );
   
   // Step 3: Poll the Wake-on-Lan Interrupt
      
      // A. Clear Interrupt Flags
      temp_char = INT1;

      // B. Start a new timeout for 1.5 seconds
      reset_timeout(ONE_SECOND+ONE_SECOND/2);
      
      // C. Check for a signal
      while(1)
      {
         // If a signal is detected, wait 250 ms, then continue
         if(INT1RD & WAKEINT){
            wait_ms(250);
            break;
         }

         // If no signal is deteced, wait 1.5s, then continue
         if(timeout_expired()){
            break;
         }

      }

   //--------------------------------------------------------------------------
   // Physical Layer Initialization (Section 15.7 of CP220x Datasheet)
   //--------------------------------------------------------------------------
   
   // Step 1: Synchronization procedure implemented above

   // Step 2: Disable the physical layer
   PHYCN = 0x00; 
   
   // Step 3: Configure the desired physical layer options including 
   // auto-negotiation and link integrity
   PHYCF = ( SMSQ | LINKINTG | JABBER | AUTONEG | ADPAUSE | AUTOPOL );
  
   // Step 4: Enable the physcial layer
      
      // A. Enable the Physical Layer
      PHYCN = PHYEN;

      // B. Wait for the physical layer to power up
      wait_ms(10);

      // C. Enable the Transmitter and Receiver
      // Auto-negotiation begins now
      PHYCN = ( PHYEN | TXEN | RXEN );
  

   // Step 5: Wait for auto-negotiation to complete

      // Clear INT1 Interrupt Flags
      temp_char = INT1;
        
      // Start a six second timeout
      reset_timeout(6*ONE_SECOND);
  
      // Check for autonegotiation fail or complete flag
      while(1){
         // If Auto-Negotiation Completes/Fails, break
         if(INT1RD & (ANCINT | ANFINT)){
            break;            
         }
      
         // If Timeout Expires, break
         if(timeout_expired()){
            break;            
         }
      }


      // Mask out all bits except for auto negotiation bits 
      temp_char = INT1RD;
      temp_char &= (ANCINT | ANFINT);

      // Check if Auto-Negotiation has FAILED 
      if(temp_char & ANFINT){
      
         // Auto-Negotiation has failed
         retval = LINK_ERROR;

         #if(UART_ENABLED)
         puts("Auto-Negotiation Failed -- Check Network Cable");
         #endif 
   
      } else 
   
      // Check if Auto-Negotiation has PASSED   
      if(temp_char == ANCINT){
      
         // Auto-Negotiation has passed
         retval = 0;

         // Enable Link LED and Activity LED                         
         IOPWR = 0x0C;   
         
         #if(UART_ENABLED)
         puts("Auto-Negotiation Passed\n");
         #endif                    

      } else 
   
      // Timeout Occured.   
      { 
          // Timeout
         retval = LINK_ERROR;
    
         #if(UART_ENABLED)
         printf("*** TIMEOUT \n");
         #endif
      }

   return retval;
     
}


//-----------------------------------------------------------------------------
// MAC_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Initializes the MAC and programs the MAC address using the MAC address
// stored at address 0x1FFA in CP220x Flash.
//-----------------------------------------------------------------------------
void MAC_Init(void)
{  

   // Check the duplex mode and perform duplex-mode specific initializations
   if(PHYCN & 0x10){
      
      // The device is in full-duplex mode, configure MAC registers
      // Padding is turned on.
      MAC_Write(MACCF, 0x40B3);
      MAC_Write(IPGT, 0x0015);
      
   } else {

      // The device is in half-duplex mode, configure MAC registers
      // Padding is turned off.
      MAC_Write(MACCF, 0x4012);
      MAC_Write(IPGT, 0x0012);

   }

   // Configure the IPGR register
   MAC_Write(IPGR, 0x0C12);

   // Configure the MAXLEN register to 1518 bytes
   MAC_Write(MAXLEN, 0x05EE);

   // Copy MAC Address Stored in Flash to MYMAC
   FLASHADDRH = 0x1F;
   FLASHADDRL = 0xFA;

   MYMAC.Char[0] = FLASHAUTORD;
   MYMAC.Char[1] = FLASHAUTORD;
   MYMAC.Char[2] = FLASHAUTORD;
   MYMAC.Char[3] = FLASHAUTORD;
   MYMAC.Char[4] = FLASHAUTORD;
   MYMAC.Char[5] = FLASHAUTORD;      

   // Program the MAC address
   MAC_SetAddress(&MYMAC); 

   // Enable Reception and configure Loopback mode
   MAC_Write(MACCN, 0x0001);           // Enable Reception without loopback

}

//-----------------------------------------------------------------------------
// Indirect MAC Register Access
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// MAC_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : 
//   1)  unsigned char mac_reg_offset - indirect register address
//   2)  unsigned int mac_reg_data - the data to write to mac_reg_offset.
//
// Writes the value <mac_reg_data> to the indirect MAC register located at 
// <mac_reg_offset>.
//-----------------------------------------------------------------------------
void MAC_Write(unsigned char mac_reg_offset, unsigned int mac_reg_data)
{

   // Step 1: Write the address of the indirect register to MACADDR.
   MACADDR = mac_reg_offset;              

   // Step 2: Copy the contents of <mac_reg_data> to MACDATAH:MACDATAL
   MACDATAH = (mac_reg_data >> 8);    // Copy High Byte
   MACDATAL = (mac_reg_data & 0xFF);  // Copy Low Byte

   // Step 3: Perform a write on MACRW to transfer the contents of MACDATAH:MACDATAL
   // to the indirect MAC register.
   MACRW = 0;
   
   return;
}


//-----------------------------------------------------------------------------
// MAC_SetAddress
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : 
//   1)  MACADDRESS* pMAC - pointer to a 6-byte MAC address structure.
// 
// Sets the current MAC address to the MAC address pointed to by <pMAC>.
//-----------------------------------------------------------------------------
void MAC_SetAddress(MACADDRESS* pMAC)
{
   UINT temp_int;

   temp_int.Char[0] = pMAC->Char[5];
   temp_int.Char[1] = pMAC->Char[4];
   MAC_Write(MACAD0, temp_int.Int);
   
   temp_int.Char[0] = pMAC->Char[3];
   temp_int.Char[1] = pMAC->Char[2];
   MAC_Write(MACAD1, temp_int.Int);
   
   temp_int.Char[0] = pMAC->Char[1];
   temp_int.Char[1] = pMAC->Char[0];
   MAC_Write(MACAD2, temp_int.Int);
   
   return;
}

//-----------------------------------------------------------------------------
// CP220x Flash Routines
//-----------------------------------------------------------------------------
/* Not used. Commented to save code space.
//-----------------------------------------------------------------------------
// CPFLASH_ByteRead
//-----------------------------------------------------------------------------
//
// Return Value :
//   unsigned char - the value of the Flash byte.
//
// Parameters   : 
//   1)  unsigned int addr - the address in CP220x Flash.
//
// Reads a Flash byte and returns its value.
//-----------------------------------------------------------------------------
unsigned char CPFLASH_ByteRead (unsigned int addr)
{  

   // Set the Flash Address Pointer to <addr>
   FLASHADDRH = (addr >> 8);           // Copy High Byte
   FLASHADDRL = (addr & 0xFF);         // Copy Low Byte
   
   // Read and Return the value in the Flash Data Register 
   return FLASHDATA;
}
//-----------------------------------------------------------------------------
// poll_flash_busy
//-----------------------------------------------------------------------------
//
// Return Value : 
//    unsigned char - Returns '0' on success or FLASH_ERROR if a problem
//    is encountered.
//
// Parameters   : None
//
// Waits for a Flash operation to start and complete
//
// Return Values:
//
//----------------------------------------------------------------------------- 
unsigned char poll_flash_busy (void)
{
 
   // Start Millisecond Timer and set timeout
   reset_timeout(DEFAULT_TIMEOUT);

   // Wait for operation to end
   while((FLASHSTA & 0x08)){       
      
      if(!AB4_RST_State()){
         #if(UART_ENABLED)
         puts("Reset Pin Driven Low. Could indicate power failure.");
         #endif
         return FLASH_ERROR;
      }
    
     if(timeout_expired()){
         #if(UART_ENABLED)
         puts("Timeout: Flash operation has not ended.");
         #endif
         return FLASH_ERROR;
      }

   }
   
   return 0;
}
//-----------------------------------------------------------------------------
// CPFLASH_ByteWrite
//-----------------------------------------------------------------------------
//
// Return Value : 
//    unsigned char - Returns '0' on success or FLASH_ERROR if a problem
//    is encountered.
//
// Parameters   : 
//   1)  unsigned int addr - the address of the Flash byte.
//   2)  unsigned char byte - the data to write to Flash.
//
// Writes the value <byte> to the Flash address <addr>.
//
// Note: The addresses 0x1FFA through 0x1FFF cannot be written using this
//       function because they contain the MAC address.
//
// Note: Software calling this function must ensure that the target Flash
//       byte has been erased (value = 0xFF).
//
// Note: The Flash must be unlocked prior to calling this function.
//----------------------------------------------------------------------------- 
unsigned char CPFLASH_ByteWrite (unsigned int addr, char byte)
{
   

   // Check if address is in-range
   if(addr < 0x1FFA)
   {
      // Set the Flash Address Pointer to <addr>
      FLASHADDRH = (addr >> 8);           // Copy High Byte
      FLASHADDRL = (addr & 0xFF);         // Copy Low Byte
      
      // Write the Flash unlock sequence 0xA5, 0xF1
      FLASHKEY = 0xA5;
      FLASHKEY = 0xF1;

      // Initiate the Flash write
      FLASHDATA = byte;
      
     
      // Wait for the Flash operation to start and complete
      return poll_flash_busy();     
     
   }

   return FLASH_ERROR;
}


//-----------------------------------------------------------------------------
// CPFLASH_PageErase
//-----------------------------------------------------------------------------
//
// Return Value : 
//    unsigned char - Returns '0' on success or FLASH_ERROR if a problem
//    is encountered.
//
// Parameters   : 
//   1)  unsigned int addr - the address of the Flash Page.
//
// Erases the Flash page containing address <addr>.
//
// Note: The last Flash page (0x1E00 - 0x1FFF) cannot be erased using this
//       function because it contains the MAC address.
//
// Note: All data stored on a Flash page will be lost once the page is erased.
//
// Note: The Flash must be unlocked prior to calling this function.
//----------------------------------------------------------------------------- 
unsigned char CPFLASH_PageErase (unsigned int addr)
{
   // Check if address is in-range
   if(addr < 0x1E00)
   {
      // Set the Flash Address Pointer to <addr>
      FLASHADDRH = (addr >> 8);           // Copy High Byte
      FLASHADDRL = (addr & 0xFF);         // Copy Low Byte
      
      // Write the Flash unlock sequence 0xA5, 0xF1
      FLASHKEY = 0xA5;
      FLASHKEY = 0xF1;

      // Initiate the Flash erase
      FLASHERASE = 0x01;
      
      // Wait for the Flash operation to start and complete
      return poll_flash_busy();  
      
   }

   return FLASH_ERROR;
}

*/
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------