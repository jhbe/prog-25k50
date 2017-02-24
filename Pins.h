#ifndef PINS_H
#define PINS_H

//
// Debug
//
#define TRIGGER        LATAbits.LATA0
#define TRIS_TRIGGER   TRISAbits.TRISA0
#define TRIGGER_2      LATAbits.LATA1
#define TRIS_TRIGGER_2 TRISAbits.TRISA1

//
// Prog18
//
#define VPP      LATBbits.LATB3
#define VDD      LATBbits.LATB2
#define PGD_OUT  LATBbits.LATB1
#define PGD_IN   PORTBbits.RB1
#define PGC      LATBbits.LATB0

#define TRIS_VPP  TRISBbits.TRISB3
#define TRIS_VDD  TRISBbits.TRISB2
#define TRIS_PGD  TRISBbits.TRISB1
#define TRIS_PGC  TRISBbits.TRISB0

//
// Prog32
//
#define MCLR      LATBbits.LATB5
#define TMS       LATBbits.LATB4
#define TDO       PORTBbits.RB2
#define TCK       LATBbits.LATB1
#define TDI       LATBbits.LATB0

#define TRIS_MCLR TRISBbits.TRISB5
#define TRIS_TMS  TRISBbits.TRISB4
#define TRIS_TDO  TRISBbits.TRISB2
#define TRIS_TCK  TRISBbits.TRISB1
#define TRIS_TDI  TRISBbits.TRISB0

#endif
