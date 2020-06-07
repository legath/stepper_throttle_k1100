//
// Created by legath on 04.06.2020.
//

#include "usart.h"
#include "display/ili9163c.h"

static UART_HandleTypeDef huart2;
USART_prop_ptr usartprop;
char str1[60] = {0};

static void uart2_llinit() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Peripheral clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

}

void uart2_init(void) {
    uart2_llinit();
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        error_handler();
    }
}

void uart2_start_receive(void) {
    HAL_UART_Receive_IT(&huart2, (uint8_t *) str1, 1);
}

void string_parse(char *buf_str) {
    HAL_UART_Transmit(&huart2, (uint8_t *) buf_str, strlen(buf_str), 0x1000);
    ili9163c_drawRectFilled(0,0,128,128, BLACK);
    ili9163c_drawStringF(10, 10 , 1, GREEN, BLACK, "%s", buf_str);
}

static void UART2_RxCpltCallback(void) {

    uint8_t b;
    b = str1[0];
    //если вдруг случайно превысим длину буфера
    if (usartprop.usart_cnt > 59) {
        usartprop.usart_cnt = 0;
        HAL_UART_Receive_IT(&huart2, (uint8_t *) str1, 1);
        return;
    }
    usartprop.usart_buf[usartprop.usart_cnt] = b;
    if (b == 0x0A) {
        usartprop.usart_buf[usartprop.usart_cnt + 1] = 0;
        string_parse((char *) usartprop.usart_buf);
        usartprop.usart_cnt = 0;
        HAL_UART_Receive_IT(&huart2, (uint8_t *) str1, 1);
        return;
    }
    usartprop.usart_cnt++;
    HAL_UART_Receive_IT(&huart2, (uint8_t *) str1, 1);
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart2);
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart==&huart2)
    {
        UART2_RxCpltCallback();
    }
}