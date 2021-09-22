/*************************************************************************
	> File Name: hal_gpio.c
	> Author: 
	> Mail: 
	> Created Time: Wed 03 Apr 2019 21:17:54 CST
 ************************************************************************/

#include "hal_gpio.h"

//-----------------------------------------------------------------------
// Simulation hardware gpio
//-----------------------------------------------------------------------
typedef struct{
    hal_gpio_func_e func;
    hal_gpio_up_down_resistor_level_e level;
    u8 input_en;
    u8 output_en;
    u8 input_value;
    u8 output_value;
}__linux_simulation_gpio_s;

#define MAX_PIN_NUM 256
static __linux_simulation_gpio_s gpio_array[MAX_PIN_NUM];
static inline void gpio_set_func(u32 pin, hal_gpio_func_e func){
    gpio_array[pin].func = func;
}

static inline void gpio_set_output_en(u32 pin, u32 value){
    gpio_array[pin].output_en = (value == 0?0:1);
}

static inline void gpio_set_input_en(u32 pin, u32 value){
    gpio_array[pin].input_en = (value == 0?0:1);
}

static inline u8 gpio_is_output_en(u32 pin){
    return gpio_array[pin].output_en;
}

static inline u8 gpio_is_input_en(u32 pin){
    return gpio_array[pin].input_en;
}

static inline void gpio_setup_up_down_resistor(u32 pin, hal_gpio_up_down_resistor_level_e level){
    gpio_array[pin].level = level;
}

static inline void gpio_write(u32 pin, u32 value){
    gpio_array[pin].output_value = value;
}

static inline void gpio_toggle(u32 pin){
    gpio_array[pin].output_value = !gpio_array[pin].output_value;
}

static inline u8 gpio_read(u32 pin){
    return gpio_array[pin].input_value;
}
//END------------------------------------------------------------------


void hal_gpio_set_func(u32 pin, hal_gpio_func_e func){
    gpio_set_func(pin, func);
}

int hal_gpio_is_output_en(u32 pin){
    return gpio_is_output_en(pin);
}

int hal_gpio_is_input_en(u32 pin){
    return gpio_is_input_en(pin);
}

void hal_gpio_set_output_en(u32 pin, u32 value){
    gpio_set_output_en(pin,value);
}

void hal_gpio_set_input_en(u32 pin, u32 value){
    gpio_set_input_en(pin,value);
}

void hal_gpio_setup_up_down_resistor(u32 pin, hal_gpio_up_down_resistor_level_e level){
    gpio_setup_up_down_resistor(pin, level);
}

void hal_gpio_write(u32 pin, u32 value){
    gpio_write(pin,value);
}

void hal_gpio_toggle(u32 pin){
    gpio_toggle(pin);
}

u32 hal_gpio_read(u32 pin){
    return gpio_read(pin);
}

