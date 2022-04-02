/* ---------------------------------------------------------------
** File name:		uart.c
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
#include "config.h"
#include "uart.h"
#include "sys.h"

/* Macro --------------------------------------------------------*/

/* Global Variables ---------------------------------------------*/

/* Extern -------------------------------------------------------*/

/* Functions ----------------------------------------------------*/

void uart_init(uart_baud_rate_t baud_rate_index)
{
    uint32_t    reg_val = 0;

    /* Set Port Mux as Uart*/
#ifdef  DEBUG
    sys_set_port_mux(PAD_GPIO_14, PAD_MUX_FUNCTION_3 | 2);  //RXD
    sys_set_port_mux(PAD_GPIO_15, PAD_MUX_FUNCTION_3);      //TXD
#else
    sys_set_port_mux(PAD_GPIO_00, PAD_MUX_FUNCTION_1);      //RXD
    sys_set_port_mux(PAD_GPIO_01, PAD_MUX_FUNCTION_1);      //TXD
#endif

    /* Clock enable */
    sys_set_module_clock( CLK_UART_EN_MASK, ON );

    /* Reset UART module */
    sys_soft_reset(UART_SOFT_RESET_MASK);

    /* Disable RX interrupt */
    reg_val = read_reg(UART_USR_ADDR);
    reg_val = reg_val & (~7);
    write_reg(UART_IER_ADDR, reg_val);

    /* Is busy? */
    while ( read_reg(UART_USR_ADDR) & 1 )
	{
		
	}

    /* Baud rate */
	//get sys clk
	reg_val = read_reg( TOP_MODULE_MODE_REG );
	reg_val &= TOP_CPU_CLK_SEL_MASK;
	reg_val = reg_val >> TOP_CPU_CLK_SEL_SHIFT;
        
    if(chip_verson_check())//AK801_M
    {
        if(2 == reg_val)
        {
            reg_val =3;
        }
        else if(1 == reg_val)
        {
            reg_val =0;
            baud_rate_index = UART_BAUDRATE_9600;
        }
        else
        {
            return;
        }

    }

	uint32_t quotient = 0x0;
	reg_val = (2000000<<reg_val);

	while(1)
	{
		if(reg_val >= baud_rate_index)
		{
			quotient++;
			reg_val -= baud_rate_index;
		}
		else
		{
			break;
		}
	}

	reg_val = (quotient + 7)>>4;
	
    write_reg(UART_LCR_ADDR, LCR_SEL_DLR | LCR_WORD_SIZE_8 | LCR_STOP_BIT_1);
    write_reg(UART_DLH_ADDR, ( reg_val & 0xFF00 ) >> 8 );
    write_reg(UART_DLL_ADDR, ( reg_val & 0x00FF ) );

    /* clear DLAB and set operation mode with none parity,
     * 8 bits and 1 stop bit
     */
    write_reg(UART_LCR_ADDR, LCR_PARITY_NONE | LCR_WORD_SIZE_8 | LCR_STOP_BIT_1);

    /* Enable RX interrupt */
    write_reg(UART_IER_ADDR, 1);
}

void uart_set_baudrate( uart_baud_rate_t baud_rate_index)
{
    uint32_t  reg_val = 0;

	//get sys clk
	reg_val = read_reg( TOP_MODULE_MODE_REG );
	reg_val &= TOP_CPU_CLK_SEL_MASK;
	reg_val = reg_val >> TOP_CPU_CLK_SEL_SHIFT;

    if(chip_verson_check())
    {
        if(2 == reg_val)
        {
            reg_val =3;
        }
        else if(1 == reg_val)
        {
            reg_val =0;
        }
        else
        {
            return;
        }

    }

    /*divisor = ( freq / baudrate + 7 ) >> 4 */
    reg_val = ((2000000<<reg_val) / baud_rate_index + 7)>>4;//freq = 2000000<<reg_val

    /* Baud rate */
    write_reg(UART_LCR_ADDR, LCR_SEL_DLR | LCR_WORD_SIZE_8 | LCR_STOP_BIT_1);
    write_reg(UART_DLH_ADDR, ( reg_val & 0xFF00 ) >> 8 );
    write_reg(UART_DLL_ADDR, ( reg_val & 0x00FF ) );

    /* clear DLAB and set operation mode with none parity,
     8 bits and 1 stop bit */
    write_reg(UART_LCR_ADDR, LCR_PARITY_NONE | LCR_WORD_SIZE_8 | LCR_STOP_BIT_1);
}

void uart_set_recv_int( uint8_t enable_recv_int )
{
    uint32_t    reg_val = 0;

    reg_val = read_reg(UART_IER_ADDR);

    if(TRUE == enable_recv_int)
    {
        reg_val = reg_val | UART_IER_ERBFI_INT_MASK;
    }
    else
    {
        reg_val = reg_val & (~UART_IER_ERBFI_INT_MASK);
    }

    write_reg(UART_IER_ADDR, reg_val);
}

uint8_t uart_get_rx_int_status()
{
    uint8_t reg_val = 0;

    reg_val = read_reg(UART_IIR_ADDR);
    return ( reg_val & IIR_IID_RX_AVAILABLE );
}

uint8_t uart_get_rx_status()
{
    uint8_t reg_val = 0;

    reg_val = read_reg(UART_LSR_ADDR);
    return ( reg_val & UART_LSR_DATA_READY_MASK );
}

void uart_putc( uint8_t c)
{
    uint32_t reg_val = 0;
    uint16_t wait_coun=0;

    while( !(reg_val & LSR_TRANS_EMPTY) )
    {
        reg_val = *((volatile uint32_t *)(UART_LSR_ADDR));
        wait_coun++;
        if(wait_coun>16000)
        {
            wait_coun =0;
            break;
        }
    };

    *((volatile uint32_t *)(UART_THR_ADDR)) = c;
}

uint8_t uart_getc()
{
    uint8_t    reg_val = 0;

    reg_val = read_reg(UART_RBR_ADDR);

    return reg_val;
}

ATTRIBUTE_ISR ATTRIBUTE_WEAK void uart_isr()
{
    uint8_t aa = uart_getc();

    uart_putc(aa);
}

