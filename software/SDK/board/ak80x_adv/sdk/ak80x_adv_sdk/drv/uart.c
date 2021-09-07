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
#ifdef  DEBUG
bool        uart_inited;
#endif //#ifdef  DEBUG
/* Functions ----------------------------------------------------*/

#ifdef UART_CHERRY
static const uint32_t uart_baud[5] =
{
	115200,
	57600,
	38400,
	19200,
	9600
};


void uart_init()
{
    uint32_t    reg_val = 0;

    /* Set Port Mux as Uart*/
#ifdef  DEBUG
    sys_set_port_mux(PAD_GPIO_00, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_01, PAD_MUX_FUNCTION_2 | 3);

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

    /* Baud rate */
    write_reg(UART_BAUD_DIV_REG, UART_DIV);

    /* Enable Rx, Tx & Rx interrupt */
    reg_val = UART_RX_EN_MASK | UART_TX_EN_MASK | UART_RX_INT_EN_MASK;
    write_reg(UART_CTRL_REG, reg_val);

#ifdef	DEBUG
    uart_inited = TRUE;
#endif
}

void uart_set_baudrate( uart_baud_rate_t baud_rate_index)
{
    uint32_t    reg_val = 0;

	reg_val = UART_CLK / uart_baud[ (uint8_t)baud_rate_index ];
	write_reg(UART_BAUD_DIV_REG, reg_val);
}

void uart_set_recv_int( uint8_t enable_recv_int )
{
    uint32_t    reg_val = 0;

    reg_val = read_reg(UART_CTRL_REG);

    if ( TRUE == enable_recv_int )
    {
        reg_val = reg_val | UART_RX_INT_EN_MASK;
    }
    else
    {
        reg_val = reg_val & (~UART_RX_INT_EN_MASK);
    }

    write_reg(UART_CTRL_REG, reg_val);
}

uint8_t uart_get_rx_int_status()
{
    uint8_t reg_val = 0;

    reg_val = read_reg(UART_INT_REG);
    return ( reg_val & UART_RX_INT_MASK ) >> UART_RX_INT_SHIFT;
}

uint8_t uart_get_rx_status()
{
    uint8_t reg_val = 0;

    reg_val = read_reg(UART_STATE_REG);
    return ( reg_val & UART_RX_BUF_FULL_MASK ) >> UART_RX_BUF_FULL_SHIFT ;
}

void uart_putc( uint8_t c)
{
    uint32_t    reg_val = 0;

    /* If TX buffer is empty */
    while ((read_reg(UART_STATE_REG) & UART_TX_BUF_FULL_MASK));
    write_reg(BB_UART_DATA_REG, c);

    /* Clear Tx interrupt */
    //write_reg(UART_INT_REG, UART_TX_INT_MASK);
}

uint8_t uart_getc()
{
    uint8_t    reg_val = 0;

    reg_val = read_reg(BB_UART_DATA_REG);

    return reg_val;
}

ATTRIBUTE_ISR ATTRIBUTE_WEAK void uart_isr()
{
	/* Test code */
    uint8_t aa = uart_getc();

    uart_putc(aa + 1);          // temp test code.   
    
#ifdef  UART_CHERRY
    write_reg(UART_INT_REG, UART_RX_INT_MASK | UART_TX_INT_MASK);
#endif
}

#else	//#ifdef UART_CHERRY

#if 0
#if 0
static const uint8_t uart_divisor[4][5] =
{
    {1,  2,  3,  7,  13},   // CPU_CLOCK_2M
    {2,  4,  6, 13,  26},   // CPU_CLOCK_4M
    {4,  9, 13, 26,  52},   // CPU_CLOCK_8M
    {9, 17, 26, 52, 104},   // CPU_CLOCK_16M
};
#else
static const uint16_t uart_divisor[4][9] =
{
    {0, 0, 1,  2,  3,  7,  13,  26,  52},   // CPU_CLOCK_2M
    {0, 1, 2,  5,  7, 13,  26,  52, 104},   // CPU_CLOCK_4M
    {0, 1, 5,  9, 13, 26,  52, 104, 209},   // CPU_CLOCK_8M
    {1, 2, 9, 18, 26, 52, 104, 209, 417},   // CPU_CLOCK_16M
};

#endif
#endif

