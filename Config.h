//============================================================================
// CONFIG WORDS
//============================================================================
#pragma config	PLLDIV = 2			// 8MHz --> 4MHz
#pragma config	CPUDIV = OSC1_PLL2	// 96MHz --> 48MHz for CPU
//#pragma config	CPUDIV = OSC2_PLL3	// 96MHz --> 32MHz for CPU
//#pragma config	CPUDIV = OSC3_PLL4	// 96MHz --> 24MHz for CPU
//#pragma config	CPUDIV = OSC4_PLL6	// 96MHz --> 16MHz for CPU
#pragma config	USBDIV = 2			// USB used PPL
#pragma config	FOSC = HSPLL_HS		// HS oscillator, PLL enabled, HS used by USB

#pragma config	FCMEN = OFF         // Fail-Safe Clock Monitor Enable
#pragma config	IESO = OFF          // Internal/External Oscillator Switchover
#pragma config	PWRT = ON           // Power-up Timer Enable
#pragma config	BOR = OFF           // Brown-out Reset Enable
#pragma config	BORV = 0            // Brown-out Voltage: 0 (max) - 3 (min)
#pragma config	VREGEN = ON         // USB Voltage Regulator Enable
#pragma config	WDT = OFF           // Watchdog Timer Enable
#pragma config	WDTPS = 1           // Watchdog Timer Postscale Select
#pragma config	MCLRE = OFF         // MCLR Pin Enable
#pragma config	LPT1OSC = OFF       // Low-Power Timer 1 Oscillator Enable
#pragma config	PBADEN = OFF        // PORTB<4:0> digital on reset
#pragma config	CCP2MX = OFF        // CCP2 MUX
#pragma config	STVREN = OFF        // Stack Full/Underflow Reset Enable
#pragma config	LVP = OFF           // Single-Supply ICSP Enable
//#pragma config	ICPRT = OFF         // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable
#pragma config	XINST = OFF         // Extended Instruction Set Enable
#pragma config	DEBUG = OFF         // Background Debugger Enable
#pragma config	CP0 = OFF           // Code Protection bit Block 0
#pragma config	CP1 = OFF           // Code Protection bit Block 1
#pragma config	CP2 = OFF           // Code Protection bit Block 2
//#pragma config	CP3 = OFF           // Code Protection bit Block 3
#pragma config	CPB = OFF           // Boot Block Code Protection bit
#pragma config	CPD = OFF           // Data EEPROM Code Protection bit
#pragma config	WRT0 = OFF          // Write Protection bit Block 0
#pragma config	WRT1 = OFF          // Write Protection bit Block 1
#pragma config	WRT2 = OFF          // Write Protection bit Block 2
//#pragma config	WRT3 = OFF          // Write Protection bit Block 3
#pragma config	WRTB = OFF          // Boot Block Write Protection bit
#pragma config	WRTC = OFF          // Configuration Register Write Protection
#pragma config	WRTD = OFF          // Data EEPROM Write Protection
#pragma config	EBTR0 = OFF         // Table Read Protection bit Block 0
#pragma config	EBTR1 = OFF         // Table Read Protection bit Block 1
#pragma config	EBTR2 = OFF         // Table Read Protection bit Block 2
//#pragma config	EBTR3 = OFF         // Table Read Protection bit Block 3
#pragma config	EBTRB = OFF         // Boot Block Table Read Protection
