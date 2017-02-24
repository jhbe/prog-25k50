//===========================================================================
//
// Name
//   Pic18
//
// Description
//   This is the firmware for the 18f2550 chip allowing it to program another
//   18f2550 (and others). It communicates with a PC over USB.
//
//   The programming specification is Microchip document DS39622F titled
//   "Flash Microcontroller Programming Specification". This firmware
//   implements the high-voltage In-Circuit Serial Programming (ICSP)
//   specification.
//
//   The following table illustrate how pins on the 18f2550 should be
//   connected to the target (another 18f2550):
//
//                          18f2550 | target
//                          ================
//                            RB3   |  MCLR
//                            RB2   |  VDD
//                            RB1   |  PGD
//                            RB0   |  PGC
//
//   
//
//===========================================================================
#include <p18cxxx.h>
#include <delays.h>
#include "Pins.h"
#include "Usb.h"
#include "Commands.h"

//
// These are (some of) the four bit commands sent to a device when
// programming. See page 11 of Microchip document DS39622F.
//
#define CORE							0x00
#define TABLE_READ_POSTINC				0x09
#define TABLE_WRITE						0x0c
#define TABLE_WRITE_POSTINC				0x0d
#define TABLE_WRITE_START_PROGRAM		0x0f

//---------------------------------------------------------------------------
// Wait2ms
//---------------------------------------------------------------------------
void Wait2ms(void)
{
	//
	// 48 / 4 = 12 cycles per us gives 24000 cycles per 2 ms.
	//
	Delay1KTCYx(24);
}

//---------------------------------------------------------------------------
// Wait6ms
//---------------------------------------------------------------------------
void Wait6ms(void)
{
	//
	// 48 / 4 = 12 cycles per us gives 72000 cycles per 6 ms.
	//
	Delay1KTCYx(72);
}

//---------------------------------------------------------------------------
// WriteCommand (Send4Bits)
//
// Pulses the PGC four times, each time holding PGD according to the lsb of
// the "command", then shifting "command" to the right. This has the effect of
// sending the 4 lowest bits in "commamd" lsb first.
//---------------------------------------------------------------------------
void WriteCommand(unsigned char command)
{
	unsigned char number_of_bits = 4;
	do
	{
		PGD_OUT = command & 0x01;
		command = command >> 1;
		PGC = 1;
		PGC = 0;
	}
	while (--number_of_bits != 0);
}

//---------------------------------------------------------------------------
// WriteByte (Send8Bits)
//
// Pulses the PGC eight times, each time holding PGD according to the lsb of
// the "byte", then shifting "byte" to the right. This has the effect of
// sending the 8 bits in "byte" lsb first.
//---------------------------------------------------------------------------
void WriteByte(unsigned char byte)
{
	unsigned char number_of_bits = 8;
	do
	{
		PGD_OUT = byte & 0x01;
		byte = byte >> 1;
		PGC = 1;
		PGC = 0;
	}
	while (--number_of_bits != 0);
}

//---------------------------------------------------------------------------
// ReadByte
//
// Sets PGD to input, then pulsing PGC 8 times. The state of PGD sets the
// msb before it is shifted right once for each PGC cycle. This has the
// effect of reading 8 bits lsb first and that byte is returned from this
// function.
//---------------------------------------------------------------------------
unsigned char ReadByte(void)
{
	unsigned char byte = 0;
	unsigned char i;
	
	TRIS_PGD = 1;
	
	for (i = 0; i < 8; i++)
	{
		PGC = 1;
		byte = ((byte >> 1) & 0x7f);
		if (PGD_IN)
		{
			byte |= 0x80;
		}
		PGC = 0;
	}

	TRIS_PGD = 0;
	return byte;
}

//---------------------------------------------------------------------------
// WriteCore
//---------------------------------------------------------------------------
void WriteCore(unsigned char arg1, unsigned char arg2)
{
	WriteCommand(CORE);
	WriteByte(arg1);
	WriteByte(arg2);
}

//---------------------------------------------------------------------------
// WriteProgrammingCore
//---------------------------------------------------------------------------
void WriteProgrammingCore(void)
{
	unsigned char i;
	
	PGD_OUT = 0;
	for (i = 0; i < 3; i++)
	{
		PGC = 1;
		PGC = 0;
	}
	
	PGC = 1;
	Wait2ms();
	PGC = 0;
	Wait2ms();
}

//---------------------------------------------------------------------------
// TableWrite
//---------------------------------------------------------------------------
void TableWrite(unsigned char arg1, unsigned char arg2)
{
	WriteCommand(TABLE_WRITE);
	WriteByte(arg1);
	WriteByte(arg2);
}

//---------------------------------------------------------------------------
// SetTBLPTR
//---------------------------------------------------------------------------
void SetTBLPTR(unsigned char address_0, unsigned char address_1, unsigned char address_2)
{
	//
	// Load the TBLPT registers with the provided address.
	//
	WriteCore(address_0, 0x0e);   // MOVLW <arg>
	WriteCore(0xf8, 0x6e);        // MOVWF TBLTRU
	WriteCore(address_1, 0x0e);   // MOVLW <arg>
	WriteCore(0xf7, 0x6e);        // MOVWF TBLTRH
	WriteCore(address_2, 0x0e);   // MOVLW <arg>
	WriteCore(0xf6, 0x6e);        // MOVWF TBLTRL
}

