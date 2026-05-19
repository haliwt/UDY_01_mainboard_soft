/*
 * bsp_buzzer.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"
#include "tim.h"

void buzzer_sound(void)
{
	LL_TIM_CC_EnableChannel(TIM14, LL_TIM_CHANNEL_CH1);
    LL_TIM_EnableCounter(TIM14);
    tx_thread_sleep(20);
}

void close_buzzer_sound(void)
{
    LL_TIM_CC_DisableChannel(TIM14, LL_TIM_CHANNEL_CH1);
    LL_TIM_DisableCounter(TIM14);
}



/*********************************************************************************************************
*	Function Name: MODH_Poll
*	Function: ???????. 1ms ?????
*	Input Ref: ?
*	Return Ref: 0 ????? 1????????
*********************************************************************************************************/

void Buzzer_Fan_Error_Sound(void)
{
   
 
    buzzer_sound();
	HAL_Delay(100);
	  buzzer_sound();
	HAL_Delay(100);
	  buzzer_sound();
	HAL_Delay(100);
	  buzzer_sound();
	HAL_Delay(100);

}

void Buzzer_Ptc_Error_Sound(void)
{

	  buzzer_sound();
	HAL_Delay(50);
	  buzzer_sound();
	HAL_Delay(50);
	  buzzer_sound();
	HAL_Delay(50);
	  buzzer_sound();
	HAL_Delay(50);
	  buzzer_sound();
	HAL_Delay(50);


}


