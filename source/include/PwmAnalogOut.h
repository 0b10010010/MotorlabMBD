/* -----------------------------------------------------------------------
// Copyright: Dale Schinstock, 2013

The PwmAnalogOut interface uses a is uses PB4, PB5, and TIM3 to generate
a pseudo, bipolar, analog output.  It uses high frequency PWM (200 kHz).
PB4 is pulsed to generate a "positive" voltage, and PB5 is pulsed to 
generate a negative voltage.  The pins must be treated as differential
and low pass filtered to get an analog voltage.

A VDD of 3.3V is assumed for the voltage scaling.

-------------------------------------------------------------------------*/

#ifndef PWMANALOGOUT_H_INCLUDED
#define PWMANALOGOUT_H_INCLUDED

//#include "stm32f4xx_gpio.h"
#include "Motorlab_PWM.h"

void setVoltage(float voltage); //Set value (-3.3V to 3.3V)    

uint32_t d_ARR; // ARR clock counts to get period

#endif
