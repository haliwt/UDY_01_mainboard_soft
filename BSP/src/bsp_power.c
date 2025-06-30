/*
 * bsp_power.c
 *
 *  Created on: 2025�?3�?4�?
 *      Author: Administrator
 */
#include "bsp.h"

#define SWITCH_THRESHOLD 2




typedef struct{

  uint8_t process_on_step;
  uint8_t process_off_step;


}POWER_RUN_STATE;

POWER_RUN_STATE gl_run;

typedef enum {
    DISPLAY_TEMP = 0,
    DISPLAY_HUM = 1,
	DISPLAY_TIME = 2
} DisplayMode;

//DisplayMode disp_temp_hum = DISPLAY_TEMP;  // 默认显示温度
uint8_t disp_temp_hum;
uint8_t send_wifi_power_on_state;


/**********************************************************************
	*
	*Functin Name: void power_on_init_ref(void)
	*Function : 
	*Input Ref: NO
	*Return Ref: NO
	*
**********************************************************************/
void power_onoff_handler(uint8_t data)
{
  static uint8_t power_on_flag;
   switch(data){	

	   case power_on :

	
		power_on_run_handler();
       
			
        break;

	  case power_off:
  
         if(power_on_flag==0){
             power_on_flag ++;
			 buzzer_sound();
		 }
         power_off_run_handler();

	   break;
  
      }
	}
/**********************************************************************
	*
	*Functin Name: void power_on_init_ref(void)
	*Function : 
	*Input Ref: NO
	*Return Ref: NO
	*
**********************************************************************/
void power_on_init_ref(void)
{
	     
	g_pro.gDry =1;
	g_pro.gPlasma =1;
	g_pro.gMouse = 1;
	g_pro.gTemp_value = 40;
	//display time timing value 
	
	g_pro.detect_fan_error_times=0;

    g_pro.gTimer_timer_minutes_counter = 0;
    g_pro.gTimer_timer_seconds_counter = 0;

	gl_run.process_off_step=0;

 
    g_pro.gTimer_display_adc_value=0;
	g_pro.delay_run_adc_counter=0;
	g_pro.power_on_read_ntc_flag = 0;
	g_pro.gTimer_display_adc_value =0;

	DRY_OPEN();
	mouse_open();

	PLASMA_OPEN();
	FAN_OPEN();



}
/**********************************************************************
	*
	*Functin Name: power_on_run_handler(void)
	*Function : 
	*Input Ref: NO
	*Return Ref: NO
	*
**********************************************************************/
void power_on_run_handler(void)
{

  
 
	switch(gl_run.process_on_step){


     case 0:  //initial reference 
       gl_run.process_off_step =0 ; //clear power off process step .

	      //Update_PtcADC_ToDisplayBoard_Value();
			 
		   
		  power_on_init_ref();
		  read_ntc_value_init();
		  
		
	     gl_run.process_on_step =1;
	 break;

	 case 1:

      if(g_pro.power_on_read_ntc_flag < 4 && g_pro.gTimer_display_adc_value > 1){
		  g_pro.power_on_read_ntc_flag++;
		  g_pro.gTimer_display_adc_value=0;

          read_ntc_value_init();


	  }
	  else if(g_pro.gTimer_display_adc_value > 4  && g_pro.power_on_read_ntc_flag > 3){
		 	g_pro.gTimer_display_adc_value=0;

           Update_PtcADC_ToDisplayBoard_Value();
			
	  }

	   gl_run.process_on_step =2;

	 case 2:


       mainboard_fun_handler();
	  

      
	   gl_run.process_on_step =1;

	 break;

	 default:

	 break;

	

	}
   
 }

/**********************************************************************
	*
	*Functin Name: void power_off_run_handler(void)
	*Function :
	*Input Ref: NO
	*Return Ref: NO
	*
**********************************************************************/
void power_off_run_handler(void)
{

   static uint8_t fan_run_one_minute,fan_flag,wifi_first_connect;
   switch(gl_run.process_off_step){

   case 0:
   	  gl_run.process_on_step =0;

   	 
      fan_run_one_minute = 1;
	  g_pro.gTimer_fan_run_one_minute =0;


	 
	 // SendData_Set_Command(CMD_POWER,close);
	 // osDelay(5);
    
	 
       mainboard_close_all_fun();

       gl_run.process_off_step = 1;
   break;

   case 1:

     if(fan_flag == 0){
	 	fan_flag++;
	  fan_run_one_minute =2;
     }

	 if(fan_run_one_minute ==1){

	     if(g_pro.gTimer_fan_run_one_minute  < 61){

              FAN_OPEN();
		 }
		 else{

		    fan_run_one_minute++;
			FAN_CLOSE();

		 }

	 }

     mainboard_close_all_fun();
	


	

   

   break;

   
   	}

}



