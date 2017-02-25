/**
  @Generated MPLAB(c) Code Configurator Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    mcc.c

  @Summary:
    This is the mcc.c file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15
        Device            :  PIC18F25K50
        Driver Version    :  1.02
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

// Configuration bits: selected in the GUI

// CONFIG1L
#pragma config PLLSEL = PLL3X    // PLL Selection->3x clock multiplier
#pragma config CFGPLLEN = ON    // PLL Enable Configuration bit->PLL Enabled
#pragma config CPUDIV = NOCLKDIV    // CPU System Clock Postscaler->CPU uses system clock (no divide)
#pragma config LS48MHZ = SYS48X8    // Low Speed USB mode with 48 MHz system clock->System clock at 48 MHz, USB clock divider is set to 8

// CONFIG1H
#pragma config FOSC = INTOSCIO    // Oscillator Selection->Internal oscillator
#pragma config PCLKEN = ON    // Primary Oscillator Shutdown->Primary oscillator enabled
#pragma config FCMEN = OFF    // Fail-Safe Clock Monitor->Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF    // Internal/External Oscillator Switchover->Oscillator Switchover mode disabled

// CONFIG2L
#pragma config nPWRTEN = OFF    // Power-up Timer Enable->Power up timer disabled
#pragma config BOREN = SBORDIS    // Brown-out Reset Enable->BOR enabled in hardware (SBOREN is ignored)
#pragma config BORV = 190    // Brown-out Reset Voltage->BOR set to 1.9V nominal
#pragma config nLPBOR = OFF    // Low-Power Brown-out Reset->Low-Power Brown-out Reset disabled

// CONFIG2H
#pragma config WDTEN = OFF    // Watchdog Timer Enable bits->WDT disabled in hardware (SWDTEN ignored)
#pragma config WDTPS = 32768    // Watchdog Timer Postscaler->1:32768

// CONFIG3H
#pragma config CCP2MX = RC1    // CCP2 MUX bit->CCP2 input/output is multiplexed with RC1
#pragma config PBADEN = OFF    // PORTB A/D Enable bit->PORTB<5:0> pins are configured as digital I/O on Reset
#pragma config T3CMX = RC0    // Timer3 Clock Input MUX bit->T3CKI function is on RC0
#pragma config SDOMX = RB3    // SDO Output MUX bit->SDO function is on RB3
#pragma config MCLRE = ON    // Master Clear Reset Pin Enable->MCLR pin enabled; RE3 input disabled

// CONFIG4L
#pragma config STVREN = ON    // Stack Full/Underflow Reset->Stack full/underflow will cause Reset
#pragma config LVP = OFF    // Single-Supply ICSP Enable bit->Single-Supply ICSP disabled
#pragma config ICPRT = OFF    // Dedicated In-Circuit Debug/Programming Port Enable->ICPORT disabled
#pragma config XINST = OFF    // Extended Instruction Set Enable bit->Instruction set extension and Indexed Addressing mode disabled
#pragma config DEBUG = OFF    // Background Debugger Enable bit->Background debugger disabled, RB6 and RB7 configured as general purpose I/O pins

// CONFIG5L
#pragma config CP0 = OFF    // Block 0 Code Protect->Block 0 is not code-protected
#pragma config CP1 = OFF    // Block 1 Code Protect->Block 1 is not code-protected
#pragma config CP2 = OFF    // Block 2 Code Protect->Block 2 is not code-protected
#pragma config CP3 = OFF    // Block 3 Code Protect->Block 3 is not code-protected

// CONFIG5H
#pragma config CPB = OFF    // Boot Block Code Protect->Boot block is not code-protected
#pragma config CPD = OFF    // Data EEPROM Code Protect->Data EEPROM is not code-protected

// CONFIG6L
#pragma config WRT0 = OFF    // Block 0 Write Protect->Block 0 (0800-1FFFh) is not write-protected
#pragma config WRT1 = OFF    // Block 1 Write Protect->Block 1 (2000-3FFFh) is not write-protected
#pragma config WRT2 = OFF    // Block 2 Write Protect->Block 2 (04000-5FFFh) is not write-protected
#pragma config WRT3 = OFF    // Block 3 Write Protect->Block 3 (06000-7FFFh) is not write-protected

// CONFIG6H
#pragma config WRTC = OFF    // Configuration Registers Write Protect->Configuration registers (300000-3000FFh) are not write-protected
#pragma config WRTB = OFF    // Boot Block Write Protect->Boot block (0000-7FFh) is not write-protected
#pragma config WRTD = OFF    // Data EEPROM Write Protect->Data EEPROM is not write-protected

// CONFIG7L
#pragma config EBTR0 = OFF    // Block 0 Table Read Protect->Block 0 is not protected from table reads executed in other blocks
#pragma config EBTR1 = OFF    // Block 1 Table Read Protect->Block 1 is not protected from table reads executed in other blocks
#pragma config EBTR2 = OFF    // Block 2 Table Read Protect->Block 2 is not protected from table reads executed in other blocks
#pragma config EBTR3 = OFF    // Block 3 Table Read Protect->Block 3 is not protected from table reads executed in other blocks

// CONFIG7H
#pragma config EBTRB = OFF    // Boot Block Table Read Protect->Boot block is not protected from table reads executed in other blocks

#include "mcc.h"

void SYSTEM_Initialize(void)
{

    PIN_MANAGER_Initialize();
    OSCILLATOR_Initialize();
}

void OSCILLATOR_Initialize(void)
{
    // SCS FOSC; IDLEN disabled; IRCF 16MHz; 
    OSCCON = 0x70;
    // INTSRC INTRC_31_25KHz; PLLEN disabled; PRISD disabled; SOSCGO disabled; 
    OSCCON2 = 0x00;
    // SPLLMULT 4xPLL; TUN 0; 
    OSCTUNE = 0x00;
    // ACTSRC SOSC; ACTUD enabled; ACTEN disabled; 
    ACTCON = 0x00;
    // Wait for PLL to stabilize
    while(PLLRDY == 0)
    {
    }
}


/**
 End of File
*/
