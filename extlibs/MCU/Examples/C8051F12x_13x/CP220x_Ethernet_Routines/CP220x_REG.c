//-----------------------------------------------------------------------------
// CP220x_REG.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// 
// CP220x Register Definitions.
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
// Direct Register Definitions
//-----------------------------------------------------------------------------                             
volatile unsigned char pdata     RAMADDRH     _at_   0x2008;   // RAM Address Pointer High Byte            
volatile unsigned char pdata     RAMADDRL     _at_   0x2009;   // RAM Address Pointer Low Byte         
volatile unsigned char pdata     RAMRXDATA    _at_   0x2002;   // RXFIFO RAM Data Register         
volatile unsigned char pdata     RAMTXDATA    _at_   0x2004;   // TXBUFF RAM Data Register         
volatile unsigned char pdata     FLASHADDRH   _at_   0x2069;   // Flash Address Pointer High Byte          
volatile unsigned char pdata     FLASHADDRL   _at_   0x2068;   // Flash Address Pointer Low Byte           
volatile unsigned char pdata     FLASHAUTORD  _at_   0x2005;   // Flash Autoread w/ increment          
volatile unsigned char pdata     FLASHDATA    _at_   0x2006;   // Flash Read/Write Data Register           
volatile unsigned char pdata     FLASHKEY     _at_   0x2067;   // Flash Lock and Key           
volatile unsigned char pdata     FLASHERASE   _at_   0x206A;   // Flash Erase          
volatile unsigned char pdata     FLASHSTA     _at_   0x207B;   // Flash Status         
volatile unsigned char pdata     MACADDR      _at_   0x200A;   // MAC Address Pointer          
volatile unsigned char pdata     MACDATAH     _at_   0x200B;   // MAC Data Register High Byte          
volatile unsigned char pdata     MACDATAL     _at_   0x200C;   // MAC Data Register Low Byte           
volatile unsigned char pdata     MACRW        _at_   0x200D;   // MAC Read/Write Initiate          
volatile unsigned char pdata     INT0         _at_   0x2063;   // Interrupt Status Register 0 (Self-Clearing)          
volatile unsigned char pdata     INT0RD       _at_   0x2076;   // Interrupt Status Register 0 (Read-Only)          
volatile unsigned char pdata     INT0EN       _at_   0x2064;   // Interrupt Enable Register 0          
volatile unsigned char pdata     INT1         _at_   0x207F;   // Interrupt Status Register 1 (Self-Clearing)          
volatile unsigned char pdata     INT1RD       _at_   0x207E;   // Interrupt Status Register 1 (Read-Only)          
volatile unsigned char pdata     INT1EN       _at_   0x207D;   // Interrupt Enable Register 1          
volatile unsigned char pdata     VDMCN        _at_   0x2013;   // VDD Monitor Control Register         
volatile unsigned char pdata     SWRST        _at_   0x2075;   // Software Reset Register          
volatile unsigned char pdata     RSTSTA       _at_   0x2073;   // Reset Source Status Register         
volatile unsigned char pdata     RSTEN        _at_   0x2072;   // Reset Enable Register            
volatile unsigned char pdata     IOPWR        _at_   0x2070;   // Port Input/Output Power          
volatile unsigned char pdata     OSCPWR       _at_   0x207C;   // Oscillator Power         
volatile unsigned char pdata     RXFILT       _at_   0x2010;   // Receive Filter Configuraton          
volatile unsigned char pdata     RXCN         _at_   0x2011;   // Receive Control          
volatile unsigned char pdata     RXSTA        _at_   0x2012;   // Receive Status           
volatile unsigned char pdata     RXAUTORD     _at_   0x2001;   // Receive Autoread w/ increment            
volatile unsigned char pdata     RXHASHH      _at_   0x200E;   // Receive Hash Table High Byte         
volatile unsigned char pdata     RXHASHL      _at_   0x200F;   // Receive Hash Table Low Byte          
volatile unsigned char pdata     CPINFOH      _at_   0x201D;   // Current RX Packet Information High Byte          
volatile unsigned char pdata     CPINFOL      _at_   0x201E;   // Current RX Packet Information Low Byte           
volatile unsigned char pdata     CPLENH       _at_   0x201F;   // Current RX Packet Length High Byte           
volatile unsigned char pdata     CPLENL       _at_   0x2020;   // Current RX Packet Length Low Byte            
volatile unsigned char pdata     CPADDRH      _at_   0x2021;   // Current RX Packet Address High Byte          
volatile unsigned char pdata     CPADDRL      _at_   0x2022;   // Current RX Packet Address Low Byte           
volatile unsigned char pdata     CPTLB        _at_   0x201A;   // Current RX Packet TLB Number         
volatile unsigned char pdata     TLBVALID     _at_   0x201C;   // TLB Valid Indicators         
volatile unsigned char pdata     TLB0INFOH    _at_   0x2023;   // TLB0 Information High Byte           
volatile unsigned char pdata     TLB0INFOL    _at_   0x2024;   // TLB0 Information Low Byte            
volatile unsigned char pdata     TLB0LENH     _at_   0x2025;   // TLB0 Length High Byte            
volatile unsigned char pdata     TLB0LENL     _at_   0x2026;   // TLB0 Length Low Byte         
volatile unsigned char pdata     TLB0ADDRH    _at_   0x2027;   // TLB0 Address High Byte           
volatile unsigned char pdata     TLB0ADDRL    _at_   0x2028;   // TLB0 Address Low Byte            
volatile unsigned char pdata     TLB1INFOH    _at_   0x2029;   // TLB1 Information High Byte           
volatile unsigned char pdata     TLB1INFOL    _at_   0x202A;   // TLB1 Information Low Byte            
volatile unsigned char pdata     TLB1LENH     _at_   0x202b;   // TLB1 Length High Byte            
volatile unsigned char pdata     TLB1LENL     _at_   0x202C;   // TLB1 Length Low Byte         
volatile unsigned char pdata     TLB1ADDRH    _at_   0x202D;   // TLB1 Address High Byte           
volatile unsigned char pdata     TLB1ADDRL    _at_   0x202E;   // TLB1 Address Low Byte            
volatile unsigned char pdata     TLB2INFOH    _at_   0x202F;   // TLB2 Information High Byte           
volatile unsigned char pdata     TLB2INFOL    _at_   0x2030;   // TLB2 Information Low Byte            
volatile unsigned char pdata     TLB2LENH     _at_   0x2031;   // TLB2 Length High Byte            
volatile unsigned char pdata     TLB2LENL     _at_   0x2032;   // TLB2 Length Low Byte         
volatile unsigned char pdata     TLB2ADDRH    _at_   0x2033;   // TLB2 Address High Byte           
volatile unsigned char pdata     TLB2ADDRL    _at_   0x2034;   // TLB2 Address Low Byte            
volatile unsigned char pdata     TLB3INFOH    _at_   0x2035;   // TLB3 Information High Byte           
volatile unsigned char pdata     TLB3INFOL    _at_   0x2036;   // TLB3 Information Low Byte            
volatile unsigned char pdata     TLB3LENH     _at_   0x2037;   // TLB3 Length High Byte            
volatile unsigned char pdata     TLB3LENL     _at_   0x2038;   // TLB3 Length Low Byte         
volatile unsigned char pdata     TLB3ADDRH    _at_   0x2039;   // TLB3 Address High Byte           
volatile unsigned char pdata     TLB3ADDRL    _at_   0x203A;   // TLB3 Address Low Byte            
volatile unsigned char pdata     TLB4INFOH    _at_   0x203B;   // TLB4 Information High Byte           
volatile unsigned char pdata     TLB4INFOL    _at_   0x203C;   // TLB4 Information Low Byte            
volatile unsigned char pdata     TLB4LENH     _at_   0x203D;   // TLB4 Length High Byte            
volatile unsigned char pdata     TLB4LENL     _at_   0x203E;   // TLB4 Length Low Byte         
volatile unsigned char pdata     TLB4ADDRH    _at_   0x203F;   // TLB4 Address High Byte           
volatile unsigned char pdata     TLB4ADDRL    _at_   0x2040;   // TLB4 Address Low Byte            
volatile unsigned char pdata     TLB5INFOH    _at_   0x2041;   // TLB5 Information High Byte           
volatile unsigned char pdata     TLB5INFOL    _at_   0x2042;   // TLB5 Information Low Byte            
volatile unsigned char pdata     TLB5LENH     _at_   0x2043;   // TLB5 Length High Byte            
volatile unsigned char pdata     TLB5LENL     _at_   0x2044;   // TLB5 Length Low Byte         
volatile unsigned char pdata     TLB5ADDRH    _at_   0x2045;   // TLB5 Address High Byte           
volatile unsigned char pdata     TLB5ADDRL    _at_   0x2046;   // TLB5 Address Low Byte            
volatile unsigned char pdata     TLB6INFOH    _at_   0x2047;   // TLB6 Information High Byte           
volatile unsigned char pdata     TLB6INFOL    _at_   0x2048;   // TLB6 Information Low Byte            
volatile unsigned char pdata     TLB6LENH     _at_   0x2049;   // TLB6 Length High Byte            
volatile unsigned char pdata     TLB6LENL     _at_   0x204A;   // TLB6 Length Low Byte         
volatile unsigned char pdata     TLB6ADDRH    _at_   0x204B;   // TLB6 Address High Byte           
volatile unsigned char pdata     TLB6ADDRL    _at_   0x204C;   // TLB6 Address Low Byte            
volatile unsigned char pdata     TLB7INFOH    _at_   0x204D;   // TLB7 Information High Byte           
volatile unsigned char pdata     TLB7INFOL    _at_   0x204E;   // TLB7 Information Low Byte            
volatile unsigned char pdata     TLB7LENH     _at_   0x204F;   // TLB7 Length High Byte            
volatile unsigned char pdata     TLB7LENL     _at_   0x2050;   // TLB7 Length Low Byte         
volatile unsigned char pdata     TLB7ADDRH    _at_   0x2051;   // TLB7 Address High Byte           
volatile unsigned char pdata     TLB7ADDRL    _at_   0x2052;   // TLB7 Address Low Byte            
volatile unsigned char pdata     RXFIFOHEADH  _at_   0x2017;   // Receive Buffer Head Pointer High Byte            
volatile unsigned char pdata     RXFIFOHEADL  _at_   0x2018;   // Receive Buffer Head Pointer Low Byte         
volatile unsigned char pdata     RXFIFOTAILH  _at_   0x2015;   // Receive Buffer Tail Pointer High Byte            
volatile unsigned char pdata     RXFIFOTAILL  _at_   0x2016;   // Receive Buffer Tail Pointer Low Byte         
volatile unsigned char pdata     RXFIFOSTA    _at_   0x205B;   // Receive Buffer Status            
volatile unsigned char pdata     TXSTARTH     _at_   0x2059;   // Transmit Data Starting Address High Byte         
volatile unsigned char pdata     TXSTARTL     _at_   0x205A;   // Transmit Data Starting Address Low Byte          
volatile unsigned char pdata     TXAUTOWR     _at_   0x2003;   // Transmit Data Autowrite          
volatile unsigned char pdata     TXENDH       _at_   0x2057;   // Transmit Data Ending Address High Byte           
volatile unsigned char pdata     TXENDL       _at_   0x2058;   // Transmit Data Ending Address Low Byte            
volatile unsigned char pdata     TXCN         _at_   0x2053;   // Transmit Control         
volatile unsigned char pdata     TXPAUSEH     _at_   0x2055;   // Transmit Pause High Byte         
volatile unsigned char pdata     TXPAUSEL     _at_   0x2056;   // Transmit Pause Low Byte          
volatile unsigned char pdata     TXBUSY       _at_   0x2054;   // Transmit Busy Indicator          
volatile unsigned char pdata     TXSTA6       _at_   0x205C;   // Transmit Status Vector 6         
volatile unsigned char pdata     TXSTA5       _at_   0x205D;   // Transmit Status Vector 5         
volatile unsigned char pdata     TXSTA4       _at_   0x205E;   // Transmit Status Vector 4         
volatile unsigned char pdata     TXSTA3       _at_   0x205F;   // Transmit Status Vector 3         
volatile unsigned char pdata     TXSTA2       _at_   0x2060;   // Transmit Status Vector 2         
volatile unsigned char pdata     TXSTA1       _at_   0x2061;   // Transmit Status Vector 1         
volatile unsigned char pdata     TXSTA0       _at_   0x2062;   // Transmit Status Vector 0         
volatile unsigned char pdata     PHYCN        _at_   0x2078;   // Physical Layer Control           
volatile unsigned char pdata     PHYCF        _at_   0x2079;   // Physical Layer Configuration         
volatile unsigned char pdata     PHYSTA       _at_   0x2080;   // Physical Layer Status            
volatile unsigned char pdata     TXPWR        _at_   0x207A;   // Transmitter Power Register           

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------