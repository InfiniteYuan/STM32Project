#include "hardware.h"
uint16_t RxBuffer;
int main(void){
	GPIO_Configuration();
	USART_Configuration();
	RCC_Configuration();
	NVIC_Configuration();
	while(1);
}
