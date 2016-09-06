/**
  ******************************************************************************
  * @file    main.c
  ******************************************************************************
  * ʵ��ƽ̨:hikit sc1���ܿ�����
  * ����    :http://www.hikit.cn
  * �Ա�    :http://shop118467498.taobao.com
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

uint8 buffer[2048];/*����һ��2KB�Ļ���*/
uint8 isGetIP = 0;

TFTP_MSG  tftp;
uint8_t g_socket_rcv_buf[MAX_MTU_SIZE];
uint8 Buffer[1024];
uint32_t tftp_server;
uint8 filename_flag = 0;

int main()
{
	//  uint8 dhcpret=0;

	RCC_Configuration(); /* ���õ�Ƭ��ϵͳʱ��*/
	GPIO_Configuration();/* ����GPIO*/
	NVIC_Configuration();/* ����Ƕ���ж�����*/
	Timer_Configuration();
	Systick_Init(72);/* ��ʼ��Systick����ʱ��*/
	USART1_Init(); /*��ʼ������ͨ��:115200@8-n-1*/
	at24c16_init();/*��ʼ��eeprom*/
	printf("W5500 EVB initialization over.\r\n");

	Reset_W5500();/*Ӳ����W5500*/
	WIZ_SPI_Init();/*��ʼ��SPI�ӿ�*/
	printf("W5500 initialized!\r\n");

	set_default();
	init_dhcp_client();
	TFTP_init(SOCKET_TFTP, g_socket_rcv_buf);
	setRTR(2000);/*�������ʱ��ֵ*/
	setRCR(3);/*����������·��ʹ���*/
	while (1)
	{
		do_dhcp();
//		do_udp();
		doTFTP();
	}
}

