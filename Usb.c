//===========================================================================
//
// Name
//   Usb
//
// Description
//
//===========================================================================
#include <p18cxxx.h>
#include <usb/usb.h>
#include <usb/usb_function_generic.h>

#include "Usb.h"

#pragma udata USB_VARIABLES=0x500

unsigned char out_packet[USBGEN_EP_SIZE];
unsigned char in_packet[USBGEN_EP_SIZE];

#pragma udata
USB_HANDLE out_handle = 0;
USB_HANDLE in_handle = 0;
#pragma udata

//---------------------------------------------------------------------------
// USER_USB_CALLBACK_EVENT_HANDLER
//---------------------------------------------------------------------------
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch(event)
    {
        case EVENT_CONFIGURED:
        	//
        	// THE USB port has been configured. Time to enable the endpoints.
        	//
			USBEnableEndpoint(USBGEN_EP_NUM, USB_OUT_ENABLED | USB_IN_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);
			
			//
			// Get a handle to the input ("out" from the host perspective).
			//
			out_handle = USBGenRead(USBGEN_EP_NUM, (BYTE*)&out_packet, USBGEN_EP_SIZE);
            break;
            
        case EVENT_TRANSFER:
            Nop();
            break;
            
        case EVENT_SET_DESCRIPTOR:
        case EVENT_EP0_REQUEST:
        case EVENT_SOF:
        case EVENT_SUSPEND:
        case EVENT_RESUME:
        case EVENT_BUS_ERROR:
        default:
            break;
    }      
    return TRUE; 
}

//---------------------------------------------------------------------------
// SendOk
//
// Sends a two-byte packet with the word "OK" in it to the PC.
//---------------------------------------------------------------------------
void SendOk(void)
{
	while(USBHandleBusy(in_handle))
	{
		USBDeviceTasks();
	}	

	in_packet[0] = 'O';
	in_packet[1] = 'K';

	in_handle = USBGenWrite(USBGEN_EP_NUM, (BYTE*)&in_packet, 2);
	while(USBHandleBusy(in_handle))
	{
		USBDeviceTasks();
	}	
}

//---------------------------------------------------------------------------
// SendFail
//
// Sends a four-byte packet with the word "FAIL" in it to the PC.
//---------------------------------------------------------------------------
void SendFail(void)
{
	while(USBHandleBusy(in_handle))
	{
		USBDeviceTasks();
	}	

	in_packet[0] = 'F';
	in_packet[1] = 'A';
	in_packet[2] = 'I';
	in_packet[3] = 'L';

	in_handle = USBGenWrite(USBGEN_EP_NUM, (BYTE*)&in_packet, 4);
	while(USBHandleBusy(in_handle))
	{
		USBDeviceTasks();
	}	
}

//---------------------------------------------------------------------------
// SendResult8
//
// 
//---------------------------------------------------------------------------
void SendResult8(unsigned char result)
{
	while(USBHandleBusy(in_handle))
	{
		USBDeviceTasks();
	}	

	in_packet[0] = result;

	in_handle = USBGenWrite(USBGEN_EP_NUM, (BYTE*)&in_packet, 1);
	while(USBHandleBusy(in_handle))
	{
		USBDeviceTasks();
	}	
}
/*
//---------------------------------------------------------------------------
// SendResult32
//
// 
//---------------------------------------------------------------------------
void SendResult32(unsigned long result)
{
	while(USBHandleBusy(in_handle))
	{
		USBDeviceTasks();
	}	

	((unsigned long *)in_packet)[0] = result;

	in_handle = USBGenWrite(USBGEN_EP_NUM, (BYTE*)&in_packet, 4);
}
*/
//---------------------------------------------------------------------------
// SendDebug
//
// 
//---------------------------------------------------------------------------
void SendDebug(unsigned char *data, unsigned char length)
{
	unsigned char i;
	
	while(USBHandleBusy(in_handle))
	{
		USBDeviceTasks();
	}	

	in_packet[0] = 'D';
	in_packet[1] = 'E';
	in_packet[2] = 'B';
	in_packet[3] = 'U';
	for (i = 0; i < length; i++)
	{
		in_packet[4 + i] = data[i];
	}	

	in_handle = USBGenWrite(USBGEN_EP_NUM, (BYTE*)&in_packet, 4 + length);
	while(USBHandleBusy(in_handle))
	{
		USBDeviceTasks();
	}	
}
/*
//---------------------------------------------------------------------------
// SendText
//
// 
//---------------------------------------------------------------------------
void SendText(const rom char *data)
{
	unsigned char i;
	
	while(USBHandleBusy(in_handle))
	{
		USBDeviceTasks();
	}	

	in_packet[0] = 'T';
	in_packet[1] = 'E';
	in_packet[2] = 'X';
	in_packet[3] = 'T';
	for (i = 0; data[i] != 0; i++)
	{
		in_packet[4 + i] = data[i];
	}	

	in_handle = USBGenWrite(USBGEN_EP_NUM, (BYTE*)&in_packet, 4 + i);

	while(USBHandleBusy(in_handle))
	{
		USBDeviceTasks();
	}	
}
*/
//---------------------------------------------------------------------------
// SendInPacket
//
// 
//---------------------------------------------------------------------------
void SendInPacket(unsigned char *bytes, unsigned char length)
{
	unsigned char i;
	
	while(USBHandleBusy(in_handle))
	{
		USBDeviceTasks();
	}	

	for (i = 0; i < length; i++)
	{
		in_packet[i] = bytes[i];
	}	

	in_handle = USBGenWrite(USBGEN_EP_NUM, (BYTE*)&in_packet, length);

	while(USBHandleBusy(in_handle))
	{
		USBDeviceTasks();
	}	
}	
