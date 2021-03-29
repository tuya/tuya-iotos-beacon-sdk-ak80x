/****************************************************************************
@FILENAME:	hal_gpio.h
@FUNCTION:	the gpio config
@AUTHOR:	flege.zhang
@DATE:		2019.09.03
@COPYRIGHT(C) ZXW 2020
*****************************************************************************/


#include "hal_gpio.h"
#include "sys.h"
#include "int.h"


void hal_gpio_set_port(uint32_t port,gpio_dir_t direct,uint8_t hight_low)
{
	//validation check
	if(port > 19)
	{
		return;
	}

	//gpio00 and gpio01 must set mode first,then set gpio function!!
	if((0x00 == port) || (0x01 == port))
	{
		uint32_t reg = 0;
		uint8_t shift_value = 0x03;

		//get GPIO0_MODE_MASK(BIT[6:5])/GPIO1_MODE_MASK(BIT[4:3])
		shift_value <<= (5 - (port<<1));

		reg = read_reg(TOP_CTRL_BASE_ADDR);

		reg &= ~shift_value;
		shift_value = (shift_value>>(6 - direct - (port<<1)))<<(6 - direct - (port<<1));
		reg |= shift_value;
		
		write_reg(TOP_CTRL_BASE_ADDR, reg);
	}
	
	if(direct == GPIO_OUTPUT)
	{
		sys_set_port_mux(PAD_MUX_BASE_ADDR + (port<<2),(PAD_MUX_FUNCTION_0 | 0x03));//BIT[1]:output loopback;BIT[0]:pull,0:enable,1:disable
	}
	else
	{
		sys_set_port_mux(PAD_MUX_BASE_ADDR + (port<<2),(PAD_MUX_FUNCTION_0 | 0x02));//BIT[1]:output loopback;BIT[0]:pull,0:enable,1:disable
	}
	
    gpio_set_bit_direction(1<<port, direct);

	if(direct == GPIO_OUTPUT)
	{
    	gpio_set_bit(1<<port, hight_low);
	}
}

void hal_gpios_set(uint32_t bits,gpio_dir_t direct,uint8_t hight_low)
{
	for(uint8_t i = 0; i < 20; i++)
	{
		if(bits&(1<<i))
		{
			hal_gpio_set_port(i,direct,hight_low);
		}
	}
}

void hal_gpio_init()
{
	sys_set_module_clock(CLK_GPIO_EN_MASK,ON);
}

void hal_gpio_cfg_before_sleep(uint32_t reg_bits)
{
	//need define not used io(except wakeup io)
	
	//disable all int,avoid being interrupted.
	int_disable_irq(INT_ALL_EN_MASK);

	//set GPIO0,GPIO1 as output
	//GPIO0 BIT[6:5];GPIO1 BIT[4:3]
	uint32_t reg = read_reg(TOP_CTRL_BASE_ADDR);
	reg &= ~0x78;
	reg |= 0x78;
	write_reg(TOP_CTRL_BASE_ADDR,reg);

	//set all io(except wakeup used io) as output
	for(int8_t i = 0; i < 20; i++)
	{
		if(reg_bits&(0x01<<i))
		{
			sys_set_port_mux(PAD_MUX_BASE_ADDR + (i<<2),(PAD_MUX_FUNCTION_7|0x01));
		}
	}

	//set as debug io,output 0
	write_reg(0x40120194,0x09);//before is write_reg(0x40120194,0x07);
}


