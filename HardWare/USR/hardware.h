#ifndef _hardware_H

#define _hardware_H
#include "stm32f10x.h"
static uint16_t UniqueNum = 0x01;
extern uint16_t RxBuffer;
void GPIO_Configuration(void);
void USART_Configuration(void);
void NVIC_Configuration(void);
void RCC_Configuration(void);
void USART_Putc(uint16_t c);

#endif
