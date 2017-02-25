/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15
        Device            :  PIC18F25K50
        Version           :  1.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set PGC aliases
#define PGC_TRIS               TRISBbits.TRISB0
#define PGC_LAT                LATBbits.LATB0
#define PGC_PORT               PORTBbits.RB0
#define PGC_WPU                WPUBbits.WPUB0
#define PGC_ANS                ANSELBbits.ANSB0
#define PGC_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define PGC_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define PGC_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define PGC_GetValue()           PORTBbits.RB0
#define PGC_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define PGC_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define PGC_SetPullup()      do { WPUBbits.WPUB0 = 1; } while(0)
#define PGC_ResetPullup()    do { WPUBbits.WPUB0 = 0; } while(0)
#define PGC_SetAnalogMode()  do { ANSELBbits.ANSB0 = 1; } while(0)
#define PGC_SetDigitalMode() do { ANSELBbits.ANSB0 = 0; } while(0)

// get/set PGD aliases
#define PGD_TRIS               TRISBbits.TRISB1
#define PGD_LAT                LATBbits.LATB1
#define PGD_PORT               PORTBbits.RB1
#define PGD_WPU                WPUBbits.WPUB1
#define PGD_ANS                ANSELBbits.ANSB1
#define PGD_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define PGD_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define PGD_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define PGD_GetValue()           PORTBbits.RB1
#define PGD_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define PGD_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define PGD_SetPullup()      do { WPUBbits.WPUB1 = 1; } while(0)
#define PGD_ResetPullup()    do { WPUBbits.WPUB1 = 0; } while(0)
#define PGD_SetAnalogMode()  do { ANSELBbits.ANSB1 = 1; } while(0)
#define PGD_SetDigitalMode() do { ANSELBbits.ANSB1 = 0; } while(0)

// get/set VDD aliases
#define VDD_TRIS               TRISBbits.TRISB2
#define VDD_LAT                LATBbits.LATB2
#define VDD_PORT               PORTBbits.RB2
#define VDD_WPU                WPUBbits.WPUB2
#define VDD_ANS                ANSELBbits.ANSB2
#define VDD_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define VDD_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define VDD_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define VDD_GetValue()           PORTBbits.RB2
#define VDD_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define VDD_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define VDD_SetPullup()      do { WPUBbits.WPUB2 = 1; } while(0)
#define VDD_ResetPullup()    do { WPUBbits.WPUB2 = 0; } while(0)
#define VDD_SetAnalogMode()  do { ANSELBbits.ANSB2 = 1; } while(0)
#define VDD_SetDigitalMode() do { ANSELBbits.ANSB2 = 0; } while(0)

// get/set VPP aliases
#define VPP_TRIS               TRISBbits.TRISB3
#define VPP_LAT                LATBbits.LATB3
#define VPP_PORT               PORTBbits.RB3
#define VPP_WPU                WPUBbits.WPUB3
#define VPP_ANS                ANSELBbits.ANSB3
#define VPP_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define VPP_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define VPP_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define VPP_GetValue()           PORTBbits.RB3
#define VPP_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define VPP_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define VPP_SetPullup()      do { WPUBbits.WPUB3 = 1; } while(0)
#define VPP_ResetPullup()    do { WPUBbits.WPUB3 = 0; } while(0)
#define VPP_SetAnalogMode()  do { ANSELBbits.ANSB3 = 1; } while(0)
#define VPP_SetDigitalMode() do { ANSELBbits.ANSB3 = 0; } while(0)

// get/set TMS aliases
#define TMS_TRIS               TRISBbits.TRISB4
#define TMS_LAT                LATBbits.LATB4
#define TMS_PORT               PORTBbits.RB4
#define TMS_WPU                WPUBbits.WPUB4
#define TMS_ANS                ANSELBbits.ANSB4
#define TMS_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define TMS_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define TMS_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define TMS_GetValue()           PORTBbits.RB4
#define TMS_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define TMS_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define TMS_SetPullup()      do { WPUBbits.WPUB4 = 1; } while(0)
#define TMS_ResetPullup()    do { WPUBbits.WPUB4 = 0; } while(0)
#define TMS_SetAnalogMode()  do { ANSELBbits.ANSB4 = 1; } while(0)
#define TMS_SetDigitalMode() do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set MCLR aliases
#define MCLR_TRIS               TRISBbits.TRISB5
#define MCLR_LAT                LATBbits.LATB5
#define MCLR_PORT               PORTBbits.RB5
#define MCLR_WPU                WPUBbits.WPUB5
#define MCLR_ANS                ANSELBbits.ANSB5
#define MCLR_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define MCLR_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define MCLR_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define MCLR_GetValue()           PORTBbits.RB5
#define MCLR_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define MCLR_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define MCLR_SetPullup()      do { WPUBbits.WPUB5 = 1; } while(0)
#define MCLR_ResetPullup()    do { WPUBbits.WPUB5 = 0; } while(0)
#define MCLR_SetAnalogMode()  do { ANSELBbits.ANSB5 = 1; } while(0)
#define MCLR_SetDigitalMode() do { ANSELBbits.ANSB5 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/