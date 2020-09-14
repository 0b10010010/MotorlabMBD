/*
 * Encoder.h
 *
 *  Created on: Apr 15, 2020
 *      Author: alexk
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

uint16_t prev_counter;     // counter value from previous read
int16_t overflows;         // number of overflows/underflows (for 32 bit count)

void MotorEncoderInit(void);
//void LoadEncoderInit(void);
int32_t readEnc(void);              // cnt=1: TIM1, cnt=2: TIM2
void set(int32_t count32); // cnt=1: TIM1, cnt=2: TIM2

#endif /* INC_ENCODER_H_ */
