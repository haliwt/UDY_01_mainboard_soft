/*
 * bsp_message.c
 *
 *  Created on: 2025�?3�?4�?
 *      Author: Administrator
 */
#include "bsp.h"

static void copy_receive_data(uint8_t cmd,uint8_t data);


uint8_t power_off_test_counter;
//uint8_t temperature_value;
uint8_t power_on_counter;

/**********************************************************************
    *
    *Function Name:uint8_t bcc_check(const unsigned char *data, int len) 
    *Function: BCC校验函数
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
uint8_t bcc_check(const unsigned char *data, int len) 
{
    unsigned char bcc = 0;
    for (int i = 0; i < len; i++) {
        bcc ^= data[i];
    }
    return bcc;
}
/**********************************************************************
    *
    *Function Name:void receive_data_from_displayboard(uint8_t *pdata,uint8_t len)
    *Function: receive usart touchkey of command 
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
void receive_data_from_displayboard(uint8_t *pdata)
{
  

    switch(pdata[2]){

     case 0:


     break;

     case 0x01: //表示�?机指�?

       if(pdata[3] == 0x01){ 

		  g_pro.gpower_on = power_on;
		  power_on_counter++;
          buzzer_sound();
		  g_pro.rx_data_power_on_f = 1;
		 
          SendWifiData_Answer_Cmd(CMD_POWER,0x01); //WT.EDIT 2025.01.07 
          tx_thread_sleep(10);
        }
        else{ //close 
         
          buzzer_sound();
	
		  g_pro.gpower_on = power_off;
          power_off_test_counter++;
		  g_pro.rx_data_power_on_f = 2;
		 SendWifiData_Answer_Cmd(CMD_POWER,0x0); //WT.EDIT 2025.01.07
		 tx_thread_sleep(10);

        }

    	

     break;


	 case 0x11: //has the second display board exist .this notice 
		   if(pdata[3] == 0x00){ // comand 判断是数据还是命�?
		   
				  
		if(pdata[4] == 0x01){ 

	  

		}
	}
	 break;

     case 0x02: //PTC打开关闭指令

    
	 	if(pdata[3]==0x01){
	 	if(g_pro.gpower_on == power_on){
		 
          buzzer_sound();
          g_pro.gDry = 1;
		
		  //manual close flag :
		   SendWifiData_Answer_Cmd(0x02,0x01); //WT.EDIT 2025.01.07
		   tx_thread_sleep(10);
		}
       }
       else if(pdata[3] == 0){
	   	 if(g_pro.gpower_on == power_on){
		
          buzzer_sound();
          g_pro.gDry =0;
	
          DRY_CLOSE();
		 
		  SendWifiData_Answer_Cmd(0x02,0x0); //WT.EDIT 2025.01.07
		  tx_thread_sleep(10);
		  
            
         
	   	 }
       }
     	
     break;

	 


	  case 0x22: //notice cmd ,PTC打开关闭指令,buzzer don't sound,温度对比后的指令



      if(pdata[3] == 0x01){
        
        if(g_pro.gpower_on == power_on){

        
		 SendWifiData_Answer_Cmd(0x22,0x01); //WT.EDIT 2025.01.07
		 tx_thread_sleep(5);
		 g_pro.gDry = 1;
     

	 	}
	  }
      else if(pdata[3] == 0x0){
        if(g_pro.gpower_on == power_on){

          SendWifiData_Answer_Cmd(0x22,0x0); //WT.EDIT 2025.01.07
		   tx_thread_sleep(5);
           g_pro.gDry =0;
		
          DRY_CLOSE();
		  }
		}
	  
     break;

     case 0x03: //PLASMA 打开关闭指令

      
	    if(pdata[3]==0x01){
       if(g_pro.gpower_on == power_on){
	   	  
            buzzer_sound();
			g_pro.gPlasma = 1;
		    PLASMA_OPEN();
		  
	
           
        }
        }
        else if(pdata[3] == 0){
        if(g_pro.gpower_on == power_on){ 
            buzzer_sound();
			g_pro.gPlasma = 0;

		     PLASMA_CLOSE();
		   
		
           
        }
        }
       	
      break;


      case 0x04: //ultrasonic  打开关闭指令

      // 00-》表示是指令或�?��?�知，不是数据，下一个数据就是命令或者�?�知
	 	if(pdata[3]==0x01){
          if(g_pro.gpower_on == power_on){ 
            buzzer_sound();
			g_pro.gMouse = 1;
		     mouse_open();
			 
		  
           
          }
       }
        else if(pdata[3] == 0x0){ //close 
		 if(g_pro.gpower_on == power_on){ 
					buzzer_sound();
					g_pro.gMouse = 0;
				
					mouse_close();
         

        }
       }
    
       	
     break;

      case 0x05: // link wifi command

     
	 	if(pdata[3]==0x01){
        if(g_pro.gpower_on == power_on){ 
		  SendWifiData_Answer_Cmd(0x05,0x01); //WT.EDIT 2024.12.28
		  tx_thread_sleep(5);
          buzzer_sound();
		  
        
	 
          g_pro.first_connect_wifi_flag=0;
         }
        
       	}
       	

     break;

     case 0x06: //buzzer sound command 

    
        if(pdata[3]==0x01){
           buzzer_sound();
		   //SendWifiData_Answer_Cmd(0x06,0x01); //WT.EDIT 2024.12.28
		   //tx_thread_sleep(5);
        }
       
    	

     break;


	 case 0x10: //has two display board.


	   	   if(pdata[3]==0x01){
         

	       }
		   else{
	
		   	}
	   	
		
     break;

     case 0x16 : //buzzer sound command with answer .

      
  

	      if(pdata[3]==0x01){

          if(g_pro.gpower_on == power_on){  
          SendWifiData_Answer_Cmd(0x16,0x01); //WT.EDIT 2025.01.07
          tx_thread_sleep(5);
          buzzer_sound();

          }
	      }
       	


     break;


      case 0x1A: //read 温度数据

       if(pdata[3] == 0x0F){ //数据
       
	     

          

	   }
      break;

      case 0x1B: //湿度数据

        if(pdata[3] == 0x0F){ //数据
            

        }
      break;

      case 0x1C: //表示时间：小时，分，�?

        if(pdata[3] == 0x0F){ //数据



        }
      break;

        case 0x1D: //表示日期�? 年，月，�?

        if(pdata[3] == 0x0F){ //数据

             
            

        }
      break;

	  
	 case 0x2A: //display board set up tempeature value send data to mainboard
	 
			

			   if(pdata[4]==0x01){ // has dat only one value ,next receive byte is value
	         
			   if(g_pro.gpower_on == power_on){ 
				buzzer_sound();
				
  
             
				
			
			    if(pdata[5] < 41 && pdata[5]> 19){
					g_pro.gset_temperture_value = pdata[5];
					
					
			    }
		
                g_disp.g_set_temp_value_flag = 1;
				
				
             }

			 }
				
	 
			
	 break;

	  

    

     case 0x27: //smart phone set AI mode

      if(pdata[3] == 0x02){ //don't AI
       
        
          
       }
       else if(pdata[3] == 0x01){ //AI mode 
       
      
       }


     break;

	case 0x4C: //set up timer timing value 
		if(pdata[3] == 0x0F){ //数据

			if(pdata[4]==0x01){ // has dat only one value ,next receive byte is value

			if(g_pro.gpower_on == power_on){ 
                buzzer_sound();


			
		
           
				

			}

			}


		}
	 	
     break;

	 case 0x5C: // display has been set up timer timing value 
		if(pdata[3] == 0x0F){ //数据

			if(pdata[4]==0x03){ // has dat only one value ,next receive byte is value

			if(g_pro.gpower_on == power_on){ 
              
		
		
				

			}

			}


		}
	 	
     break;

     case 0xFF: //copy send cmd acknowlege

	 copy_receive_data(pdata[3],pdata[4]);

	       
     break;

	 default:
	 break;
        
     
     }

   
 
}

/********************************************************************
	*
	*Function Name: static void copy_receive_data(uint8_t cmd,uint8_t data)
	*Function: receive display board command .     		   
	*Input Ref:cmd-display board command . data- 1:open or 0:close
	*Return Ref:NO
	*
*********************************************************************/
static void copy_receive_data(uint8_t cmd,uint8_t data)
{
     switch(cmd){

       case CMD_POWER:

	        if(data == 1){
                buzzer_sound();
			    g_pro.gpower_on = power_on;

			}
			else{
               buzzer_sound();
			   g_pro.gpower_on = power_off;
			}
	 

	   break;
	  

	   case CMD_PTC :
	   	if(data == 1){

		   //buzzer_sound();
		   g_pro.gDry=1;
		   DRY_OPEN();
		  

		  }
		  else{
			  g_pro.gDry=0;

              DRY_CLOSE();
          

		 }

	   break;

	   case CMD_CONNECT_WIFI:
	   	if(data == 1){
			//buzzer_sound();
          

		}
		else{



		}

	   break;


	 }


}


