//===========================================================================
//
// Name
//   Prog_2550
//
// Description
//
//===========================================================================
#include <p18cxxx.h>
#include <delays.h>
#include <stdio.h>
#include <usb/usb.h>
#include <usb/usb_function_generic.h>

#include "Config.h"
#include "Usb.h"
#include "Pic16.h"
#include "Pic18.h"
#include "Pic32.h"
#include "Pins.h"

//===========================================================================
// DEFINES
//===========================================================================

//---------------------------------------------------------------------------
// Init
//---------------------------------------------------------------------------
void Init(void)
{
	//
	// External oscillator, no tuning done.
	//
	OSCCON = 0b01110000;
	OSCTUNE = 0;

	//
	// A/D converters do not use RA0 - RA3.
	//
	ADCON1 = 0b00001111;
	
	//
	// Comparators do not use RA0 - RA3.
	//
	CMCON = 0b00000111;
	
    //
    // Initialise the outputs.
    //
	VPP = 0;
    MCLR = 1;

	//
	// Set output/inputs. Pins shared with PIC32 are set after we know what
    // device is being programmed.
	//
    TRIS_VPP = 0;
    TRIS_MCLR = 0;

    //
    // Two pins for debug purposes, mainly to act as a triggers.
    //
	TRIS_TRIGGER = 0;
    TRIGGER = 0;
    TRIS_TRIGGER_2 = 0;
    TRIGGER_2 = 0;
}	

//---------------------------------------------------------------------------
// AllocatePins16And18
//---------------------------------------------------------------------------
void AllocatePins16And18(void)
{
    static UINT8 pins_allocated = 0;
    if (pins_allocated != 0)
    {
        return;
    }
    pins_allocated = 1;
    
	//
	// Set output/inputs for PIC16 and PIC18 devices.
	//
	TRIS_VDD = 0;
	TRIS_PGD = 0;
	TRIS_PGC = 0;

    //
    // Initialise the outputs.
    //
    VDD = 0;
    PGC = 0;
    PGD_OUT = 0;
    VDD = 0;
}

//---------------------------------------------------------------------------
// ProcessMessage
//---------------------------------------------------------------------------
void ProcessMessage(unsigned char *message, unsigned char length)
{
    switch (message[0] & 0xf0)
    {
        case 0x00:
            AllocatePins16And18();
            ProcessMessage18(message, length);
            break;

        case 0x10:
            ProcessMessage32(message, length);
            break;

        case 0x20:
            AllocatePins16And18();
            ProcessMessage16(message, length);
            break;
    }
}

//---------------------------------------------------------------------------
// main
//---------------------------------------------------------------------------
void main(void)
{
	Init();
	USBDeviceInit();
		
	while(1)
	{
		TRIGGER = 1;
		USBDeviceTasks();
		TRIGGER = 0;

		if((USBDeviceState < CONFIGURED_STATE) || (USBSuspendControl == 1))
		{
			continue;
		}

	    if(!USBHandleBusy(out_handle))
	    {
		    unsigned char length = (unsigned char)USBHandleGetLength(out_handle);
			ProcessMessage(out_packet, length);

			out_handle = USBGenRead(USBGEN_EP_NUM, (BYTE*)&out_packet, USBGEN_EP_SIZE);
		}	
	}
}
