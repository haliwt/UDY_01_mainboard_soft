/*
 * bsp_fan.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"




#define FAN_PWM_100     40

#define FAN_PWM_90      36

#define FAN_PWM_80      32


/********************************************************
*
*Function Name:void SetLevel_Fan_PWMA(uint8_t levelval)
*Function: 
*
*
********************************************************/



void FAN_Stop(void)
{
 FAN_COM_SetLow();
 FAN_CCW_SetLow() ;
  
}





void Fan_Full_Speed(void)
{

	FAN_COM_SetLow();
	FAN_CCW_SetHigh();


}







