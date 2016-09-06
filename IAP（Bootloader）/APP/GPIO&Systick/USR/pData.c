/*********************************

1、时钟挂载，使能，系统初始化，复用，
2、外部时钟配置，外部晶振打开，AHB时钟设置为系统时钟大小、
高速AHB时钟为系统时钟大小，低速AHB时钟为系统时钟二分频，设置PLL时钟源，
输出倍频大小，PLL使能，设置系统时钟为PLL输出，
3、GPIO配置复用推挽输出，PWM波重映射输出引脚

*********************************/
#include "pData.h"
void RCC_Configuration(void){
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
}
void delay_ms(u16 nms){
	u32 temp;
	SysTick->LOAD = 9000*nms;//72M时钟8分频，就是9M，1us就是9次；
	SysTick->VAL = 0x00;//赋初值
	SysTick->CTRL = 0x01;
	do{
		temp = SysTick->CTRL;//读取当前寄存器状态
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
		temp = SysTick->CTRL;//读取当前寄存器状态
	}while((temp&0x01)&&(!(temp&(1<<16))));//等待时间的到来
	
	SysTick->CTRL = 0x00;//关闭计数器
	SysTick->VAL = 0x00;//清空计数器
}
void RCC_HSE_Configuration(void){
	RCC_DeInit(); /*将外设RCC寄存器重设为缺省值 */ 
	RCC_HSEConfig(RCC_HSE_ON); 	 /*设置外部高速晶振打开*/

	if(RCC_WaitForHSEStartUp() == SUCCESS) {  /*等待HSE起振，晶振稳定且就绪*/

		RCC_HCLKConfig(RCC_SYSCLK_Div1);/*设置AHB时钟*/
		RCC_PCLK2Config(RCC_HCLK_Div1); /*设置高速AHB时钟*/
		RCC_PCLK1Config(RCC_HCLK_Div2); /*设置低速AHB时钟*/

		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);/*设置PLL时钟源、倍频系数*/		 
		RCC_PLLCmd(ENABLE);//使能PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);//检查指定RCC标志位

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //设置系统时钟
		while(RCC_GetSYSCLKSource() != 0x08);    //PLL作为系统时钟
	}
}
void GPIO_Configuration(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//LED
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}
