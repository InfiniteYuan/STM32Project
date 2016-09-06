/**
  ******************************************************************************
  * @file    main.c
  ******************************************************************************
  * 实验平台:hikit sc1智能开发板
  * 官网    :http://www.hikit.cn
  * 淘宝    :http://shop118467498.taobao.com
  *
  ******************************************************************************
  */
#include "mcu_init.h"
#include "config.h"
#include "device.h"
#include "spi2.h"
#include "socket.h"
#include "w5500.h"
#include "at24c16.h"
#include "stm32f10x_it.h"
#include "util.h"
#include "dhcp.h"
#include "udp_demo.h"
#include "tftp.h"

#include <stdio.h>
#include <string.h>

extern uint8 txsize[];
extern uint8 rxsize[];

uint8 buffer[2048];/*定义一个2KB的缓存*/
uint8 isGetIP = 0;

TFTP_MSG  tftp;
uint8_t g_socket_rcv_buf[MAX_MTU_SIZE];
uint8 Buffer[1024];
uint32_t tftp_server;
uint8 filename_flag = 0;

int main()
{
	//  uint8 dhcpret=0;

	RCC_Configuration(); /* 配置单片机系统时钟*/
	GPIO_Configuration();/* 配置GPIO*/
	NVIC_Configuration();/* 配置嵌套中断向量*/
	Timer_Configuration();
	Systick_Init(72);/* 初始化Systick工作时钟*/
	USART1_Init(); /*初始化串口通信:115200@8-n-1*/
	at24c16_init();/*初始化eeprom*/
	printf("W5500 EVB initialization over.\r\n");

	Reset_W5500();/*硬重启W5500*/
	WIZ_SPI_Init();/*初始化SPI接口*/
	printf("W5500 initialized!\r\n");

	set_default();
	init_dhcp_client();
	TFTP_init(SOCKET_TFTP, g_socket_rcv_buf);
	setRTR(2000);/*设置溢出时间值*/
	setRCR(3);/*设置最大重新发送次数*/
	while (1)
	{
		do_dhcp();
//		do_udp();
		doTFTP();
	}
}

