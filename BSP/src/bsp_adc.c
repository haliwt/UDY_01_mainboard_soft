#include "bsp_adc.h"
#include "bsp.h"

#define FAN_THRESHOLD_VALUE     600

#define TOLERANCE_ERROR      100

// ADC DMA 相关定义
#define ADC_DMA_BUFFER_SIZE     2   // 两个通道：IN0 和 IN1
#define ADC_CHANNEL_NUM         2   // 通道数量

// ADC DMA 缓冲区
 
uint16_t adc_dma_buffer[ADC_DMA_BUFFER_SIZE];

// ADC 转换完成标志
volatile uint8_t adc_dma_conversion_complete = 0;

static void Judge_PTC_Temperature_Value(uint16_t adc_ptc);

static void Judge_Fan_State(uint16_t adc_value);

// ADC DMA 初始化函数
void bsp_adc_dma_init(void);

// 获取 DMA 转换结果
uint16_t bsp_adc_get_dma_result(uint32_t ch);



/*****************************************************************
*
	*Function Name: void bsp_adc_dma_init(void)
	*Function: Initialize ADC with DMA
	*Input Ref: None
	*Return Ref: None
	*
*****************************************************************/
void bsp_adc_dma_init(void)
{
    // 1. 重新配置ADC参数
    // 配置ADC为扫描模式，连续转换
    LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_ENABLE_2RANKS);
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_CONTINUOUS);
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
    
    // 2. 配置通道序列
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_0);
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_1);
    
    // 3. 设置采样时间
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_0, LL_ADC_SAMPLINGTIME_COMMON_1);
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_1, LL_ADC_SAMPLINGTIME_COMMON_1);
    
    // 4. 重新配置DMA
    // 禁用DMA通道，以便重新配置
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
    
    // 配置DMA地址
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_2,
                           (uint32_t)&ADC1->DR,
                           (uint32_t)adc_dma_buffer,
                           LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
    
    // 设置DMA传输长度
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, ADC_DMA_BUFFER_SIZE);
    
    // 设置DMA为循环模式，这样DMA会自动循环传输，不需要手动重新启动
    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MODE_NORMAL);//LL_DMA_MODE_CIRCULAR); //LL_DMA_MODE_NORMAL);
    
    // 确保数据对齐正确（半字对齐，16位）
    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PDATAALIGN_HALFWORD);
    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MDATAALIGN_HALFWORD);
    
    // 5. 使能DMA中断
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
    
    // 6. 启动ADC和DMA
    // 使能ADC
    LL_ADC_Enable(ADC1);
    
    // 启动ADC转换
    LL_ADC_REG_StartConversion(ADC1);
    
    // 使能DMA通道
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
    
    // 7. 初始化DMA缓冲区
    adc_dma_buffer[0] = 0;
    adc_dma_buffer[1] = 0;
    adc_dma_conversion_complete = 0;
}

/*****************************************************************
*
	*Function Name: uint16_t bsp_adc_get_dma_result(uint32_t ch)
	*Function: Get ADC result from DMA buffer
	*Input Ref: ADC channel
	*Return Ref: ADC value
	*
*****************************************************************/
uint16_t bsp_adc_get_dma_result(uint32_t ch)
{
    if(ch == LL_ADC_CHANNEL_0)
    {
        return adc_dma_buffer[0];
    }
    else if(ch == LL_ADC_CHANNEL_1)
    {
        return adc_dma_buffer[1];
    }
    return 0;
}

/*****************************************************************
*
	*Function Name: uint16_t Get_Adc_Channel(uint32_t ch)
	*Function: Get ADC value (DMA mode)
	*Input Ref: ADC channel
	*Return Ref: ADC value
	*
*****************************************************************/
uint16_t Get_Adc_Channel(uint32_t ch)   
{
    return bsp_adc_get_dma_result(ch);
}




void Get_PTC_Temperature_Voltage(uint32_t channel,uint8_t times)
{
    
	uint16_t adcx;
	
	adcx = Get_Adc_Channel(channel);

	//tx_thread_sleep(10);

    g_pro.read_ptc_voltage  =(uint16_t)((adcx * 3300 )/4096); //amplification 1000 ,3.11V -> 3110000 uV

	tx_thread_sleep(1);

    g_pro.read_ptc_voltage = g_pro.read_ptc_voltage-100; // 放大后减去100mV对应的100000uV


   
	#ifdef DEBUG
      printf("ptc= %d",gctl_t.ptc_temp_voltage);
	#endif 

	// Judge_PTC_Temperature_Value(ptc_temp_voltage);

     
}

/*****************************************************************
	*
	*Function Name: void read_ntc_value_init(void)
	*Function :
	*Input Ref: NO
	*Return Ref: No
	*
*****************************************************************/
void read_ntc_value_init(void)
{
    static uint8_t power_on_first,copy_temperature_value;

	



		Get_PTC_Temperature_Voltage(LL_ADC_CHANNEL_1,10);

	    //Get_Ntc_Resistance_Temperature_Handler(g_pro.read_ptc_voltage);
	    getNtc_temperatureValue_init(g_pro.read_ptc_voltage);

		sendData_Real_Temp(g_pro.read_ntc_tem_value);
		   
		//tx_thread_sleep(10);
		copy_temperature_value= g_pro.read_ntc_tem_value;



	  
		//sendData_Real_Temp(copy_temperature_value);
		///tx_thread_sleep(5);

	
   




}

/*****************************************************************
	*
	*Function Name: void Update_PtcADC_ToDisplayBoard_Value(void)
	*Function :
	*Input Ref: NO
	*Return Ref: No
	*
*****************************************************************/
void Update_PtcADC_ToDisplayBoard_Value(void)
{

   // static uint8_t power_on_first,
   static uint8_t copy_temperature_value;

//	if(power_on_first ==0){
//	    power_on_first++;
//	    g_pro.adc_judge_flag = Get_Adc_Channel(ADC_CHANNEL_1) ;
//
//	}



		Get_PTC_Temperature_Voltage(LL_ADC_CHANNEL_1,10);

	    Get_Ntc_Resistance_Temperature_Handler(g_pro.read_ptc_voltage);

		sendData_Real_Temp(g_pro.read_ntc_temperature_value);
		   
		//tx_thread_sleep(10);
		copy_temperature_value= g_pro.read_ntc_temperature_value;

	
	
		   
	
}


