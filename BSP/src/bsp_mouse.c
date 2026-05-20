#include "bsp.h"
#include "tim.h"


static void tim3_ultr_pwm_config(void)
{
    LL_TIM_DisableCounter(TIM3);
	LL_TIM_SetAutoReload(TIM3,39);//PWM = 1/(39+1)MHZ = 0.025MHZ = 25KHZ.
	LL_TIM_OC_SetCompareCH1(TIM3,20); //pwm duty = 20/40 =50%
	LL_TIM_CC_EnableChannel(TIM3,LL_TIM_CHANNEL_CH2);
	
	LL_TIM_EnableAllOutputs(TIM3); // 等价于 TIM1->BDTR |= TIM_BDTR_MOE;TIM1,TIM16,TIM17 must add "LL_TIM_EnableAllOutputs()"
	LL_TIM_EnableCounter(TIM3);
	//LL_TIM_EnableAllOutputs(TIM1); //TIM1,TIM16,TIM17 must add "LL_TIM_EnableAllOutputs()"

}

static void tim3_stop_ultr_pmw_config(void)
{
  LL_TIM_DisableCounter(TIM3);
  LL_TIM_CC_DisableChannel(TIM3,LL_TIM_CHANNEL_CH2);


}



void mouse_open(void)
{
   tim3_ultr_pwm_config();
}

void mouse_close(void)
{
  tim3_stop_ultr_pmw_config();
}

