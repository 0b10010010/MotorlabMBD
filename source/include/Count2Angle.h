/*
 * Count2Angle.h
 *
 *  Created on: Apr 15, 2020
 *      Author: alexk
 */

#ifndef INC_COUNT2ANGLE_H_
#define INC_COUNT2ANGLE_H_

uint16_t prev_counter;     // counter value from previous read
int16_t overflows;         // number of overflows/underflows (for 32 bit count)

int32_t Count2Angle(uint32_t counter);

#endif /* INC_COUNT2ANGLE_H_ */
