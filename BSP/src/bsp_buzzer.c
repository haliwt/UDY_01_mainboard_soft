/*
 * bsp_buzzer.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"
#include "tim.h"

static void tim14_buzzer_sound_config(void)
{
   LL_TIM_DisableCounter(TIM14);
   LL_TIM_GenerateEvent_UPDATE(TIM14);
   LL_TIM_SetAutoReload(TIM14, 249); //1/(249+1) =0.004MHZ = 4KHZ buzzer sound 
   LL_TIM_OC_SetCompareCH1(TIM14, 125);
   LL_TIM_CC_EnableChannel(TIM14,LL_TIM_CHANNEL_CH1);
   LL_TIM_EnableCounter(TIM14);
   LL_TIM_EnableAllOutputs(TIM14);
}

static void tim14_stop_buzzer_sound_config(void)
{
   // 禁用计数器
   LL_TIM_DisableCounter(TIM14);
   
   // 禁用通道
   LL_TIM_CC_DisableChannel(TIM14,LL_TIM_CHANNEL_CH1);
   
   // 禁用所有输出
   LL_TIM_DisableAllOutputs(TIM14);
   
   // 清除比较值
   LL_TIM_OC_SetCompareCH1(TIM14,0);


}



void buzzer_sound(void)
{
	tim14_buzzer_sound_config();
   
	open_beep_sound();// tx_thread_sleep(2);
	tx_thread_sleep(2);
	tim14_stop_buzzer_sound_config();

}

void close_buzzer_sound(void)
{
    tim14_stop_buzzer_sound_config();
}






