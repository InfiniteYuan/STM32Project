#include "Interrupt.h"
#include "app.h"
#include "stm32f10x.h"

App app;

void Timer2_IRQ();

int	main(void){
	SystemInit();
	app.Init();
//	timer.Start();

	while(true){
		app.loop();
	}
}

//��ʱ��2 �жϣ�1ms�ж�һ�Σ�����ˢ��
void Timer2_IRQ()
{
	app.TimerInterrupt();
}


//	com1<<wifi.SetEcho(false);//�رջ���
//	com1<<wifi.SetMode(esp8266::esp8266_MODE_STATION_AP, esp8266::esp8266_PATTERN_NULL);//����station+apģʽ
//	com1<<wifi.SetMUX(false);//������ģʽ
//	com1<<wifi.SetApParam(app.mApSetName, app.mApSetPasswd, 1, esp8266::esp8266_ENCRYPTION_WPA2_PSK, esp8266::esp8266_PATTERN_NULL);//�����ȵ���Ϣ
//	com1<<wifi.JoinAP(app.mApJoinName, app.mApJoinPasswd, esp8266::esp8266_PATTERN_NULL);//����AP	


//	wifi.SetEcho(false);//�رջ���
//	wifi.SetMode(esp8266::esp8266_MODE_STATION_AP, esp8266::esp8266_PATTERN_DEF);//����station+apģʽ
//	wifi.SetMUX(false);//������ģʽ
//	wifi.SetApParam(app.mApSetName, app.mApSetPasswd, 1, esp8266::esp8266_ENCRYPTION_WPA2_PSK, esp8266::esp8266_PATTERN_DEF);//�����ȵ���Ϣ
//	wifi.JoinAP(app.mApJoinName, app.mApJoinPasswd, esp8266::esp8266_PATTERN_NULL);//����AP
	


