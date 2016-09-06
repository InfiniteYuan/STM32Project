#include "pData.h"
int main(void){
	
//	SCB->VTOR = FLASH_BASE | 0x8000;
	
	RCC_Configuration();
	RCC_HSE_Configuration();
	GPIO_Configuration();
	while(1){
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		delay_ms(500);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		delay_ms(500);
	}
}

