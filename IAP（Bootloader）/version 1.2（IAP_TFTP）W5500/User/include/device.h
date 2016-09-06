#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "stm32f10x.h"

#define DEVICE_ID "W5200"

#define FW_VER_HIGH  1
#define FW_VER_LOW   0

typedef  void (*pFunction)(void);

extern uint8  	remote_ip[4];															/*‘∂∂ÀIPµÿ÷∑*/
extern uint16 	remote_port;		

void set_network(void);
void set_default(void);
void write_config_to_eeprom(void);

void Reset_W5500(void);

void USART1_Init(void);

void reboot(void);
void reboot_app(void);
void get_config(void);
#endif
