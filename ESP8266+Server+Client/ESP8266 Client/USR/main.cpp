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

//定时器2 中断，1ms中断一次，用来刷新
void Timer2_IRQ()
{
	app.TimerInterrupt();
}


//	com1<<wifi.SetEcho(false);//关闭回响
//	com1<<wifi.SetMode(esp8266::esp8266_MODE_STATION_AP, esp8266::esp8266_PATTERN_NULL);//设置station+ap模式
//	com1<<wifi.SetMUX(false);//单链接模式
//	com1<<wifi.SetApParam(app.mApSetName, app.mApSetPasswd, 1, esp8266::esp8266_ENCRYPTION_WPA2_PSK, esp8266::esp8266_PATTERN_NULL);//设置热点信息
//	com1<<wifi.JoinAP(app.mApJoinName, app.mApJoinPasswd, esp8266::esp8266_PATTERN_NULL);//加入AP	


//	wifi.SetEcho(false);//关闭回响
//	wifi.SetMode(esp8266::esp8266_MODE_STATION_AP, esp8266::esp8266_PATTERN_DEF);//设置station+ap模式
//	wifi.SetMUX(false);//单链接模式
//	wifi.SetApParam(app.mApSetName, app.mApSetPasswd, 1, esp8266::esp8266_ENCRYPTION_WPA2_PSK, esp8266::esp8266_PATTERN_DEF);//设置热点信息
//	wifi.JoinAP(app.mApJoinName, app.mApJoinPasswd, esp8266::esp8266_PATTERN_NULL);//加入AP
	


