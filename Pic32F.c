/*
 * Copyright (C) 2017 Johan Bergkvist
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 * 
 * This is the firmware for the 18f2550 chip allowing it to program a
 * PIC32MX2xx (and others).
 *
 * The programming specification is Microchip document DS66145J titled
 * "PIC32MX Flash Programming Specification". This firmware
 * implements the 4-wire JTAG interface (not the 2-wire In-Circuit Serial
 * Programming (ICSP) specification).
 *
 * The following table illustrate how pins on the 18f2550 should be
 * connected to the target (a pic32mx___):
 *
 *                        18f2550 | target
 *                        ================
 *                          RB5   |  MCLR
 *                          RB4   |  TMS
 *                          RB2   |  TDO
 *                          RB1   |  TCK
 *                          RB0   |  TDI
 */
#include <xc.h>
#include "Pins.h"
#include "Tap.h"
#include "Usb.h"
#include "Commands.h"
#include "Delays.h"

#define NVMCON_OFFSET       0x00
#define NVMCONCLR_OFFSET    0x04
#define NVMCONSET_OFFSET    0x08
#define NVMKEY_OFFSET       0x10
#define NVMADDR_OFFSET      0x20
#define NVMDATA_OFFSET      0x30
#define NVMSRCADDR_OFFSET   0x40

//---------------------------------------------------------------------------
// AllocateOutputPins32
//---------------------------------------------------------------------------
void AllocateOutputPins32(void)
{
    TCK = 0;
    TMS = 0;
    TDI = 0;

    TRIS_TCK = 0;
	TRIS_TMS = 0;
	TRIS_TDI = 0;
	TRIS_TDO = 1;
}

//---------------------------------------------------------------------------
// DeallocateOutputPins32
//---------------------------------------------------------------------------
void DeallocateOutputPins32(void)
{
	TRIS_TCK = 1;
	TRIS_TMS = 1;
	TRIS_TDI = 1;
	TRIS_TDO = 1;
}

//---------------------------------------------------------------------------
// ProcessorAccess
//---------------------------------------------------------------------------
unsigned long ProcessorAccess(unsigned long in_data, unsigned char is_an_instruction)
{
    unsigned long control;
    unsigned long out_data = 0xcdcdcdcd;
    unsigned char is_a_write = 0;
    unsigned char done = 0;
    unsigned long address;

	do
	{
	    //
	    // Wait until the CPU is ready.
	    //
	    SendTapCommand(ETAP_CONTROL);
	    while (!((control = XferData32(0x0004c000)) & PRACC_MASK));
	    
	    //
	    // Read or Write?
	    //
	   	is_a_write = (control & 0x00080000) ? 1 : 0;

		//
		// Get the address.
		//	
	    SendTapCommand(ETAP_ADDRESS);
	   	address = XferData32(0);
	   	
		//
		// Is this an access to a fastdata register?
		//
		if (0xff200000 <= address && address < 0xff200010)
		{
		    SendTapCommand(ETAP_DATA);
		    if (is_a_write)
		    {
				out_data = XferData32(0);
			}
			else
			{
				XferData32(is_an_instruction ? 0xffeeddcc : in_data);
			}	

			done = !is_an_instruction;
			
		}
		else
		{
		    SendTapCommand(ETAP_DATA);
			XferData32(is_an_instruction ? in_data : 0x00000000);

			done = is_an_instruction;
		}		
	
/*		{
			unsigned long words[10];
			words[0] = is_an_instruction;
			words[1] = address;
			words[2] = control;
			words[3] = in_data;
			words[4] = out_data;
			SendDebug((unsigned char *)words, 20);
		}
*/
	    SendTapCommand(ETAP_CONTROL);
		XferData32(0x0000c000);
	}
	while (!done);
	
	return out_data;
}

void XferInstruction(unsigned long instruction)
{
	ProcessorAccess(instruction, 1);
}
unsigned long XferFastData(unsigned long in_data)
{
	unsigned long out_data = ProcessorAccess(in_data, 0);

//	SendDebug((unsigned char *)&out_data, 4);
	return out_data;
}	

//===========================================================================

