/* ---------------------------------------------------------------
** File name:		timer.h
** Brief:
** COPYRIGHT(C) ZXW 2020
** ---------------------------------------------------------------
** Module Information:
**
** Revision History ----------------------------------------------
** ---------------------------------------------------------------
**  Date        Version     Action                  Author
** ---------------------------------------------------------------
**  2019-5-17   1.0         Created
**
** ---------------------------------------------------------------
*/

#ifndef   __TIMER_H_
#define   __TIMER_H_

/* Includes -----------------------------------------------------*/
#include "config.h"
#include "type.h"
/* Macro --------------------------------------------------------*/

/* Registers ----------------------------------------------------*/

/* Functions prototype ------------------------------------------*/




#define TIME_BASE_ADDR                  	(0x40030000)
#define TIMER0_LOAD_COUNT_ADDR			    (0x40030000)
#define TIMER0_CURENT_VALUE_ADDR		    (0X40030004)
#define TIMER0_CONTROL_REG			        (0X40030008)
#define TIMER0_EOI				            (0x4003000C)
#define TIMER0_INT_STATUS			        (0x40030010)

#define TIMER1_LOAD_COUNT_ADDR			    (0x40030014)
#define TIMER1_CURENT_VALUE_ADDR		    (0X40030018)
#define TIMER1_CONTROL_REG			        (0X4003001C)
#define TIMER1_EOI				            (0x40030020)
#define TIMER1_INT_STATUS			        (0x40030024)

#define TIMER2_LOAD_COUNT_ADDR			    (0x40030028)
#define TIMER2_CURENT_VALUE_ADDR		    (0X4003002C)
#define TIMER2_CONTROL_REG			        (0X40030030)
#define TIMER2_EOI				            (0x40030034)
#define TIMER2_INT_STATUS			        (0x40030038)


#define TIMERS_INT_STATUS			        (0x400300A0)
#define TIMERS_EOI				            (0x400300A4)
#define TIMERS_RAW_INT_STATUS			    (0x400300A8)
#define TIMERS_COMP_VERSION			        (0x400300AC)

#define CORE_TIMER_CORET_CSR_REG            (0xE000E010)
#define CORE_TIMER_CORET_RVR_REG            (0xE000E014)
#define CORE_TIMER_CORET_CVR_REG            (0xE000E018)
#define CORE_TIMER_CORET_CALIB_REG          (0xE000E01C)


#define TIMER_ON_ENABLE                     (0x01)
#define TIMER_ON_DISABLE                    (0x00)

/*
#define TIMER1_PWM_CTL				        (0x400300B0)
#define TIMER1_PWM0_DUTY_CYCLE			    (0x400300B4)
#define TIMER1_PWM1_DUTY_CYCLE			    (0x400300B8)
#define TIMER1_PWM2_DUTY_CYCLE			    (0x400300BC)
#define TIMER1_PWM3_DUTY_CYCLE			    (0x400300C0)
#define TIMER1_PWM4_DUTY_CYCLE			    (0x400300C4)
#define TIMER1_PWM5_DUTY_CYCLE			    (0x400300C8)
#define TIMER1_PWM_EN				        (0x400300CC)
#define TIMER0_PWM_CYCLE_DIV                (0x400300D0)
*/

/*
 * top_clk_sel
 * @ 0x08
 */
#define TOP_CLK_SEL_REG                     (0x40120108)
#define TOP_TIMER3_CLK_SEL_MASK             (0x30)
#define TOP_TIMER3_CLK_SEL_SHIFT            (4)
#define TOP_TIMER2_CLK_SEL_MASK             (0xC)
#define TOP_TIMER2_CLK_SEL_SHIFT            (2)
#define TOP_TIMER1_CLK_SEL_MASK             (0x3)
#define TOP_TIMER1_CLK_SEL_SHIFT            (0)

#define TIMER_CLK_SEL						(0x40120108)

//function define
#define TIMER_EN_ENABLE				        (0x01)//BIT[0]
#define TIMER_EN_DISABLE			        (0x00)

#define EX_RELOAD_EN_ENABLE			        (0x02)//BIT[1]
#define EX_RELOAD_EN_DISABLE			    (0x00)


#define TIMER_INPUT_MASK_ENABLE		        (0x04)//BIT[2]
#define TIMER_INPUT_MASK_DISABLE     		(0x00)


#define TIMEOUT     						(20000)//160
#define TIME0OUT     						(0xAAAA)
#define TIME1OUT     						(0xCCCC)
#define TIME2OUT     						(0xFFFF)

enum timer_e
{
    TIMER0,
    TIMER1,
    TIMER2
};

enum timer_clock_e
{
    TIMER_CLOCK_16M     = 0,
    TIMER_CLOCK_8M      = 1,
    TIMER_CLOCK_4M      = 2,
    TIMER_CLOCK_2M      = 3
};
void timer0_init(uint8_t clk_sel,uint16_t timer_load_count);
void timer1_init(uint8_t clk_sel,uint16_t timer_load_count);
void timer2_init(uint8_t clk_sel,uint16_t timer_load_count);
void core_timer_init(uint32_t timer_load_count);    //24bits
void core_timer_set_int(BOOL on_off);
void core_timer_enable();
void core_timer_disable();
uint32_t    get_core_timer_value();

void timer_clk_config(uint8_t timer, uint8_t clk_sel);


#endif  // #ifndef   __TIMER_H_
