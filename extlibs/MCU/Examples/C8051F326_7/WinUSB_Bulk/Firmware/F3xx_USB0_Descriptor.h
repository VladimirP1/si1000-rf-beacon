//-----------------------------------------------------------------------------
// F3xx_USB0_Descriptor.h
//-----------------------------------------------------------------------------
// Copyright 2005-2012 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Source file for USB firmware. Includes descriptor data.
//
//
// How To Test:    See Readme.txt
//
//
// Target:         C8051F320/1,
//                 C8051F326/7,
//                 C8051F34x,
//                 C8051F38x
//
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.6
// Command Line:   See Readme.txt
// Project Name:   USB0_Bulk
//
// Release 2.0
//    -Rewrite (CM)
//    -02 NOV 2012
//
// Release 1.3
//    - Changes by PD, GP
//    - Modified Descriptors
//    - Changed name from USB_Descriptor.c
//    - 07 December 2005
//
// Release 1.0
//    -Initial Revision (DM)
//    -22 NOV 2002
//

#ifndef _USB_DESC_H_
#define _USB_DESC_H_

//-----------------------------------------------------------------------------
// Type Definitions
//-----------------------------------------------------------------------------

// Standard Device Descriptor Type Defintion
typedef struct Device_Descriptor
{
   U8    bLength;             // Size of this Descriptor in Bytes
   U8    bDescriptorType;     // Descriptor Type (=1)
   U8    bcdUsbLsb;           // USB Spec Release Number in BCD
   U8    bcdUsbMsb;
   U8    bDeviceClass;        // Device Class Code
   U8    bDeviceSubClass;     // Device Subclass Code
   U8    bDeviceProtocol;     // Device Protocol Code
   U8    bMaxPacketSize0;     // Maximum Packet Size for EP0
   U8    idVendorLsb;         // Vendor ID
   U8    idVendorMsb;
   U8    idProductLsb;        // Product ID
   U8    idProductMsb;
   U8    bcdDeviceLsb;        // Device Release Number in BCD
   U8    bcdDeviceMsb;
   U8    iManufacturer;       // Index of String Desc for Manufacturer
   U8    iProduct;            // Index of String Desc for Product
   U8    iSerialNumber;       // Index of String Desc for SerNo
   U8    bNumConfigurations;  // Number of possible Configurations

} Device_Descriptor;

// Standard Configuration Descriptor Type Definition
typedef struct Configuration_Descriptor
{
   U8    bLength;             // Size of this Descriptor in Bytes
   U8    bDescriptorType;     // Descriptor Type (=2)
   U8    wTotalLengthLsb;     // Total Length of Data for this Conf
   U8    wTotalLengthMsb;
   U8    bNumInterfaces;      // Number of Interfaces supported by this
                              // Conf
   U8    bConfigurationValue; // Designator Value for *this*
                              // Configuration
   U8    iConfiguration;      // Index of String Desc for this Conf
   U8    bmAttributes;        // Configuration Characteristics (see below)
   U8    bMaxPower;           // Max. Power Consumption in this
                              // Conf (*2mA)
} Configuration_Descriptor;

// Standard Interface Descriptor Type Definition
typedef struct Interface_Descriptor
{
   U8    bLength;             // Size of this Descriptor in Bytes
   U8    bDescriptorType;     // Descriptor Type (=4)
   U8    bInterfaceNumber;    // Number of *this* Interface (0..)
   U8    bAlternateSetting;   // Alternative for this Interface (if any)
   U8    bNumEndpoints;       // No of EPs used by this IF (excl. EP0)
   U8    bInterfaceClass;     // Interface Class Code
   U8    bInterfaceSubClass;  // Interface Subclass Code
   U8    bInterfaceProtocol;  // Interface Protocol Code
   U8    iInterface;          // Index of String Desc for this Interface

} Interface_Descriptor;

// Standard Endpoint Descriptor Type Definition
typedef struct Endpoint_Descriptor
{
   U8    bLength;             // Size of this Descriptor in Bytes
   U8    bDescriptorType;     // Descriptor Type (=5)
   U8    bEndpointAddress;    // Endpoint Address (Number + Direction)
   U8    bmAttributes;        // Endpoint Attributes (Transfer Type)
   U8    wMaxPacketSizeLsb;   // Max. Endpoint Packet Size
   U8    wMaxPacketSizeMsb;
   U8    bInterval;           // Polling Interval (Interrupt) ms

} Endpoint_Descriptor;

// Setup Packet Type Definition
typedef struct Setup_Buffer
{
   U8    bmRequestType;       // Request recipient, type, and dir.
   U8    bRequest;            // Specific standard request number
   UU16  wValue;              // varies according to request
   UU16  wIndex;              // varies according to request
   UU16  wLength;             // Number of bytes to transfer

} Setup_Buffer;

//-----------------------------------------------------------------------------
// External Global Variables
//-----------------------------------------------------------------------------

extern SEGMENT_VARIABLE(DEVICE_DESC, const Device_Descriptor, SEG_CODE);
extern SEGMENT_VARIABLE(CONFIG_DESC, const Configuration_Descriptor, SEG_CODE);
extern SEGMENT_VARIABLE(INTERFACE_DESC, const Interface_Descriptor, SEG_CODE);
extern SEGMENT_VARIABLE(IN_EP1_DESC, const Endpoint_Descriptor, SEG_CODE);
extern SEGMENT_VARIABLE(OUT_EP1_DESC, const Endpoint_Descriptor, SEG_CODE);
extern SEGMENT_VARIABLE_SEGMENT_POINTER(STRING_DESC_TABLE[], U8, const SEG_CODE, const SEG_CODE);

#endif // _USB_DESC_H_
