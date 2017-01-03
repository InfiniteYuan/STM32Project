#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

/***** System *****/
#include "Configuration.h"
#include "TaskManager.h"

/***** OnChip *****/
#include "USART.h"
#include "SPI.h"
#include "LED.h"
#include "GPIO.h"

/***** ToolBox*****/
#include "GidLink.h"
#include "IAP.h"

/******ESP8266*****/
#include "Socket_esp8266.h"

/******MFRC522*******/
#include "MFRC522.h"

/**USART1 Communication**/
//UidLink comLink1(com);

/****IAP_BootLoader*****/
IAP_BootLoader _iapboot(0);

/****TaskManager****/
TaskManager tskmgr;

/****** MFRC522 ***********/
USART usart1(USART1, 9600);			//MFRC522
MFRC522 rf(usart1);

/*****WIFI_ESP8266******/
USART com(USART2, 115200);
Socket_esp8266 wifi(com);

/*******HMI*************/
USART com3(USART3, 9600);

/********* GPIO ReSet ********/
GPIO MFRC522_ReSet(GPIOA, 5);

/****** Beeper ****************/
GPIO beer(GPIOB, 15);

char mApSetName[15];
char mApSetPasswd[18];
char mApJoinName[15] = "InfiniteYuan";
char mApJoinPasswd[18] = "741852963";
u8 AdminSerialNum[4] = { 0xA4, 0x37, 0xFE, 0x5F };		//管理员标签序列，解锁阅读器
u8	dataRfRcv[20];																		//MFRC522数据接收区

u8  Producer[10] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02 };
u8	Dealer[4] = { 0x00, 0x00, 0x00, 0x02 };
u8	Qie[2] = { 0x01, 0x05 };
char HMI_End[3] = { 0xff, 0xff, 0xff };				//HMI结束符

typedef enum
{
	PRODUCER,
	DEALER,
	QIE
}Role;

void Communication();
bool isSerialNumEqual(u8 *s1, u8 *s2);
bool SendID(unsigned char* IDNum);
RCState RCWriteHandle(Role role, u8 *RawData, u8 *writeData);

void Delay(){
	for (int i = 0; i < 255 * 1000; i++){
		;
	}
}

void Wifi_Init()
{
//WIFI initialize
	if (!wifi.Kick())//检查连接
	{
//		com << "no wifi!\n\n\n";
		return;
	}
	else
	{
//		com << "wifi is healthy\n";
		;
	}
	wifi.SetEcho(false);//关闭回响
	wifi.SetMode(esp8266::esp8266_MODE_STATION_AP, esp8266::esp8266_PATTERN_NULL);//设置station+ap模式
	wifi.SetMUX(false);//单链接模式
	wifi.SetApParam(mApSetName, mApSetPasswd, 1, esp8266::esp8266_ENCRYPTION_WPA2_PSK, esp8266::esp8266_PATTERN_NULL);
	while (!wifi.JoinAP(mApJoinName, mApJoinPasswd, esp8266::esp8266_PATTERN_NULL));//加入AP

	while (!wifi.Connect((char*)"192.168.191.2", 8090, Socket_Type_Stream, Socket_Protocol_IPV4))
	{
//		com << "connect server fail!\n\n\n";
		;
	}
//	com << "WIFI initialize complete!\n";
}

void APP_Init()
{
	Wifi_Init();
	rf.HardWareReset(MFRC522_ReSet);
	rf.InitializeSystem();
	Delay();
}
/////**********HMI 界面***********////////////////
void into_main()
{
	com3 << "page page0" << HMI_End;
	tskmgr.DelayMs(10);
}

void NetLink_Interface_Ok()
{
	com3 << "page wifiok" << HMI_End;
	tskmgr.DelayMs(10);
}

void setjifen(int16_t balan, int16_t jife)
{
	char* balance;
	char* jifen;
	////*****int to string*****////////
	sprintf(balance, "%d", balan);
	sprintf(jifen, "%d", jife);
	com3 << "page jifen" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t1.txt=\"" << balance << "\"" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t3.txt=\"" << jifen << "\"" << HMI_End;
	tskmgr.DelayMs(10);
}

void setuser(char* user, char* pass)
{
	com3 << "page page1" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t1.txt=\"" << user << "\"" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t3.txt=\"" << pass << "\"" << HMI_End;
	tskmgr.DelayMs(10);
}

void setyouhui(char* mess1, char* mess2, char* mess3)
{
	com3 << "page youhui" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t1.txt=\"" << mess1 << "\"" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t2.txt=\"" << mess2 << "\"" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t3.txt=\"" << mess3 << "\"" << HMI_End;
	tskmgr.DelayMs(10);
}

