/*
 * Copyright (C) 2017 Johan Bergkvist
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
#ifndef PINS_H
#define PINS_H

#include <xc.h>

//
// PIC16 and PIC18
//
#define VPP      LATBbits.LB3
#define VDD      LATBbits.LB2
#define PGD_OUT  LATBbits.LB1
#define PGD_IN   PORTBbits.RB1
#define PGC      LATBbits.LB0

#define TRIS_VPP  TRISBbits.RB3
#define TRIS_VDD  TRISBbits.RB2
#define TRIS_PGD  TRISBbits.RB1
#define TRIS_PGC  TRISBbits.RB0

//
// PIC32
//
#define MCLR      LATBbits.LB5
#define TMS       LATBbits.LB4
#define TDO       PORTBbits.RB2
#define TCK       LATBbits.LB1
#define TDI       LATBbits.LB0

#define TRIS_MCLR TRISBbits.RB5
#define TRIS_TMS  TRISBbits.RB4
#define TRIS_TDO  TRISBbits.RB2
#define TRIS_TCK  TRISBbits.RB1
#define TRIS_TDI  TRISBbits.RB0

#endif
