#ifndef __HARDWARE_H_
#define __HARDWARE_H_

#include "LED.h"
#include "GPIO.h"
#include "F103_PWM.h"
#include "USART.h"
#include "Timer.h"
#include "Socket_esp8266.h"

//light
extern GPIO ledGpio;
extern LED light;
//TIM2 定时器
//extern Timer timer;//TIM2 定时器 1ms中断一次，刷新状态；
//USART
extern USART com1;

//wifi
extern USART com2;
extern Socket_esp8266 wifi;//wifi

//PWM PWM(TIM_TypeDef* timer,bool enCh1=false,bool enCh2=false,bool enCh3=false,bool enCh4=false,u16 frequency=500,bool remap = false)
extern PWM mMotor14;
extern PWM mMotor56;


#endif


