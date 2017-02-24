#ifndef TAP_H
#define TAP_H

#define PRACC_MASK          0x00040000
#define ROCC_MASK           0x80000000

#define MTAP_COMMAND        0x07
#define MTAP_SW_MTAP        0x04
#define MTAP_SW_ETAP        0x05
#define MTAP_IDCODE         0x01

#define ETAP_ADDRESS        0x08
#define ETAP_DATA           0x09
#define ETAP_CONTROL        0x0a
#define ETAP_EJTAGBOOT      0x0c
#define ETAP_FASTDATA       0x0e

#define MCHP_STATUS         0x00
#define MCHP_ASSERT_RST     0xd1
#define MCHP_DEASSERT_RST   0xd0
#define MCHP_ERASE          0xfc
#define MCHP_FLASH_ENABLE   0xfe
#define MCHP_FLASH_DISABLE  0xfd

#define MCHP_STATUS_DEVRST  0x01
#define MCHP_STATUS_FAEN    0x02
#define MCHP_STATUS_FCBUSY  0x04
#define MCHP_STATUS_CFGRDY  0x08
#define MCHP_STATUS_NVMERR  0x20
#define MCHP_STATUS_CPS     0x80

void ResetTap(void);
void TapToShiftIRState(void);
void TapToShiftDRState(void);
void TapToRunTestIdleState(void);
void SendTapCommand(unsigned char command);
void ShiftOneBit8(unsigned char mask, unsigned char in_data, unsigned char *out_data);
unsigned char XferData8(unsigned char in_data);
unsigned long XferData32(unsigned long in_data);

#endif
