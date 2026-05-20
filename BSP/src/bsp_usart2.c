#include "bsp.h"



typedef void (*Usart2RxCallback)(uint8_t data);

static Usart2RxCallback  usart2_rx_cb = NULL;

// 注册回调函数
void usart2_register_rx_callback(Usart2RxCallback cb)
{
   usart2_rx_cb = cb;

}
/**
*@brief // 提供一个接口给中断调用
*@note
*@param
*@interval
**/


/**
*@brief  //注册一个实现函数
*@note
*@param
*@interval
**/


/**
*@brief  
*@note
*@param
*@interval
**/
typedef struct Msg
{

	uint8_t  usData[12];
    uint8_t  ucMessageID;
    uint8_t  rx_data_counter;
 
    uint8_t  bcc_check_code;
    uint8_t   data_length;

}MSG_T;

MSG_T   gl_tMsg; /* 定义丢�个结构体用于消息队列 */

uint8_t check_code;



/********************************************************************************
	**
	*Function Name:void usart1_isr_callback_handler(void)
	*Function :  receive the second display board data outside
	*Input Ref:NO
	*Return Ref:NO
	*
*******************************************************************************/
void usart2_rx_callbck_handler(uint8_t data)
{
    static volatile uint8_t state ;
    switch(state)
		{
		case 0:  //#0
			if(data == 0xA5){  // 0xA5 -- second display board ID
               gl_tMsg.rx_data_counter=0;
               gl_tMsg.usData[gl_tMsg.rx_data_counter] = data;
				state=1; //=1

             }

		break;


		case 1: //#1

             /* 初始化结构体指针 */
               gl_tMsg.rx_data_counter++;

	          gl_tMsg.usData[gl_tMsg.rx_data_counter] = data;

			  if(gl_tMsg.usData[gl_tMsg.rx_data_counter] == 0xFE && gl_tMsg.rx_data_counter >3){

                   state = 2;
			  }
			  

	    break;

		case 2:
	
				gl_tMsg.rx_data_counter++;
				
			    gl_tMsg.usData[gl_tMsg.rx_data_counter] = data;

               

                gl_tMsg.data_length = gl_tMsg.rx_data_counter;
                gl_tMsg.rx_data_counter =0;
				 state = 0;


                gl_tMsg.bcc_check_code= data;

                 // 在 ThreadX 中，这里可以使用事件通知或者信号量来通知任务处理数据
                  vtask_isq_handler(); //暂时注释掉，因为没有定义
               

          break;

		  }
}

/**
*
*@brief
*@notice
*@param
*
**/
void usart2_rx_decoder(void)
{


	check_code =bcc_check(gl_tMsg.usData,gl_tMsg.data_length);

	if(check_code == gl_tMsg.bcc_check_code ){

	receive_data_from_displayboard(gl_tMsg.usData);
	
	}



}




int fputc(int ch, FILE *f)
{
    while (!LL_USART_IsActiveFlag_TXE(USART2));   // 等待 TXE 空
    LL_USART_TransmitData8(USART2, (uint8_t)ch);  // 发送 1 字节
    return ch;
}


void USART2_DMA_Send(const uint8_t *txBuf,uint16_t txlen)
{
     if(txBuf ==NULL || txlen ==0) return ;
    
   // dma_tx_done = 0;
	
	LL_DMA_DisableChannel(DMA1,LL_DMA_CHANNEL_1);
    /* 5. 配置传输参数 */
    LL_DMA_ConfigAddresses(DMA1,LL_DMA_CHANNEL_1,
                            (uint32_t)txBuf,
                            (uint32_t)&USART2->TDR,
                            LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
     /* 3. 配置长度 */
    LL_DMA_SetDataLength(DMA1,LL_DMA_CHANNEL_1,txlen);

   
    /* 4. 清除所有相关标志（顺序必须在 EnableChannel 前） */
    LL_DMA_ClearFlag_TC1(DMA1);
    LL_DMA_ClearFlag_TE1(DMA1);
    
    /* 使能 DMA 中断 */
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);

    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
    
      /* 6. 启动 DMA */
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

    /* 7. 使能 USART2 的 DMA TX 请求 */
    LL_USART_EnableDMAReq_TX(USART2);
    



}



#if 0
/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
     static uint8_t state,rx_end_flag ;
     BaseType_t xHigherPriorityTaskWoken = pdFALSE;


    if(huart->Instance == USART2) // mainBoard receive data from display board send data USART1
	{

     //  DISABLE_INT();
       switch(state)
		{
		case 0:  //#0
			if(inputBuf[0] == 0xA5){  // 0xA5 -- second display board ID
               gl_tMsg.rx_data_counter=0;
				         gl_tMsg.ulid=0;
               gl_tMsg.usData[gl_tMsg.rx_data_counter] = inputBuf[0];
				state=1; //=1

             }
            else
                state=0;
		break;


		case 1: //#1

            if(gl_tMsg.disp_rx_cmd_done_flag ==0){
              /* 初始化结构体指针 */
               gl_tMsg.rx_data_counter++;

	          gl_tMsg.usData[gl_tMsg.rx_data_counter] = inputBuf[0];


              if(rx_end_flag == 1){

                state = 0;

                gl_tMsg.ulid = gl_tMsg.rx_data_counter;
                rx_end_flag=0;

                gl_tMsg.rx_data_counter =0;

                gl_tMsg.disp_rx_cmd_done_flag = 1 ;

                gl_tMsg.bcc_check_code=inputBuf[0];

                #if 1
                xTaskNotifyFromISR(xHandleTaskStart,  /* 目标任务 */
                                    DECODER_BIT_9,     /* 设置目标任务事件标志位bit0  */
                                    eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志�? */
                                    &xHigherPriorityTaskWoken);

                /* 如果xHigherPriorityTaskWoken = pdTRUE，那么�??出中断后切到当前�?高优先级任务执行 */
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
				#endif 

              }

              }

              if(gl_tMsg.usData[gl_tMsg.rx_data_counter] ==0xFE && rx_end_flag == 0 &&   gl_tMsg.rx_data_counter > 4){

                     rx_end_flag = 1 ;

              }

        break;



		}

        //  ENABLE_INT();
    __HAL_UART_CLEAR_OREFLAG(&huart2);
	HAL_UART_Receive_IT(&huart2,inputBuf,1);//UART receive data interrupt 1 byte

   }
   
}
#endif 

