
#ifndef _pbdata_H
#define _pbdata_H

#include "stm32f10x.h"

void delay_us(u32 nus);
void delay_ms(u16 nms);
void RCC_HSE_Configuration(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
#endif
