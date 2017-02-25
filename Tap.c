/*
 * Copyright (C) 2017 Johan Bergkvist
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
#include <xc.h>
#include "Tap.h"
#include "Pins.h"
#include "Delays.h"

//---------------------------------------------------------------------------
// Delay
//---------------------------------------------------------------------------
void Delay(void)
{
	Delayus(1);
}

//---------------------------------------------------------------------------
// ResetTap
//
// Forces the TAP state machine into the "Run Test/Idle" state.
//---------------------------------------------------------------------------
void ResetTap(void)
{
    TMS = 1; Delay(); TCK = 1; Delay(); TCK = 0; Delay(); 
                      TCK = 1; Delay(); TCK = 0; Delay(); 
                      TCK = 1; Delay(); TCK = 0; Delay(); 
                      TCK = 1; Delay(); TCK = 0; Delay(); 
                      TCK = 1; Delay(); TCK = 0; Delay(); 
    TMS = 0; Delay(); TCK = 1; Delay(); TCK = 0; Delay(); 
}

//---------------------------------------------------------------------------
// TapToShiftIRState
//
// Transitions the TAP state machine from the "Run Test/Idle" state to the
// "Shift IR" state.
//---------------------------------------------------------------------------
void TapToShiftIRState(void)
{
    TMS = 1; Delay(); TCK = 1; Delay(); TCK = 0; Delay(); 
                      TCK = 1; Delay(); TCK = 0; Delay(); 
    TMS = 0; Delay(); TCK = 1; Delay(); TCK = 0; Delay(); 
                      TCK = 1; Delay(); TCK = 0; Delay(); 
}

//---------------------------------------------------------------------------
// TapToShiftDRState
//
// Transitions the TAP state machine from the "Run Test/Idle" state to the
// "Shift DR" state.
//---------------------------------------------------------------------------
void TapToShiftDRState(void)
{
    TMS = 1; Delay(); TCK = 1; Delay(); TCK = 0; Delay(); 
    TMS = 0; Delay(); TCK = 1; Delay(); TCK = 0; Delay(); 
                      TCK = 1; Delay(); TCK = 0; Delay(); 
}

//---------------------------------------------------------------------------
// TapToRunTestIdleState
//
// Return the TAP state from either the "Shift IR" or "Shift DR" states back
// to the "Run Test/Idle" state.
//---------------------------------------------------------------------------
void TapToRunTestIdleState(void)
{
    TMS = 1; Delay(); TCK = 1; Delay(); TCK = 0; Delay(); 
    TMS = 0; Delay(); TCK = 1; Delay(); TCK = 0; Delay(); 
}

//---------------------------------------------------------------------------
// SendTapCommand
//
// 
//---------------------------------------------------------------------------
void SendTapCommand(unsigned char command)
{
    TapToShiftIRState();

	TMS = 0;
	TDI = (command & 0x01) != 0 ? 1 : 0; Delay(); TCK = 1; Delay(); TCK = 0; Delay(); 
	TDI = (command & 0x02) != 0 ? 1 : 0; Delay(); TCK = 1; Delay(); TCK = 0; Delay(); 
	TDI = (command & 0x04) != 0 ? 1 : 0; Delay(); TCK = 1; Delay(); TCK = 0; Delay(); 
	TDI = (command & 0x08) != 0 ? 1 : 0; Delay(); TCK = 1; Delay(); TCK = 0; Delay(); 
	TMS = 1;
	TDI = (command & 0x10) != 0 ? 1 : 0; Delay(); TCK = 1; Delay(); TCK = 0; Delay(); 

    TapToRunTestIdleState();
}

//---------------------------------------------------------------------------
// ShiftOneBit8
//
// 
//---------------------------------------------------------------------------
void ShiftOneBit8(unsigned char mask, unsigned char in_data, unsigned char *out_data)
{
	TDI = (in_data & mask) ? 1 : 0;
	TCK = 1;
	if (TDO)
	{
		*out_data |= mask;
	}
	TCK = 0;
}

//---------------------------------------------------------------------------
// XferData8
//
// 
//---------------------------------------------------------------------------
unsigned char XferData8(unsigned char in_data)
{
    unsigned char out_data = 0;

    TapToShiftDRState();
    
	TMS = 0;
	ShiftOneBit8(0x01, in_data, &out_data);
	ShiftOneBit8(0x02, in_data, &out_data);
	ShiftOneBit8(0x04, in_data, &out_data);
	ShiftOneBit8(0x08, in_data, &out_data);
	ShiftOneBit8(0x10, in_data, &out_data);
	ShiftOneBit8(0x20, in_data, &out_data);
	ShiftOneBit8(0x40, in_data, &out_data);
	TMS = 1;
	ShiftOneBit8(0x80, in_data, &out_data);

    TapToRunTestIdleState();

    return out_data;
}

//---------------------------------------------------------------------------
// XferData32
//
// 
//---------------------------------------------------------------------------
unsigned long XferData32(unsigned long in_data)
{
	unsigned long out_data = 0;
    unsigned char i;
    unsigned char *in_data_bytes = (unsigned char *)&in_data;
    unsigned char *out_data_bytes = (unsigned char *)&out_data;

    TapToShiftDRState();
	
	TMS = 0;
	ShiftOneBit8(0x01, in_data_bytes[0], &out_data_bytes[0]);
	ShiftOneBit8(0x02, in_data_bytes[0], &out_data_bytes[0]);
	ShiftOneBit8(0x04, in_data_bytes[0], &out_data_bytes[0]);
	ShiftOneBit8(0x08, in_data_bytes[0], &out_data_bytes[0]);
	ShiftOneBit8(0x10, in_data_bytes[0], &out_data_bytes[0]);
	ShiftOneBit8(0x20, in_data_bytes[0], &out_data_bytes[0]);
	ShiftOneBit8(0x40, in_data_bytes[0], &out_data_bytes[0]);
	ShiftOneBit8(0x80, in_data_bytes[0], &out_data_bytes[0]);

	ShiftOneBit8(0x01, in_data_bytes[1], &out_data_bytes[1]);
	ShiftOneBit8(0x02, in_data_bytes[1], &out_data_bytes[1]);
	ShiftOneBit8(0x04, in_data_bytes[1], &out_data_bytes[1]);
	ShiftOneBit8(0x08, in_data_bytes[1], &out_data_bytes[1]);
	ShiftOneBit8(0x10, in_data_bytes[1], &out_data_bytes[1]);
	ShiftOneBit8(0x20, in_data_bytes[1], &out_data_bytes[1]);
	ShiftOneBit8(0x40, in_data_bytes[1], &out_data_bytes[1]);
	ShiftOneBit8(0x80, in_data_bytes[1], &out_data_bytes[1]);

	ShiftOneBit8(0x01, in_data_bytes[2], &out_data_bytes[2]);
	ShiftOneBit8(0x02, in_data_bytes[2], &out_data_bytes[2]);
	ShiftOneBit8(0x04, in_data_bytes[2], &out_data_bytes[2]);
	ShiftOneBit8(0x08, in_data_bytes[2], &out_data_bytes[2]);
	ShiftOneBit8(0x10, in_data_bytes[2], &out_data_bytes[2]);
	ShiftOneBit8(0x20, in_data_bytes[2], &out_data_bytes[2]);
	ShiftOneBit8(0x40, in_data_bytes[2], &out_data_bytes[2]);
	ShiftOneBit8(0x80, in_data_bytes[2], &out_data_bytes[2]);

	ShiftOneBit8(0x01, in_data_bytes[3], &out_data_bytes[3]);
	ShiftOneBit8(0x02, in_data_bytes[3], &out_data_bytes[3]);
	ShiftOneBit8(0x04, in_data_bytes[3], &out_data_bytes[3]);
	ShiftOneBit8(0x08, in_data_bytes[3], &out_data_bytes[3]);
	ShiftOneBit8(0x10, in_data_bytes[3], &out_data_bytes[3]);
	ShiftOneBit8(0x20, in_data_bytes[3], &out_data_bytes[3]);
	ShiftOneBit8(0x40, in_data_bytes[3], &out_data_bytes[3]);
	TMS = 1;
	ShiftOneBit8(0x80, in_data_bytes[3], &out_data_bytes[3]);

    TapToRunTestIdleState();

	return out_data;
}
