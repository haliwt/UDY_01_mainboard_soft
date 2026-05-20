#include "bsp.h"

#define POWER_BIT_0	        (1 << 0)
#define MODE_BIT_1	        (1 << 1)
#define DOWN_BIT_2           (1 << 2)
#define UP_BIT_3           (1 << 3)

#define KEY_LONG_POWER_BIT_4      (1<<4)
#define KEY_LONG_MODE_BIT_5			(1<<5)


#define MODE_LONG_KEY_8         (1 << 8)
#define DECODER_BIT_9          (1<< 9)




#define DEBUG_ENABLE   1



/***********************************************************************************************************
											函数声明
***********************************************************************************************************/
#define STACK_SIZE_DECODER  1280//512//128//1792//3072//2048//1024//896//768
#define STACK_SIZE_RUN      896//1024//1536//1280//1024//1536//1024//896//1792//1664//1280


__attribute__((aligned(8))) static UCHAR stack_run_pro[STACK_SIZE_RUN];
__attribute__((aligned(8))) static UCHAR stack_decoder_pro[STACK_SIZE_DECODER];


/*在 ThreadX 里，优先级数字越小，优先级越高：*/

static TX_THREAD thread_run;
static TX_THREAD thread_decoder;


/* 定义信号量 */
TX_SEMAPHORE decoder_semaphore;

TX_TIMER beep_timer;

/* 创建任务通信机制 */
static void tx_thread_stack_error_handler(TX_THREAD *thread_ptr);

static void vTaskRunPro(ULONG thread_input);
static void vTaskStart(ULONG thread_input);
static void beep_timer_callback(ULONG input);




static void  threadx_handler(void);


#if DEBUG_ENABLE

static void debug_stack_run_check(void);

static void debug_stack_decoder_check(void);

ULONG unused_run,unused_decoder ;
#endif 

//uint8_t inputBuf[1];



/**
 * @brief  :  static void vTaskStart(void *pvParameters
 * @note    
 * @param   None
 * @retval  None
 */

void tx_application_define(void *first_unused_memory)
{
  
    /* 1. 消除参数未引用警告 */
    (void)first_unused_memory;

    #if DEBUG_ENABLE
    /* 2. 只有当 stack_msg_pro 是全局定义的静态数组时，这样写才有效 */
    memset(stack_run_pro, 0xEF, sizeof(stack_run_pro));
	memset(stack_decoder_pro, 0xEF, sizeof(stack_decoder_pro));

    #endif 

    /* 3. 注册堆栈错误回调（推荐保持） */
 
    tx_thread_stack_error_notify(tx_thread_stack_error_handler);

    // 创建线程、信号量、事件组、队列
    
     threadx_handler();
}



/**********************************************************************************************************
*	Function Name: static void vTaskRunPro(void *pvParameters)
*	Function:
*	Input Ref: pvParameters 是在创建该任务时传��的形参
*	Return Ref:
*   priority: 1  (数�?�越小优先级越低，这个跟uCOS相反)
**********************************************************************************************************/
static void vTaskRunPro(ULONG thread_input)
{
  (void)thread_input;  /* 消除未使用的参数警告 */
  while(1){
    
   
	power_onoff_handler(g_pro.gpower_on);
	LL_IWDG_ReloadCounter(IWDG);
      #if DEBUG_ENABLE
				 debug_stack_run_check();
		   #endif 

    tx_thread_sleep(1);

	  
    }
	  
}


