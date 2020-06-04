//
// Created by legath on 04.06.2020.
//

#ifndef STEPPER_THROTTLE_K1100_USART_H
#define STEPPER_THROTTLE_K1100_USART_H
#include "string.h"
#include "stm32f1xx_hal.h"
#include "error_handler.h"

typedef struct USART_prop {

    uint8_t usart_buf[60];

    uint8_t usart_cnt;

    uint8_t is_tcp_connect;//статус попытки создать соединение TCP с сервером

    uint8_t is_text;//статус попытки передать текст серверу

} USART_prop_ptr;

void uart2_init(void);
void uart2_start_receive(void);

#endif //STEPPER_THROTTLE_K1100_USART_H
