/*
 * bsp.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"


process_t g_pro;

 uint16_t mainboard_time;

 static void mainboard_special_fun(void);

/******************************************************************************
	*
	*Function Name:void bsp_init(void)
	*Funcion: 
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void bsp_init(void)
{
	
	
	 

}

/******************************************************************************
	*
	*Function Name:void mainboard_fun_handler(void)
	*Funcion: dry,plasma,mouse,fan ADC
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void mainboard_fun_handler(void)
{
 
   static uint8_t ac220v_fan_run_flag,run_cmd_flag;

	if(g_pro.gTimer_run_function_counter > 0){// 2s  //300 ~= 6s, 50 ~=1s
    
	   g_pro.gTimer_run_function_counter=0;




	if(g_pro.gDry == 1 ){
		DRY_OPEN();
		FAN_OPEN();
	    ac220v_fan_run_flag =0;
		run_cmd_flag=0;
	}
	else{
		
		DRY_CLOSE();
		
	}

	if(g_pro.gPlasma == 1){
		
		PLASMA_OPEN();
		FAN_OPEN();
		ac220v_fan_run_flag =0;
	    run_cmd_flag=0;
	}
	else{
		
		PLASMA_CLOSE();
	}
	

	if(g_pro.gMouse == 1){
		
		mouse_open();
	}
	else{
	
		mouse_close();
	}


	if(g_pro.gMouse == 1 && g_pro.gDry == 0 && g_pro.gPlasma == 0 && run_cmd_flag !=1){
		
		mouse_open();
		ac220v_fan_run_flag = 1;
	    g_pro.gTimer_fan_run_one_minute=0;
	}
	else if(g_pro.gMouse == 0 && g_pro.gDry == 0 && g_pro.gPlasma == 0 && run_cmd_flag !=1){
        mouse_close();
        
	    ac220v_fan_run_flag =1;
	    g_pro.gTimer_fan_run_one_minute=0;
	    //FAN_CLOSE();

	}


	}
	if(ac220v_fan_run_flag ==1){
		
		run_cmd_flag=1;

	    if(g_pro.gTimer_fan_run_one_minute < 60){
		    FAN_OPEN();

        }
		else{

		    FAN_CLOSE();
		    ac220v_fan_run_flag++;


		}
         
	}
	
   
}

static void mainboard_special_fun(void)
{
   //static uint16_t mainboard_time;



	if(g_pro.gDry == 1){
		DRY_OPEN();
		
	}
	else{
		DRY_CLOSE();
		

	}

	if(g_pro.gMouse == 1){
		mouse_open();
		
	}
	else{
		mouse_close();
		
	}

	if(g_pro.gPlasma == 1){
		PLASMA_OPEN();
		
	}
	else{
		PLASMA_CLOSE();
		
	}
	
  
	

}

void mainboard_close_all_fun(void)
{
	DRY_CLOSE();
	mouse_close();

	PLASMA_CLOSE();


}


/**********************************************************************
    *
    *Functin Name: void works_run_eight_hours_state(void)
    *Function :  
    *Input Ref: NO
    *Return Ref: NO
    *
************************************************************************/
void works_run_eight_hours_state(void)
{
  

}



/**********************************************************************
    *
    *Functin Name: void works_run_eight_hours_state(void)
    *Function :  
    *Input Ref: NO
    *Return Ref: NO
    *
************************************************************************/
void copy_cmd_hanlder(void)
{
    if(g_pro.g_copy_power_onoff_flag == power_on){
		
	   
	    SendWifiData_Answer_Cmd(CMD_POWER,0x01); //WT.EDIT 2025.01.07 
			osDelay(5);
      g_pro.g_copy_power_onoff_flag =0xff;

	}
	else if( g_pro.g_copy_power_onoff_flag ==0x02){

	  g_pro.g_copy_power_onoff_flag =0xfe;
	  SendWifiData_Answer_Cmd(CMD_POWER,0x0); //WT.EDIT 2025.01.07 
        osDelay(5);

	}

}


/**********************************************************************
    *
    *Functin Name: void works_run_eight_hours_state(void)
    *Function :  
    *Input Ref: NO
    *Return Ref: NO
    *
************************************************************************/


/**********************************************************************
    *
    *Functin Name: void fault_handler(void)
    *Function :  
    *Input Ref: NO
    *Return Ref: NO
    *
************************************************************************/
void fault_handler(void)
{

  



}


