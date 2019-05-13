//-----------------------------------------------------------------------------
// typedefs.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// 
// Contains global type definitions
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
// Global Type Definitions
//-----------------------------------------------------------------------------

#ifndef	MACADDRESS
typedef union MACADDRESS {
   unsigned int Int[3];
   unsigned char Char[6];
} MACADDRESS;
#endif

#ifndef	ULONG
typedef union ULONG {
   unsigned long Long;
   unsigned int Int[2];
   unsigned char Char[4];
} ULONG;
#endif

#ifndef	LONG
typedef union LONG {
   long Long;
   unsigned int Int[2];
   unsigned char Char[4];
} LONG;
#endif

#ifndef UINT
typedef union UINT {
   unsigned int Int;
   unsigned char Char[2];
} UINT;
#endif

#ifndef UCHAR
typedef unsigned char UCHAR;
#endif

typedef union IPADDRESS {                 // The 32-bit IP address
   unsigned long Long;
   unsigned int Int[2];
   unsigned char Char[4];
} IPADDRESS;


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------