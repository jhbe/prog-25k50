//===========================================================================
//
// Name
//   Pic16
//
// Description
//   This is the firmware for the 18f2550 chip allowing it to program a
//   16f6x8A. It communicates with a PC over USB.
//
//   The programming specification is Microchip document DS41196E titled
//   "PIC16F6xx/A EEPROM Programming Specification". This firmware
//   implements the high-voltage In-Circuit Serial Programming (ICSP)
//   specification.
//
//   The following table illustrate how pins on the 18f2550 should be
//   connected to the target (it is the same as for the PIC18):
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
// These are the six bit commands sent to a device when
// programming. See page 5 of Microchip document DS41196E.
//
#define LOAD_CONFIGURATION				0x00
#define LOAD_DATA_FOR_PROGRAM_MEMORY	0x02
#define LOAD_DATA_FOR_DATA_MEMORY		0x03
#define INCREMENT_ADDRESS				0x06
#define READ_DATA_FROM_PROGRAM_MEMORY	0x04
#define READ_DATA_FROM_DATA_MEMORY		0x05
#define BEGIN_PROGRAMMING_CYCLE         0x08
#define BULK_ERASE_PROGRAM_MEMORY		0x09
#define BULK_ERASE_DATA_MEMORY          0x0b

unsigned short int g_current_address = 0x0000;

//---------------------------------------------------------------------------
// Delay100ns
//---------------------------------------------------------------------------
void Delay100ns(void)
{
    //
    // 12 instructions per us gives 80ns per instruction. So two NOPs will do.
    //
    _asm
        nop
        nop
    _endasm
}

//---------------------------------------------------------------------------
// Delay1us
//---------------------------------------------------------------------------
void Delay1us(void)
{
    //
    // 12 instructions per us.
    //
	Delay10TCYx(2);
}

//---------------------------------------------------------------------------
// Delay2ms
//---------------------------------------------------------------------------
void Delay2ms(void)
{
	//
	// 48 / 4 = 12 cycles per us gives 24000 cycles per 2 ms.
	//
	Delay1KTCYx(24);
}

//---------------------------------------------------------------------------
// Delay10ms
//---------------------------------------------------------------------------
void Delay10ms(void)
{
	//
	// 48 / 4 = 12 cycles per us gives 120000 cycles per 10 ms.
	//
	Delay1KTCYx(120);
}


//---------------------------------------------------------------------------
// WriteCommand (Send6Bits)
//
// Pulses the PGC six times, each time holding PGD according to the lsb of
// the "command", then shifting "command" to the right. This has the effect of
// sending the 6 lowest bits in "commamd" lsb first.
//---------------------------------------------------------------------------
void WriteCommand16(UINT8 command)
{
    unsigned char number_of_bits = 6;

    Delay1us();

    do
	{
		PGC = 1;
		PGD_OUT = command & 0x01;
		command = command >> 1;
        Delay100ns();
		PGC = 0;
        Delay100ns();
	}
	while (--number_of_bits != 0);
}

//---------------------------------------------------------------------------
// WriteWord
//---------------------------------------------------------------------------
void WriteWord16(UINT16 word)
{
	unsigned char number_of_bits = 16;

    word <<= 1;
    
    Delay1us();

    do
	{
		PGC = 1;
		PGD_OUT = word & 0x01;
		word = word >> 1;
        Delay100ns();
		PGC = 0;
        Delay100ns();
	}
	while (--number_of_bits != 0);
}

//---------------------------------------------------------------------------
// ReadWord
//---------------------------------------------------------------------------
UINT16 ReadWord16(void)
{
	UINT16 word = 0;
	unsigned char i;
	
    Delay1us();

    //
    // The 16F will go OUTPUT after the second raising clock edge.
    //
    PGC = 1;
    Delay100ns();
    PGC = 0;
	TRIS_PGD = 1;
    Delay100ns();
    PGC = 1;
    Delay100ns();

	for (i = 0; i < 14; i++)
	{
		word >>= 1;
		if (PGD_IN)
		{
			word |= 0x8000;
		}

		PGC = 0;
        Delay100ns();
        PGC = 1;
        Delay100ns();
	}

    PGC = 0;

    Delay100ns();
	TRIS_PGD = 0;

    return word >> 2;
}

//---------------------------------------------------------------------------
// ResetDevice
//
// Resets the device by dropping and then re-asserting Vpp and Vdd. This has
// the effect of resetting the PC to 0x0000.
//---------------------------------------------------------------------------
void ResetDevice16(void)
{
    VDD = 0;
    VPP = 0;

    Delay2ms();

    VPP = 1;
    Delay2ms();

    VDD = 1;
    Delay2ms();

    g_current_address = 0x0000;
}

