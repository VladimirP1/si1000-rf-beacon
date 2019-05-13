//-----------------------------------------------------------------------------
// CP220x_ETH.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// 
// Header file for basic send and receive functions for the CP220x.
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
// Global Constants
//-----------------------------------------------------------------------------

// Common Ethernet Packet Types
#define IP_PACKET   0x0800
#define ARP_PACKET  0x0806


//-----------------------------------------------------------------------------
// Exported Function Definitions
//-----------------------------------------------------------------------------
extern void CP220x_Send(MACADDRESS* pDestAddr, unsigned char* buffer, unsigned int buffer_length, unsigned int packet_type);
extern unsigned int CP220x_Receive(unsigned char* buffer, unsigned int buffer_length);

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------