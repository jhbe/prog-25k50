/*
 * Copyright (C) 2017 Johan Bergkvist
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/pin_manager.h"
#include "usb_device.h"
#include "usb_device_generic.h"

#include "Usb.h"
#include "Pic16F.h"
#include "Pic18F.h"
#include "Pic32F.h"
#include "Pins.h"

//---------------------------------------------------------------------------
// AllocatePins16And18
//---------------------------------------------------------------------------
void AllocatePins16And18(void)
{
    static uint8_t pins_allocated = 0;
    if (pins_allocated != 0)
    {
        return;
    }
    pins_allocated = 1;
    
	//
	// Set output/inputs for PIC16 and PIC18 devices.
	//
	VDD_SetDigitalOutput();
	PGD_SetDigitalOutput();
	PGC_SetDigitalOutput();

    //
    // Initialise the outputs.
    //
    VDD = 0;
    VPP = 0;
    PGC = 0;
    PGD_OUT = 0;
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
    SYSTEM_Initialize();

	USBDeviceInit();
		
    //
    // Initialise the outputs.
    //
	VPP = 0;
    MCLR = 1;

	//
	// Set output/inputs. Pins shared with PIC32 are set after we know what
    // device is being programmed.
	//
    VPP_SetDigitalOutput();
    MCLR_SetDigitalOutput();

	while(1)
	{
		USBDeviceTasks();

		if((USBDeviceState < CONFIGURED_STATE) || (USBSuspendControl == 1))
		{
			continue;
		}

	    if(!USBHandleBusy(out_handle))
	    {
		    unsigned char length = (unsigned char)USBHandleGetLength(out_handle);
			ProcessMessage(out_packet, length);

			out_handle = USBGenRead(USBGEN_EP_NUM, out_packet, USBGEN_EP_SIZE);
		}	
	}
}
