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
	uint8 buffer[2048];/*����һ��2KB�Ļ���*/
//	uint8 pc_ip[4] = { 192, 168, 10, 10 };/*���÷�������IP��ַ*/
//	uint16 pc_port = 5000;							/*����������һ���˿ڲ���ʼ��*/
	uint16 local_port = 5000;
	
	if(isGetIP){
		switch (getSn_SR(SOCKET_UDP))/*��ȡsocket 0��״̬*/
		{
			case SOCK_UDP:/*socket ���׽��ֳ�ʼ�����*/
			if (getSn_IR(SOCKET_UDP) & Sn_IR_RECV)
			{
				setSn_IR(SOCKET_UDP, Sn_IR_RECV);/*Sn_IR�ĵ�0λ��1*/
				printf("Sendto Fail\r\n");
			}
			if ((len = getSn_RX_RSR(SOCKET_UDP)) > 0)
			{
				len = recvfrom(SOCKET_UDP, buffer, len, remote_ip, &remote_port);/*W5500���ռ����������������*/
				printf("%d.%d.%d.%d:%d \r\n", remote_ip[0], remote_ip[1], remote_ip[2], remote_ip[3], remote_port);
				if (len != sendto(SOCKET_UDP, buffer, len, remote_ip, remote_port))
				{
					printf("Sendto Fail\r\n");
				}
			}
			break;
			
			case SOCK_CLOSED:/*socket �ر�*/
			socket(SOCKET_UDP, Sn_MR_UDP, local_port, 0);/*��ʼ��socket 0���׽���*/
			break;
		}
	}
}