//---------------------------------------------------------------------------
// CheckDevice
//---------------------------------------------------------------------------
void CheckDevice(void)
{
	#define MAX_NUMBER_OF_ATTEMPTS  10
	
	unsigned char mchp_status;
	unsigned char number_of_attempts = 0;

	//
	// First we assert MCLR to make sure the CPU doesn't run.
	//
	MCLR = 0;

    //
    // Set the pins to output (most of them). We can do that now as the PIC32
    // is in reset.
    //
    AllocateOutputPins32();

	//
	// Next we reset the TAP.
	//
	ResetTap();

	//
	// Then we read the STATUS register and wait until FCBUSY == 0 and
	// CFGRDY == 1.
	//
	SendTapCommand(MTAP_SW_MTAP);
	SendTapCommand(MTAP_COMMAND);

	while (mchp_status = XferData8(MCHP_STATUS),
		   ((mchp_status & MCHP_STATUS_FCBUSY) || !(mchp_status & MCHP_STATUS_CFGRDY)) && number_of_attempts < MAX_NUMBER_OF_ATTEMPTS)
	{
/*		unsigned char words[2];

		words[0] = mchp_status;
		words[1] = number_of_attempts;
		SendDebug((unsigned char *)words, 2);
*/
		Delayms(100);
		number_of_attempts++;
	}
	
	SendResult8(mchp_status);
}

//---------------------------------------------------------------------------
// ChipErase
//---------------------------------------------------------------------------
void ChipErase(void)
{
	unsigned char mchp_status;
	unsigned char number_of_attempts = 0;
	
	//
	// Select the MTAP, tell it to expect a command and issue an ERASE
	// command.
	//
	SendTapCommand(MTAP_SW_MTAP);
	SendTapCommand(MTAP_COMMAND);
	XferData8(MCHP_ERASE);

	//
	// Deassert MCLR. It seems the flash controller won't actually run while
	// MCLR is asserted.
	//
	MCLR = 1;

	//
	// Now we give the erase command 1 ms to finish.
	//
	Delayms(1);

	//
	// Then we read the STATUS register and wait until FCBUSY == 0 and
	// CFGRDY == 1.
	//
	while (mchp_status = XferData8(MCHP_STATUS),
		   ((mchp_status & MCHP_STATUS_FCBUSY) || !(mchp_status & MCHP_STATUS_CFGRDY)) && number_of_attempts < 200)
	{
		Delayms(100);
		number_of_attempts++;
	}

	SendResult8(mchp_status);
}

//---------------------------------------------------------------------------
// EnterSerialExecutionMode
//---------------------------------------------------------------------------
void EnterSerialExecutionMode(void)
{
	unsigned char control;
	
	//
	// Assert MCLR again.
	// 
	MCLR = 0;

    //
    // Set the pins to output (most of them). We can do that now as the PIC32
    // is in reset. This should have no efect as we already allocated the pins
    // in CheckDevice.
    //
    AllocateOutputPins32();

	//
	// Select the ETAP.
	//
	SendTapCommand(MTAP_SW_ETAP);
	
	//
	// Put the CPU in serial execution mode.
	//
	SendTapCommand(ETAP_EJTAGBOOT);
	
	//
	// Now we can deassert MCLR.
	//
	MCLR = 1;
/*
	//
	// Now we give the erase command time to finish.
	// 48 / 4 = 12 cycles per us gives 12000 cycles per 1 ms.
	//
	Delay1KTCYx(12);

	//
	// Wait for the Flash Controller to finish.
	//	
	SendTapCommand(MTAP_SW_MTAP);
	SendTapCommand(MTAP_COMMAND);
	while ((control = XferData8(MCHP_STATUS)) & MCHP_STATUS_FCBUSY);
	SendTapCommand(MTAP_SW_ETAP);
*/
	SendOk();
}

//---------------------------------------------------------------------------
// GetNVMCON
//---------------------------------------------------------------------------
unsigned long GetNVMCON(void)
{
	XferInstruction(0x8C910000);  // lw          s1,0(a0)
	XferInstruction(0xACB10000);  // sw          s1,0(a1)

	return XferFastData(0x00000000);
}

void Set(unsigned char offset, unsigned long value)
{
	XferInstruction(0x3C110000 | ((value & 0xffff0000) >> 16));  // lui s1,<value{31:16}>
	XferInstruction(0x36310000 | ((value & 0x0000ffff)      ));  // ori s1,s1,<value[15:0]>
	XferInstruction(0xAC910000 | offset);                        // sw  s1,<offset>(a0)
}
		
