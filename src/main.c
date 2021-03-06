#include "stm32f1xx_hal.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "clocker/clock.h"
#include "spi.h"
#include "gpio.h"
#include "display/ili9163c.h"
#include "measure.h"
#include "usart.h"
extern volatile uint16_t adc[2] ;
int main(void) {

    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_SWJ_NOJTAG();

    HAL_Init();
    clock_set_pre(CLOCK_SOURCE_PLL, CLOCK_AHB_PRE_1, CLOCK_APB_PRE_1, CLOCK_APB_PRE_1);
    clock_set_systick(CLOCK_DEFAULT_SYSTICK_FQ);
    clock_start(CLOCK_SOURCE_PLL, 8000000, CLOCK_PLL_MUL_9);
    while (clock_values.Source != CLOCK_SOURCE_PLL);

    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef g;
    g.Mode = GPIO_MODE_OUTPUT_PP;
    g.Speed = GPIO_SPEED_FREQ_LOW;
    g.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &g);
    spi1_init();
    gpio_init();
    ili9163c_init();
    ili9163c_drawRectFilled(0, 0, 128, 128, BLACK);
    measure_init();
    measure_start();


    while (1) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        ili9163c_drawRectFilled(0,0,128,8,BLACK);
        ili9163c_drawStringF(0, 0,1, GREEN, BLACK,"adc1 %d, adc2 %d", adc[0], adc[1] );
        HAL_Delay(1000);
    }


}

void SysTick_Handler(void) {
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}


