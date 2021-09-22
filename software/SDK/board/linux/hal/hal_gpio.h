/*************************************************************************
	> File Name: hal_gpio.h
	> Author: 
	> Mail: 
	> Created Time: Wed 03 Apr 2019 21:17:50 CST
 ************************************************************************/

#ifndef _HAL_GPIO_H
#define _HAL_GPIO_H

#include "hal_sys.h"

//gpio function choice
typedef enum{
    GPIO_FUNC_AS_NOT_GPIO = 0x00,
    GPIO_FUNC_AS_GPIO,
    GPIO_FUNC_AS_UART,
    GPIO_FUNC_AS_PWM,
    GPIO_FUNC_AS_I2C,
    GPIO_FUNC_AS_SPI,
    GPIO_FUNC_AS_ADC,
}hal_gpio_func_e;

//gpio up down resistor level
typedef enum{
    GPIO_RESISTOR_FLOAT = 0x00,
    GPIO_RESISTOR_PULLUP_1M,
    GPIO_RESISTOR_PULLUP_10K,
    GPIO_RESISTOR_PULLDOWN_100K,
}hal_gpio_up_down_resistor_level_e;

// special: set the gpio some function
extern void hal_gpio_set_func(u32 pin, hal_gpio_func_e func);

// judge if input or output
extern int hal_gpio_is_output_en(u32 pin);
extern int hal_gpio_is_input_en(u32 pin);

// set input or output
extern void hal_gpio_set_output_en(u32 pin, u32 value);//0-disable  1-enable
extern void hal_gpio_set_input_en(u32 pin, u32 value);//0-disable  1-enable
extern void hal_gpio_setup_up_down_resistor(u32 pin, hal_gpio_up_down_resistor_level_e level);

// read & write
extern void hal_gpio_write(u32 pin ,u32 value);//0-low 1-high
extern void hal_gpio_toggle(u32 pin);
extern u32 hal_gpio_read(u32 pin);//0 !0


//GPIO DEFINE
#define GPIO_PA0 0
#define GPIO_PA1 1
#define GPIO_PA2 2
#define GPIO_PA3 3
#define GPIO_PA4 4
#define GPIO_PA5 5
#define GPIO_PA6 6
#define GPIO_PA7 7
#define GPIO_PB0 8
#define GPIO_PB1 9
#define GPIO_PB2 10
#define GPIO_PB3 11
#define GPIO_PB4 12
#define GPIO_PB5 13
#define GPIO_PB6 14
#define GPIO_PB7 15
#define GPIO_PC0 16
#define GPIO_PC1 17
#define GPIO_PC2 18
#define GPIO_PC3 19
#define GPIO_PC4 20
#define GPIO_PC5 21
#define GPIO_PC6 22
#define GPIO_PC7 23
#define GPIO_PD0 24
#define GPIO_PD1 25
#define GPIO_PD2 26
#define GPIO_PD3 27
#define GPIO_PD4 28
#define GPIO_PD5 29
#define GPIO_PD6 30
#define GPIO_PD7 31
#define GPIO_PE0 32
#define GPIO_PE1 33
#define GPIO_PE2 34
#define GPIO_PE3 35
#define GPIO_PE4 36
#define GPIO_PE5 37
#define GPIO_PE6 38
#define GPIO_PE7 39


#endif
