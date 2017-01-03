#include "stm32f10x.h"

/***** System *****/
#include "Configuration.h"
#include "TaskManager.h"

/***** OnChip *****/
#include "USART.h"
//#include "LED.h"

/***** ToolBox*****/
#include "GidLink.h"
#include "IAP.h"

USART com(USART1, 115200);   //USART1
UidLink comLink1(com);
IAP_BootLoader _iapboot(0);
TaskManager tskmgr;

void Delay(){
	for(int i =0; i<255;i++){
		;
	}
}
void Communication()
{
	static float curTime = 0, oldTime = 0;
	static bool flag = false;
	static u8 packetnum = 0;
	static u8 filelength = 0;
	/* message from PC*/
	if (comLink1.CheckFrame())
	{
		switch (comLink1.rxFrame.fnCode)
		{
			case FC_IAP_UPDATE:
				if(comLink1.rxFrame.data[1] != packetnum){
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
					if(comLink1.rxFrame.data[0]<48){
						flag = true;
						_iapboot.RecvIndex  = 0;
					}
				}
				break;
			case FC_IAP_INFO:
				if((comLink1.rxFrame.data[1] == comLink1.rxFrame.data[0])&&(comLink1.rxFrame.data[0] == 0xFF)&&(comLink1.rxFrame.data[1] == comLink1.rxFrame.data[2]))
				{
					if (((*(vu32*)(FLASH_APP1_ADDR + 4)) & 0xFF000000) == 0x08000000)//判断是否为0X08XXXXXX.
					{
//						com<<"kaishi flash app\r\n";
						FLASH_Lock();
						_iapboot.iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
					}
				}
				else if((comLink1.rxFrame.data[1] == comLink1.rxFrame.data[0])&&(comLink1.rxFrame.data[0] == 0xFF)&&(comLink1.rxFrame.data[1] != comLink1.rxFrame.data[2]))
				{
					filelength = comLink1.rxFrame.data[2];
				}
				break;
		}
	}
	else if((comLink1.rxFrame.fnCode == FC_IAP_UPDATE)&&!flag&&(packetnum!=filelength))
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

int main()
{
//	LED led(GPIOC, 13, true);
	float curTime = 0, oldTime = 0;
	FLASH_Unlock();
	while(1){
		curTime = tskmgr.Time();
		float deltaT = curTime - oldTime;
		if (deltaT > 0.2){
			oldTime = curTime;
			Communication();
//			led.Hz(10);
		}
	}
}

