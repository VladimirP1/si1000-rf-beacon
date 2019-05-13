//-----------------------------------------------------------------------------
// CP220x_REG.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
// 
// Header File for CP220x Register Definitions.
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
// Indirect Register Definitions
//-----------------------------------------------------------------------------
#define MACCN   0x00                                     // MAC Control 
#define MACCF   0x01                                     // MAC Configuration
#define IPGT    0x02                                     // Back-to-Back Interpacket Delay
#define IPGR    0x03                                     // Non-Back-to-Back Interpacket Delay
#define CWMAXR  0x04                                     // Collision Window and Maximum Retransmit 
#define MAXLEN  0x05                                     // Maximum Frame Length
#define MACAD0  0x10                                     // MAC Address 0
#define MACAD1  0x11                                     // MAC Address 1
#define MACAD2  0x12                                     // MAC Address 2

//-----------------------------------------------------------------------------
// Direct Register Definitions
//-----------------------------------------------------------------------------                                      
extern volatile unsigned char pdata      RAMADDRH    ;   // RAM Address Pointer High Byte            
extern volatile unsigned char pdata      RAMADDRL    ;   // RAM Address Pointer Low Byte         
extern volatile unsigned char pdata      RAMRXDATA   ;   // RXFIFO RAM Data Register         
extern volatile unsigned char pdata      RAMTXDATA   ;   // TXBUFF RAM Data Register         
extern volatile unsigned char pdata      FLASHADDRH  ;   // Flash Address Pointer High Byte          
extern volatile unsigned char pdata      FLASHADDRL  ;   // Flash Address Pointer Low Byte           
extern volatile unsigned char pdata      FLASHAUTORD ;   // Flash Autoread w/ increment          
extern volatile unsigned char pdata      FLASHDATA   ;   // Flash Read/Write Data Register           
extern volatile unsigned char pdata      FLASHKEY    ;   // Flash Lock and Key           
extern volatile unsigned char pdata      FLASHERASE  ;   // Flash Erase          
extern volatile unsigned char pdata      FLASHSTA    ;   // Flash Status         
extern volatile unsigned char pdata      MACADDR     ;   // MAC Address Pointer          
extern volatile unsigned char pdata      MACDATAH    ;   // MAC Data Register High Byte          
extern volatile unsigned char pdata      MACDATAL    ;   // MAC Data Register Low Byte           
extern volatile unsigned char pdata      MACRW       ;   // MAC Read/Write Initiate          
extern volatile unsigned char pdata      INT0        ;   // Interrupt Status Register 0 (Self-Clearing)          
extern volatile unsigned char pdata      INT0RD      ;   // Interrupt Status Register 0 (Read-Only)          
extern volatile unsigned char pdata      INT0EN      ;   // Interrupt Enable Register 0          
extern volatile unsigned char pdata      INT1        ;   // Interrupt Status Register 1 (Self-Clearing)          
extern volatile unsigned char pdata      INT1RD      ;   // Interrupt Status Register 1 (Read-Only)          
extern volatile unsigned char pdata      INT1EN      ;   // Interrupt Enable Register 1          
extern volatile unsigned char pdata      VDMCN       ;   // VDD Monitor Control Register         
extern volatile unsigned char pdata      SWRST       ;   // Software Reset Register          
extern volatile unsigned char pdata      RSTSTA      ;   // Reset Source Status Register         
extern volatile unsigned char pdata      RSTEN       ;   // Reset Enable Register            
extern volatile unsigned char pdata      IOPWR       ;   // Port Input/Output Power          
extern volatile unsigned char pdata      OSCPWR      ;   // Oscillator Power         
extern volatile unsigned char pdata      RXFILT      ;   // Receive Filter Configuraton          
extern volatile unsigned char pdata      RXCN        ;   // Receive Control          
extern volatile unsigned char pdata      RXSTA       ;   // Receive Status           
extern volatile unsigned char pdata      RXAUTORD    ;   // Receive Autoread w/ increment            
extern volatile unsigned char pdata      RXHASHH     ;   // Receive Hash Table High Byte         
extern volatile unsigned char pdata      RXHASHL     ;   // Receive Hash Table Low Byte          
extern volatile unsigned char pdata      CPINFOH     ;   // Current RX Packet Information High Byte          
extern volatile unsigned char pdata      CPINFOL     ;   // Current RX Packet Information Low Byte           
extern volatile unsigned char pdata      CPLENH      ;   // Current RX Packet Length High Byte           
extern volatile unsigned char pdata      CPLENL      ;   // Current RX Packet Length Low Byte            
extern volatile unsigned char pdata      CPADDRH     ;   // Current RX Packet Address High Byte          
extern volatile unsigned char pdata      CPADDRL     ;   // Current RX Packet Address Low Byte           
extern volatile unsigned char pdata      CPTLB       ;   // Current RX Packet TLB Number         
extern volatile unsigned char pdata      TLBVALID    ;   // TLB Valid Indicators         
extern volatile unsigned char pdata      TLB0INFOH   ;   // TLB0 Information High Byte           
extern volatile unsigned char pdata      TLB0INFOL   ;   // TLB0 Information Low Byte            
extern volatile unsigned char pdata      TLB0LENH    ;   // TLB0 Length High Byte            
extern volatile unsigned char pdata      TLB0LENL    ;   // TLB0 Length Low Byte         
extern volatile unsigned char pdata      TLB0ADDRH   ;   // TLB0 Address High Byte           
extern volatile unsigned char pdata      TLB0ADDRL   ;   // TLB0 Address Low Byte            
extern volatile unsigned char pdata      TLB1INFOH   ;   // TLB1 Information High Byte           
extern volatile unsigned char pdata      TLB1INFOL   ;   // TLB1 Information Low Byte            
extern volatile unsigned char pdata      TLB1LENH    ;   // TLB1 Length High Byte            
extern volatile unsigned char pdata      TLB1LENL    ;   // TLB1 Length Low Byte         
extern volatile unsigned char pdata      TLB1ADDRH   ;   // TLB1 Address High Byte           
extern volatile unsigned char pdata      TLB1ADDRL   ;   // TLB1 Address Low Byte            
extern volatile unsigned char pdata      TLB2INFOH   ;   // TLB2 Information High Byte           
extern volatile unsigned char pdata      TLB2INFOL   ;   // TLB2 Information Low Byte            
extern volatile unsigned char pdata      TLB2LENH    ;   // TLB2 Length High Byte            
extern volatile unsigned char pdata      TLB2LENL    ;   // TLB2 Length Low Byte         
extern volatile unsigned char pdata      TLB2ADDRH   ;   // TLB2 Address High Byte           
extern volatile unsigned char pdata      TLB2ADDRL   ;   // TLB2 Address Low Byte            
extern volatile unsigned char pdata      TLB3INFOH   ;   // TLB3 Information High Byte           
extern volatile unsigned char pdata      TLB3INFOL   ;   // TLB3 Information Low Byte            
extern volatile unsigned char pdata      TLB3LENH    ;   // TLB3 Length High Byte            
extern volatile unsigned char pdata      TLB3LENL    ;   // TLB3 Length Low Byte         
extern volatile unsigned char pdata      TLB3ADDRH   ;   // TLB3 Address High Byte           
extern volatile unsigned char pdata      TLB3ADDRL   ;   // TLB3 Address Low Byte            
extern volatile unsigned char pdata      TLB4INFOH   ;   // TLB4 Information High Byte           
extern volatile unsigned char pdata      TLB4INFOL   ;   // TLB4 Information Low Byte            
extern volatile unsigned char pdata      TLB4LENH    ;   // TLB4 Length High Byte            
extern volatile unsigned char pdata      TLB4LENL    ;   // TLB4 Length Low Byte         
extern volatile unsigned char pdata      TLB4ADDRH   ;   // TLB4 Address High Byte           
extern volatile unsigned char pdata      TLB4ADDRL   ;   // TLB4 Address Low Byte            
extern volatile unsigned char pdata      TLB5INFOH   ;   // TLB5 Information High Byte           
extern volatile unsigned char pdata      TLB5INFOL   ;   // TLB5 Information Low Byte            
extern volatile unsigned char pdata      TLB5LENH    ;   // TLB5 Length High Byte            
extern volatile unsigned char pdata      TLB5LENL    ;   // TLB5 Length Low Byte         
extern volatile unsigned char pdata      TLB5ADDRH   ;   // TLB5 Address High Byte           
extern volatile unsigned char pdata      TLB5ADDRL   ;   // TLB5 Address Low Byte            
extern volatile unsigned char pdata      TLB6INFOH   ;   // TLB6 Information High Byte           
extern volatile unsigned char pdata      TLB6INFOL   ;   // TLB6 Information Low Byte            
extern volatile unsigned char pdata      TLB6LENH    ;   // TLB6 Length High Byte            
extern volatile unsigned char pdata      TLB6LENL    ;   // TLB6 Length Low Byte         
extern volatile unsigned char pdata      TLB6ADDRH   ;   // TLB6 Address High Byte           
extern volatile unsigned char pdata      TLB6ADDRL   ;   // TLB6 Address Low Byte            
extern volatile unsigned char pdata      TLB7INFOH   ;   // TLB7 Information High Byte           
extern volatile unsigned char pdata      TLB7INFOL   ;   // TLB7 Information Low Byte            
extern volatile unsigned char pdata      TLB7LENH    ;   // TLB7 Length High Byte            
extern volatile unsigned char pdata      TLB7LENL    ;   // TLB7 Length Low Byte         
extern volatile unsigned char pdata      TLB7ADDRH   ;   // TLB7 Address High Byte           
extern volatile unsigned char pdata      TLB7ADDRL   ;   // TLB7 Address Low Byte            
extern volatile unsigned char pdata      RXFIFOHEADH ;   // Receive Buffer Head Pointer High Byte            
extern volatile unsigned char pdata      RXFIFOHEADL ;   // Receive Buffer Head Pointer Low Byte         
extern volatile unsigned char pdata      RXFIFOTAILH ;   // Receive Buffer Tail Pointer High Byte            
extern volatile unsigned char pdata      RXFIFOTAILL ;   // Receive Buffer Tail Pointer Low Byte         
extern volatile unsigned char pdata      RXFIFOSTA   ;   // Receive Buffer Status            
extern volatile unsigned char pdata      TXSTARTH    ;   // Transmit Data Starting Address High Byte         
extern volatile unsigned char pdata      TXSTARTL    ;   // Transmit Data Starting Address Low Byte          
extern volatile unsigned char pdata      TXAUTOWR    ;   // Transmit Data Autowrite          
extern volatile unsigned char pdata      TXENDH      ;   // Transmit Data Ending Address High Byte           
extern volatile unsigned char pdata      TXENDL      ;   // Transmit Data Ending Address Low Byte            
extern volatile unsigned char pdata      TXCN        ;   // Transmit Control         
extern volatile unsigned char pdata      TXPAUSEH    ;   // Transmit Pause High Byte         
extern volatile unsigned char pdata      TXPAUSEL    ;   // Transmit Pause Low Byte          
extern volatile unsigned char pdata      TXBUSY      ;   // Transmit Busy Indicator          
extern volatile unsigned char pdata      TXSTA6      ;   // Transmit Status Vector 6         
extern volatile unsigned char pdata      TXSTA5      ;   // Transmit Status Vector 5         
extern volatile unsigned char pdata      TXSTA4      ;   // Transmit Status Vector 4         
extern volatile unsigned char pdata      TXSTA3      ;   // Transmit Status Vector 3         
extern volatile unsigned char pdata      TXSTA2      ;   // Transmit Status Vector 2         
extern volatile unsigned char pdata      TXSTA1      ;   // Transmit Status Vector 1         
extern volatile unsigned char pdata      TXSTA0      ;   // Transmit Status Vector 0         
extern volatile unsigned char pdata      PHYCN       ;   // Physical Layer Control           
extern volatile unsigned char pdata      PHYCF       ;   // Physical Layer Configuration         
extern volatile unsigned char pdata      PHYSTA      ;   // Physical Layer Status            
extern volatile unsigned char pdata      TXPWR       ;   // Transmitter Power Register 

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------
           
