#ifndef ENCODER_H_INCLUDED
#define ENCODER_H_INCLUDED

#include "stm32f4xx.h"   // include defs for stm32f4xx including std periph lib
#include "stm32f4xx_tim.h"

uint16_t prev_counter;     // counter value from previous read
int16_t overflows;         // number of overflows/underflows (for 32 bit count)

void MotorEncoderInit(void);
void LoadEncoderInit(void);
int32_t readEnc(void);              // cnt=1: TIM1, cnt=2: TIM2
void set(int32_t count32); // cnt=1: TIM1, cnt=2: TIM2

#endif