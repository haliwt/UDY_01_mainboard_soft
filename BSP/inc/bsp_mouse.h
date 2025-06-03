#ifndef __BSP_MOUSE_H
#define __BSP_MOUSE_H
#include "main.h"


#define DRY_OPEN()    do{RELAY_CTL_GPIO_Port->BSRR |= RELAY_CTL_Pin;}while(0)
#define DRY_CLOSE()   do{RELAY_CTL_GPIO_Port->BSRR |=(uint32_t)RELAY_CTL_Pin<<16;}while(0)


#define PLASMA_OPEN()   do{PLASMA_CTL_GPIO_Port->BSRR = PLASMA_CTL_Pin;}while(0)
#define PLASMA_CLOSE()  do{PLASMA_CTL_GPIO_Port->BSRR |= (uint32_t)PLASMA_CTL_Pin<<16;}while(0)


void mouse_open(void);

void mouse_close(void);




#endif 