// TLB0INFOH Bit Definitions
#define  RXVALID        0x80
#define  RXVLAN         0x40
#define  RXUNSUP        0x20
#define  RXPCF          0x10
#define  RXCF           0x08
#define  RXADATA        0x04
#define  BCAST          0x02
#define  MCAST          0x01

// TLB0INFOL Bit Definitions
#define  RXOK           0x80
#define  LENGTH         0x40
#define  LENERR         0x20
#define  CRCERR         0x10
#define  RXVLT          0x08
#define  RXLEN          0x02
#define  RXDROP         0x01

// INT0 Bit Definitions
#define  EEOPINT         0x80
#define  ERXEINT         0x40
#define  ESELFINT        0x20
#define  EOSCINT         0x10
#define  EFLWEINT        0x08
#define  ETXINT          0x04
#define  ERXFINT         0x02
#define  ERXINT          0x01

// INT0EN Bit Definitions
#define  EOPINT         0x80
#define  RXEINT         0x40
#define  SELFINT        0x20
#define  OSCINT         0x10
#define  FLWEINT        0x08
#define  TXINT          0x04
#define  RXFINT         0x02
#define  RXINT          0x01

// INT1 Bit Definitions
#define  WAKEINT        0x20
#define  LINKINT        0x10
#define  JABINT         0x08
#define  ANFINT         0x04
#define  RFINT          0x02
#define  ANCINT         0x01

