#include "app.h"

const char App::mDeviceNumber[] = { 1, 2, 3, 4, 5, 10 };

App::App()
	:mApSetName("CMCC-EDU"), mApSetPasswd("741852963 "),
	mApJoinName("InfiniteYuan"), mApJoinPasswd("741852963")
{
	mToServer.head = 0xabac;
	memcpy(mToServer.deviceNumber, mDeviceNumber, 6);
	mToServer.data = mToServerData;
	mMotorGroup.comment = mToServerComment;
	mMotorGroup.commentLength = 1;
}

void App::TimerInterrupt()
{
	com1 << "定时器3 1ms中断！\r\n";
}

void App::Init()
{
	light.Off();
	WifiInit();

	TaskManager::DelayS(5);

	com1 << "initialize complete!\n";
}

void App::loop()
{
	static float time;
	if (TaskManager::Time() - time >= 20)
	{
		time = TaskManager::Time();
		com1 << ".";
	}
	if (!CheckHardware())
		com1 << "haredware error!\n";
	//	SendMotorInfoToServer();
	//接收数据并处理
	ReceiveAndDeal();
}

void App::WifiInit()
{
	//WIFI initialize
	if (!wifi.Kick())//检查连接
	{
		com1 << "no wifi!\n\n\n";
		light.Blink(3, 300);
		return;
	}
	else
		com1 << "wifi is healthy\n";
	wifi.SetEcho(false);//关闭回响
	wifi.SetMode(esp8266::esp8266_MODE_STATION_AP, esp8266::esp8266_PATTERN_NULL);//设置station+ap模式
	wifi.SetMUX(false);//单链接模式
	wifi.SetApParam(mApSetName, mApSetPasswd, 1, esp8266::esp8266_ENCRYPTION_WPA2_PSK, esp8266::esp8266_PATTERN_NULL);
	wifi.JoinAP(mApJoinName, mApJoinPasswd, esp8266::esp8266_PATTERN_NULL);//加入AP

	while (!wifi.Connect((char*)"192.168.191.1", 8090, Socket_Type_Stream, Socket_Protocol_IPV4))
	{
		com1 << "connect server fail!\n\n\n";
		light.Blink(4, 300);
	}
	com1 << "WIFI initialize complete!\n";
	light.Blink(2, 200);
	light.Off();
}

bool App::CheckHardware()
{
	return true;//wifi.Kick();
}

bool App::CheckConnectionToServer()
{
	if (TaskManager::Time() - mTimeReceivedKeepAlive > 320)//超时（5）分钟，没有收到来自服务器的保持包
	{
		mIsAlive = false;
		//尝试重新连接
		uint8_t time = 0;
		while((!wifi.Connect((char*)"192.168.191.1", 8090, Socket_Type_Stream, Socket_Protocol_IPV4))||(time != 3)){
			time++;
		}
		return false;
	}
	return true;
}

void App::ReceiveAndDeal()
{
	static unsigned short size = 0;
	if (com2.ReceiveBufferSize() > 5)
	{
		size = wifi.Read(mDataTemp);
		if (size > 0)
		{
			void* data;
			static Protocol::OperationType operationType;
			short dataType = Decode(&data, &operationType);
			com1 << "type:" << dataType << "\r\n";
			if (dataType == Protocol::MotorGroup::dataType)
			{
				com1 << "Protocol::MotorGroup::dataType";
				if (operationType == Protocol::OperationType_Control){//舵机控制请求
					com1 << "OperationType_Control";
					mMotor14.SetDuty(((Protocol::MotorGroup*)data)->Motor[0], ((Protocol::MotorGroup*)data)->Motor[1],
						((Protocol::MotorGroup*)data)->Motor[2], ((Protocol::MotorGroup*)data)->Motor[3]);
					mMotor56.SetDuty(1, ((Protocol::MotorGroup*)data)->Motor[4]);
					mMotor56.SetDuty(2, ((Protocol::MotorGroup*)data)->Motor[5]);

//					com1<<"Momor[0]"<<((Protocol::MotorGroup*)data)->Motor[0]<<"\r\n";
//					com1<<"11";

					SendMotorInfoToServer();
				}
				else if (operationType == Protocol::OperationType_Ask){//舵机询问请求
					com1 << "OperationType_Ask";
					SendMotorInfoToServer();
				}
			}
			else if (dataType == Protocol::KeepAlive::dataType)
			{
				SendKeepAliveToServer();
			}
		}
	}
}

