#ifndef __BSP_USART2_H
#define __BSP_USART2_H
#include "main.h"



void usart2_rx_callbck_handler(uint8_t data);

void usart2_rx_decoder(void);

void USART2_DMA_Send(const uint8_t *txBuf,uint16_t txlen);

#endif 

