/*
 * interrupt_manager.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"

/********************************************************************************
	**
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
   static  uint16_t tm0;


    if(htim->Instance==TIM17){ //timer number14 is 100ms.
       tm0++;
	  
	
	   if(tm0> 999){ //1s
	      tm0=0;
	
			g_pro.gTimer_fan_run_one_minute++;
		
		   g_pro.gTimer_two_hours_counter++;
		 
		   g_pro.gTimer_display_adc_value++;
	
		   g_pro.gTimer_send_dht11_disp++;
	       g_pro.gTimer_run_function_counter++;
		
		 }
	   

   }
}

