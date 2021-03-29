/****************************************************************************
@FILENAME:	timer_mgt.h
@FUNCTION:	timer management
@AUTHOR:	flege.zhang
@DATE:		2019.09.04
@COPYRIGHT(C) ZXW 2020
*****************************************************************************/


#ifndef _TIMER_MGT__H_
#define _TIMER_MGT__H_


#include "app_cfg.h"
#include "timer.h"


/*[16BIT]Timer0,default used by PWM!!*/
/*CLK 2M, MAX 32ms*/
#define TIMER0_IRQ_INTERVAL							(5)//5ms
#define TIMER0_2M_CLK_1MS							(0x07CE)
#define TIMER0_COUNT_VALUE							(TIMER0_2M_CLK_1MS*TIMER0_IRQ_INTERVAL)

/*[16BIT]Timer1*/
/*CLK 2M, MAX 32ms*/
#define TIMER1_IRQ_INTERVAL							(5)//5ms
#define TIMER1_2M_CLK_1MS							(0x07CE)//using 2M timer clock,1ms timer count
#define TIMER1_COUNT_VALUE							(TIMER1_2M_CLK_1MS*TIMER1_IRQ_INTERVAL)

/*[16BIT]Timer2*/
/*CLK 2M, MAX 32ms*/
#define TIMER2_2M_CLK_1MS							(0x07CE)

/*[24BIT]Core Timer*/
/*CLK 16M,MAX 1048ms*/
#define CORE_TIMER_IRQ_INTERVAL						(100)
#define CORE_TIMER_1MS								(16000)
#define CORE_TIMER_COUNT_VALUE						(CORE_TIMER_1MS*CORE_TIMER_IRQ_INTERVAL)


#ifdef USER_DEFINED_SLEEP_TIME
#define SYS_SLEEP_TIME_MS							(USER_DEFINED_SLEEP_TIME)
#else
#define SYS_SLEEP_TIME_MS							(1000)
#endif
#endif

