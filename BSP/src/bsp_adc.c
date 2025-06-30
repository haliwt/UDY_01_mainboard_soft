#include "bsp_adc.h"
#include "bsp.h"

#define FAN_THRESHOLD_VALUE     600

#define TOLERANCE_ERROR      100



static uint16_t Get_Adc_Average(uint32_t ch,uint8_t times);

static void Judge_PTC_Temperature_Value(uint16_t adc_ptc);

static void Judge_Fan_State(uint16_t adc_value);



/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
	*
*****************************************************************/
 uint16_t Get_Adc_Channel(uint32_t ch)   
{

    static uint8_t adc_result;
	ADC_ChannelConfTypeDef ADC1_ChanConf;

	ADC1_ChanConf.Channel=ch;                                   //Í¨µÀ
    ADC1_ChanConf.Rank= ADC_REGULAR_RANK_1;                                    //第一个序�?
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_19CYCLES_5;   //²ÉÑùÊ±¼ä               


	HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //Í¨µÀÅäÖÃ
	
    HAL_ADC_Start(&hadc1);                               //start ADC transmit
	
    adc_result=HAL_ADC_PollForConversion(&hadc1,10);                //轮询

	if(adc_result == HAL_OK){
 
	   return (uint16_t)HAL_ADC_GetValue(&hadc1);	        	//·µ»Ø×î½üÒ»´ÎADC1¹æÔò×éµÄ×ª»»½á¹û
	}
	else{
      return HAL_TIMEOUT;
	}
}
/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
	*
*****************************************************************/
static uint16_t Get_Adc_Average(uint32_t ch,uint8_t times)
{
	uint32_t temp_val=0;
	uint8_t t;
	
	for(t=0;t<times;t++)
	{
        
		temp_val+=Get_Adc_Channel(ch);
		
	}
	return temp_val/times;
} 



void Get_PTC_Temperature_Voltage(uint32_t channel,uint8_t times)
{
    
	uint16_t adcx;
	
	adcx = Get_Adc_Average(channel,times);

	osDelay(10);

    g_pro.read_ptc_voltage  =(uint16_t)((adcx * 3300)/4096); //amplification 100 ,3.11V -> 311

	osDelay(10);

    g_pro.read_ptc_voltage = g_pro.read_ptc_voltage-100;


   
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

	if(power_on_first ==0){
	    power_on_first++;
	    g_pro.adc_judge_flag = Get_Adc_Channel(ADC_CHANNEL_1) ;

	}

	if(g_pro.adc_judge_flag !=HAL_TIMEOUT){

		Get_PTC_Temperature_Voltage(ADC_CHANNEL_1,10);

	    //Get_Ntc_Resistance_Temperature_Handler(g_pro.read_ptc_voltage);
	    getNtc_temperatureValue_init(g_pro.read_ptc_voltage);

		sendData_Real_Temp(g_pro.read_ntc_tem_value);
			   
		osDelay(5);
		copy_temperature_value= g_pro.read_ntc_tem_value;

	}
	else if(g_pro.adc_judge_flag==HAL_TIMEOUT){

	    power_on_first=0;
		sendData_Real_Temp(copy_temperature_value);
		osDelay(5);

	}
   




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

	if(g_pro.adc_judge_flag !=HAL_TIMEOUT){

		Get_PTC_Temperature_Voltage(ADC_CHANNEL_1,10);

	    Get_Ntc_Resistance_Temperature_Handler(g_pro.read_ptc_voltage);

		sendData_Real_Temp(g_pro.read_ntc_temperature_value);
			   
		osDelay(5);
		copy_temperature_value= g_pro.read_ntc_temperature_value;

	}
	else if(g_pro.adc_judge_flag==HAL_TIMEOUT){

	   // power_on_first=0;
		sendData_Real_Temp(copy_temperature_value);
		osDelay(5);

	}
   
		   
	
}


