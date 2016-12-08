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
	unsigned char Real_Locate;  				//实时定位有效位
	unsigned char Located;              //定位有效位
	unsigned char Locate_Mode;        	//定位模式
	char UTC_Time[7];                   //时间
	char UTC_Date[7];                   //日期
	char Latitude[10];                  //纬度
	char NS_Indicator;                  //NS
	char Longitude[11];                 //经度
	char EW_Indicator;                  //EW
	double Speed;                       //速率
	double Course;                      //航向
	double PDOP;                        //位置精度
	double HDOP;                        //水平精度
	double VDOP;                        //垂直精度
	double MSL_Altitude;                //MSl海拔高度
	unsigned char Use_EPH_Sum;       		//使用的卫星数量
	unsigned char User_EPH[12];         //当前搜索到的卫星编号
	unsigned short EPH_State[12][4]; 		//当前使用的12卫星的编号、仰角、方位角、信嗓比
};

struct _GPS_Real_buf{
	char data[256];            				 //GPS输入缓冲区
	volatile unsigned short rx_pc;     //指针
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
***USART1接收数据；
***对数据按逗号进行分割；末尾加上字符串结束符；
***通过数据的各个信息进行解码；获取相应的GPS信息
***
************************************************/

