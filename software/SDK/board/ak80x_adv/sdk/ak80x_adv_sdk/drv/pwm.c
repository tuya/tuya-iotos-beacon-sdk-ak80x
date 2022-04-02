/* ---------------------------------------------------------------
** File name:		pwm.c
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


/* Includes -----------------------------------------------------*/
#include "config.h"
#include "sys.h"
#include "pwm.h"

/* Macro --------------------------------------------------------*/

/* Global Variables ---------------------------------------------*/

/* Extern -------------------------------------------------------*/

/* Functions ----------------------------------------------------*/


void pwm_init()
{
    uint32_t    reg_val = 0;

    /* Reset pwm module */
    sys_soft_reset(TIMER0_SOFT_RESET_MASK);

    /* Disable all PWM and clear some registers */
    write_reg( TIMER0_PWN_EN, 0 );
    write_reg( TIMER0_PWM_CYCLE_DIV, 0 );

    /* Diable all complement */
    write_reg( TIMER0_PWM_CONTROL, 0);

    /* Diable dead zone in all channel */
    write_reg( TIMER0_PWM_BOUND_EN_REG, 0 );

    /* Timer0 clock enable */
    reg_val = read_reg( TOP_CLK_CTRL_REG );
    reg_val = reg_val | CLK_TIMER0_EN_MASK;
    write_reg( TOP_CLK_CTRL_REG, reg_val );
    if(chip_verson_check())
    {
        reg_val = read_reg(0x40120108);
        reg_val &= 0xFFFFFFFC;
        reg_val |= 0x1;
        write_reg(0x40120108, reg_val);
    }

}

void pwm_set_cycle( uint32_t timer_count)
{
    write_reg( TIMER0_LOAD_COUNT, timer_count);
}

void pwm_set_port_mux(uint8_t enable, uint8_t function)
{
	for(int i =0;i<6;i++)
	{
		if(enable &(0x1<<i))
		{
			sys_set_port_mux((PAD_GPIO_03 + (i << 2)), function);
		}	
	}
}

void pwm_enable( uint8_t enable )
{
    pwm_set_port_mux(enable, PAD_MUX_FUNCTION_1); //set this port mux as pwm function
    write_reg( TIMER0_CONTROL_REG, 3);  // Timer0 enable
    write_reg( TIMER0_PWN_EN, enable);  // pwm output enable
}

void pwm_disable( uint8_t enable )
{
    uint32_t reg_val = read_reg(TIMER0_PWN_EN);

    reg_val &= ~enable;
    write_reg( TIMER0_PWN_EN, enable);
}

void pwm_set_duty(pwm_channel_t pwm_channel, uint32_t duty_cycle)
{
    uint32_t reg_val = 0;

    /* If dead zone enabled, the duty cycle will minus dead zone width */
    reg_val = pwm_get_dz_status();
    reg_val &= 1 << (uint8_t)pwm_channel;
    if ( 0 != reg_val )
    {
        reg_val = pwm_get_dz_width();

        if ( duty_cycle <= (reg_val << 2 ) )
        {
            reg_val = 0;
        }
        else
        {
            reg_val = duty_cycle - (reg_val << 2);
        }
    }
    else
    {
        reg_val = duty_cycle;
    }

    write_reg( ( TIMER0_PWM0_DUTY_CYCLE + ((uint8_t)pwm_channel<<2)), reg_val);
}

uint32_t pwm_get_duty(pwm_channel_t pwm_channel)
{
    uint32_t reg_val = 0;

	if(pwm_channel > PWM_CHANNEL_MAX)
	{
    	return 0x0;
	}

	reg_val = read_reg( TIMER0_PWM0_DUTY_CYCLE + ((uint8_t)pwm_channel<<2));

    return reg_val;
}

error_t pwm_set_cycle_div(pwm_channel_t pwm_channel,pwm_cycle_div_t cycle_div)
{
    uint32_t    reg_val = 0;

    /* Judge the parameter */
    reg_val = read_reg(TIMER0_LOAD_COUNT);
    reg_val = reg_val % (1 << (uint8_t)cycle_div);
	
    if (0 != reg_val )
    {
        return ERR_NOT_SUPPORTED;
    }

	if(pwm_channel > PWM_CHANNEL_MAX)
	{
		return ERR_NOT_SUPPORTED;
	}

    reg_val = read_reg(TIMER0_PWM_CYCLE_DIV);

	reg_val = (reg_val&(~(0x03<<(pwm_channel<<1)))) | (cycle_div<<(pwm_channel<<1));
	write_reg(TIMER0_PWM_CYCLE_DIV, reg_val);

    return ERR_NO_ERROR;
}

uint8_t pwm_get_cycle_div( pwm_channel_t pwm_channel)
{
    uint32_t reg_val = read_reg( TIMER0_PWM_CYCLE_DIV );

    reg_val = ( reg_val >> ((uint8_t)pwm_channel)<<1) & 0x03;

    return reg_val;
}

void pwm_set_reverse_en(uint8_t pwm_x_cmpl_enable)
{
    uint32_t reg_val = 0;

    reg_val = read_reg(TIMER0_PWM_CONTROL);

    reg_val = ( reg_val & 0xC0 ) | pwm_x_cmpl_enable;
    write_reg(TIMER0_PWM_CONTROL, reg_val);
}

void pwm_set_dz(uint8_t dead_zone_enable, uint8_t dead_zone_width)
{
    uint32_t reg_val = 0;

    /* Config dead zone width. Max value is 0x3F */
    if ( dead_zone_width > PWM_DEAD_ZONE_WIDTH_MAX )
    {
        reg_val = PWM_DEAD_ZONE_WIDTH_MAX;
    }
    else
    {
        reg_val = dead_zone_width;
    }
    write_reg( TIMER0_PWM_BOUND_VAL_REG, reg_val );

    /* Enable dead zone */
    write_reg( TIMER0_PWM_BOUND_EN_REG, dead_zone_enable );
}

uint8_t pwm_get_dz_status()
{
    uint32_t reg_val = 0;

    reg_val = read_reg( TIMER0_PWM_BOUND_EN_REG );

    return (uint8_t)reg_val;
}

uint8_t pwm_get_dz_width()
{
    uint32_t reg_val = 0;

    reg_val = read_reg( TIMER0_PWM_BOUND_VAL_REG );

    return (uint8_t)reg_val;
}