//===========================================================================

//---------------------------------------------------------------------------
// ProgramBytes
//---------------------------------------------------------------------------
void ProgramBytes(unsigned char address_0,
				  unsigned char address_1,
				  unsigned char address_2,
				  unsigned char *bytes,
				  unsigned char length)
{
	unsigned char i;
	
	//
	// "BSF EECON1, EEPGD"
	//
	WriteCore(0xa6, 0x8e);

	//
	// "BSF EECON1, CFGS"
	//
	WriteCore(0xa6, 0x9c);
	
	//
	// Start writing at address "address_x".
	//
	SetTBLPTR(address_0, address_1, address_2);
	
	//
	// Write two bytes at a time and save the last two for later; they shall
	// not use POSTINC.
	//
	for (i = 0; i < length - 2; i += 2)
	{
		WriteCommand(TABLE_WRITE_POSTINC);
		WriteByte(bytes[i]);
		WriteByte(bytes[i + 1]);
	}
	
	//
	// Write the last two bytes.
	//
	WriteCommand(TABLE_WRITE_START_PROGRAM);
	WriteByte(bytes[i]);
	WriteByte(bytes[i + 1]);

	//
	// Finish with a special NOP where the fourth command PGC is held high
	// while the actual programming takes place.
	//
	WriteProgrammingCore();
	WriteByte(0);
	WriteByte(0);
	
	SendOk();
}

//---------------------------------------------------------------------------
// ProgramConfigByte
//---------------------------------------------------------------------------
void ProgramConfigByte(unsigned char offset, unsigned char byte)
{
	//
	// "BSF EECON1, EEPGD"
	//
	WriteCore(0xa6, 0x8e);

	//
	// "BSF EECON1, CFGS"
	//
	WriteCore(0xa6, 0x8c);
	
	//
	// Write "byte" to address 0x300000+offset. If the address in an even
	// address one of the bytes will be written and the other ignored. If the
	// address is odd the other bytes is used instead.
	//
	SetTBLPTR(0x30, 0x00, offset);
	
	WriteCommand(TABLE_WRITE_START_PROGRAM);
	WriteByte(byte);
	WriteByte(byte);
	
	//
	// Finish with a special NOP where the fourth command PGC is held high
	// while the actual programming takes place.
	//
	WriteProgrammingCore();
	WriteByte(0);
	WriteByte(0);
	
	SendOk();
}

//---------------------------------------------------------------------------
// ReadBytes
//---------------------------------------------------------------------------
void ReadBytes(unsigned char address_0,
			   unsigned char address_1,
			   unsigned char address_2,
			   unsigned char length)
{
	unsigned char bytes[64];
	unsigned char i;
	
	//
	// Start reading at address "address_x".
	//
	SetTBLPTR(address_0, address_1, address_2);
	
	//
	// Read "length" bytes.
	//
	for (i = 0; i < length; i++)
	{
		WriteCommand(TABLE_READ_POSTINC);
		WriteByte(0);
		bytes[i] = ReadByte();
	}
	
	SendInPacket(bytes, length);
}

//---------------------------------------------------------------------------
// Erase
//---------------------------------------------------------------------------
void Erase(void)
{
	//
	// Write 0x0f to 0x3c0005.
	//
	SetTBLPTR(0x3c, 0x00, 0x05);
	TableWrite(0x0f, 0x0f);
	
	//
	// Write 0x87 to 0x3c0004.
	//
	SetTBLPTR(0x3c, 0x00, 0x04);
	TableWrite(0x87, 0x87);
	
	//
	// NOP.
	//
	WriteCore(0x00, 0x00);
	
	//
	// Special NOP with a long gap between the end of the four bit command
	// and the 16 bit argument.
	//
	WriteCommand(CORE);
	Wait6ms();
	WriteByte(0);
	WriteByte(0);
	
	SendOk();
}

//---------------------------------------------------------------------------
// ProcessMessage
//---------------------------------------------------------------------------
void ProcessMessage18(unsigned char *message, unsigned char length)
{
	switch (message[0])
	{
	case COMMAND_VDD_ON:
	case COMMAND_VPP_ON:
	case COMMAND_VPP_VDD_OFF:
		PGC = 0;
		PGD_OUT = 0;
		switch (message[0])
		{
		case COMMAND_VDD_ON     :          VDD = 1; break;
		case COMMAND_VPP_ON     : VPP = 1;          break;
		case COMMAND_VPP_VDD_OFF: VPP = 0; VDD = 0; break;
		}
		SendOk();
		break;

	case COMMAND_ERASE:
		Erase();
		break;
		
	case COMMAND_PROGRAM_BYTES:
		ProgramBytes(message[1], message[2], message[3], &message[4], length - 4);
		break;
		
	case COMMAND_PROGRAM_CONFIG:
		ProgramConfigByte(message[1], message[2]);
		break;
		
	case COMMAND_READ_BYTES:
		ReadBytes(message[1], message[2], message[3], message[4]);
		break;
	}
}
