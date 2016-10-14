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

	//���Ӳ������״̬
	bool CheckHardware();

	//�������״̬
	bool CheckConnectionToServer();

	//�������Է����������ݲ�����
	void ReceiveAndDeal();

	bool SendMotorInfoToServer();
	bool SendKeepAliveToServer();

	bool Write(Protocol::ToServer&);

	//��С��ģʽ��short����ת��Ϊ��˵����飬��short�ĸ�λ������ĵ��ֽ�
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
	char mToServerData[50];   //ͨ��֡�����ݲ���
	char mToServerComment[10];//ͨ��֡ע�Ͳ���
	char mDataTemp[1024];     //���յ�wifi��Ϣ��������Ϣʱ�Ĵ�Ŵ�

	Protocol::ToServer mToServer;
	Protocol::MotorGroup mMotorGroup;
	Protocol::KeepAlive mKeepAlive;

	short Decode(void* *data, Protocol::OperationType*);

	bool mIsAlive;
	float mTimeReceivedKeepAlive;

};


#endif
