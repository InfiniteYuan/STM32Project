#include "GPS.h"

uint16_t GPS_BUF[GPS_BUF_LENGTH];
u8 DataIterator = 0x00;

int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,(u8)ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

int main(void){

//	u8 d4[5];
//	d4[0]='a';
//	d4[1]='b';
//	d4[2]='c';
//	d4[3]='d';
//	d4[4]='\0';

	RCC_Config();
	GPIO_Config();
	USART_Config();
	NVIC_Config();

	while(1){
//		printf("×Ö·û´®Êä³ö d4=%s\r\n",d4);
		printf("Use_EPH_Sum=%d\r\n", GPS_Information.Use_EPH_Sum);
		printf("MSL_Al=%lf\r\n", GPS_Information.MSL_Altitude);
		printf("Longitude=%s\r\n", GPS_Information.Longitude);
		printf("Latitude=%s\r\n", GPS_Information.Latitude);
		printf("NS_Ind=%c\r\n", GPS_Information.NS_Indicator);
		printf("Speed=%lf\r\n", GPS_Information.Speed);
		printf("EW_Ind=%c\r\n", GPS_Information.EW_Indicator);
		printf("Course=%lf\r\n", GPS_Information.Course);
		printf("PDOP=%lf\r\n", GPS_Information.PDOP);
		printf("HDOP=%lf\r\n", GPS_Information.HDOP);
		printf("VDOP=%lf\r\n", GPS_Information.VDOP);
		
	}
}
