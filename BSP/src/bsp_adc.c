#include "bsp_adc.h"
#include "bsp.h"

#define FAN_THRESHOLD_VALUE     600

#define TOLERANCE_ERROR      100

static uint16_t Get_Adc_Channel(uint32_t ch) ;

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
static uint16_t Get_Adc_Channel(uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;

	ADC1_ChanConf.Channel=ch;                                   //Í¨µÀ
    ADC1_ChanConf.Rank= ADC_REGULAR_RANK_1;                                    //第一个序列
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_1CYCLE_5;//ADC_SAMPLETIME_239CYCLES_5;      //²ÉÑùÊ±¼ä               


	HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //Í¨µÀÅäÖÃ
	
    HAL_ADC_Start(&hadc1);                               //start ADC transmit
	
    HAL_ADC_PollForConversion(&hadc1,10);                //轮询转换
 
	return (uint16_t)HAL_ADC_GetValue(&hadc1);	        	//·µ»Ø×î½üÒ»´ÎADC1¹æÔò×éµÄ×ª»»½á¹û
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
		//osDelay(5);//delay_ms(5);
	}
	return temp_val/times;
} 



void Get_PTC_Temperature_Voltage(uint32_t channel,uint8_t times)
{
    
	uint16_t adcx;
	
	adcx = Get_Adc_Average(channel,times);

    g_pro.read_ptc_voltage  =(uint16_t)((adcx * 3300)/4096); //amplification 100 ,3.11V -> 311

    g_pro.read_ptc_voltage = g_pro.read_ptc_voltage-100;


   
	#ifdef DEBUG
      printf("ptc= %d",gctl_t.ptc_temp_voltage);
	#endif 

	// Judge_PTC_Temperature_Value(ptc_temp_voltage);

     
}


/*****************************************************************
	*
	*Function Name: void Judge_PTC_Temperature_Value(void)
	*Function: PTC adc read voltage
	*Input Ref: NO
	*Return Ref: No
	*
	*
*****************************************************************/
static void Judge_PTC_Temperature_Value(uint16_t adc_ptc)
{
  
 #if 0
  if(adc_ptc < 215 || adc_ptc == 215){  //115 degree 
         g_pro.ptc_warning =1;

		 g_pro.gDry=0 ;

         DRY_CLOSE();//Ptc_Off();
		
		HAL_Delay(50);
		
      

	
		HAL_Delay(200);  
        
	
		HAL_Delay(100);  
		
		Buzzer_Ptc_Error_Sound();
		
			  
				
	   	}
  #endif 
}
#if 0
/*****************************************************************
	*
	*Function Name: void Get_Fan_Adc_Fun(uint8_t channel,uint8_t times)
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?, hexadecimal of average
	*Return Ref: No
	*
	*
*****************************************************************/
void Get_Fan_Adc_Fun(uint32_t channel,uint8_t times)
{
	uint16_t adc_fan_hex;
	

	
	adc_fan_hex = Get_Adc_Average(channel,times);

    g_pro.fan_detect_voltage  =(uint16_t)((adc_fan_hex * 3300)/4096); //amplification 1000 ,3.111V -> 3111
	osDelay(10);


	Judge_Fan_State(g_pro.fan_detect_voltage);


    
}


static void Judge_Fan_State(uint16_t adc_value)
{

  if(adc_value <FAN_THRESHOLD_VALUE){ //500
         g_pro.detect_fan_error_times++;
	          
		if(g_pro.detect_fan_error_times >1){
			g_pro.detect_fan_error_times=2;
		   g_pro.fan_warning = 1;

		   DRY_CLOSE();

		
		  Buzzer_Fan_Error_Sound();

		  SendWifiData_To_Cmd(0x09,0x01);//Fan fault warning .
		  osDelay(5);

		}
		

     }



}

#endif 


void Update_PtcADC_ToDisplayBoard_Value(void)
{

	Get_PTC_Temperature_Voltage(ADC_CHANNEL_1,10);

    Get_Ntc_Resistance_Temperature_Handler(g_pro.read_ptc_voltage);

	sendData_Real_Temp(g_pro.read_ntc_temperature_value);
		   
	osDelay(5);
   
		   
	
}


