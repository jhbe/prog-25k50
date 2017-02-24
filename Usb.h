#ifndef USB_H
#define USB_H

#include <usb/usb.h>
#include <usb/usb_function_generic.h>

extern unsigned char out_packet[USBGEN_EP_SIZE];
extern unsigned char in_packet[USBGEN_EP_SIZE];

extern USB_HANDLE out_handle;
extern USB_HANDLE in_handle;

void SendOk(void);
void SendFail(void);
void SendResult8(unsigned char result);
void SendResult32(unsigned long result);
void SendDebug(unsigned char *data, unsigned char length);
void SendText(const rom char *data);

void SendInPacket(unsigned char *bytes, unsigned char length);

#endif
