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
#include "lvd.h"
#include "int.h"
#include "sys.h"
/* Macro --------------------------------------------------------*/

/* Global Variables ---------------------------------------------*/

/* Extern -------------------------------------------------------*/

/* Functions ----------------------------------------------------*/

void lvd_powerup_enable()
{
	uint32_t reg_val =0;
	reg_val = read_reg(TOP_CTRL_REG);
	reg_val |= LVD_PWR_CTRL_EN;//BIT[1];
	write_reg(TOP_CTRL_REG,reg_val);
}
void lvd_powerup_disable()
{
	uint32_t reg_val =0;
	reg_val = read_reg(TOP_CTRL_REG);
	reg_val &= ~LVD_PWR_CTRL_EN;//BIT[1];
	write_reg(TOP_CTRL_REG,reg_val);
}

void lvd_module_select(void)
{
	uint32_t reg_val =0;
	reg_val = read_reg(MODULE_MODE_REG);
	reg_val |= LVD_EN;
    
    if(chip_verson_check())
    {
        if(!(reg_val & TOP_WATCH_DOG_ON_MASK))
        {
            reg_val |= TOP_WATCH_DOG_ON_MASK;
        }
        else
        {
            reg_val &= ~TOP_WATCH_DOG_ON_MASK;
        }

    }
	write_reg(MODULE_MODE_REG,reg_val);
}
void int_type_cfg(uint32_t irq_bit)
{
	uint32_t reg_val =0;
	reg_val = read_reg(INT_TYPE_CFG_L);
	reg_val |=(irq_bit & 0x00FF);
	write_reg(INT_TYPE_CFG_L,reg_val);
	
	reg_val = read_reg(INT_TYPE_CFG_H);
	reg_val |=(irq_bit>>8);
	write_reg(INT_TYPE_CFG_H,reg_val);
}
void lvd_threshold_set(lvd_threshold_t lvd_thr)
{
	uint32_t reg_val =0;
    reg_val = read_reg(TOP_VDD_VCO_REG_1);
    reg_val &= ~LVD_THR_MASK;
    reg_val |= lvd_thr<<LVD_THR_SHIFT;
    write_reg(TOP_VDD_VCO_REG_1, reg_val);
}
void lvd_init()
{
	lvd_powerup_enable();
	int_enable_irq(INT_LV_L_EN_MASK);
	lvd_module_select();
	int_type_cfg(INT_LV_L_EN_MASK);
}

ATTRIBUTE_ISR ATTRIBUTE_WEAK void lvd_isr()
{
#ifdef MODULE_TEST
	if(!(0x01 & read_reg(LVD_ULL_STATE)))
	{
		write_reg(INT_VIC_ICPR_REG, INT_LV_EN_MASK);	
		write_reg(RETENTION_MEMEORY_ADDR, 0x5A);
		write_reg(TOP_POWER_CTRL_REG, 0x01);//sleep down.
	}
#else
	
#endif
}
ATTRIBUTE_ISR ATTRIBUTE_WEAK void lvd_l_isr()
{
#ifdef MODULE_TEST
	if(!(0x01 & read_reg(LVD_ULL_STATE)))
	{
		write_reg(INT_VIC_ICPR_REG, INT_LV_L_EN_MASK);	
		write_reg(RETENTION_MEMEORY_ADDR, 0x5A);
		write_reg(TOP_POWER_CTRL_REG, 0x01);//sleep down.
	}
#else
	
#endif
}
ATTRIBUTE_ISR ATTRIBUTE_WEAK void lvd_h_isr()
{
#ifdef MODULE_TEST
	if(!(0x01 & read_reg(LVD_ULL_STATE)))
	{
		write_reg(INT_VIC_ICPR_REG, INT_LV_H_EN_MASK);	
		write_reg(RETENTION_MEMEORY_ADDR, 0x5A);
		write_reg(TOP_POWER_CTRL_REG, 0x01);//sleep down.
	}
#else
	
#endif
}

