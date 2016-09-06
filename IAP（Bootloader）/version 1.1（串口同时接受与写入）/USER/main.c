#include "LED.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "iap.h"

//PA4 run sram app
//PA3 run flash app
//PA2 erase flash
//PA0 copy app to flash
int main(void)
{
	u8 key;
	u16 oldcount = 0;				//老的串口接收数据值
	u16 applenth = 0;				//接收到的app代码长度

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	//串口初始化为115200
	delay_init();	   	 	//延时初始化 
	LED_Init();		  		//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	FLASH_Unlock();
	while (1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出高
		if (USART_RX_CNT)
		{
			if (oldcount == USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				applenth = USART_RX_CNT;
				oldcount = 0;
				USART_RX_CNT = 0;
				printf("用户程序接收完成!\r\n");
				printf("代码长度:%dBytes\r\n", applenth);
				FLASH_Lock();
			}
			else oldcount = USART_RX_CNT;
		}
		delay_ms(10);
		key = KEY_Scan(0);
		
		
		if (key == WKUP_PRES)
		{
			if (applenth)
			{
				printf("开始更新固件...\r\n");
				if (((*(vu32*)(0X20001000 + 4)) & 0xFF000000) == 0x08000000)//判断是否为0X08XXXXXX.
				{
					iap_write_appbin(FLASH_APP1_ADDR, USART_RX_BUF, applenth);//更新FLASH代码   
					printf("固件更新完成!\r\n");
				}
				else
				{
					printf("非FLASH应用程序!\r\n");
				}
			}
			else
			{
				printf("没有可以更新的固件!\r\n");
			}						 
		}
		
		if (key == KEY2_PRES)
		{
			if (applenth)
			{
				printf("固件清除完成!\r\n");
				applenth = 0;
			}
			else
			{
				printf("没有可以清除的固件!\r\n");
			}							 
		}
		
		if (key == KEY1_PRES)
		{
			if (((*(vu32*)(FLASH_APP1_ADDR + 4)) & 0xFF000000) == 0x08000000)//判断是否为0X08XXXXXX.
			{
				printf("开始执行FLASH用户代码!!\r\n");
				iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
			}
			else
			{
				printf("非FLASH应用程序,无法执行!\r\n");
			}
		}
		
		
		if (key == KEY0_PRES)
		{
			printf("开始执行SRAM用户代码!!\r\n");
			if (((*(vu32*)(0X20001000 + 4)) & 0xFF000000) == 0x20000000)//判断是否为0X20XXXXXX.
			{
				iap_load_app(0X20001000);//SRAM地址
			}
			else
			{
				printf("非SRAM应用程序,无法执行!\r\n");
			}
		}

	}
}

__asm void wait()
{
	BX lr
}

void HardFault_Handler(void){
	printf("HardFault_Handler()\r\n");
	wait();
}



