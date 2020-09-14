/*
 * USART2.c
 *
 *  Created on: Apr 15, 2020
 *      Author: alexk
 */

#include "USART2.h"

struct circular_buffer cb_TX; // Circular buffer Transmit buffer (stores encoder data)

char buffer[32] = {0};

void copyStr(double cnt)
{
	// cnt*=0.225f; // .225 degree per encoder count
	sprintf(buffer, "%.3f\n\r", cnt);
	uprintf(buffer);
}

void uprintf(char *str)
{
//    /* Enable the UART Transmit data register empty Interrupt */
//    __HAL_UART_ENABLE_IT(&huart2, UART_IT_TXE);
	HAL_UART_Transmit_IT(&huart2, (uint8_t *)str, 16);
}

// Circular Buffer Implementation
void initCircularBuff(void)
{
	uint16_t capacity = 2048;
	uint8_t sz = 16;
	cb_init(&cb_TX, capacity, sz);
}

void cb_init(circular_buffer *cb, size_t capacity, size_t sz)
{
    cb->buffer = malloc(capacity * sz);
    if(cb->buffer == NULL)
        // handle error
    cb->buffer_end = (char *)cb->buffer + capacity * sz;
    cb->capacity = capacity;
    cb->count = 0;
    cb->sz = sz;
    cb->head = cb->buffer;
    cb->tail = cb->buffer;
}

void cb_free(circular_buffer *cb)
{
    free(cb->buffer);
    // clear out other fields too, just to be safe
}

void cb_push_back(circular_buffer *cb, const void *item)
{
    if(cb->count == cb->capacity){
        // handle error
    }
    memcpy(cb->head, item, cb->sz);
    cb->head = (char*)cb->head + cb->sz;
    if(cb->head == cb->buffer_end)
        cb->head = cb->buffer;
    cb->count++;
}

void cb_pop_front(circular_buffer *cb, void *item)
{
    if(cb->count == 0){
        // handle error
    }
    memcpy(item, cb->tail, cb->sz);
    cb->tail = (char*)cb->tail + cb->sz;
    if(cb->tail == cb->buffer_end)
        cb->tail = cb->buffer;
    cb->count--;
}
