/*
 * interrupt_manager.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"

/********************************************************************************
	**
	*Function Name:void tim17_isr_callback_handler(void)
	*Function : interrupt is 10ms 
	*Input Ref: 
	*Return Ref:NO
	*
*******************************************************************************/
void tim17_isr_callback_handler(void)
{
   static  uint16_t tm0;
 
         tm0++;
	  
	
	   if(tm0> 99){ //10ms * 100 = 1000ms = 1s
	      tm0=0;
	       g_pro.gTimer_timer_seconds_counter++;
			g_pro.gTimer_fan_run_one_minute++;
		
		   g_pro.gTimer_timer_minutes_counter++;
		 
		   g_pro.gTimer_display_adc_value++;
	
	
	       g_pro.gTimer_run_function_counter++;

		   if( g_pro.gTimer_timer_seconds_counter > 59){
			   g_pro.gTimer_timer_seconds_counter=0;
		       g_pro.gTimer_timer_minutes_counter++;

		   }
		
		 }
	   

   
}