//---------------------------------------------------------------------------
// SetPC
//
// Sets the PC to the desired address. If the address is lower than the PC,
// the device is reset. If the addrses id 0x2000 or above a LOAD_CONFIGURATION
// command is issued. Then a number of INCREMENT_ADDRESS commands are issued
// until the address is right.
//---------------------------------------------------------------------------
void SetPC16(UINT16 address)
{
    if (address < g_current_address)
    {
        ResetDevice16();
    }
    if (address >= 0x2000)
    {
        WriteCommand16(LOAD_CONFIGURATION);
        WriteWord16(0x3fff);
        g_current_address = 0x2000;
    }
    while (g_current_address < address)
    {
        WriteCommand16(INCREMENT_ADDRESS);
        g_current_address++;
    }
}

//===========================================================================

//---------------------------------------------------------------------------
// Erase
//---------------------------------------------------------------------------
void Erase16(void)
{
    WriteCommand16(LOAD_CONFIGURATION);
    WriteWord16(0xffff);
    g_current_address = 0x2000;

    WriteCommand16(LOAD_DATA_FOR_PROGRAM_MEMORY);
    WriteWord16(0x3fff);

    WriteCommand16(BULK_ERASE_PROGRAM_MEMORY);
    Delay10ms();

    WriteCommand16(BULK_ERASE_DATA_MEMORY);
    Delay10ms();

	SendOk();
}

//---------------------------------------------------------------------------
// ProgramBytes
//---------------------------------------------------------------------------
void ProgramBytes16(UINT16 address,
				    UINT8 *bytes,
				    UINT8 length)
{
	unsigned char i;

    SetPC16(address);

	for (i = 0; i < length; i += 2)
	{
        UINT word = (((UINT)(bytes[i + 1])) << 8) | (UINT)(bytes[i]);

		WriteCommand16(LOAD_DATA_FOR_PROGRAM_MEMORY);
		WriteWord16(word);

        WriteCommand16(BEGIN_PROGRAMMING_CYCLE);
        Delay10ms();

        WriteCommand16(INCREMENT_ADDRESS);
        g_current_address++;
	}
	
	SendOk();
}

//---------------------------------------------------------------------------
// ProgramConfigWord16
//---------------------------------------------------------------------------
void ProgramConfigWord16(UINT16 word)
{
    unsigned char i;

    WriteCommand16(LOAD_CONFIGURATION);
    WriteWord16(0x0000);

    WriteCommand16(INCREMENT_ADDRESS);
    WriteCommand16(INCREMENT_ADDRESS);
    WriteCommand16(INCREMENT_ADDRESS);
    WriteCommand16(INCREMENT_ADDRESS);
    WriteCommand16(INCREMENT_ADDRESS);
    WriteCommand16(INCREMENT_ADDRESS);
    WriteCommand16(INCREMENT_ADDRESS);
    g_current_address += 0x07;

    WriteCommand16(LOAD_DATA_FOR_PROGRAM_MEMORY);
    WriteWord16(word);
    WriteCommand16(BEGIN_PROGRAMMING_CYCLE);
    Delay10ms();

    SendOk();
}

//---------------------------------------------------------------------------
// ReadBytes
//---------------------------------------------------------------------------
void ReadBytes16(UINT16 address,
			     UINT8 length)
{
	unsigned char bytes[64];
	unsigned char i = 0;

    SetPC16(address);

	//
	// Read "length" bytes.
	//
	for (i = 0; i < length; i = i + 2)
	{
        UINT16 word;
        
        WriteCommand16(READ_DATA_FROM_PROGRAM_MEMORY);
		word = ReadWord16();

        bytes[i + 0] = word & 0x00ff;
        bytes[i + 1] = word >> 8;

        WriteCommand16(INCREMENT_ADDRESS);
        g_current_address++;
	}
	
	SendInPacket(bytes, length);
}

//---------------------------------------------------------------------------
// ProcessMessage
//---------------------------------------------------------------------------
void ProcessMessage16(unsigned char *message, unsigned char length)
{
    UINT16 address, word;

	switch (message[0])
	{
	case COMMAND_VDD_ON_16:
	case COMMAND_VPP_ON_16:
	case COMMAND_VPP_VDD_OFF_16:
		PGC = 0;
		PGD_OUT = 0;
        g_current_address = 0x0000;
		switch (message[0])
		{
		case COMMAND_VDD_ON_16     :          VDD = 1; break;
		case COMMAND_VPP_ON_16     : VPP = 1;          break;
		case COMMAND_VPP_VDD_OFF_16: VPP = 0; VDD = 0; break;
		}
		SendOk();
		break;

	case COMMAND_ERASE_16:
		Erase16();
		break;
		
	case COMMAND_PROGRAM_BYTES_16:
        address = ((UINT16)(message[1]) << 8) | (UINT16)(message[2]);
		ProgramBytes16(address, &message[3], length - 3);
		break;
		
	case COMMAND_PROGRAM_CONFIG_16:
        word = ((UINT16)(message[1])) | ((UINT16)(message[2]) << 8);
		ProgramConfigWord16(word);
		break;
		
	case COMMAND_READ_BYTES_16:
        address = ((UINT16)(message[1]) << 8) | (UINT16)(message[2]);
		ReadBytes16(address, message[3]);
		break;
	}
}
