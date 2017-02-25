/*
 * Copyright (C) 2017 Johan Bergkvist
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
#include <xc.h>
#include "mcc_generated_files/mcc.h"

void Delay100ns(void)
{
    //
    // 12 instructions per us gives 80ns per instruction. So two NOPs will do.
    //
    asm("nop");
    asm("nop");
}

void Delayus(int us)
{
    while (us-- > 0) {
    	__delay_us(1);
    }
}

void Delayms(int ms)
{
    while (ms-- > 0) {
        __delay_ms(1);
    }
}
