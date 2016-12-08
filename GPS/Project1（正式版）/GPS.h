#ifndef _GPS_H

#define _GPS_H

#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include <ctype.h>
#include "TaskManager.h"

#define NOP               0xFF  // Define No Operation, might be used to read status register
#define GPS_BUF_LENGTH    128
#define Valid							0x01
#define Invalid 					0x00

struct _GPS_Information{
	unsigned char Real_Locate;  				//ʵʱ��λ��Чλ
	unsigned char Located;              //��λ��Чλ
	unsigned char Locate_Mode;        	//��λģʽ
	char UTC_Time[7];                   //ʱ��
	char UTC_Date[7];                   //����
	char Latitude[10];                  //γ��
	char NS_Indicator;                  //NS
	char Longitude[11];                 //����
	char EW_Indicator;                  //EW
	double Speed;                       //����
	double Course;                      //����
	double PDOP;                        //λ�þ���
	double HDOP;                        //ˮƽ����
	double VDOP;                        //��ֱ����
	double MSL_Altitude;                //MSl���θ߶�
	unsigned char Use_EPH_Sum;       		//ʹ�õ���������
	unsigned char User_EPH[12];         //��ǰ�����������Ǳ��
	unsigned short EPH_State[12][4]; 		//��ǰʹ�õ�12���ǵı�š����ǡ���λ�ǡ���ɤ��
};

struct _GPS_Real_buf{
	char data[256];            				 //GPS���뻺����
	volatile unsigned short rx_pc;     //ָ��
};

extern u8 DataIterator;
extern uint16_t GPS_BUF[GPS_BUF_LENGTH];
extern unsigned char gps_sum;
extern unsigned char Real_Process_Enabled;
extern unsigned char DH_id_sep[32];
extern struct _GPS_Real_buf	GPS_Real_buf;
extern struct _GPS_Information GPS_Information;

void USART_ReadByte(void);
void RCC_Config(void);
void GPIO_Config(void);
void USART_Config(void);
void NVIC_Config(void);
u8 USART_RW(u8 data);
void InstructionDecode(void);
void Creat_DH_Index( char* buffer );
char* Real_Process_DH( char* buffer, unsigned char num );
void Real_GPS_Command_Process( void );
unsigned char Calc_GPS_Sum( const char* Buffer );

int fputc(int ch,FILE *f);


#endif
/************************************************
***USART1�������ݣ�
***�����ݰ����Ž��зָĩβ�����ַ�����������
***ͨ�����ݵĸ�����Ϣ���н��룻��ȡ��Ӧ��GPS��Ϣ
***
************************************************/

