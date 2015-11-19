#include "pData.h"
void delay_ms(u16 nms){
	u32 temp;
	SysTick->LOAD = 9000*nms;//72M时钟8分频，就是9M，1us就是9次；
	SysTick->VAL = 0x00;
	SysTick->CTRL = 0x01;
	do{
		temp = SysTick->CTRL;
	}while((temp&0x01)&&(!(temp&(1<<16))));
	
	SysTick->CTRL = 0x00;
	SysTick->VAL = 0x00;
}
void delay_us(u32 nus){
	u32 temp;
	SysTick->LOAD = 9*nus;
	SysTick->VAL = 0x00;//清空计数器
	SysTick->CTRL = 0x01;//使能，减到零是无动作，采用外部时钟源
	do{
		temp = SysTick->CTRL;//读取当前倒计时数值
	}while((temp&0x01)&&(!(temp&(1<<16))));//等待时间的到来
	
	SysTick->CTRL = 0x00;//关闭计数器
	SysTick->VAL = 0x00;//清空计数器
}
void RCC_HSE_Configuration(void){
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	
	if(RCC_WaitForHSEStartUp() == SUCCESS){
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
		RCC_PLLCmd(ENABLE);
		while(RCC_GetSYSCLKSource() != 0x08);
	}
}