/**********************************************************************************************************
*
*	Function Name: vTaskStart
*	Function:
*	Input Ref: pvParameters 是在创建该任务时传�?�的形参
*	Return Ref:
*	priority: 3  (数�?�越小优先级越低，这个跟uCOS相反)
*
**********************************************************************************************************/
static void vTaskDecoderPro(ULONG thread_input)
{

	(void)thread_input;  /* 消除未使用的参数警告 */
	
    while(1)
    {

   
     // 阻塞等待 ISR 投递
      if(tx_semaphore_get(&decoder_semaphore, TX_WAIT_FOREVER) == TX_SUCCESS)
      {
          g_pro.rx_data_success_f ++;
				usart2_rx_decoder();
				LL_IWDG_ReloadCounter(IWDG);
				 #if DEBUG_ENABLE
				 debug_stack_decoder_check();
		   #endif 

	  }
      tx_thread_sleep(1) ;
    }
}
 /**********************************************************************************************************
*	Function Name: AppTaskCreate
*	功能说明: 创建应用任务
*	Input Ref:
*	Return Ref:
**********************************************************************************************************/
static void threadx_handler(void) 
{
	 /* 创建信号量 */
	tx_semaphore_create(&decoder_semaphore, "DecoderSemaphore", 0);
	

	
	 tx_thread_create(&thread_decoder,		 /* 任务控制块地址 */ 
					 "DecoderPro",			 /* 任务名 */
					 vTaskDecoderPro,		// 每个消息大小，这里用 1 字节
					 0, 					  /* 传递给任务的参数 */
					 stack_decoder_pro, 	 /* 堆栈基地址 */
					 STACK_SIZE_DECODER,	   /* 堆栈空间大小 */ 
					 1,
					 1,
					 TX_NO_TIME_SLICE,
					 TX_AUTO_START);
				 
	 tx_thread_create(&thread_run,				   /* 任务控制块地址 */ 
					  "UiPro",					   /* 任务名 */
					  vTaskRunPro,				   /* 启动任务函数地址 */
					  0,							/* 传递给任务的参数 */
					  stack_run_pro, 			   /* 堆栈基地址 */
					  STACK_SIZE_RUN,			   /* 堆栈空间大小 */ 
					  2,							/* 任务优先级*/
					  2,							/* 任务抢占阀值 , 允许它不被优先级 1-0 之间的任务抢占，除非是中断 */
					  TX_NO_TIME_SLICE, 			/* 不开启时间片 */
					  TX_AUTO_START);				/* 创建后立即启动 */
    
   tx_timer_create(&beep_timer,     /* timer of  block */
				   "20msTimer",
				   beep_timer_callback, /*callback function */
   				   0,
				   2,                    /* 第一次延迟 20ms*/
				   2,                    /*周期 20 ticks*/
				   TX_AUTO_ACTIVATE);
	
}

void vtask_isq_handler(void)
{
    tx_semaphore_put(&decoder_semaphore);
}




static void beep_timer_callback(ULONG input)
{
	(void)input;
	close_buzzer_sound();


}
void open_beep_sound(void)
{
  tx_timer_activate(&beep_timer);
}


/****************************************************************
*
*	Function Name: vTaskStart
*	Function:
*	Input Ref: pvParameters 是在创建该任务时传�?�的形参
*	Return Ref:
*	priority: 3  (数�?�越小优先级越低，这个跟uCOS相反)
*
******************************************************************/
void tx_thread_stack_error_handler(TX_THREAD *thread_ptr)
{
    /* 栈溢出处理：这里你可以做任何你想做的动作 */

    /* 1. 打印线程名（如果有 UART） */
    // printf("Stack overflow in thread: %s\n", thread_ptr->tx_thread_name);

    /* 2. 拉高故障指示灯 */
    // Fault_LED_On();
 
    /* 3. 记录日志（如果有 Flash/EEPROM） */
    // Log_Fault(FAULT_STACK_OVERFLOW, thread_ptr->tx_thread_name);

    /* 4. 触发系统复位（汽车级） */
    NVIC_SystemReset();

    while(1);  // 调试阶段可以卡住
}

#if DEBUG_ENABLE
static void debug_stack_run_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // 使用局部变量进行统计

  
    // 从数组起始位置（栈底/低地址）开始数连续的 0xEF
    for (i = 0; i < STACK_SIZE_RUN; i++)
    {
        if (stack_run_pro[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
  
 
	
	unused_run = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}


static void debug_stack_decoder_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // 使用局部变量进行统计


    // 从数组起始位置（栈底/低地址）开始数连续的 0xEF
    for (i = 0; i < STACK_SIZE_DECODER; i++)
    {
        if (stack_decoder_pro[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
    unused_decoder = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}



#endif 