void uart_init(uart_baud_rate_t baud_rate_index)
{
    uint32_t    reg_val = 0;

    /* Set Port Mux as Uart*/
//#ifdef  DEBUG
    //sys_set_port_mux(PAD_GPIO_00, PAD_MUX_FUNCTION_2 | 3);
    //sys_set_port_mux(PAD_GPIO_01, PAD_MUX_FUNCTION_2 | 3);

    sys_set_port_mux(PAD_GPIO_14, PAD_MUX_FUNCTION_3 | 2);  //RXD
    sys_set_port_mux(PAD_GPIO_15, PAD_MUX_FUNCTION_3);      //TXD
//#else
//    sys_set_port_mux(PAD_GPIO_00, PAD_MUX_FUNCTION_1);      //RXD
//    sys_set_port_mux(PAD_GPIO_01, PAD_MUX_FUNCTION_1);      //TXD
//#endif

    /* Clock enable */
    sys_set_module_clock( CLK_UART_EN_MASK, ON );

    /* Reset UART module */
    sys_soft_reset(UART_SOFT_RESET_MASK);

    /* Disable RX interrupt */
    reg_val = read_reg(UART_USR_ADDR);
    reg_val = reg_val & (~7);
    write_reg(UART_IER_ADDR, reg_val);

    /* Is busy? */
    while ( read_reg(UART_USR_ADDR) & 1 );

    /* Baud rate */
#if 0
    write_reg(UART_LCR_ADDR, LCR_SEL_DLR | LCR_WORD_SIZE_8 | LCR_STOP_BIT_1);
    write_reg(UART_DLH_ADDR, ( UART_DIVISOR_115200 & 0xFF00 ) >> 8 );
    write_reg(UART_DLL_ADDR, ( UART_DIVISOR_115200 & 0x00FF ) );
#else
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

	/*divisor = (freq / baudrate + 7) >> 4,freq = 2000000<<reg_val*/
#if 0
    reg_val = (((2000000<<reg_val) / baud_rate_index) + 7)>>4;
#else
	uint32_t quotient = 0x0;
	reg_val = (2000000<<reg_val);

	/*convert division to subtraction,add by flege 20200704*/
	//for divisor of uint32_t,it can reduce more space than division.example:115200 reduce 76bytes
	//but for divisor of uint16_t,it will add a little more space.example:9600 add 16bytes
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
#endif
	
    write_reg(UART_LCR_ADDR, LCR_SEL_DLR | LCR_WORD_SIZE_8 | LCR_STOP_BIT_1);
    write_reg(UART_DLH_ADDR, ( reg_val & 0xFF00 ) >> 8 );
    write_reg(UART_DLL_ADDR, ( reg_val & 0x00FF ) );
#endif

    /* clear DLAB and set operation mode with none parity,
     * 8 bits and 1 stop bit
     */
    write_reg(UART_LCR_ADDR, LCR_PARITY_NONE | LCR_WORD_SIZE_8 | LCR_STOP_BIT_1);

    /* Enable RX interrupt */
    write_reg(UART_IER_ADDR, 1);
#ifdef	DEBUG
    uart_inited = TRUE;
#endif
}

#if 0 //table look-up scheme
void uart_set_baudrate( uart_baud_rate_t baud_rate_index)
{
    uint32_t    reg_val = 0;

	reg_val = read_reg( TOP_MODULE_MODE_REG );
	reg_val &= TOP_CPU_CLK_SEL_MASK;
	reg_val = reg_val >> TOP_CPU_CLK_SEL_SHIFT;

	reg_val = uart_divisor[reg_val][baud_rate_index];

    /* Baud rate */
    write_reg(UART_LCR_ADDR, LCR_SEL_DLR | LCR_WORD_SIZE_8 | LCR_STOP_BIT_1);
    write_reg(UART_DLH_ADDR, ( reg_val & 0xFF00 ) >> 8 );
    write_reg(UART_DLL_ADDR, ( reg_val & 0x00FF ) );

    /* clear DLAB and set operation mode with none parity,
     8 bits and 1 stop bit */
    write_reg(UART_LCR_ADDR, LCR_PARITY_NONE | LCR_WORD_SIZE_8 | LCR_STOP_BIT_1);
}
#else //calculate
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
#endif

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

    uart_putc(aa + 1);
}
#endif	//#ifdef UART_CHERRY

