#include "DigitalOut_SPL.h"

void ampEnable(void) {
    int initValue = 1;
    d_GPIO_PIN = GPIO_Pin_11;
    d_RCC_AHB1Periph = RCC_AHB1Periph_GPIOB;
//     d_PORT = GPIOB;
    
    // GPIOD Periph clock enable
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    // Configure the GPIO_LED pin
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOB, &GPIO_InitStructure);

    if(initValue) {
    GPIO_SetBits(GPIOB, GPIO_Pin_11);
    }
    else {
    GPIO_ResetBits(GPIOB, GPIO_Pin_11);
    }
}

int read(void) {
    return GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_11);    
}

void write(int value) {
    if(value) {
        GPIO_SetBits(GPIOB, GPIO_Pin_11);
    }
    else {
        GPIO_ResetBits(GPIOB, GPIO_Pin_11);
    }
}