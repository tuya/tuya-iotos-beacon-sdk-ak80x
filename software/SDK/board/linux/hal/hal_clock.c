/*************************************************************************
	> File Name: hal_clock.c
	> Author: 
	> Mail: 
	> Created Time: Wed 03 Jul 2019 22:10:00 CST
 ************************************************************************/

#include "hal_clock.h"
#include "hal_sys.h"

//get system tick function

u32 hal_clock_get_system_tick(void){
    struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

u32 hal_clock_time_exceed(u32 ref, u32 span_us){
    return ((hal_clock_get_system_tick() - ref) > span_us * HAL_CLOCK_1US_TICKS); 
}
