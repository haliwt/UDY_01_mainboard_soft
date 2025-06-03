/*
 * bsp_power.c
 *
 *  Created on: 2025年3月4日
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
       
       if(gl_run.process_on_step !=0){ //logically rigorous

	    if(g_pro.fan_warning ==0 && g_pro.ptc_warning ==0){
		    
	        

			works_run_two_hours_state();
	    }

        }
			
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
	       g_pro.gAI =1;
		   g_pro.gDry =1;
		   g_pro.gPlasma =1;
		   g_pro.gMouse = 1;
		   g_pro.gTemp_value = 40;
		   //display time timing value 
		   g_pro.gdisp_hours_value =0;
		   g_pro.gdisp_timer_hours_value =0; //设置定时时间，

	
		   g_pro.g_disp_timer_or_temp_flag = normal_time_mode;
		
		 
		   // function led is turn on 
         
		   //display smg led turn on

		  
		
		   
           //timer 
           g_pro.g_manual_shutoff_dry_flag = 0;

	    



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

   static uint8_t read_error_flag,switch_adc,switch_dht11;
 
	switch(gl_run.process_on_step){


     case 0:  //initial reference 
       gl_run.process_off_step =0 ; //clear power off process step .

	  
		   
		   	  
		   	SendData_Set_Command(CMD_POWER,open);
			osDelay(5);
		   	
		 
        
		Update_DHT11_ToDisplayBoard_Value();
			 
		   
		  power_on_init_ref();
		
		  
		 
	   g_pro.gTimer_send_dht11_disp=5;
       
	   g_pro.gTimer_two_hours_counter = 0;
	  
	   gl_run.process_off_step=0;
	   
	

	   g_pro.g_manual_shutoff_dry_flag = 0;
	
		
	   g_pro.works_two_hours_interval_flag=0; //WT.EDIT 2025.05.07
	   g_pro.fan_warning =0 ;
	   g_pro.ptc_warning =0;
	   g_pro.gTimer_display_adc_value=0;
	   g_pro.delay_run_adc_counter=0;

	   Update_DHT11_ToDisplayBoard_Value();
         Fan_Full_Speed();
	   gl_run.process_on_step =1;
	 break;

	 case 1:

      if( g_pro.fan_warning ==0 && g_pro.ptc_warning ==0){
	
		

		  
		    if(g_pro.gTimer_send_dht11_disp > 2){ //3s
		       g_pro.gTimer_send_dht11_disp=0;
	           Update_DHT11_ToDisplayBoard_Value();

		   
		  }

		  gl_run.process_on_step =3; 
		  
      }
	  else{
	  
	     fault_handler();
         gl_run.process_on_step =3; 
	  }

	 case 3: //WIFI link process
	 
        

	    if(g_pro.gTimer_display_adc_value > 5 && g_pro.works_two_hours_interval_flag==0){
		 	g_pro.gTimer_display_adc_value=0;

            switch_adc = switch_adc ^ 0x01;
		    if(switch_adc==1){
               Get_PTC_Temperature_Voltage(ADC_CHANNEL_1,10);
			}
			else{
				
				if(g_pro.delay_run_adc_counter < 5){
					g_pro.delay_run_adc_counter++;
					g_pro.fan_detect_voltage= 0xFEE;
				  
				}
                else if(g_pro.works_two_hours_interval_flag==0){
	                Get_Fan_Adc_Fun(ADC_CHANNEL_0,10);

		        }

            }
		   }

        
	  gl_run.process_on_step =1;

	 break;

	 default :

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


	 
	  SendData_Set_Command(CMD_POWER,close);
	  osDelay(5);
    
	 
        

       gl_run.process_off_step = 1;


	   g_pro.fan_warning =0 ;
	   g_pro.ptc_warning =0;
	   g_pro.works_two_hours_interval_flag=0; //WT.EDIT 2025.05.07
	  

   break;

   case 1:

     if(fan_flag == 0){
	 	fan_flag++;
	  fan_run_one_minute =2;
     }

	 if(fan_run_one_minute ==1){

	     if(g_pro.gTimer_fan_run_one_minute  < 61){

              Fan_Full_Speed();
		 }
		 else{

		    fan_run_one_minute++;
			FAN_Stop();

		 }

	 }

     mainboard_close_all_fun();
	


	

   

   break;

   
   	}

}



