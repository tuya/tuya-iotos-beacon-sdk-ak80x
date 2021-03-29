/****************************************************************************
@FILENAME:	iic.h
@FUNCTION:	using gpio to simulate iic function,iic master function base P24C02A Datasheet Rev.1.4
@AUTHOR:	flege.zhang
@DATE:		2019.09.09
@COPYRIGHT(C) ZXW 2020
*****************************************************************************/


#ifndef _IIC__H_
#define _IIC__H_

#include "type.h"
#include "app_cfg.h"


#define IIC_ROLE_MASTER													0x00
#define IIC_ROLE_SLAVE													0x01
#define DEVICE_ROLE_SELECTION											IIC_ROLE_MASTER

#if(DEVICE_ROLE_SELECTION == IIC_ROLE_MASTER)
#define E2PROM_WCB_ENABLE												DISABLE//internal E2PROM not used,disable write control
#endif

#define SDA_GPIO_PORT_NUM												13//E2PROM SDA pin(internal)
#define SCL_GPIO_PORT_NUM												12//E2PROM SCL pin(internal)
#define WCB_GPIO_PORT_NUM												0 //E2PROM WCB pin(internal not used),Low enable write

#define IIC_OPERATION_WRITE												0xA0
#define IIC_OPERATION_READ												0xA1

#define IIC_ACK															0x00
#define IIC_NACK														0x01					

#define IIC_DELAY														(20)


#if(DEVICE_ROLE_SELECTION == IIC_ROLE_MASTER)
void	iic_init(void);
uint8_t iic_write_bytes(uint16_t sub_addr,uint8_t* bytes,uint8_t length);
uint8_t iic_read_bytes(uint16_t sub_addr,uint8_t* bytes,uint8_t length);
#elif(DEVICE_ROLE_SELECTION == IIC_ROLE_SLAVE)
#endif
#endif

