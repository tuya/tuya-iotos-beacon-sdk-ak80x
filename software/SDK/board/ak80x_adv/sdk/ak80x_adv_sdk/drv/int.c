/* ---------------------------------------------------------------
** File name:		int.c
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
#include "type.h"
#include "sys.h"
#include "int.h"
#include "uart.h"

/* Macro --------------------------------------------------------*/

/* Global Variables ---------------------------------------------*/

/* Extern -------------------------------------------------------*/

/* Functions ----------------------------------------------------*/

void int_init()
{
    uint32_t reg_val = 0;

    /* Enable interrupts */
    //reg_val = 0xFFFFFFFF;
    reg_val = 0x3F;
    write_reg( INT_VIC_ISER_REG, reg_val );

    /* Clear all int status */
    write_reg(INT_VIC_IABR_REG, 0);

    asm("psrset ee, ie");
}

void int_enable_irq(uint32_t irq_bit)
{
    uint32_t reg_val = 0;

    reg_val = read_reg(INT_VIC_ISER_REG);

    reg_val |= irq_bit;

    write_reg(INT_VIC_ISER_REG, reg_val);

    /* Clear all int status */
    write_reg(INT_VIC_ICPR_REG, INT_ALL_EN_MASK); // 9 int sources
    write_reg(INT_VIC_IABR_REG, ~INT_ALL_EN_MASK);

    asm("psrset ee, ie");
}

void int_disable_irq(uint32_t irq_bit)
{
    /* Clear all int status */
    write_reg(INT_VIC_IABR_REG, 0);

    /* Clear enable corresponding int*/
    write_reg(INT_VIC_ICER_REG, irq_bit);
}

void int_set_priority(enum en_irqn irqn,enum en_irq_pri priority)
{
	uint32_t reg_val = 0;
	
	if(irqn <= IRQN_MAX)
	{
		reg_val = read_reg(INT_VIC_IPR_REG + ((irqn>>2)<<2));

		//IRQN:6
		//ADDR:INT_VIC_IPR_REG + 0x04
		//PRIT:1 ->> 2
		//DATA:0x00400000 ->> 0x00800000
		reg_val &= ~(0xC0<<((irqn%4)<<3));
		reg_val |= (priority<<((irqn%4)<<3));

		write_reg(INT_VIC_IPR_REG + ((irqn>>2)<<2), reg_val);
	}
}

void int_config_global_irq(uint8_t en_global_irq)
{
    if ( en_global_irq )
    {
        asm("psrset ee, ie");
    }
    else
    {
        asm("psrclr ee, ie");
    }
}
uint16_t enabled_int_bits = 0;

inline void GLOBAL_INT_DISABLE()
{
#if 0
	asm("psrclr ee, ie");
#else

    /*When closing the global interrupt, the lvd_l interrupt must be reserved*/

    enabled_int_bits = read_reg(INT_VIC_ISER_REG);
    enabled_int_bits &=~(INT_LV_L_EN_MASK|INT_LV_EN_MASK|INT_LV_H_EN_MASK);

    int_disable_irq(enabled_int_bits);
#endif
}

inline void GLOBAL_INT_RESTORE()
{
#if 0
    asm("psrset ee, ie");
#else
    int_enable_irq(enabled_int_bits);
#endif

}
