/*
 * bsp.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"
#include "bsp_adc.h"

#define DEFAULT_TEMP    50 


process_t g_pro;
display_t g_disp;

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
	// 初始化 ADC DMA
	bsp_adc_dma_init();
    
    // 确保蜂鸣器在初始化后处于关闭状态
    close_buzzer_sound();
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
			tx_thread_sleep(5);
      g_pro.g_copy_power_onoff_flag =0xff;

	}
	else if( g_pro.g_copy_power_onoff_flag ==0x02){

	  g_pro.g_copy_power_onoff_flag =0xfe;
	  SendWifiData_Answer_Cmd(CMD_POWER,0x0); //WT.EDIT 2025.01.07 
        tx_thread_sleep(5);

	}

}

/**********************************************************************
    *
    *Functin Name: void temperature_control_handler(void)
    *Function : 温度控制函数，根据实际温度和设置温度控制加热
    *           温度设置值范围：30~50度
    *           实际温度 < 设置温度：打开加热(DRY_OPEN)
    *           实际温度 >= 设置温度：停止加热(DRY_CLOSE)
    *           实际温度 >= 50度：必须停止加热
    *           实际温度 <= 48度：才能重新打开加热
    *Input Ref: NO
    *Return Ref: NO
    *
************************************************************************/
#if 0
void temperature_control_handler(void)
{
    static uint8_t heating_state = 0;       // 加热状态：0-关闭，1-打开
    static uint8_t over_temp_flag = 0;      // 过温标志：0-正常，1-超过50度
    static uint8_t sensor_fault_flag = 0;   // 传感器故障标志：0-正常，1-故障
    static uint16_t sensor_fault_counter = 0; // 传感器故障计数器
    static uint16_t over_temp_counter = 0;  // 过温次数计数器
    
    // 获取实际温度
    uint8_t actual_temp = g_pro.read_ntc_temperature_value;
    uint8_t set_temp = 0;
    
    // 1. 温度传感器故障检测
    if (actual_temp == 0 || actual_temp == 255) {
        // 温度值不合理，可能是传感器故障
        sensor_fault_counter++;
        if (sensor_fault_counter >= 10) { // 连续10次检测到故障才判定为故障
            sensor_fault_flag = 1;
            // 传感器故障时停止加热
            DRY_CLOSE();
            heating_state = 0;
            send_tx_cmd(0x02, 0x0); // 通知显示板关闭加热
            
            #ifdef DEBUG
            printf("Temperature sensor fault detected!\n");
            #endif
        }
        return; // 传感器故障，不进行温度控制
    } else {
        // 温度值正常，重置故障计数器和标志
        sensor_fault_counter = 0;
        sensor_fault_flag = 0;
    }
    
    // 2. 设置温度处理
    if(g_pro.set_temp_f == 1) {
        // 使用用户设置的温度
        set_temp = g_pro.gset_temperture_value;
        
        // 确保设置温度在30~50度范围内
        if (set_temp < 30) {
            set_temp = 30;
            g_pro.gset_temperture_value = 30;
        } else if (set_temp > 50) {
            set_temp = 50;
            g_pro.gset_temperture_value = 50;
        }
    } else {
        // 使用默认温度
        set_temp = DEFAULT_TEMP;
        
        // 默认温度下的特殊处理
        if(actual_temp >= DEFAULT_TEMP) {
            DRY_CLOSE(); 
            send_tx_cmd(0x02, 0x0); // 通知显示板关闭加热
            
            #ifdef DEBUG
            printf("Default temp mode: %d°C >= %d°C, heating off!\n", actual_temp, DEFAULT_TEMP);
            #endif
            
            return;
        }
    }
    
    // 3. 温度控制逻辑
    if (actual_temp >= 50) {
        // 实际温度大于等于50度，必须停止加热
        DRY_CLOSE();
        heating_state = 0;
        over_temp_flag = 1;
        over_temp_counter++;
        send_tx_cmd(0x02, 0x0); // 通知显示板关闭加热
        
        #ifdef DEBUG
        printf("Temperature over 50°C, heating stopped! Over temp count: %d\n", over_temp_counter);
        #endif
    }
    else if (over_temp_flag == 1) {
        // 之前超过50度，需要温度下降到48度以下才能重新打开加热
        if (actual_temp <= 48) {
            over_temp_flag = 0;
            // 如果当前温度小于设置温度，打开加热
            if (actual_temp < set_temp) {
                DRY_OPEN();
                heating_state = 1;
                send_tx_cmd(0x02, 0x1); // 通知显示板打开加热
                
                #ifdef DEBUG
                printf("Temperature recovered to %d°C, heating resumed!\n", actual_temp);
                #endif
            }
        }
    }
    else if (actual_temp < set_temp) {
        // 实际温度小于设置温度，打开加热
        if (heating_state == 0) { // 只有当前关闭时才打开，避免频繁开关
            DRY_OPEN();
            heating_state = 1;
            send_tx_cmd(0x02, 0x1); // 通知显示板打开加热
            
            #ifdef DEBUG
            printf("Temperature %d°C < set %d°C, heating on!\n", actual_temp, set_temp);
            #endif
        }
    }
    else if (actual_temp >= set_temp) {
        // 实际温度大于等于设置温度，停止加热
        if (heating_state == 1) { // 只有当前打开时才关闭，避免频繁开关
            DRY_CLOSE();
            heating_state = 0;
            send_tx_cmd(0x02, 0x0); // 通知显示板关闭加热
            
            #ifdef DEBUG
            printf("Temperature %d°C >= set %d°C, heating off!\n", actual_temp, set_temp);
            #endif
        }
    }
    
    // 4. 更新全局状态
    g_pro.heating_state = heating_state;
    g_pro.over_temp_flag = over_temp_flag;
    g_pro.over_temp_counter = over_temp_counter;
}

#endif 


