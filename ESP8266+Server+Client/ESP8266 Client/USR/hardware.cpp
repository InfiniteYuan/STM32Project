#include "hardware.h"

//light
GPIO ledGpio(GPIOB, 5);
LED light(ledGpio, false);
////TIM2 定时 
//Timer timer(TIM2, 0, 1, 0);//定时器3， 1ms中断一次 刷新状态；
////USART
USART com1(1, 115200, true);

////wifi
USART com2(2, 115200, false);
Socket_esp8266 wifi(com2);//wifi

////PWM PWM(TIM_TypeDef* timer,bool enCh1=false,bool enCh2=false,bool enCh3=false,bool enCh4=false,u16 frequency=500,bool remap = false)
PWM mMotor14(TIM3, true, true, true, true, 200, false);
PWM mMotor56(TIM4, true, true, false, false, 200, false);

