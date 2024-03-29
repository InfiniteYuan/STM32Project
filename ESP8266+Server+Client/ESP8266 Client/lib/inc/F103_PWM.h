#ifndef _F103_PWM_H_
#define _F103_PWM_H_
 
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
 
class PWM{
	
	private:
				TIM_TypeDef* mTimer;
				u16 mPsc;				//分频系数
				
				u16 mFrequency;			//add by jason

				GPIO_InitTypeDef GPIO_InitStruct;						//GPIO结构体
				TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;	//时钟结构体
				TIM_OCInitTypeDef TIM_OCInitStructure;	//定义输出结构体
				void PWMInit(TIM_TypeDef* timer,bool enCh1,bool enCh2,bool enCh3,bool enCh4,u16 frequency,bool remap = false);
				void Calcute(u16 frequency);//逐步毕竟频率，计算PSC和ARR；
				void Calcute2(u32 quo,u32 quo2,u32 rem2,bool flagBest);//重复的代码在一起；
	
    public:
			
				u16 mArr;
				/**
				 *@brief 构造函数
				 *@param timer 选择的时钟
				 *@param enCh1 通道1是否打开
				 *@param enCh2 通道2是否打开
				 *@param enCh3 通道3是否打开
				 *@param enCh4 通道4是否打开
				 *@param frequency 期望频率
				 *@param remap 引脚是否映射,这个版本还没有做
				 */
				PWM(TIM_TypeDef* timer,bool enCh1=false,bool enCh2=false,bool enCh3=false,bool enCh4=false,u16 frequency=500,bool remap = false)
				{
						mPsc = 0;
						mArr = 0;
						Calcute(frequency);
						mFrequency = frequency;		//add by jason
						PWMInit(timer,enCh1,enCh2,enCh3,enCh4,frequency,remap);
				};
				
				/**
				 *@brief 改变一个通道的占空比
				 *@param channel 选择通道
				 *@param duty    设置占空比
				 *@param isSetPositiveDuty 设置高电平的占空比还是设置低电平的占空比
				 */
				void SetDuty(u8 channel,float duty,bool isSetPositiveDuty=true);
				/**
				 *@brief 改变一个通道的占空比
				 *@param channel 选择通道
				 *@param duty    设置占空比
				 *@param isSetPositiveDuty 设置高电平的占空比还是设置低电平的占空比
				 */
				void SetDuty(u8 channel,char duty,bool isSetPositiveDuty=true);
				/**
				 *@brief 改变通道的占空比
				 *@param dutyCh1 设置通道1的占空比
				 *@param dutyCh2 设置通道2的占空比
				 *@param dutyCh3 设置通道3的占空比
				 *@param dutyCh4 设置通道4的占空比
				 *@param *@param isSetPositiveDuty 设置高电平的占空比还是设置低电平的占空比
				 */
				void SetDuty(float dutyCh1,float dutyCh2,float dutyCh3,float dutyCh4,bool isSetPositiveDuty=true);//重载
				/**
				 *@brief 改变通道的占空比
				 *@param dutyCh1 设置通道1的占空比
				 *@param dutyCh2 设置通道2的占空比
				 *@param dutyCh3 设置通道3的占空比
				 *@param dutyCh4 设置通道4的占空比
				 *@param *@param isSetPositiveDuty 设置高电平的占空比还是设置低电平的占空比
				 */
				void SetDuty(char dutyCh1,char dutyCh2,char dutyCh3,char dutyCh4,bool isSetPositiveDuty=true);//重载
				/**
				*@brief 获取四个通道占空比
				*/
				void GetDuty(char *dutyCh);
				/**
				*@brief 获取单个通道占空比
				*/
				void GetDuty(u8 channel, char& dutyCh);
				/**
				 *@brief 得到PWM的频率
				 *@retval PWM频率
				 */
				u16 GetmFrequency();//Get the PWM's frequence.(add by jason)
				
};
 

 
#endif
