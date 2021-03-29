/* ---------------------------------------------------------------
** File name:		pwm.h
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

#ifndef   __PWM_H_
#define   __PWM_H_

/* Includes -----------------------------------------------------*/
#include "type.h"
#include "timer.h"

/* Macro --------------------------------------------------------*/

typedef enum pwm_channel
{
    PWM_CHANNEL_0   = 0,
    PWM_CHANNEL_1   = 1,
    PWM_CHANNEL_2   = 2,
    PWM_CHANNEL_3   = 3,
    PWM_CHANNEL_4   = 4,
    PWM_CHANNEL_5   = 5,

	PWM_CHANNEL_MAX = PWM_CHANNEL_5,
} pwm_channel_t;

typedef enum pwm_cycle_div
{
    PWM_CYCLE_DIV_2 = 1,
    PWM_CYCLE_DIV_4 = 2,
    PWM_CYCLE_DIV_8 = 3
} pwm_cycle_div_t;

#define PWM_DEAD_ZONE_WIDTH_MAX         (0x3F)

#define PWM0_EN                         (0x01)
#define PWM1_EN                         (0x02)
#define PWM2_EN                         (0x04)
#define PWM3_EN                         (0x08)
#define PWM4_EN                         (0x10)
#define PWM5_EN                         (0x20)

#define PWM0_RVSE_EN                    (0x01)
#define PWM1_RVSE_EN                    (0x02)
#define PWM2_RVSE_EN                    (0x04)
#define PWM3_RVSE_EN                    (0x08)
#define PWM4_RVSE_EN                    (0x10)
#define PWM5_RVSE_EN                    (0x20)


/* Registers ----------------------------------------------------*/

#define TIMER0_LOAD_COUNT               (0x40030000)
//#define TIMER0_CONTROL_REG              (0x40030008)

#define TIMER0_PWM_CONTROL              (0x400300B0)
#define TIMER0_PWM0_DUTY_CYCLE          (0x400300B4)
#define TIMER0_PWM1_DUTY_CYCLE          (0x400300B8)
#define TIMER0_PWM2_DUTY_CYCLE          (0x400300BC)
#define TIMER0_PWM3_DUTY_CYCLE          (0x400300C0)
#define TIMER0_PWM4_DUTY_CYCLE          (0x400300C4)
#define TIMER0_PWM5_DUTY_CYCLE          (0x400300C8)
#define TIMER0_PWN_EN                   (0x400300CC)

#define TIMER0_PWM_CYCLE_DIV            (0x400300D0)

#define TIMER0_PWM_BOUND_EN_REG         (0x400300D4)
#define PWM_BOUND_EN_0_MASK             (0x01)
#define PWM_BOUND_EN_0_SHIFT            (0x00)
#define PWM_BOUND_EN_1_MASK             (0x02)
#define PWM_BOUND_EN_1_SHIFT            (0x01)
#define PWM_BOUND_EN_2_MASK             (0x04)
#define PWM_BOUND_EN_2_SHIFT            (0x02)
#define PWM_BOUND_EN_3_MASK             (0x08)
#define PWM_BOUND_EN_3_SHIFT            (0x03)
#define PWM_BOUND_EN_4_MASK             (0x10)
#define PWM_BOUND_EN_4_SHIFT            (0x04)
#define PWM_BOUND_EN_5_MASK             (0x20)
#define PWM_BOUND_EN_5_SHIFT            (0x05)

#define TIMER0_PWM_BOUND_VAL_REG        (0x400300D8)
#define PWM_BOUND_VAL_MASK              (0x3F)
#define PWM_BOUND_VAL_SHIFT             (0x00)


/* Functions prototype ------------------------------------------*/

void 		pwm_init( void );
void 		pwm_set_cycle( uint32_t timer_count);
void 		pwm_enable( uint8_t enable );
void 		pwm_disable( uint8_t enable );
void 		pwm_set_duty(pwm_channel_t pwm_channel, uint32_t duty_cycle);
uint32_t 	pwm_get_duty(pwm_channel_t pwm_channel);
error_t 	pwm_set_cycle_div(pwm_channel_t pwm_channel,pwm_cycle_div_t cycle_div);
uint8_t 	pwm_get_cycle_div( pwm_channel_t pwm_channel);
void 		pwm_set_reverse_en(uint8_t enable);
void        pwm_set_dz(pwm_channel_t pwm_channel, uint8_t dead_zone_width);
uint8_t 	pwm_get_dz_status();
uint8_t 	pwm_get_dz_width();
#endif  // #ifndef   __PWM_H_
