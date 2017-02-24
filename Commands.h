//===========================================================================
//
// Name
//   Commands.h
//
// Description
//   
//===========================================================================
#ifndef COMMANDS_H
#define COMMANDS_H

//
// These are the commands used between the PC and the programmer when
// programming PIC16F devices.
//
#define COMMAND_READ_BYTES_16                  0x20
#define COMMAND_PROGRAM_BYTES_16               0x21
#define	COMMAND_PROGRAM_CONFIG_16              0x22
#define COMMAND_ERASE_16                       0x23
#define COMMAND_VDD_ON_16                      0x24
#define COMMAND_VPP_ON_16                      0x25
#define COMMAND_VPP_VDD_OFF_16                 0x26

//
// These are the commands used between the PC and the programmer when
// programming PIC18F and PIC16F devices.
//
#define COMMAND_READ_BYTES                  0x00
#define COMMAND_PROGRAM_BYTES               0x01
#define	COMMAND_PROGRAM_CONFIG              0x02
#define COMMAND_ERASE                       0x03
#define COMMAND_VDD_ON                      0x04
#define COMMAND_VPP_ON                      0x05
#define COMMAND_VPP_VDD_OFF                 0x06

//
// These are the commands used between the PC and the programmer when
// programming PIC32MX devices.
//
#define COMMAND_CHECK_DEVICE                0x10
#define COMMAND_CHIP_ERASE		    0x11
#define COMMAND_ENTER_SERIAL_EXECUTION_MODE 0x12
#define COMMAND_EXIT_PROGRAMMING_MODE       0x13
#define COMMAND_WRITE_WORDS                 0x14
#define COMMAND_READ_WORDS                  0x15
#define COMMAND_SEND_WORDS                  0x16
#define COMMAND_PROGRAM_WORDS               0x17

#endif
