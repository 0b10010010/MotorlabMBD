/*
 * Count2Angle.c
 *
 *  Created on: Apr 15, 2020
 *      Author: alexk
 */

#include <stdio.h>
#include "Count2Angle.h"
 
int32_t Count2Angle(uint32_t counter)
{
	int32_t count32;

    if ((prev_counter > 0xc000)&&(counter < 0x4000))     // overflow
        overflows += 1;
    else if ((prev_counter < 0x4000)&&(counter > 0xc000))  // underflow
        overflows -= 1;

    count32 = overflows*0x10000 + counter;
    prev_counter = counter;

    return (count32);
}
