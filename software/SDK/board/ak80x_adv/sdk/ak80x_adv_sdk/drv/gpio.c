/* ---------------------------------------------------------------
** File name:		gpio.c
** Brief:
** COPYRIGHT(C) ZXW 2020
** ---------------------------------------------------------------
** Module Information:
**
** Revision History ----------------------------------------------
** ---------------------------------------------------------------
**  Date        Version     Action                  Author
** ---------------------------------------------------------------
**  2019-5-27   1.0         Created
**
** ---------------------------------------------------------------
*/


/* Includes -----------------------------------------------------*/
#include "type.h"
#include "sys.h"
#include "gpio.h"
#include "int.h"
/* Macro --------------------------------------------------------*/

/* Global Variables ---------------------------------------------*/

/* Extern -------------------------------------------------------*/

/* Functions ----------------------------------------------------*/

void gpio_set_port_direction(  gpio_dir_t direction)
{
    if ( GPIO_INPUT == direction )
    {
        write_reg(GPIO_SWPORT_A_DDR, 0x0);
    }
    else
    {
        write_reg(GPIO_SWPORT_A_DDR, 0x0FFFFF);
    }
}

void gpio_set_bit_direction( uint32_t bit, gpio_dir_t direction)
{
    uint32_t reg = 0;

    reg = read_reg(GPIO_SWPORT_A_DDR);

    if ( GPIO_INPUT == direction )
    {
        reg = reg & (~bit);
    }
    else
    {
        reg = reg | bit;
    }

    write_reg(GPIO_SWPORT_A_DDR, reg);
}

void gpio_set_port( uint32_t data)
{
    write_reg(GPIO_SWPORT_A_DR, data);
}

uint32_t gpio_get_port()
{
    return read_reg(GPIO_EXT_PORT_A);
}

void gpio_set_bit(uint32_t bit, uint8_t data)
{
    uint32_t reg = 0;

    reg = read_reg(GPIO_SWPORT_A_DR);

    if ( data )
    {
        reg = reg | bit;
    }
    else
    {
        reg = reg & (~bit);
    }

    write_reg(GPIO_SWPORT_A_DR, reg);
}

uint32_t gpio_get_bits(uint32_t bit)
{
    volatile uint32_t reg = 0;

    reg = read_reg( GPIO_EXT_PORT_A) ;
    reg &= bit;

    return reg;
}
void gpio_double_edge_trigger_irq(uint32_t bits)
{
    uint32_t    reg = 0;

    reg = read_reg(GPIO_EDGE_INT_REG);
    reg |= bits;

    write_reg(GPIO_EDGE_INT_REG, reg);
}
void gpio_enable_irq(uint32_t bits)
{
    uint32_t    reg = 0;

    reg = read_reg( GPIO_INT_EN );
    reg |= bits;
    write_reg( GPIO_INT_EN, reg );
}

void gpio_disable_irq(uint32_t bits)
{
    uint32_t    reg = 0;

    reg = read_reg( GPIO_INT_EN );
    reg &= ~bits;
    write_reg( GPIO_INT_EN, reg );
}

void gpio_mask_irq(uint32_t bits)
{
    uint32_t    reg = 0;

    reg = read_reg( GPIO_INT_MASK );
    reg |= bits;

    write_reg( GPIO_INT_MASK, reg );
}

void gpio_unmask_irq(uint32_t bits)
{
    uint32_t    reg = 0;

    reg = read_reg( GPIO_INT_MASK );
    reg &= ~bits;
    write_reg( GPIO_INT_MASK, reg );
}

void gpio_clear_irq( uint32_t bits)
{
    write_reg( GPIO_EOI, bits );
}

error_t gpio_set_irq_type( gpio_irq_type_t      type, uint32_t bits)
{
    error_t    ret     = ERR_NO_ERROR;
    uint32_t        reg_val = 0;

    reg_val = read_reg( GPIO_INT_TYPE_LEVEL );

    switch ( type )
    {
    case GPIO_LEVEL_SENSITIVE:
        reg_val &= ~bits;
        break;
    case GPIO_EDGE_SENSITIVE :
        reg_val |= bits;
        break;

    default:
        ret = ERR_NOT_SUPPORTED;
        break;
    }

    write_reg( GPIO_INT_TYPE_LEVEL, reg_val);

    return ret;
}

void set_int_polarity(gpio_polarity_type_t polarity, uint32_t bits)
{
    uint32_t    reg_val = 0;
    reg_val = read_reg( GPIO_INT_POLORITY );
    if(GPIO_HIGH_LEVEL_OR_RISING_EDGE == polarity)
    {
        reg_val |= bits;
    }
    else
    {
        reg_val &= ~bits;
    }
	
	write_reg( GPIO_INT_POLORITY, reg_val );
}

ATTRIBUTE_ISR ATTRIBUTE_WEAK void gpio_isr()
{
#ifdef MODULE_TEST
    if(read_reg(GPIO_INT_STATUS))
    {
        gpio_clear_irq(0x7F1FF);
        gpio_set_bit(BIT19, 0x1);
    }
#else
	
#endif
}



