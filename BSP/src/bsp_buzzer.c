/*
 * bsp_buzzer.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"

void buzzer_sound(void)
{
	HAL_TIM_PWM_Start(&htim14,TIM_CHANNEL_1);
    osDelay(20);
    HAL_TIM_PWM_Stop(&htim14,TIM_CHANNEL_1);


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


