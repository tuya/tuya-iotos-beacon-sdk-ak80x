/****************************************************************************
@FILENAME:	iic.c
@FUNCTION:	using gpio to simulate iic function,iic master function base P24C02A Datasheet Rev.1.4
@AUTHOR:	flege.zhang
@DATE:		2019.09.09
@COPYRIGHT(C) ZXW 2020
*****************************************************************************/


#include "iic.h"
#include "hal_gpio.h"
#include "delay.h"


#if(DEVICE_ROLE_SELECTION == IIC_ROLE_MASTER)
#define SDA_OUTPUT(x)										hal_gpio_set_port(SDA_GPIO_PORT_NUM,GPIO_OUTPUT,(x))
#define SDA_INPUT											hal_gpio_set_port(SDA_GPIO_PORT_NUM,GPIO_INPUT,GPIO_LOW)
#define SDA_HIGHT											gpio_set_bit(1<<SDA_GPIO_PORT_NUM,GPIO_HIGH)
#define SDA_LOW												gpio_set_bit(1<<SDA_GPIO_PORT_NUM,GPIO_LOW)
#define SET_SDA(x)											gpio_set_bit(1<<SDA_GPIO_PORT_NUM,x)
#define GET_SDA												(gpio_get_bits(1<<SDA_GPIO_PORT_NUM) ? GPIO_HIGH : GPIO_LOW)

#define SCL_OUTPUT(x)										hal_gpio_set_port(SCL_GPIO_PORT_NUM,GPIO_OUTPUT,x)
#define	SCL_HIGHT											gpio_set_bit(1<<SCL_GPIO_PORT_NUM,GPIO_HIGH)
#define	SCL_LOW												gpio_set_bit(1<<SCL_GPIO_PORT_NUM,GPIO_LOW)

#if(E2PROM_WCB_ENABLE)
#define WCB_WRITE_ENABLE									hal_gpio_set_port(WCB_GPIO_PORT_NUM,GPIO_OUTPUT,GPIO_LOW)
#define WCB_WRITE_DISABLE									hal_gpio_set_port(WCB_GPIO_PORT_NUM,GPIO_OUTPUT,GPIO_HIGH)
#else
#define WCB_WRITE_ENABLE
#define WCB_WRITE_DISABLE
#endif

void iic_init()
{
	SCL_OUTPUT(GPIO_HIGH);
	SDA_OUTPUT(GPIO_HIGH);
	WCB_WRITE_DISABLE;
}

void iic_start()
{
	/*
	*    ___________
	*SCL:			|_____
	*	 ____
	*SDA:    |____________
	*/
	
	SCL_OUTPUT(GPIO_HIGH);
	SCL_HIGHT;
	SDA_OUTPUT(GPIO_HIGH);
	delay_us(IIC_DELAY);
	SDA_LOW;
	delay_us(IIC_DELAY);
	SCL_LOW;
}

void iic_stop()
{
	/*
	*    	  _______
	*SCL:____|			
	*	         _____
	*SDA:_______|
	*/

	SDA_OUTPUT(GPIO_LOW);
	
	SCL_LOW;
	delay_us(IIC_DELAY);
	SDA_LOW;
	
	SCL_HIGHT;
	delay_us(IIC_DELAY);
	SDA_HIGHT;
	delay_us(IIC_DELAY);

	WCB_WRITE_DISABLE;
}

void iic_write_byte(uint8_t byte)
{
	/*EXM:SDA:10b
	*    ______       ______
	*SCL:	   |______|	    |_______
	*	          ____________
	*SDA:_________|           |_____
	*/
	
	SDA_OUTPUT(GPIO_HIGH);

	for(uint8_t i = 0; i < 8; i++)
	{
		SCL_LOW;
		SET_SDA((byte>>(7-i))&0x1);
		delay_us(IIC_DELAY);
		SCL_HIGHT;
		delay_us(IIC_DELAY);
	}
}

