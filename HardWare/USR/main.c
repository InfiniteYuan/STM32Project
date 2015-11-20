#include "hardware.h"
void HardwareInit(void){
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*Config USART1_Tx as alternate function push -pull*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*Config the USART*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//TX RX ENABLE
	USART_Init(USART1,&USART_InitStructure);//USART_config
	
	/*Enable the USART*/
	USART_Cmd(USART1,ENABLE);//USART ENABLE
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//RX ENABLE
	USART_ITConfig(USART1,USART_IT_TXE,ENABLE);//TX ENABLE
}

void InterruptInit(void){
	/*Enable the USART Interrupt*/
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void USART_Putc(uint16_t c){
	USART_SendData(USART1,c);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//Loop until the end of transmission
}

int main(void){
	while(1);
	return 0;
}
void UART4_IRQHandler(void)
{	
	if(USART_GetITStatus(UART4,USART_IT_RXNE) != RESET){
		RxBuffer[RxCounter++] = USART_ReceiveData(UART4);

		if (RxCounter ==NbrOfDataToRead){
			USART_ITConfig(UART4, USART_IT_RXNE, DISABLE); //????
		} 
	}
	
	if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)
	{ 
		/* Write one byte to the transmit data register */
			USART_SendData(UART4, TxBuffer[TxCounter++]);

		if(TxCounter ==NbrOfDataToTransfer){
			//TxCounter = 0;
			/* Disable the USARTy Transmit interrupt */
			USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
		} 
	}
}
