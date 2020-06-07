//
// Created by legath on 01.06.2020.
//

#include "spi.h"
#include "error_handler.h"

static SPI_HandleTypeDef hspi1;

static void spi1_lowlevelInit(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA4     ------> SPI1_NSS
    PA5     ------> SPI1_SCK
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* SPI1 interrupt Init */
    HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);


}

void spi1_init(void) {
    spi1_lowlevelInit();
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;//SPI_BAUDRATEPRESCALER_8;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        error_handler();
    }
}
void spi1_writeByte(uint8_t val){
    //while((SPI1->SR & SPI_SR_TXE)==RESET){}
    //SPI1->DR = val;
    HAL_SPI_Transmit(&hspi1, &val, 1, 0x1000);
}


void SPI1_IRQHandler(void) {
    HAL_SPI_IRQHandler(&hspi1);
}
