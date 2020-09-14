// Copyright: Dale Schinstock, 2013
#include "PwmAnalogOut.h"

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
