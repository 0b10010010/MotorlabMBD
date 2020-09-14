#ifndef DIGITAL_OUT_H_INCLUDED
#define DIGITAL_OUT_H_INCLUDED

#include "stm32f4xx_gpio.h"

void ampEnable(void);
int read(void);
void write(int value);

uint16_t d_GPIO_PIN;
uint32_t d_RCC_AHB1Periph;
// You can specify the operating output Otype to be:
//  GPIO_OType_PP
//  GPIO_OType_OD
// Default is PP
    
// You can define the operating Pull-up/Pull down to be:     
//  GPIO_PuPd_NOPULL
//  GPIO_PuPd_UP
//  GPIO_PuPd_DOWN
// Default is NOPULL  


#endif // DIGITAL_OUT_H