short App::Decode(void* *data, Protocol::OperationType* operationType)
{
	//验证头
	if ((uint8_t)mDataTemp[0] != 0xab || (uint8_t)mDataTemp[1] != 0xac)
		return 0;
	mToServer.dataLength = (short)mDataTemp[17] << 8 | mDataTemp[18];
	//CRC校验
	short parity = CRC16Calculate((const uint8_t*)mDataTemp, 19 + mToServer.dataLength);
	short parity2 = ((short)mDataTemp[19 + mToServer.dataLength] << 8 | mDataTemp[20 + mToServer.dataLength]);
	if (parity != parity2)
		return 0;
	//校验成功，数据分解
	mToServer.operationType = mDataTemp[4];
	mToServer.dataType = ((short)mDataTemp[2] << 8 | mDataTemp[3]);
	memcpy(mToServer.deviceNumber, mDataTemp + 5, 6);
	memcpy(mToServer.UTC, mDataTemp + 11, 6);
	mToServer.dataLength = (short)mDataTemp[17] << 8 | mDataTemp[18];
	memcpy(mToServer.data, mDataTemp + 19, mToServer.dataLength);

	if (mToServer.dataType == Protocol::MotorGroup::dataType)
	{
		*data = &mMotorGroup;
		memcpy(mMotorGroup.Motor, mToServer.data, 6);
		mMotorGroup.commentLength = (short)mToServer.data[6] << 8 | mToServer.data[7];
		memcpy(mMotorGroup.comment, mToServer.data + 6, mMotorGroup.commentLength);
	}
	else if (mToServer.dataType == Protocol::KeepAlive::dataType)
	{
		*data = &mKeepAlive;
		mKeepAlive.type = mToServer.data[0];//设备
	}
	*operationType = (Protocol::OperationType)mToServer.operationType;
	return mToServer.dataType;
}

bool App::SendMotorInfoToServer()
{
	mToServer.operationType = Protocol::OperationType_Ack;
	mToServer.dataType = Protocol::MotorGroup::dataType;
	mToServer.dataLength = 8;
	mMotor14.GetDuty(mToServer.data);
	mMotor56.GetDuty(1, mToServer.data[4]);
	mMotor56.GetDuty(2, mToServer.data[5]);
//	com1<<mToServer.data[0];
	mToServer.data[6] = 0x01;
	mToServer.data[7] = 0x01;
	if (Write(mToServer))
	{
		mIsAlive = true;
		mTimeReceivedKeepAlive = TaskManager::Time();
		return true;
	}
	mIsAlive = false;
	return false;
}

bool App::SendKeepAliveToServer()
{
	mToServer.operationType = Protocol::OperationType_Ack;
	mToServer.dataType = Protocol::KeepAlive::dataType;
	mToServer.dataLength = 1;
	mToServer.data[0] = 1;
	if (Write(mToServer))
	{
		mIsAlive = true;
		mTimeReceivedKeepAlive = TaskManager::Time();
		return true;
	}
	mIsAlive = false;
	return false;
}

bool App::Write(Protocol::ToServer &toServer)
{
	LittleEndianToBigEndian(mDataTemp, toServer.head);//起始标志
	LittleEndianToBigEndian(mDataTemp + 2, toServer.dataType);//数据类型
	mDataTemp[4] = toServer.operationType;//操作类型
	memcpy(mDataTemp + 5, toServer.deviceNumber, 6);
	memcpy(mDataTemp + 11, toServer.UTC, 6);
	LittleEndianToBigEndian(mDataTemp + 17, toServer.dataLength);//数据长度
	memcpy(mDataTemp + 19, toServer.data, toServer.dataLength);
	toServer.crc16 = CRC16Calculate((const uint8_t*)mDataTemp, 19 + toServer.dataLength);
	LittleEndianToBigEndian(mDataTemp + 19 + toServer.dataLength, toServer.crc16);
	return wifi.Write(mDataTemp, 21 + toServer.dataLength);
}

void App::LittleEndianToBigEndian(char* array, short bigEndian)
{
	char temp = bigEndian;
	bigEndian >>= 8;
	bigEndian &= 0x00ff;
	bigEndian |= (short)temp << 8;
	memcpy(array, (char*)&bigEndian, 2);
}
