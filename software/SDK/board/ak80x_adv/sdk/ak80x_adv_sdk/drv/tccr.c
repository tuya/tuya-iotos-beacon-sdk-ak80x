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
#include "type.h"
#include "sys.h"
#include "tccr.h"
#include "int.h"
#include "lvd.h"

/* Macro --------------------------------------------------------*/

/* Global Variables ---------------------------------------------*/

/* Extern -------------------------------------------------------*/

/* Functions ----------------------------------------------------*/


void tccr_init(uint32_t check_count,enum en_tccr_int_type enable_int_type)
{
    uint32_t reg_val;

    //tccr clock enable
    reg_val = read_reg(TOP_CLK_CTRL_REG);
    reg_val |= CLK_TCCR_EN_MASK;
    write_reg(TOP_CLK_CTRL_REG, reg_val);
	
	write_reg(TCCR_EN_ADDR, 0x00);//disable tccr first

    //config pad mux as tccr function
    //write_reg(TCCR_GPIO_PORT, 0x33);//0x33 need check
    sys_set_port_mux(PAD_GPIO_07, PAD_MUX_FUNCTION_3 | 0x03);

    //timer reload
    write_reg(TCCR_TIMER_RELOAD_ADDR, check_count);//from 0 -> 0xFFFFFF,clk/2

    //When the TCCR is initialized, software force loads the RELOAD value
    write_reg(TCCR_EN_ADDR,TIMER_RELOAD_SEL_SOFT); 

    //tccr enable
    write_reg(TCCR_EN_ADDR, (TIMER_CLK_SEL_NORMAL | TIMER_RELOAD_SEL_FALL_EDG | TCCR_EX_RELOAD_EN_ENABLE | TCCR_EN_ENABLE));

    //int enable
    write_reg(TCCR_INT_EN_ADDR,enable_int_type);

    //int mask
    write_reg(TCCR_INT_MASK_ADDR, 0x0);

    //enable mcu gloabl irq
    int_enable_irq(INT_TCCR_EN_MASK);//priority default is max(0x0)
    int_set_priority(IRQN_TCCR,IRQ_PRI_0);
}

ATTRIBUTE_ISR ATTRIBUTE_WEAK void tccr_isr()
{
    /*
    __      ______
      |     |     |
      |_____|     |_________________________
      A     B     A       C
      A:irq,tccr timer reload,record current timer count as TCCR_CC1C_ADDR
      B:tccr record current timer count as TCCR_CC0C_ADDR
      C:irq, tccr timeout
    */
    
    uint32_t reg_val;
	
#ifdef MODULE_TEST
    uint32_t tccr_count0 = 0x0;
    uint32_t tccr_count1 = 0x0;

    //read irq status
    reg_val = read_reg(TCCR_INT_STATUS_ADDR);

    //clear tccr all int
    write_reg(TCCR_INT_CLEAR_ADDR, 0x03);

    //clear cpu irq status
    write_reg(INT_VIC_IABR_REG, 0x0);//clear all,need check!!!

    if((reg_val & 0x2) == 0x02) //tccr timeout irq
    {
        //this means, there is no ac,need sleep
    }

    if((reg_val & 0x1) == 0x01) //negedge irq
    {
        tccr_count0 = read_reg(TCCR_CC0C_ADDR);
        tccr_count1 = read_reg(TCCR_CC1C_ADDR);

        tccr_count1 = tccr_count1 - tccr_count0;//high time count
        tccr_count0 = tccr_count0 - CHECK_AC_TIMEOUT;//low time count
    }
#endif
}