//---------------------------------------------------------------------------
// ExecuteNVM
//---------------------------------------------------------------------------
unsigned long ExecuteNVM(unsigned long mop, unsigned long address, unsigned long data, unsigned long src_address)
{
	//
	// a0 = 1f80f400
	// a1 = ff200004
	// s2 = mop
	// s3 = 0x4000
	//
	XferInstruction(0x3C041F80);        // lui         a0,0x1f80
	XferInstruction(0x3484F400);        // ori         a0,a0,0xf400
	XferInstruction(0x3C05FF20);        // lui         a1,0xff20
	XferInstruction(0x34A50004);        // ori         a1,a1,0x0004

	Set(NVMADDR_OFFSET, address);
	Set(NVMDATA_OFFSET, data);
	Set(NVMSRCADDR_OFFSET, src_address);
	Set(NVMCON_OFFSET, mop);

	XferInstruction(0x8C910000);
	XferInstruction(0xACB10000);
	XferFastData(0x00000000);
	XferInstruction(0x8C910020);
	XferInstruction(0xACB10000);
	XferFastData(0x00000000);
	XferInstruction(0x8C910030);
	XferInstruction(0xACB10000);
	XferFastData(0x00000000);

	//
	//	NVMKEY = 0xaa996655;
	//	NVMKEY = 0x556699aa;
	//	NVMCONSET = 0x00008000;
	//
	XferInstruction(0x3C11AA99);  // lui         s1,0xaa99
	XferInstruction(0x36316655);  // ori         s1,s1,0x6655
	XferInstruction(0x3C125566);  // lui         s2,0x5566
	XferInstruction(0x365299AA);  // ori         s2,s2,0x99aa
	XferInstruction(0x3C130000);  // lui         s3,0x0
	XferInstruction(0x36738000);  // ori         s3,s3,0x8000
	XferInstruction(0xAC910010);  // sw          s1,16(a0)      NVMKEY = 0xaa996655;
	XferInstruction(0xAC920010);  // sw          s2,16(a0)      NVMKEY = 0x556699aa;
	XferInstruction(0xAC930008);  // sw          s3,8(a0)      	NVMCONSET = 0x00008000;

	while(GetNVMCON() & 0x00008000);

	XferInstruction(0x00000000);  // nop         
	XferInstruction(0x00000000);  // nop         
	XferInstruction(0x00000000);  // nop         
	XferInstruction(0x00000000);  // nop         
	XferInstruction(0x00000000);  // nop         

	//
	//	NVMCONCLR = 0x4000;
	//
	Set(NVMCONCLR_OFFSET, 0x4000);

	return GetNVMCON();
}

//---------------------------------------------------------------------------
// WriteWords
//---------------------------------------------------------------------------
void WriteWords(unsigned long address,
				unsigned long *words,
				unsigned char number_of_words)
{
	unsigned long nvmcon;
	unsigned char i;
	unsigned long read_word;

	for (i = 0; i < number_of_words; i++)
	{
		nvmcon = ExecuteNVM(0x4001, address, words[i], 0);
		
		if (nvmcon & 0x00002000)
		{
/*			unsigned long debug_words[2];
			debug_words[0] = address;
			debug_words[1] = nvmcon;
			SendDebug((unsigned char *)debug_words, 8);
*/			
			SendFail();
			return;
		}
		
	    //
	    // Read memory location.
	    //
        XferInstruction(0x3C030000 | (address >> 16));        // lui v1,<FLASH_WORD_ADDR(31:16)>"
        XferInstruction(0x34630000 | (address & 0x0000ffff)); // ori v1,v1,<FLASH_WORD_ADDR(15:0)>"
        //
        // Write data to FastData location.
        //
		XferInstruction(0x3C02FF20); // lui v0,0xff20
		XferInstruction(0x34420008); // ori v0,v0,0x8
        XferInstruction(0x8C630000); // lw v1,0(v1)
        XferInstruction(0xAC430000); // sw v1,0(v0)

        //
        // Read the fastdata register.
        //
        read_word = XferFastData(0x00000000);


		if (words[i] != read_word)
		{
/*			unsigned long debug_words[4];
			debug_words[0] = address;
			debug_words[1] = nvmcon;
			debug_words[2] = words[i];
			debug_words[3] = read_word;
			SendDebug((unsigned char *)debug_words, 16);
*/			
			SendFail();
			return;
		}

		address += 4;  
	}

	SendOk();
}

