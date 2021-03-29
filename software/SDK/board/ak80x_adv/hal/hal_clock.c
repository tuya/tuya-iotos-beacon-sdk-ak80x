/*************************************************************************
	> File Name: hal_clock.c
	> Author: 
	> Mail: 
	> Created Time: Wed 03 Jul 2019 22:10:00 CST
 ************************************************************************/

#include "hal_clock.h"
#include "hal_sys.h"

//system tick
#define PHY_SYSTEM_TICK reg_system_tick

//get system tick function
/*
AK801 can only use core timer(@16MHz,24Bit),max 1048ms
1.init core timer at main(),as follows:
	static volatile uint32_t core_timer_count_ms = 0x0;
	core_timer_init(CLOCK_SYS_CLOCK_HZ);//interrupt frequency:1000ms
	core_timer_set_int(ON);//enable core timer int
	core_timer_enable();//enable core timer
	
2.increace core_timer_count_ms at core_timer_isr(),as follows:
	ATTRIBUTE_ISR void core_timer_isr()
	{
		read_reg(CORE_TIMER_CORET_CSR_REG);//read clear
		core_timer_count_ms += 1000;//increace 1000ms
	}
*/
extern volatile uint32_t core_timer_count_ms;

u32 hal_clock_get_system_tick(void){
    unsigned int tick_ms;
	tick_ms = core_timer_count_ms+(CLOCK_SYS_CLOCK_HZ - get_core_timer_value())/(CLOCK_SYS_CLOCK_HZ/1000);//core timer is a down counter,ticks to ms

    return tick_ms*HAL_CLOCK_1MS_TICKS;//tick
}

u32 hal_clock_time_exceed(u32 ref, u32 span_us){
    return ((hal_clock_get_system_tick() - ref) > span_us*HAL_CLOCK_1US_TICKS); 
}
