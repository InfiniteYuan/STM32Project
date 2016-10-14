#ifndef __APP_H
#define __APP_H

#include "hardware.h"
#include "Protocol.h"
#include "stdlib.h"
#include "CRC.h"

class App
{
public:
	App();
	void TimerInterrupt();
	void Init();
	void loop();

	void WifiInit();

	//	bool SignIn();

	//检查硬件健康状态
	bool CheckHardware();

	//检查链接状态
	bool CheckConnectionToServer();

	//接收来自服务器的数据并处理
	void ReceiveAndDeal();

	bool SendMotorInfoToServer();
	bool SendKeepAliveToServer();

	bool Write(Protocol::ToServer&);

	//将小端模式的short类型转换为大端的数组，即short的高位在数组的低字节
	void LittleEndianToBigEndian(char*, short);

public:

	char mApSetName[15];
	char mApSetPasswd[18];
	char mApJoinName[15];
	char mApJoinPasswd[18];
//	char mMotor[6];
//	char mUserName[12];
//	char mUserPasswd[16];

	static const char mDeviceNumber[6];
	char mToServerData[50];   //通信帧的数据部分
	char mToServerComment[10];//通信帧注释部分
	char mDataTemp[1024];     //接收的wifi信息！发送信息时的存放处

	Protocol::ToServer mToServer;
	Protocol::MotorGroup mMotorGroup;
	Protocol::KeepAlive mKeepAlive;

	short Decode(void* *data, Protocol::OperationType*);

	bool mIsAlive;
	float mTimeReceivedKeepAlive;

};


#endif
