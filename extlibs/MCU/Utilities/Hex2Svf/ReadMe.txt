-------------------------------------------------------------------------------
Silicon Labs .HEX to .SVF Conversion Utility Readme
Version 1.31
Copyright 2012, Silicon Laboratories
-------------------------------------------------------------------------------

Description:

This utility is provided to convert standard Intel .HEX files into .SVF
(Serial Vector Format) files, which can be used to program a Silicon
Labs MCU over the JTAG port.

The utility is written using the Microsoft .NET 2.0 libraries, and requires
that Microsoft .NET 2.0 be installed on the machine prior to running.

Initial development of this utility was performed with the help of Corelis
Inc. (http://www.corelis.com).  The tool has been tested and verified for all
device families using Corelis USB-1149.1/E USB 2.0 High-Speed JTAG controller, 
and Corelis ScanExpress Runner software.

-------------------------------------------------------------------------------

Usage:

1) Launch the program using "SilabsHEX2SVF.exe".

2) Select the Silicon Labs device number you want to generate an SVF file
	for in the "Device Number" drop-down box.
	
3) Select the functions you want the resulting .SVF file to perform in the
	"SVF Output Options" box.
	
4) Where applicable, click on the "Input .HEX File" fields, to bring up the
	file selection dialog, and select the input .hex files to be used.
	
5) Click on the "Output .SVF File" field to bring up a file selection
	dialog box, and either select or create the output .svf file to use.
	
6) Click "Generate SVF".

-------------------------------------------------------------------------------

SVF Output Options:

The following is a list of options available in the "SVF Output Options" box,
and a description of the vectors which each option will create in the output
SVF file.  The numbers listed in parenthesis indicate the order in which
the vectors to perform these functions will be written into the .SVF file.

Erase Device (1) - Generates vectors to perform a full device erase.

Blank Check (2) - Generates vectors to read back every programmable byte on
	the selected device, and compare them with 0xFF.
	
Write Main Code (3) - Generates vectors to write the code memory with the 
	contents of the specified .hex file(s). 
	Note that if the .hex file(s) specify values other than 0xFF for 
	the write or read lock security bytes, these will be written in 
	steps 6 and 7 if the Write or Read Lock options are not checked.
	
Write Scratch Page (4) - Generates vectors to write the scratch pad memory
	with the contents of a specified .hex file.
	
  Skip 0xFF During Writes - This is not a separate step.  Rather, it instructs
	the software to skip over any memory writes of value 0xFF when 
	generating vectors.  If a device erase is performed prior to the
	write, this will remove any unnecessary writes to Flash memory.
	This applies to writes in both the main code and scratchpad areas.
	
  Check For Write Failures - This is not a separate step. Rather this will
	instruct the software to generate vectors for checking whether a
	write failure occured after each byte write. This is a check of a
	status bit in the device, not a read verification of the contents.
	This applies to writes in both the main code and scratchpad areas.

Verify Main Code (5) - Generates vectors to read flash memory and
	validate the correct information was written to every location in
	main code memory space.

Verify Scratch Pad (6) - Generates vectors to read flash memory and
	validate the correct information was written to every location in
	scratch pad memory space.

  Skip 0xFF During Verfiy - This is not a separate step.  Rather, it instructs
	the software to skip over any memory reads of value 0xFF when 
	generating vectors.
	This applies to verfication of both the main code and scratchpad areas.
	
Write Lock Device (7) - Generates vectors to write the value of 0x00 to the 
	Write Lock location.
	Note that this selection over-rides any value specified in the .hex
	file(s) for the write lock byte.  If the .hex file specifies a write
	lock byte value other than 0xFF, that values be written during 
	this time if the "Write Lock Device" option is left unchecked.
	
Read Lock Device (8) - Generates vectors to write the value of 0x00 to the 
	Read Lock location.
	Note that this selection over-rides any value specified in the .hex
	file(s) for the read lock byte.  If the .hex file specifies a read
	lock byte value other than 0xFF, it will still be written during 
	this time if the "Read Lock Device" option is left unchecked.
	
Header/Footer - Allows specification of header and footer information.  This
	opens a window where the user can specify the following parameters
	for the HDR, HIR, TDR and TIR elements of the .SVF file:
	
	Bits : Number of bits in the header or trailer information.
	TDI : Specifies TDI value (in hexadecimal)
	TDO : Specifies TDI value (in hexadecimal)
	MASK : Specifies MASK value (in hexadecimal)
	SMASK : Specifies SMASK value (in hexadecimal)
	
-------------------------------------------------------------------------------