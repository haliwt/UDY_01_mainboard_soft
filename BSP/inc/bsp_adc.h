#ifndef __BSP_ADC_H_
#define __BSP_ADC_H_
#include "main.h"


#if 0

uint16_t Get_Adc_Channel(uint32_t ch) ;

void Get_PTC_Temperature_Voltage(uint32_t channel,uint8_t times);


//id Get_Fan_Adc_Fun(uint32_t channel,uint8_t times);
void read_ntc_value_init(void);

void Update_PtcADC_ToDisplayBoard_Value(void);


#endif 

uint16_t ADC_GetValue(void);
void Update_PtcADC_ToDisplayBoard_Value(void);


#endif
