/*
 * bsp.h
 *
 *  Created on: 2025�?3�?4�?
 *      Author: Administrator
 */

#ifndef BSP_INC_BSP_H_
#define BSP_INC_BSP_H_
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "tx_api.h"



#include "bsp_ntc.h"
#include "bsp_buzzer.h"
#include "bsp_fan.h"
#include "bsp_mouse.h"


#include "bsp_power.h"



#include "bsp_message.h"
#include "bsp_cmd_link.h"

#include "bsp_adc.h"
#include "interrupt_manager.h"
#include "bsp_usart2.h"
#include "bsp_threadx.h"



//mainboard

#include "adc.h"
#include "dma.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"





#define  THREADX_ENABLE      1
  
#define  TEST_UNIT        0


#if THREADX_ENABLE  == 1
	
	#define DISABLE_INT()   UINT __old_post = tx_interrupt_control(TX_INT_DISABLE)
    #define ENABLE_INT()     tx_interrupt_control(__old_post)
#else
	/* ����ȫ���жϵĺ� */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
	#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */
#endif


typedef enum{
   power_off,
   power_on,
}process_state;

typedef enum{
	wifi_no_link,
	wifi_link_success,
}wifi_state;

typedef enum{
	normal_time_mode,
	timer_time_mode=1,
	beijing_time_mode=2,
	disp_timer_mode=3,
	disp_temp_hum_mode=4,
	input_set_timer_mode=5, //WT.EDIT 2025.04.23
	input_set_temperature_mode, //WT.EDIT 2025.04.23
	input_set_null,
	input_set_timer_mode_null,
	input_set_normal_time_mode,
	input_normal_null
	
}time_state;

typedef struct _process{

   //main board reference
   uint16_t read_ptc_voltage;
   uint16_t gTimer_timer_minutes_counter;
   uint8_t gpower_on;
   uint8_t gDry;
   uint8_t gPlasma;

   uint8_t gMouse;
  
   



   uint8_t g_humidity_value;
   uint8_t g_temperature_value;
   uint8_t detect_fan_error_times;

    uint8_t temperature_decade;    // �?
    uint8_t temperature_unit;//度�??
	uint8_t read_ntc_temperature_value;
	uint8_t power_on_read_ntc_flag ;
    uint8_t read_ntc_tem_value;
    

 
  volatile uint8_t adc_judge_flag;

   uint8_t gTemp_value;
   uint8_t gset_temperture_value;
   uint8_t temperature_init_value;
   uint8_t set_temp_f;            // 设置温度标志：0-使用默认温度，1-使用用户设置温度

 


   uint8_t gdisp_minutes_value;
   uint8_t  disp_timer_minutes_value;
   uint8_t heating_state;         // 加热状态：0-关闭，1-打开
   uint8_t over_temp_flag;        // 过温标志：0-正常，1-超过50度
   uint16_t over_temp_counter;    // 过温次数计数器

   uint8_t g_copy_power_onoff_flag;
   uint8_t delay_run_adc_counter;


   uint8_t first_connect_wifi_flag;
   uint8_t DMA_txComplete;


   
   



   //timer
   
   uint8_t gTimer_run_function_counter;
 
   uint8_t   gTimer_fan_run_one_minute;

  
   uint8_t gTimer_display_adc_value;
   uint8_t gTimer_timer_seconds_counter;


  
   
}process_t;

extern process_t g_pro;

// Display related structure
typedef struct _display{
    uint8_t g_second_disp_flag;
    uint8_t g_set_temp_value_flag;
}display_t;

extern display_t g_disp;



void bsp_init(void);

void mainboard_fun_handler(void);

void mainboard_close_all_fun(void);

void works_run_eight_hours_state(void);

void copy_cmd_hanlder(void);


void  smart_phone_timer_power_on_handler(void);

void fault_handler(void);





#endif /* BSP_INC_BSP_H_ */
