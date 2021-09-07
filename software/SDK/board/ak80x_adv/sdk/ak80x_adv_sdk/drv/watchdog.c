/* ---------------------------------------------------------------
** File name:		watchdog.c
** Brief:
** COPYRIGHT(C) ZXW 2020
** ---------------------------------------------------------------
** Module Information:
**
** Revision History ----------------------------------------------
** ---------------------------------------------------------------
**  Date        Version     Action                  Author
** ---------------------------------------------------------------
**  2019-6-20  1.0         Created
**
** ---------------------------------------------------------------
*/


/* Includes -----------------------------------------------------*/
#include "type.h"
#include "config.h"
#include "watchdog.h"
#include "int.h"
#include "sys.h"
/* Macro --------------------------------------------------------*/

/* Global Variables ---------------------------------------------*/

/* Extern -------------------------------------------------------*/

/* Functions ----------------------------------------------------*/

void set_wdg_comp_value(uint32_t value)
{
    write_reg(WATCHDOG_COMP_VALUE_H_REG, (value >> 16));
    write_reg(WATCHDOG_COMP_VALUE_M_REG, (value >> 8));
    write_reg(WATCHDOG_COMP_VALUE_L_REG, (value &0xFF));
}
void set_wdg_comp_value_vld()
{
    write_reg(WATCHDOG_COMP_VLD_REG, 0x1);
}

void wdg_feed_dog()
{
    write_reg(WATCHDOG_FEED_DOG_REG, 0x1);
}

void wdg_int_clear()
{
    write_reg(WATCHDOG_NTERRUPT_CLEAR_REG, 0x1);

}
uint32_t read_int_state()
{
    uint32_t reg = 0;
    reg = read_reg(WATCHDOG_STATE_REG);
    return reg;
}
void watchdog_enable(void)
{
	uint32_t reg =0;
	reg = read_reg(WATCHDOG_MODULE_SELECT_REG);

    if(chip_verson_check())//Ak801_M
    {
        reg &= ~WDG_EN;//WDG_EN;
    }
    else
    {
        reg |= WDG_EN;//WDG_EN;
    }

	write_reg(WATCHDOG_MODULE_SELECT_REG,reg);
}
void watchdog_disable(void)
{
	uint32_t reg =0;
	reg = read_reg(WATCHDOG_MODULE_SELECT_REG);
    if(chip_verson_check())//AK801_M
    {
        reg |= WDG_EN;//WDG_EN;
    }
    else
    {
        reg &= ~WDG_EN;//WDG_EN;
    }
	write_reg(WATCHDOG_MODULE_SELECT_REG,reg);

}
void watchdog_init(uint32_t nms)
{
    uint32_t tmp_low13_bit =0;
    uint32_t reg = 0;
    reg = nms<<5;//T = reg/32000 (s)

    if(chip_verson_check())
    {
        /*Watchdog reset value high 11bit negate*/
        tmp_low13_bit = (reg&0x1FFF);
        
        reg = (~((reg>>13)&0x7FF))<<13;
        reg |=tmp_low13_bit;
    }

    set_wdg_comp_value(reg);
    set_wdg_comp_value_vld();
    watchdog_enable();
}

ATTRIBUTE_ISR ATTRIBUTE_WEAK void wdt_isr()
{
    if(WDG_INT_FLAG & read_reg(WATCHDOG_STATE_REG))
    {
        wdg_int_clear();
        wdg_feed_dog();
    }
}
