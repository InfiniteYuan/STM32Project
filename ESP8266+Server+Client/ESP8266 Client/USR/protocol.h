#ifndef __PROTOCOL_H
#define __PROTOCOL_H
#include "stm32f10x.h"
class Protocol
{
public:
	enum OperationType{ OperationType_Control = 1, OperationType_Ack = 2, OperationType_Ask = 3 };

	struct ToServer
	{
		short head;
		short dataType;
		char operationType;
		unsigned char deviceNumber[6];
		unsigned char UTC[6];
		short dataLength;
		char* data;
		short crc16;
	};

	struct MotorGroup
	{
		static short dataType;
		char Motor[6];
		short commentLength;
		char* comment;
	};
	struct KeepAlive
	{
		static short dataType;
		char type;
	};

public:
	Protocol();

private:

};


#endif 