// INT1EN Bit Definitions
#define  EWAKEINT        0x20
#define  ELINKINT        0x10
#define  EJABINT         0x08
#define  EANFINT         0x04
#define  ERFINT          0x02
#define  EANCINT         0x01

// RXCN Bit Definitions
#define  RXINH          0x08
#define  RXCLRV         0x04
#define  RXSKIP         0x02
#define  RXCLEAR        0x01

// TXCN Bit Definitions
#define  OVRRIDE        0x80
#define  CRCENOV        0x20
#define  PADENOV        0x10
#define  TXPPKT         0x08
#define  BCKPRES        0x04
#define  FDPLXOV        0x02
#define  TXGO           0x01

// PHYCN Bit Definitions
#define  PHYEN          0x80
#define  TXEN           0x40
#define  RXEN           0x20
#define  DPLXMD         0x10
#define  LBMD           0x08
#define  LPRFAULT       0x04
#define  POLREV         0x02
#define  LINKSTA        0x01

// PHYCF Bit Definitions
#define  SMSQ           0x80
#define  LINKINTG       0x40
#define  JABBER         0x20
#define  AUTONEG        0x10
#define  ADRFAULT       0x08
#define  ADPAUSE        0x04
#define  AUTOPOL        0x02
#define  REVPOL         0x01

// MACCN Bit Definitions
#define  RANDRST        0x4000
#define  LOOPBCK        0x0010
#define  TXPAUSE        0x0008
#define  RXPAUSE        0x0004
#define  PASSALL        0x0002
#define  RCVEN          0x0001

// MACCF Bit Definitions
#define  ABORTD         0x4000
#define  EBBPD          0x2000
#define  EBD            0x1000
#define  RLPRE          0x0200
#define  PUREPRE        0x0100
#define  PADMD1         0x0080
#define  PADMD0         0x0040
#define  PADEN          0x0020
#define  CRCEN          0x0010
#define  PHEADER        0x0008
#define  LENCHK         0x0002
#define  FLLDPLX        0x0001

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------