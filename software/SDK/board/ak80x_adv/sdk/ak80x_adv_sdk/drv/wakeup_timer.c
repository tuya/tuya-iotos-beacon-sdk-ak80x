/* ---------------------------------------------------------------
** File name:		wakeup.c
** Brief:
** COPYRIGHT(C) ZXW 2020
** ---------------------------------------------------------------
** Module Information:
**
** Revision History ----------------------------------------------
** ---------------------------------------------------------------
**  Date        Version     Action                  Author
** ---------------------------------------------------------------
**  2019-6-27   1.0         Created
**
** ---------------------------------------------------------------
*/


/* Includes -----------------------------------------------------*/
#include "sys.h"
#include "type.h"
#include "wakeup_timer.h"
#include "gpio.h"
#include "delay.h"
/* Macro --------------------------------------------------------*/

/* Global Variables ---------------------------------------------*/

/* Extern -------------------------------------------------------*/

/* Functions ----------------------------------------------------*/

void set_debounce(uint8_t deb_ms)
{
    uint32_t reg_val = 0;
    reg_val = ((deb_ms<<5) << 8)&WT_DEB_VAL;//ms -> 32K,modified by flege,20200602
    write_reg( WT_CTL, reg_val);
}
void wakeup_timer_enable(void)
{
    uint32_t reg_val = read_reg(WT_CTL);
    reg_val |= WT_EN;
    write_reg( WT_CTL, reg_val);
}

void wakeup_timer_disable(void)
{
    #if 0
    uint32_t reg_val = read_reg(WT_CTL);
    reg_val &= ~WT_EN;
    write_reg( WT_CTL, reg_val);
    #else
    write_reg( WT_CTL, 0);
    #endif
}

void set_wt_compare_val(uint8_t wt_compare_val)
{
    write_reg( WT_COMPARE, wt_compare_val);
}

void wt_input_enable(uint32_t reg_bits)
{
    write_reg(WT_INPUT_EN, reg_bits);
}

void wt_input_disable()
{
    write_reg(WT_INPUT_EN, 0x00);
}

void wt_pol_set(uint32_t reg_bits)
{
    write_reg(WT_POL, reg_bits);
}

void wt_int_enable()
{
    write_reg(WT_INT_EN, 0x01);
}

void wt_int_disable()
{
    write_reg(WT_INT_EN, 0x00);
}

void wt_int_mask_enable(void)
{
    write_reg(WT_INT_MASK, 0x01);
}

void wt_int_mask_disable(void)
{
    write_reg(WT_INT_MASK, 0x00);
}

void wt_int_clear(void)
{
    write_reg(WT_INT_CLR, 0x1);
}

void wt_soft_reset(void)
{
    write_reg(TOP_SOFT_RESET2,WT_SOFT_RESET);
	delay_us(10);
	write_reg(TOP_SOFT_RESET2,0x0);
}

/*****************************************************************************************
 * @function init wakeup timer
 *
 * @param[in] reg_bits     	enabled wakeup io bits
 * @param[in] pull_en      	pull enable/disable,0x01:enable;0x00:disable
 * @param[in] wakeup_level 	wakeup level,hight/low,0x01:high;0x00:low
 * @param[in] debounce   	debounce
 *****************************************************************************************/
void wakeup_timer_init(uint32_t reg_bits,uint8_t pull_en,uint8_t wakeup_level,uint8_t debounce_ms)
{
	uint8_t loopback_pull_en = 0x03;//default enable loopback and disable pull
	uint32_t wakeup_level_bits = 0x00;//default low level wakeup

	//bug fixed,flege add at 20200602
	wt_soft_reset();
	
    wt_input_enable(reg_bits);//INPUT_EN, in[bit]
    set_wt_compare_val(EVENT_COUNT);//COUNTER
    wt_int_enable();//INT_EN

	/*set wakeup level,hight/low level*/
    if(wakeup_level)
    {
    	wakeup_level_bits = reg_bits;	
    }
	
	wt_pol_set(wakeup_level_bits);

	/*set pull enable*/
	loopback_pull_en ^= pull_en;

	/*set debounce time*/
    set_debounce(debounce_ms);//DEBOUNCE
    
    /*wakeup io fuction config and io pullup*/
    for(int8_t i = 0; i < 20; i++)
    {
        if(reg_bits&(0x01<<i))
        {
            sys_set_port_mux((PAD_MUX_BASE_ADDR + (i << 2)), PAD_MUX_FUNCTION_6 | loopback_pull_en);
        }
    }

    wakeup_timer_enable(); //enable wakeup timer module	
}

void clear_sleep_status(void)
{
    write_reg(TOP_POWER_CTRL_REG, 0x0); //sleep
}