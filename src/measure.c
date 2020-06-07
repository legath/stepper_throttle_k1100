//
// Created by legath on 07.06.2020.
//

#include "measure.h"
#include "error_handler.h"

static ADC_HandleTypeDef hadc1;
static DMA_HandleTypeDef hdma_adc1;
static volatile uint16_t adc[2] = {0,};

static void adc_dma_init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

static void adc_ll_init() {
    adc_dma_init();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_NORMAL;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
        error_handler();
    }

    __HAL_LINKDMA(&hadc1,DMA_Handle,hdma_adc1);
}

static void adc1_init(void) {
    ADC_ChannelConfTypeDef sConfig = {0};
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 2;
    if (HAL_ADC_Init(&hadc1) != HAL_OK) {
        error_handler();
    }
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        error_handler();
    }

    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = ADC_REGULAR_RANK_2;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        error_handler();
    }
}

static void adc_clk_init() {
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        error_handler();
    }
}

void measure_init() {
    adc_clk_init();
    adc_ll_init();
    adc1_init();
    HAL_ADCEx_Calibration_Start(&hadc1);
}

void measure_start(){
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc, 2);
}

void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_adc1);
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1) //check if the interrupt comes from ACD1
    {
        HAL_ADC_Stop_DMA(&hadc1); // это необязательно
        adc[0] = 0;
        adc[1] = 0;
        HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc, 2);
    }
}
