#include "bsp.h"



void mouse_open(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);//ultrasnoic On
}
void mouse_close(void)
{
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);//ultrasnoic Off
}

