#include <stdio.h>
#include <string.h>

#include "config.h"
#include "device.h"
#include "socket.h"
#include "w5500.h"
#include "dhcp.h"
#include "sockutil.h"
#include "util.h"//delay
#include "udp_demo.h"

void do_udp(void){
	uint16 len = 0;
	uint8 buffer[2048];/*定义一个2KB的缓存*/
//	uint8 pc_ip[4] = { 192, 168, 10, 10 };/*配置服务器的IP地址*/
//	uint16 pc_port = 5000;							/*定义计算机的一个端口并初始化*/
	uint16 local_port = 5000;
	
	if(isGetIP){
		switch (getSn_SR(SOCKET_UDP))/*获取socket 0的状态*/
		{
			case SOCK_UDP:/*socket 的套接字初始化完成*/
			if (getSn_IR(SOCKET_UDP) & Sn_IR_RECV)
			{
				setSn_IR(SOCKET_UDP, Sn_IR_RECV);/*Sn_IR的第0位置1*/
				printf("Sendto Fail\r\n");
			}
			if ((len = getSn_RX_RSR(SOCKET_UDP)) > 0)
			{
				len = recvfrom(SOCKET_UDP, buffer, len, remote_ip, &remote_port);/*W5500接收计算机发送来的数据*/
				printf("%d.%d.%d.%d:%d \r\n", remote_ip[0], remote_ip[1], remote_ip[2], remote_ip[3], remote_port);
				if (len != sendto(SOCKET_UDP, buffer, len, remote_ip, remote_port))
				{
					printf("Sendto Fail\r\n");
				}
			}
			break;
			
			case SOCK_CLOSED:/*socket 关闭*/
			socket(SOCKET_UDP, Sn_MR_UDP, local_port, 0);/*初始化socket 0的套接字*/
			break;
		}
	}
}
