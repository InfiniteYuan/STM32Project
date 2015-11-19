#include "pData.h"
void delay_ms(u16 nms){
	u32 temp;
	SysTick->LOAD = 9000*nms;//72Mʱ��8��Ƶ������9M��1us����9�Σ�
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
	SysTick->VAL = 0x00;//��ռ�����
	SysTick->CTRL = 0x01;//ʹ�ܣ����������޶����������ⲿʱ��Դ
	do{
		temp = SysTick->CTRL;//��ȡ��ǰ����ʱ��ֵ
	}while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�ʱ��ĵ���
	
	SysTick->CTRL = 0x00;//�رռ�����
	SysTick->VAL = 0x00;//��ռ�����
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
