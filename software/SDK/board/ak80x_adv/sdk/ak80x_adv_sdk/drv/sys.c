/* ---------------------------------------------------------------
** File name:		sys.c
** Brief:
** COPYRIGHT(C) ZXW 2020
** ---------------------------------------------------------------
** Module Information:
**
** Revision History ----------------------------------------------
** ---------------------------------------------------------------
**	Date        Version     Action              Author
** ---------------------------------------------------------------
**	2019-6-13   1.0         Created
**
** ---------------------------------------------------------------
*/
#include "config.h"
#include "sys.h"


inline void sys_set_port_mux(uint32_t port, uint8_t function)
{
    //*((volatile uint32_t *)(port)) = function;
    write_reg(port, function);
}

void sys_set_module_clock( sys_clock_ctrl_t clock_sel, uint8_t on_off)
{
    uint32_t    reg_val = 0;

    reg_val = read_reg( TOP_CLK_CTRL_REG );

    if ( ON == on_off )
    {
        reg_val |= clock_sel;
    }
    else// if ( OFF == on_off )//saving 4 bytes
    {
        reg_val &= ~clock_sel;
    }

    write_reg(TOP_CLK_CTRL_REG, reg_val);
}

void sys_soft_reset( uint8_t module )
{
    uint32_t    reg_val = 0;

    reg_val = read_reg( TOP_SOFT_RESET_REG );

    reg_val |= module;
    write_reg(TOP_SOFT_RESET_REG, reg_val);
    reg_val &= ~module;
    write_reg(TOP_SOFT_RESET_REG, reg_val);
}
uint8_t chip_verson_check(void)
{
    uint32_t    reg_val = 0;
	uint8_t     ret =0;
    reg_val = read_reg(0x6000001C);
	if(0x55 == reg_val)
	{
		ret =1;
	}
    return ret;
}

void sys_set_clock( cpu_clock_sel_t clock )
{
    uint32_t    reg_val = 0;
    uint8_t tmp_val = clock;

    reg_val = read_reg( TOP_MODULE_MODE_REG );
    reg_val &= ~TOP_CPU_CLK_SEL_MASK;
    //reg_val |= clock << TOP_CPU_CLK_SEL_SHIFT;
    if(chip_verson_check())
    {
        if(0 == clock)
        {
            tmp_val = 1;
        }
        else
        {
            tmp_val = 2;
        }
        
        if(!(reg_val & TOP_WATCH_DOG_ON_MASK))
        {
            reg_val |= TOP_WATCH_DOG_ON_MASK;
        }
        else
        {
            reg_val &= ~TOP_WATCH_DOG_ON_MASK;
        }

    }

    reg_val |= tmp_val << TOP_CPU_CLK_SEL_SHIFT;

    write_reg( TOP_MODULE_MODE_REG, reg_val );
}

void sys_init()
{
	write_reg(0x40120134,				0xD8);		

	write_reg(TOP_VDD_VCO_REG_0,        0x12);      
    write_reg(TOP_VDD_VCO_REG_1,        0x37);      // For PLL drift while high temperature.
    write_reg(TOP_CTRL_ADC_DELAY_REG,    0x7);      

    write_reg(CORE_CTRL_VCO_MH_ADDR,    0x53);

    write_reg(ABB_LL,   0x76);
    write_reg(ABB_MH,   0x83);

    write_reg(TOP_CTRL_GAIN_DAC_INI_ADDR,0x57);

#ifdef ENABLE_JTAG_ON_GPIO9_11
    sys_set_port_mux(PAD_GPIO_09, PAD_MUX_FUNCTION_1|3);//RXD
    sys_set_port_mux(PAD_GPIO_10, PAD_MUX_FUNCTION_1|3);//TXD
    sys_set_port_mux(PAD_GPIO_11, PAD_MUX_FUNCTION_1|3);//TXD
    //sys_set_port_mux(PAD_GPIO_08, PAD_MUX_FUNCTION_7);//TXD
#elif defined (ENABLE_JTAG_ON_GPIO4_6)
    sys_set_port_mux(PAD_GPIO_04, PAD_MUX_FUNCTION_3|3);//RXD
    sys_set_port_mux(PAD_GPIO_05, PAD_MUX_FUNCTION_3|3);//TXD
    sys_set_port_mux(PAD_GPIO_06, PAD_MUX_FUNCTION_3|3);//TXD
#endif
}

void sys_sleep_down(void)
{
	write_reg(0x40120134,0xD0);
    write_reg(TOP_POWER_CTRL_REG, 0x1);//sleep
}