uint8_t iic_read_byte()
{
	uint8_t byte = 0x0;

	SCL_LOW;
	delay_us(IIC_DELAY);
	SDA_INPUT;
	delay_us(IIC_DELAY);

	for(uint8_t i = 0; i < 8; i++)
	{
		SCL_HIGHT;
		delay_us(IIC_DELAY);

		byte = (byte<<1) | GET_SDA;

		SCL_LOW;
		delay_us(IIC_DELAY);
	}

	return byte;
}

uint8_t iic_get_ack()
{
	uint8_t ack = IIC_NACK;

	SCL_LOW;
	SDA_INPUT;
	delay_us(IIC_DELAY);
	SCL_HIGHT;
	delay_us(IIC_DELAY);
	ack = GET_SDA;
	SCL_LOW;
	delay_us(IIC_DELAY);

	return ack;
}

void iic_put_ack(uint8_t ack)
{
	SDA_HIGHT;
	delay_us(IIC_DELAY);
	
	SDA_OUTPUT(GPIO_HIGH);
	SCL_LOW;
	delay_us(IIC_DELAY);
	SET_SDA(ack);
	delay_us(IIC_DELAY);
	SCL_HIGHT;
	delay_us(IIC_DELAY);
}

uint8_t iic_write_bytes(uint16_t sub_addr,uint8_t* bytes,uint8_t length)
{
	uint8_t ret = 0x0;

    GLOBAL_INT_DISABLE();

	//1.start 
	iic_start();
	
	WCB_WRITE_ENABLE;

	iic_write_byte(IIC_OPERATION_WRITE);
	if(IIC_NACK == iic_get_ack())
	{
		ret = 0x01;
		goto iic_operation_end_label;
	}
	
	iic_write_byte(sub_addr);
	if(IIC_NACK == iic_get_ack())
	{
		ret = 0x01;
		goto iic_operation_end_label;
	}
	
	//Write Data
	do
	{
		iic_write_byte(*bytes++);
		if(IIC_NACK == iic_get_ack())
		{
			ret = 0x01;
			goto iic_operation_end_label;
		}
	}while(--length);

iic_operation_end_label:
	
	iic_stop();

    GLOBAL_INT_RESTORE();

	return ret;
}

uint8_t iic_read_bytes(uint16_t sub_addr,uint8_t* bytes,uint8_t length)
{
	uint8_t ret = 0x0;

    GLOBAL_INT_DISABLE();

	//DUMMY WRITE
	iic_start();
	
	iic_write_byte(IIC_OPERATION_WRITE);
	if(IIC_NACK == iic_get_ack())
	{
		ret = 0x01;
		goto iic_operation_end_label;
	}
	
	iic_write_byte(sub_addr);
	if(IIC_NACK == iic_get_ack())
	{
		ret = 0x01;
		goto iic_operation_end_label;
	}

	//READ
	iic_start();

	iic_write_byte(IIC_OPERATION_READ);
	if(IIC_NACK == iic_get_ack())
	{
		ret = 0x01;
		goto iic_operation_end_label;
	}

	for (;;)
	{
		*bytes++ = iic_read_byte();
		
		if(--length == 0)
		{
			iic_put_ack(IIC_NACK);
			break;
		}
		
		iic_put_ack(IIC_ACK);
	}

iic_operation_end_label:

	iic_stop();

    GLOBAL_INT_RESTORE();

    return ret;
}

void iic_soft_reset()
{
	iic_start();

	SDA_HIGHT;

	//9 clock cycle
	for(uint8_t i = 0; i < 9;i++)
	{
		SCL_LOW;
		delay_us(IIC_DELAY);
		SCL_HIGHT;
		delay_us(IIC_DELAY);
	}

	iic_start();

	iic_stop();
}
#elif(DEVICE_ROLE_SELECTION == IIC_ROLE_SLAVE)

#endif