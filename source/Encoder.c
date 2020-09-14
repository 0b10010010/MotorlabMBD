/*
 * Encoder.c
 *
 *  Created on: Apr 15, 2020
 *      Author: alexk
 */

#include "USART_CubeMX1_TIM.h"
#include "Encoder.h"

void MotorEncoderInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // GPIOE clock enable
    __HAL_RCC_GPIOE_CLK_ENABLE();
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    // TIM1 clock enable
    __HAL_RCC_TIM1_CLK_ENABLE();
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitStructure.Pin = GPIO_PIN_9|GPIO_PIN_11;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;         // Alternate function mode
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;   // Speed doesn't matter (input)
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;       // Open Drain - but shouldn't matter (input)
    GPIO_InitStructure.Pull = GPIO_NOPULL ;    // no pull up or pull down resistor
    GPIO_InitStructure.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

    TIM_Encoder_InitTypeDef TIM1_InitStructure;
    TIM1_InitStructure.EncoderMode = TIM_ENCODERMODE_TI12;
    TIM1_InitStructure.IC1Polarity = TIM_ICPOLARITY_RISING;
    TIM1_InitStructure.IC2Polarity = TIM_ICPOLARITY_RISING;
    TIM1_InitStructure.IC1Filter = 0xF;
    TIM1_InitStructure.IC2Filter = 0xF;
    HAL_TIM_Encoder_Init(&htim1, &TIM1_InitStructure);

    __HAL_TIM_SET_AUTORELOAD(&htim1, 0xffff);
//    TIM_SetAutoreload(TIM1, 0xffff);
//    TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

//    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);   // Assign PE9 and PE11 to TIM1 as encoder channels
//    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);

    __HAL_TIM_ENABLE(&htim1);
//    TIM_Cmd(TIM1, ENABLE);

//    TIM1->CCMR1 |= 0xF0F0;  // Implements digital filter
}

int32_t readEnc(void)
{
    uint16_t counter;
    int32_t count32;
    int cnt = 1;

    if (cnt == 1) {
      counter = (&htim1)->Instance->CNT;
    }
//    if (cnt == 2) {
//      counter = TIM2->CNT;
//    }
    // The following assumes this function is called frequently enough that
    // the encoder cannot change more 0x8000 counts between calls, and that
    // the counter overflows from 0xffff to 0 and underflows from 0 to 0xffff
    if ((prev_counter > 0xc000)&&(counter < 0x4000))     // overflow
        overflows += 1;
    else if ((prev_counter < 0x4000)&&(counter > 0xc000))  // underflow
        overflows -= 1;

    count32 = overflows*0x10000 + counter;
    prev_counter = counter;

    return (count32*0.225);
}

void set(int32_t count32) {
    uint16_t counter;
    int cnt = 1;
    if (count32 <0)
        overflows = count32/((int32_t)0x10000) - 1;
    else
        overflows = count32/((int32_t)0x10000);

    counter = (uint16_t)(count32 - overflows*0x10000);
    prev_counter = counter;

    if (cnt == 1) {
        TIM1->CNT = counter;
    }
//    if (cnt == 2) {
//        TIM2->CNT = counter;
//    }
}
