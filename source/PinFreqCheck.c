#include "PinFreqCheck.h"

void togglePin(void) {
    // GPIOD Periph clock enable
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    // Configure the GPIO_LED pin
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOE, &GPIO_InitStructure);

//     if(input) {
//         GPIO_ToggleBits(GPIOE, GPIO_Pin_6);
//     }
    GPIO_ToggleBits(GPIOE, GPIO_Pin_6);
}