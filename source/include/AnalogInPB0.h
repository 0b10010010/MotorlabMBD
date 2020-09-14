#ifndef ANALOG_INPB0_H
#define ANALOG_INPB0_H

#include "stm32f4xx.h"

//constructors
void AnalogInPB0(void);

//functions
float getAvgVoltage(void);
 

// used for testing speed
// void setupInterrupt(void);
    
//The raw oversampled data   
__IO uint16_t d_ADCRawValues[20];

#endif