//---------------------------------------------------------------------------
// ReadWords
//---------------------------------------------------------------------------
void ReadWords(unsigned long address,
			   unsigned char number_of_words)
{
    unsigned char i = 0;
	unsigned long words[16];
	unsigned char mchp_status;

    //
    // Initialise some constants.
    //
	XferInstruction(0x3C02FF20);    // lui v0,0xff20
	XferInstruction(0x34420004);    // ori v0,v0,0x4
    
    for (i = 0; i < number_of_words; i++)
    {
	    //
	    // Read memory location.
	    //
        XferInstruction(0x3C030000 | (address >> 16));        // lui v1,<FLASH_WORD_ADDR(31:16)>"
        XferInstruction(0x34630000 | (address & 0x0000ffff)); // ori v1,v1,<FLASH_WORD_ADDR(15:0)>"
        
        //
        // Write data to FastData location.
        //
        XferInstruction(0x8C630000); // lw v1,0(v1)
        XferInstruction(0xAC430000); // sw v1,0(v0)
 
        //
        // Read the fastdata register.
        //
        words[i] = XferFastData(0x00000000);
//		SendDebug((unsigned char *)&words[i], 4);
		
        address += 4;
    }

    SendInPacket((unsigned char *)words, number_of_words * 4);
}

//---------------------------------------------------------------------------
// SendWords
//---------------------------------------------------------------------------
void SendWords(unsigned char offset,
			   unsigned long *words)
{
	unsigned char i;
	
	XferInstruction(0x3c100000); // lui s0, 0x0000
	
	for (i = 0; i < 8; i++) // 32 bytes is 8 words
	{
		XferInstruction(0x3c080000 | (words[i] >> 16));    // lui t0,<DATA[31:16]>
		XferInstruction(0x35080000 | (words[i] & 0xffff)); // ori t0,<DATA[15:0]>
		XferInstruction(0xae080000 | (offset + i * 4));    // sw t0,<OFFSET>(s0)
	}

	SendOk();
}

//---------------------------------------------------------------------------
// WriteWords
//---------------------------------------------------------------------------
void ProgramWords(unsigned long address)
{
	unsigned long nvmcon;
	nvmcon = ExecuteNVM(0x4003, address, 0, 0x00000000);
	
	if (nvmcon & 0x00002000)
	{
/*		unsigned long debug_words[2];
		debug_words[0] = address;
		debug_words[1] = nvmcon;
		SendDebug((unsigned char *)debug_words, 8);
*/			
		SendFail();
		return;
	}

	SendOk();		
}

//---------------------------------------------------------------------------
// ExitProgrammingMode
//---------------------------------------------------------------------------
void ExitProgrammingMode(void)
{
	//
	// Reset the TAP.
	//
	ResetTap();
	
	//
	// Briefly assert MCLR, thus reseting the device. Make sure we set the
    // programming pins to input, as the newly programmed PIC32 may drive them
    // as soon as we release MCLR.
	//
	MCLR = 0;
	Delayms(1);
    DeallocateOutputPins32();
	MCLR = 1;
	
	SendOk();
}

//---------------------------------------------------------------------------
// ProcessMessage
//---------------------------------------------------------------------------
void ProcessMessage32(unsigned char *message, unsigned char length)
{
	unsigned long address;
	
	switch (message[0])
	{
	case COMMAND_CHECK_DEVICE:
        CheckDevice();
		break;
		
	case COMMAND_CHIP_ERASE:
		ChipErase();
		break;
		
	case COMMAND_ENTER_SERIAL_EXECUTION_MODE:
		EnterSerialExecutionMode();
		break;
		
	case COMMAND_WRITE_WORDS:
		address = *(unsigned long *)&message[1];
		WriteWords(address, (unsigned long *)&message[6], message[5]);
		break;
		
	case COMMAND_READ_WORDS:
		address = *(unsigned long *)&message[1];
		ReadWords(address, message[5]);
		break;
		
	case COMMAND_SEND_WORDS:
		SendWords(message[1], (unsigned long *)&message[2]);
		break;
		
	case COMMAND_PROGRAM_WORDS:
		address = *(unsigned long *)&message[1];
		ProgramWords(address);
		break;
		
	case COMMAND_EXIT_PROGRAMMING_MODE:
		ExitProgrammingMode();
		break;
	}
}
