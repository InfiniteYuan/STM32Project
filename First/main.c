#include "pData.h"
void RCC_Configuration(void){
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//π“…Ë ±÷”£ª
}
void GPIO_Configuration(void){
	GPIO_InitTypeDef GPIO_initStructure;
	GPIO_initStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_initStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_initStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_initStructure);
}
int main(void){
	RCC_Configuration();
	RCC_HSE_Configuration();
	GPIO_Configuration();
	while(1){
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		delay_ms(500);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		delay_ms(5000);
	}
}

