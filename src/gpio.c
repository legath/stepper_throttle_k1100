//
// Created by legath on 01.06.2020.
//

#include "gpio.h"
void gpio_init(void ){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

    /*Configure GPIO pins : PB0 PB1 */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void gpio_disp_res_pin_set(GPIO_PinState pinState){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, pinState);
}

void gpio_disp_cmd_pin_set(GPIO_PinState pinState){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, pinState);
}