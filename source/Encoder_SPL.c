#include "Encoder_SPL.h"

void MotorEncoderInit(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    // GPIOE clock enable
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    // TIM1 clock enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         // Alternate function mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   // Speed doesn't matter (input)
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;       // Open Drain - but shouldn't matter (input)
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;    // no pull up or pull down resistor
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    TIM_SetAutoreload(TIM1, 0xffff);
    TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);   // Assign PE9 and PE11 to TIM1 as encoder channels 
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);

    TIM_Cmd(TIM1, ENABLE);

    TIM1->CCMR1 |= 0xF0F0;  // Implements digital filter    
}
        
        
void LoadEncoderInit(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    // GPIOA clock enable
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // TIM2 clock enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         // Alternate function mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   // Speed doesn't matter (input)
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;       // Open Drain - but shouldn't matter (input)
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;    // no pull up or pull down resistor
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_SetAutoreload(TIM2, 0xffff);
    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);   // Assign PA1 and PA15 to TIM2 as encoder channels
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);

    TIM_Cmd(TIM2, ENABLE);

    TIM2->CCMR1 |= 0xF0F0;   // Implements digital filter
}

int32_t readEnc(void) {
    uint16_t counter;
    int32_t count32;
    int cnt = 1;

    if (cnt == 1) {
      counter = TIM1->CNT;
    }
    if (cnt == 2) {
      counter = TIM2->CNT;
    }
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
    if (cnt == 2) {
        TIM2->CNT = counter;
    }
}