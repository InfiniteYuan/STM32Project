#include "hardware.h"

void GPIO_Configuration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*Config USART1_Tx as alternate function push -pull*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void USART_Configuration(void){
	USART_InitTypeDef  USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;

	USART_Init(USART1,&USART_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
//USART_ITConfig(USART1,USART_IT_TXE,ENABLE);//TX ENABLE
	USART_Cmd(USART1,ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_TC);
//	USART_ClearFlag(USART1,USART_FLAG_RXNE);
}

void NVIC_Configuration(void){
	/*Enable the USART Interrupt*/
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void RCC_Configuration(void){
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	USART_ClockInitTypeDef USART_ClockInitStructure;
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	//USART_ClockInit(USART1,&USART_ClockInitStructure);
}

void UART1_IRQHandler(void){	
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET){
		
		USART_SendData(USART1,USART_ReceiveData(USART1));
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	}
	
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{
		/*Clear IT */
		USART_ClearITPendingBit(USART1,USART_IT_TXE);
		/* Write one byte to the transmit data register */
		USART_SendData(USART1,UniqueNum);
	}
}

