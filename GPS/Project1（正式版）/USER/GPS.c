#include "GPS.h"

void RCC_Config(void){
	USART_ClockInitTypeDef USART_ClockInitStructure;
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(USART1,&USART_ClockInitStructure);
}

void GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void USART_Config(void){
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1,&USART_InitStructure);
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
	USART_Cmd(USART1,ENABLE);
	USART_GetFlagStatus(USART1,USART_FLAG_TC);
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_Init(USART2,&USART_InitStructure);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);
	USART_GetFlagStatus(USART2,USART_FLAG_TC);
//	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}

void NVIC_Config(void){
	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	
//	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
}

//void USART1_IRQHandler(void){
//	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET){
//		USART_ReadByte();
//	}
//}

//void USART_ReadByte(void){
//	u8 i;
//	GPS_BUF[DataIterator++] = USART_ReceiveData(USART1);
//	if(GPS_BUF[0] != 0x00){
//		DataIterator = 0;
//	}else if(DataIterator == 0x00){
//		DataIterator = 0;
//		InstructionDecode();						
//		for(i = 0; i < 0x00; i++)					
//			USART_RW(GPS_BUF[i]);
//	}
//}

u8 USART_RW(u8 data){
	if(data != NOP){ 																						  //?NOP??????
		
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//?USART1???????????
		USART_SendData(USART1, data);                               //????
		return NOP;
	}else{
		
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);//?USART1??????????
		return USART_ReceiveData(USART1);                           //????????
	}
}

void InstructionDecode(void){
	if(GPS_BUF[0] != 0x00)
		return;
	
	if(GPS_BUF[GPS_BUF_LENGTH - 1] != 0x03)
		return;
	
	switch(GPS_BUF[1]){
		
		case 0x00:
			break;
		
		case 0x02:
			break;
		
		case 0x03:
			break;
		
		case 0x04:
			break;
	}
}



//int GetComma(int num,char *str)
//{
//	int i,j=0;
//	int len=strlen(str);
//	for(i=0;i<len;i++)
//	{
//		if(str[i]==',')j++;
//		if(j==num)return i+1;		//���ص�ǰ�ҵ��Ķ���λ�õ���һ��λ��
//	}
//	return 0;	
//}

//double get_double_number(char *s)
//{
//	char buf[128];
//	int i;
//	double rev;
//	i=GetComma(1,s);
//	strncpy(buf,s,i);
//	buf[i]=0;
//	rev=atof(buf);
//	return rev;
//}

//void UTC2BTC(date_time *GPS)
//{

//		GPS->second++; //���������ȳ����ٳ�ʱ�����ݣ���ʱ������+1��
//		if(GPS->second>59){
//			GPS->second=0;
//			GPS->minute++;
//			if(GPS->minute>59){
//				GPS->minute=0;
//				GPS->hour++;
//			}
//		}	

////***************************************************
//		GPS->hour+=8;//����ʱ���UTCʱ���8��Сʱ
//		if(GPS->hour>23)
//		{
//			GPS->hour-=24;
//			GPS->day+=1;
//			if(GPS->month==2 ||GPS->month==4 ||GPS->month==6 ||GPS->month==9 ||GPS->month==11 ){
//				if(GPS->day>30){//���߼�������30��ÿ�£�2�·ݻ�����30��
//			   		GPS->day=1;
//					GPS->month++;
//				}
//			}
//			else{
//				if(GPS->day>31){//ʣ�µļ����·ݶ���31��ÿ��
//			   		GPS->day=1;
//					GPS->month++;
//				}
//			}
//			if(GPS->year % 4 == 0 ){
//		   		if(GPS->day > 29 && GPS->month ==2){//����Ķ�����29��
//		   			GPS->day=1;
//					GPS->month++;
//				}
//			}
//			else{
//		   		if(GPS->day>28 &&GPS->month ==2){//�����Ķ�����28��ÿ��
//		   			GPS->day=1;
//					GPS->month++;
//				}
//			}
//			if(GPS->month > 12){
//				GPS->month -= 12;
//				GPS->year++;
//			}		
//		}
//}

//void gps_parse(char *line,GPS_INFO *GPS)//line��Ϊ������ַ������ݣ�GPS�Ǵ���ֵ��ȫ�ֱ���
//{
//	int i,tmp,start,end;
//	char c;
//	char* buf=line;
//	c=buf[5];

//	if(c=='C'){//"GPRMC"
//		GPS->D.hour   =(buf[ 7]-'0')*10+(buf[ 8]-'0');
//		GPS->D.minute =(buf[ 9]-'0')*10+(buf[10]-'0');
//		GPS->D.second =(buf[11]-'0')*10+(buf[12]-'0');
//		tmp = GetComma(9,buf);//�õ���9�����ŵ���һ�ַ����
//		GPS->D.day    =(buf[tmp+0]-'0')*10+(buf[tmp+1]-'0');
//		GPS->D.month  =(buf[tmp+2]-'0')*10+(buf[tmp+3]-'0');
//		GPS->D.year   =(buf[tmp+4]-'0')*10+(buf[tmp+5]-'0')+2000;
//		//------------------------------
//		GPS->status   =buf[GetComma(2,buf)];//״̬
//		GPS->latitude =get_double_number(&buf[GetComma(3,buf)]);//γ��
//		GPS->NS       =buf[GetComma(4,buf)];				//�ϱ�γ
//		GPS->longitude=get_double_number(&buf[GetComma(5,buf)]);//����
//		GPS->EW       =buf[GetComma(6,buf)];//������
//		UTC2BTC(&GPS->D);//ת����ʱ��
//	}
//	if(c=='A'){ //"$GPGGA"
//		GPS->high     = get_double_number(&buf[GetComma(9,buf)]);
//		
//	}
//}
