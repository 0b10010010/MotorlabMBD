#include "DigitalOut.h"

int read(void) {
    return HAL_GPIO_ReadPin(AmpEnable_GPIO_Port, AmpEnable_Pin);
}

void write(int value) {
    if(value) {
        HAL_GPIO_WritePin(AmpEnable_GPIO_Port, AmpEnable_Pin, GPIO_PIN_SET);
    }
    else {
        HAL_GPIO_WritePin(AmpEnable_GPIO_Port, AmpEnable_Pin, GPIO_PIN_RESET);
    }
}
