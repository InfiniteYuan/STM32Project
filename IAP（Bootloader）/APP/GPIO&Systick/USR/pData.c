/*********************************

1��ʱ�ӹ��أ�ʹ�ܣ�ϵͳ��ʼ�������ã�
2���ⲿʱ�����ã��ⲿ����򿪣�AHBʱ������Ϊϵͳʱ�Ӵ�С��
����AHBʱ��Ϊϵͳʱ�Ӵ�С������AHBʱ��Ϊϵͳʱ�Ӷ���Ƶ������PLLʱ��Դ��
�����Ƶ��С��PLLʹ�ܣ�����ϵͳʱ��ΪPLL�����
3��GPIO���ø������������PWM����ӳ���������

*********************************/
#include "pData.h"
void RCC_Configuration(void){
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
}
void delay_ms(u16 nms){
	u32 temp;
	SysTick->LOAD = 9000*nms;//72Mʱ��8��Ƶ������9M��1us����9�Σ�
	SysTick->VAL = 0x00;//����ֵ
	SysTick->CTRL = 0x01;
	do{
		temp = SysTick->CTRL;//��ȡ��ǰ�Ĵ���״̬
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
		temp = SysTick->CTRL;//��ȡ��ǰ�Ĵ���״̬
	}while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�ʱ��ĵ���
	
	SysTick->CTRL = 0x00;//�رռ�����
	SysTick->VAL = 0x00;//��ռ�����
}
void RCC_HSE_Configuration(void){
	RCC_DeInit(); /*������RCC�Ĵ�������Ϊȱʡֵ */ 
	RCC_HSEConfig(RCC_HSE_ON); 	 /*�����ⲿ���پ����*/

	if(RCC_WaitForHSEStartUp() == SUCCESS) {  /*�ȴ�HSE���񣬾����ȶ��Ҿ���*/

		RCC_HCLKConfig(RCC_SYSCLK_Div1);/*����AHBʱ��*/
		RCC_PCLK2Config(RCC_HCLK_Div1); /*���ø���AHBʱ��*/
		RCC_PCLK1Config(RCC_HCLK_Div2); /*���õ���AHBʱ��*/

		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);/*����PLLʱ��Դ����Ƶϵ��*/		 
		RCC_PLLCmd(ENABLE);//ʹ��PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);//���ָ��RCC��־λ

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //����ϵͳʱ��
		while(RCC_GetSYSCLKSource() != 0x08);    //PLL��Ϊϵͳʱ��
	}
}
void GPIO_Configuration(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//LED
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}
