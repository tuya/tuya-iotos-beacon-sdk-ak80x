/****************************************************************************
@FILENAME:	app_gpio.h
@FUNCTION:	the gpio config
@AUTHOR:	flege.zhang
@DATE:		2019.09.03
@COPYRIGHT(C) ZXW 2020
*****************************************************************************/

#ifndef _HAL_GPIO__H_
#define _HAL_GPIO__H_

#include "app_cfg.h"
#include "gpio.h"



#define GPIO_LOW									(0x00)
#define GPIO_HIGH									(0x01)



void	hal_gpio_set_port(uint32_t port,gpio_dir_t direct,uint8_t hight_low);
void 	hal_gpios_set(uint32_t bits,gpio_dir_t direct,uint8_t hight_low);
void	hal_gpio_init(void);
void    hal_gpio_cfg_before_sleep(uint32_t reg_bits);

#endif
