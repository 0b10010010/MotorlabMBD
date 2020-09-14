/*
 * USART2.h
 *
 *  Created on: Apr 15, 2020
 *      Author: alexk
 */

#ifndef INC_USART2_H_
#define INC_USART2_H_
//#include "USART_CubeMX1_USART.h"
#include "Motorlab_PWM.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


extern UART_HandleTypeDef huart2;
// extern char buffer[32] = {0};
//int32_t count32;

extern void copyStr(double cnt); // func prototype
extern void uprintf(char *str);

typedef struct circular_buffer
{
    void *buffer;     // data buffer
    void *buffer_end; // end of data buffer
    size_t capacity;  // maximum number of items in the buffer
    size_t count;     // number of items in the buffer
    size_t sz;        // size of each item in the buffer
    void *head;       // pointer to head
    void *tail;       // pointer to tail
} circular_buffer;

void initCircularBuff(void);
void cb_init(circular_buffer *cb, size_t capacity, size_t sz);
void cb_free(circular_buffer *cb);
void cb_push_back(circular_buffer *cb, const void *item);
void cb_pop_front(circular_buffer *cb, void *item);

#endif /* INC_USART2_H_ */
