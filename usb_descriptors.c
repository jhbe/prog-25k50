//===========================================================================
//
// Name
//   usb_descriptors.c
//
// Description
//   
//===========================================================================
#include <p18cxxx.h>
#include "usb/usb.h"
#define ROM rom
#pragma romdata

//===========================================================================
// Device Descriptor
//===========================================================================

ROM USB_DEVICE_DESCRIPTOR device_dsc = {
    0x12,                   // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,  // DEVICE descriptor type
    0x0200,                 // USB Spec Release Number in BCD format        
    0x00,                   // Class Code
    0x00,                   // Subclass code
    0x00,                   // Protocol code
    USB_EP0_BUFF_SIZE,      // Max packet size for EP0, see usb_config.h
    0x3456,                 // Vendor ID
    0x1130,                 // Product ID
    0x0000,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x00,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

//===========================================================================
// Configuration Descriptor
//===========================================================================

ROM BYTE config_descriptor[] = {
    //
    // Configuration Descriptor
    //
    0x09,                          // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,  // CONFIGURATION descriptor type
    0x20,0x00,                     // Total length of data for this cfg
    1,                      // Number of interfaces in this cfg
    1,                      // Index value of this configuration
    0,                      // Configuration string index
    _DEFAULT, // | _SELF,   // Attributes: _SELF, _RWU
    50,                     // Max power consumption (2X mA)
							
    //
    // Interface Descriptor
    //
    0x09,                      // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,  // INTERFACE descriptor type
    0,                         // Interface Number
    0,                      // Alternate Setting Number
    2,                      // Number of endpoints in this intf
    0xFF,                   // Class code
    0xFF,                   // Subclass code
    0xFF,                   // Protocol code
    0,                      // Interface string index
    
    //
    // Endpoint Descriptor
    //
    0x07,                     // Size
    USB_DESCRIPTOR_ENDPOINT,  // Endpoint descriptor
    _EP01_OUT,                // Endpoint address
    _BULK,                    // Attributes: _BULK, _INT, _ISO or _CTRL. ISO: _AD, _FE
    USBGEN_EP_SIZE,0x00,      // Size
    1,                        // Interval
    
    //
    // Endpoint Descriptor
    //
    0x07,                     // Size
    USB_DESCRIPTOR_ENDPOINT,  // Endpoint descriptor
    _EP01_IN,                 // Endpoint address
    _BULK,                    // Attributes: _BULK, _INT, _ISO or _CTRL. ISO: _AD, _FE
    USBGEN_EP_SIZE,0x00,      // Size
    1                         // Interval
};

//===========================================================================
// Strings
//===========================================================================

//
// Language code string descriptor
//
ROM struct{BYTE bLength;BYTE bDscType;WORD string[1];}sd000 = {
sizeof(sd000),USB_DESCRIPTOR_STRING,{0x0409}};

//
// Manufacturer string descriptor
//
ROM struct{BYTE bLength;BYTE bDscType;WORD string[4];}sd001 = {
sizeof(sd001),USB_DESCRIPTOR_STRING,
{'J','H','B','E'}};

//
// Product string descriptor
//
ROM struct{BYTE bLength;BYTE bDscType;WORD string[17];}sd002 = {
sizeof(sd002),USB_DESCRIPTOR_STRING,
{'P','r','o','g',' ','1','6','/','1','8','/','3','2',' ','U','S','B'}};

//
// Array of configuration descriptors
//
ROM BYTE *ROM USB_CD_Ptr[] = {
    (ROM BYTE *ROM)&config_descriptor
};

//
// Array of string descriptors
//
ROM BYTE *ROM USB_SD_Ptr[] = {
    (ROM BYTE *ROM)&sd000,
    (ROM BYTE *ROM)&sd001,
    (ROM BYTE *ROM)&sd002
};
