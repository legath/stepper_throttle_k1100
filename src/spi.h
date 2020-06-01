//
// Created by legath on 01.06.2020.
//

#ifndef STEPPER_THROTTLE_K1100_SPI_H
#define STEPPER_THROTTLE_K1100_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
void spi1_init(void);

void spi1_writeByte(uint8_t val);

#ifdef __cplusplus
}
#endif
#endif //STEPPER_THROTTLE_K1100_SPI_H
