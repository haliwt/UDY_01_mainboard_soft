/*
 * bsp_freertos.h
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#ifndef __BSP_THREADX_H_
#define __BSP_THREADX_H_
#include "main.h"

extern uint8_t inputBuf[1];


void vtask_isq_handler(void);


void open_beep_sound(void);




#endif /* BSP_INC_BSP_FREERTOS_H_ */
