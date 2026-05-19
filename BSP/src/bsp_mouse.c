#include "bsp.h"
#include "tim.h"

void mouse_open(void)
{
    LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH2);
    LL_TIM_EnableCounter(TIM3);
}

void mouse_close(void)
{
    LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH2);
    LL_TIM_DisableCounter(TIM3);
}

