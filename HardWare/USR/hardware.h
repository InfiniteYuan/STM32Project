#ifndef _hardware_H

#define _hardware_H
#include "stm32f10x.h"
static uint16_t UniqueNum = 0x01;
uint16_t RxBuffer[20],TxBuffer[20];
uint16_t RxCounter = 0,TxCounter = 0,NbrOfDataToRead = 10,NbrOfDataToTransfer = 10;

#endif
