//
// Created by legath on 01.06.2020.
//

#ifndef STEPPER_THROTTLE_K1100_GPIO_H
#define STEPPER_THROTTLE_K1100_GPIO_H
#include "stm32f1xx_hal.h"
void gpio_init(void );
void gpio_disp_res_pin_set(GPIO_PinState pinState);
void gpio_disp_cmd_pin_set(GPIO_PinState pinState);
#endif //STEPPER_THROTTLE_K1100_GPIO_H
