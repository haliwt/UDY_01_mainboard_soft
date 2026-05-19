#ifndef __BSP_ADC_H_
#define __BSP_ADC_H_
#include "main.h"


uint16_t Get_Adc_Channel(uint32_t ch) ;

void Get_PTC_Temperature_Voltage(uint32_t channel,uint8_t times);

// ADC DMA 初始化函数
void bsp_adc_dma_init(void);

// 获取 DMA 转换结果
uint16_t bsp_adc_get_dma_result(uint32_t ch);

// ADC 转换完成标志
extern volatile uint8_t adc_dma_conversion_complete;




void read_ntc_value_init(void);


void Update_PtcADC_ToDisplayBoard_Value(void);

#endif