void setrichen(char* richen1, char* richen2, char* richen3)
{
	com3 << "page richen" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t0.txt=\"" << richen1 << "\"" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t1.txt=\"" << richen2 << "\"" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t2.txt=\"" << richen3 << "\"" << HMI_End;
	tskmgr.DelayMs(10);
}

void setjilu(char* julu1, char* julu2, char* julu3)
{
	com3 << "page jilu" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t0.txt=\"" << julu1 << "\"" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t1.txt=\"" << julu2 << "\"" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t2.txt=\"" << julu3 << "\"" << HMI_End;
	tskmgr.DelayMs(10);
}

void registerok()
{
	com3 << "page box" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t0.txt=\"" << "注册成功!" << "\"" << HMI_End;
	tskmgr.DelayMs(10);
}

void setIDcard(unsigned char* data, char* user)
{
	int i = 0;
	char str[8];
	char dst[9];
	/*******16进制转字符串*********/
	for (i = 0; i < 4; i++)
	{
		str[2 * i] = data[i] >> 4;
		str[2 * i + 1] = data[i] & 0xf;
	}
	for (i = 0; i < 8; i++)
	{
		sprintf(&dst[i], "%X/n", str[i]);
	}
	dst[8] = '\0';
	com3 << "page rfidcard" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t1.txt=\"" << dst << "\"" << HMI_End;
	tskmgr.DelayMs(10);
	com3 << "t3.txt=\"" << user << "\"" << HMI_End;
	tskmgr.DelayMs(10);
}

void comm()
{
	char name[10] = { 0 };
	if (wifi.Read(name) > 0)
	{
		if ((u8)name[0] == 0xAA)
		{
			registerok();
		}
	}
}
/////**********HMI 界面***********////////////////
int main()
{
	unsigned char ErrorCount = 0;
	unsigned char re_code = PICC_REQIDL;			//请求码为唤醒未休眠的卡
	char name[10] = { 0 };
	//	RCState Rstatus = MI_OK;

	bool isAdmin = false;
	float curTime = 0, oldTime = 0;
	FLASH_Unlock();
	into_main();

	APP_Init();

	NetLink_Interface_Ok();

	while (!isAdmin)								//验证使用者 Admin
	{
		if (rf.ReadyComunication(re_code) == MI_OK)
		{
			beer.SetLevel(1);
			SendID(rf.GetmSerialNum());
			beer.SetLevel(0);
			if (wifi.Read(name) > 0)
				setIDcard(rf.GetmSerialNum(), name);
			else
				setIDcard(rf.GetmSerialNum(), (char*)"******");
			if (isSerialNumEqual(AdminSerialNum, rf.GetmSerialNum()))
			{
				isAdmin = true;
			}
//			rf.WaitCardOff();
		}
		else
		{
			ErrorCount++;
		}
		if (ErrorCount >= 100)
		{
			ErrorCount = 0;
			rf.HardWareReset(MFRC522_ReSet);
			rf.InitializeSystem();
		}
	}

	while (1){
		curTime = tskmgr.Time();
		float deltaT = curTime - oldTime;
		if (deltaT > 0.2){
			oldTime = curTime;
			if (rf.ReadyComunication(re_code) == MI_OK)
			{
				beer.SetLevel(1);
				setIDcard(rf.GetmSerialNum(), mApJoinPasswd);//HMI
				SendID(rf.GetmSerialNum());//WIFI
				beer.SetLevel(0);
				if (isSerialNumEqual(AdminSerialNum, rf.GetmSerialNum()))
				{
					isAdmin = true;
				}
			}
			else
			{
				ErrorCount++;
			}
			if (ErrorCount >= 100)
			{
				ErrorCount = 0;
				rf.HardWareReset(MFRC522_ReSet);
				rf.InitializeSystem();
			}
//			Communication();
//			wifi.Write(mApJoinName, 15);
/*
		if(rf.ReadyComunication(re_code) == MI_OK)
		{
			if(!isSerialNumEqual(AdminSerialNum,rf.GetmSerialNum()))		//??Admin?????
			{
				Rstatus = rf.PcdRead(2, dataRfRcv);				//读第二块数据区
				if(Rstatus == MI_OK)
				{
					Rstatus = RCWriteHandle(DEALER,dataRfRcv,Dealer);
					if(Rstatus == MI_OK)
					{
						Rstatus = rf.PcdRead(2, dataRfRcv);

						beer.SetLevel(0);
						tskmgr.DelayMs(500);
						beer.SetLevel(1);
					}
					u8 dataSendServer[50];			//显示序列号
					strcpy((char *)dataSendServer,(char *)rf.GetmSerialNum());
					for(int i=4;i<20;i++)
					dataSendServer[i] = dataRfRcv[i-4];
					wifi.Write((char*)dataSendServer,20);	//将数据发送出去
					rf.PiccHalt();					//将该卡休眠
				}
				else
				{
					ErrorCount++;
				}
			}
		}
		else
		{
			ErrorCount++;
		}
		if(ErrorCount >= 3)
		{
			ErrorCount = 0;
			rf.HardWareReset(MFRC522_ReSet);
			rf.InitializeSystem();
		}
		tskmgr.DelayMs(500);
*/
		}
	}
}

