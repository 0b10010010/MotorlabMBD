// Copyright: Dale Schinstock, 2013
#include "PwmAnalogOut_SPL.h"

void PwmInit(void) {
  uint32_t frequency = 200000;
  // TIM3 clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  // GPIOB  clock enable
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  // GPIO Configuration: TIM3 to CH1(PB4), CH2(PB5)
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  // Connect TIM3 pin to AF2  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);
  
  uint32_t counterClock;
  
  if(frequency < 1000) {
    counterClock = 100000;
  }
  else {
    counterClock = 28000000;
  }
  
  uint16_t PrescalerValue = (uint16_t) ((SystemCoreClock /2) / counterClock) - 1;
  d_ARR = counterClock/frequency - 1;

  // Time base configuration
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  
  TIM_TimeBaseStructure.TIM_Period = d_ARR;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  // PWM Mode configuration
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  

  // Channel 1
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  // Channel 2
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);    
  
  TIM_ARRPreloadConfig(TIM3, ENABLE);

  // TIM3 enable counter
  TIM_Cmd(TIM3, ENABLE);  
}

void setVoltage(float voltage) {
  float dutyCycle = voltage/3.3f;
  
  if(dutyCycle<0) {
    if (dutyCycle < -1.0f)  dutyCycle = 1.0f;
    else dutyCycle = -dutyCycle;
    TIM3->CCR1 = 0;
    TIM3->CCR2 = (uint32_t) (dutyCycle*d_ARR);
  }
  else {
    if (dutyCycle > 1.0f)  dutyCycle = 1.0f;
    TIM3->CCR1 = (uint32_t) (dutyCycle*d_ARR);
    TIM3->CCR2 = 0;
  }
}
