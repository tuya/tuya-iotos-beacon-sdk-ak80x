/*************************************************************************
	> File Name: hal_clock.h
	> Author: 
	> Mail: 
	> Created Time: Wed 03 Jul 2019 22:09:57 CST
 ************************************************************************/

#ifndef _HAL_CLOCK_H
#define _HAL_CLOCK_H

#include "hal_sys.h"

#define HAL_CLOCK_HZ CLOCK_SYS_CLOCK_HZ

enum{
    //how much 1us = ? ticks
    HAL_CLOCK_1S_TICKS = HAL_CLOCK_HZ,
    HAL_CLOCK_1MS_TICKS = (HAL_CLOCK_1S_TICKS / 1000),
    HAL_CLOCK_1US_TICKS = (HAL_CLOCK_1MS_TICKS / 1000),
};
//#define HAL_CLOCK_1US_TICKS  (0.001)

extern u32 hal_clock_get_system_tick(void);
extern u32 hal_clock_time_exceed(u32 ref, u32 span_us);


#endif

