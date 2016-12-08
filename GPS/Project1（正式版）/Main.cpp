#include "GPS_Antenna_G503.h"

u8 DataIterator = 0x00;
uint16_t GPS_BUF[GPS_BUF_LENGTH];

void RCC_Config(void){
			SystemInit();
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		}