RCState RCWriteHandle(Role role, u8 *RawData, u8 *writeData)
{
	RCState status = MI_OK;
	int i = 0;
	switch (role)
	{
	case PRODUCER:
		for (i = 0; i < 10; i++)
			*(RawData + i) = *(writeData + i);
		break;

	case DEALER:
		for (i = 10; i < 14; i++)
			*(RawData + i) = *(writeData + i - 10);
		break;

	case QIE:
		for (i = 14; i < 16; i++)
			*(RawData + i) = *(writeData + i - 14);
		break;
	}

	status = rf.PcdWrite(2, RawData);

	return status;
}

bool isSerialNumEqual(u8 *s1, u8 *s2)
{
	int i = 0;
	for (; i < 4; i++)
	{
		if (*(s1 + i) != *(s2 + i))
			break;
	}
	if (i < 4)
		return false;
	else
		return true;
}

char CheckSum(char * data, u8 length)
{
	char check = 0;
	for (int i = 0; i < length; i++)
	{
		check += data[i];
	}
	return check;
}

bool SendID(unsigned char* IDNum)
{
	char send[8];
	send[0] = 0xFF;
	send[1] = 0x01;
	send[2] = 0x04;
	send[3] = (char)IDNum[0];
	send[4] = (char)IDNum[1];
	send[5] = (char)IDNum[2];
	send[6] = (char)IDNum[3];
	send[7] = CheckSum(send, 7);
	return wifi.Write(send, 8);
}

/*
void Communication()
{
	static float curTime = 0, oldTime = 0;
	static bool flag = false;
	static u8 packetnum = 0;
	static u8 filelength = 0;
	// message from PC
	if (comLink1.CheckFrame())
	{
		switch (comLink1.rxFrame.fnCode)
		{
		case FC_IAP_UPDATE:
			if (comLink1.rxFrame.data[1] != packetnum){
				packetnum = comLink1.rxFrame.data[1];
				flag = false;
				_iapboot.Put_APP((comLink1.rxFrame.data), comLink1.rxFrame.data[0]);
				curTime = tskmgr.Time();
				oldTime = curTime;
				//					Delay();
				comLink1.txFrame.fnCode = FC_IAP_UPDATE;
				comLink1.txFrame.data[0] = 1;					//确认
				comLink1.txFrame.isUpdated = true;
				comLink1.Send();
				if (comLink1.rxFrame.data[0] < 48){
					flag = true;
					_iapboot.RecvIndex = 0;
				}
			}
			break;
		case FC_IAP_INFO:
			if ((comLink1.rxFrame.data[1] == comLink1.rxFrame.data[0]) && (comLink1.rxFrame.data[0] == 0xFF) && (comLink1.rxFrame.data[1] == comLink1.rxFrame.data[2]))
			{
				if (((*(vu32*)(FLASH_APP1_ADDR + 4)) & 0xFF000000) == 0x08000000)//判断是否为0X08XXXXXX.
				{
					//						com<<"kaishi flash app\r\n";
					FLASH_Lock();
					_iapboot.iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
				}
			}
			else if ((comLink1.rxFrame.data[1] == comLink1.rxFrame.data[0]) && (comLink1.rxFrame.data[0] == 0xFF) && (comLink1.rxFrame.data[1] != comLink1.rxFrame.data[2]))
			{
				filelength = comLink1.rxFrame.data[2];
			}
			break;
		}
	}
	else if ((comLink1.rxFrame.fnCode == FC_IAP_UPDATE) && !flag && (packetnum != filelength))
	{
		curTime = tskmgr.Time();
		float deltaT = curTime - oldTime;
		if (deltaT > 0.5)
		{
			oldTime = curTime;
			comLink1.txFrame.fnCode = FC_IAP_UPDATE;
			comLink1.txFrame.data[0] = 0;						//确认错误
			comLink1.txFrame.isUpdated = true;
			comLink1.Send();
		}
	}
}
*/
