/*
 * Copyright (C) 2017 Johan Bergkvist
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
#ifndef USB_H
#define USB_H

#include <usb.h>

extern unsigned char out_packet[USBGEN_EP_SIZE];
extern unsigned char in_packet[USBGEN_EP_SIZE];

extern USB_HANDLE out_handle;
extern USB_HANDLE in_handle;

void SendOk(void);
void SendFail(void);
void SendResult8(unsigned char result);
void SendResult32(unsigned long result);
void SendDebug(unsigned char *data, unsigned char length);
void SendText(const char *data);

void SendInPacket(unsigned char *bytes, unsigned char length);

#endif
