-------------------------------------------------------------------------------
 Readme.txt
-------------------------------------------------------------------------------

Copyright 2006 Silicon Laboratories, Inc.
http://www.silabs.com

Program Description:
-------------------

This project implements basic send and receive routines for the CP220x and 
can be used as a template for new driver development.


How To Test:
-----------

1) Connect an AB4 to an 'F340 Target Board.
2) Connect the embedded system to a UART terminal @ 115200 8-N-1.
3) Connect the embedded system to a network.
4) Download the code to an 'F340 Target Board using the Silicon Labs IDE.
5) All received packets will be printed on the UART terminal. The embedded
   system will also transmit a packet containing all zeros approximately 
   every second. You can view the transmitted packets using a packet 
   capture utility such as Ethereal <www.ethereal.com>.

Note: The variable DESTMAC should be initialized to the address of the PC
running the packet capture utility. This variable is initalized in 
main.c.

Note: This software is also compatible with the 'F12x Target Board. Please
change the appropriate constants in config.h.

Porting Notes:
--------------
1. Most device specific routines are located in a separate file group named
   "Device Sepecific Files". 

2. The non-device specific functionality implemented in main.c is the 
   enabling and disabling of global interrupts using the EA bit.

2. To add support for a new device, implement all the functions in MCU_Init.h.


Expected Warnings:
------------------
1. Uncalled Segment Warning:

*** WARNING L16: UNCALLED SEGMENT, IGNORED FOR OVERLAY PROCESS
    SEGMENT: ?PR?_GETKEY?F34X_INIT


Target and Tool Chain Information:
---------------------------------

FID:            
Target:         Multiple Devices
Tool chain:     Keil C51 7.50 / Keil EVAL C51
                Silicon Laboratories IDE version 2.72
Project Name:   


Command Line Options:
--------------------

Assembler : Default
Compiler  : Default
Linker    : Default


File List:
---------

config.h
const.h
global.h
Readme.txt (this file)
typedefs.h

main.c
main.h

F12x_Init.c
F34x_Init.c
MCU_Init.h
STARTUP.A51

CP220x_CORE.c
CP220x_CORE.h
CP220x_ETH.c
CP220x_ETH.h
CP220x_REG.c
CP220x_REG.c

Release Information:
-------------------

Release 1.0
  -Initial Release

-------------------------------------------------------------------------------
 End Of File
-------------------------------------------------------------------------------