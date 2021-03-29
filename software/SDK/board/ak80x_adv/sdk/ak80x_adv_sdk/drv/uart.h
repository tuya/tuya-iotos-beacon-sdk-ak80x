/* ---------------------------------------------------------------
** File name:		uart.h
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

#ifndef     __UART_H_
#define     __UART_H_


/* Includes -----------------------------------------------------*/
#include "type.h"
#include "config.h"

/* Macro --------------------------------------------------------*/

#ifdef UART_CHERRY

#define	UART_BASE_ADDR						(0x40000000)

/*
 * uart_data
 * @ 0x00
 */
#define BB_UART_DATA_REG                    (UART_BASE_ADDR + 0x00)
#define UART_DATA_MASK                      (0xFF)
#define UART_DATA_SHIFT                     (0)

/*
 * uart_state
 * @ 0x04
 */
#define UART_STATE_REG                      (UART_BASE_ADDR + 0x04)
#define UART_RX_BUF_OVERRUN_MASK            (0x8)
#define UART_RX_BUF_OVERRUN_SHIFT           (3)
#define UART_TX_BUF_OVERRUN_MASK            (0x4)
#define UART_TX_BUF_OVERRUN_SHIFT           (2)
#define UART_RX_BUF_FULL_MASK               (0x2)
#define UART_RX_BUF_FULL_SHIFT              (1)
#define UART_TX_BUF_FULL_MASK               (0x1)
#define UART_TX_BUF_FULL_SHIFT              (0)

/*
 * uart_ctrl
 * @ 0x08
 */
#define UART_CTRL_REG                       (UART_BASE_ADDR + 0x08)
#define UART_TX_HIGHSPEED_MODE_MASK         (0x40)
#define UART_TX_HIGHSPEED_MODE_SHIFT        (6)
#define UART_RX_OVERRUN_INT_EN_MASK         (0x20)
#define UART_RX_OVERRUN_INT_EN_SHIFT        (5)
#define UART_TX_OVERRUN_INT_EN_MASK         (0x10)
#define UART_TX_OVERRUN_INT_EN_SHIFT        (4)
#define UART_RX_INT_EN_MASK                 (0x8)
#define UART_RX_INT_EN_SHIFT                (3)
#define UART_TX_INT_EN_MASK                 (0x4)
#define UART_TX_INT_EN_SHIFT                (2)
#define UART_RX_EN_MASK                     (0x2)
#define UART_RX_EN_SHIFT                    (1)
#define UART_TX_EN_MASK                     (0x1)
#define UART_TX_EN_SHIFT                    (0)

/*
 * uart_int
 * @ 0x0c
 */
#define UART_INT_REG                        (UART_BASE_ADDR + 0x0c)
#define UART_RX_OVERRUN_INT_MASK            (0x8)
#define UART_RX_OVERRUN_INT_SHIFT           (3)
#define UART_TX_OVERRUN_INT_MASK            (0x4)
#define UART_TX_OVERRUN_INT_SHIFT           (2)
#define UART_RX_INT_MASK                    (0x2)
#define UART_RX_INT_SHIFT                   (1)
#define UART_TX_INT_MASK                    (0x1)
#define UART_TX_INT_SHIFT                   (0)

/*
 * uart_baud_div
 * @ 0x0c
 */
#define UART_BAUD_DIV_REG                   (UART_BASE_ADDR + 0x10)
#define UART_BAUD_DIV_MASK                  (0xFFFFF)
#define UART_BAUD_DIV_SHIFT                 (0)


#define	UART_CLK							(24000000)
#define	UART_DIV							(UART_CLK / 115200)
//#define	UART_DIV							(UART_CLK / 750000)
//#define	UART_DIV							(UART_CLK / 500000)
//#define	UART_DIV							(UART_CLK / 2000000)

#else 	// #ifdef UART_CHERRY

/* UART registers addr definition */
#define UART_RBR_ADDR                   (0x40000000)	/* Receive Buffer Register (32 bits, R) 	*/
#define UART_THR_ADDR                   (0x40000000)	/* Transmit Holding Register (32 bits, W) 	*/
#define UART_DLL_ADDR                   (0x40000000)	/* Divisor Latch(Low)  (32 bits, R/W) 		*/
#define UART_IER_ADDR                   (0x40000004)	/* Interrupt Enable Register (32 bits, R/W) */
#define UART_DLH_ADDR                   (0x40000004)	/* Divisor Latch(High) (32 bits, R/W) 		*/
#define UART_IIR_ADDR                   (0x40000008)	/* Interrupt Identity Register (32 bits, R) */
#define UART_FCR_ADDR                   (0x40000008)	/* fifo Countrol Register (32 bits, W) 		*/
#define UART_LCR_ADDR                   (0x4000000C)	/* Line Control Register (32 bits, R/W) 	*/
#define UART_MCR_ADDR                   (0x40000010)	/* Modem Control Register (32 bits, W) 		*/
#define UART_LSR_ADDR                   (0x40000014)	/* Line Status Register (32 bits, R) 		*/
#define UART_MSR_ADDR                   (0x40000018)	/* Modem Status Register (32 bits, R/W) 	*/
#define UART_SCR_ADDR                   (0x4000001C)	/* Scratchpad Register*/
#define UART_LPDLL_ADDR                 (0x40000020)
#define UART_LPDLH_ADDR                 (0x40000024)
#define UART_FAR_ADDR                   (0x40000070)
#define UART_TFR_ADDR                   (0x40000074)
#define UART_RFW_ADDR                   (0x40000078)
#define UART_USR_ADDR                   (0x4000007C)	/* UART Status Register (32 bits, R/W) 		*/
#define UART_TFL_ADDR                   (0x40000080)
#define UART_RFL_ADDR                   (0x40000084)
#define UART_SRR_ADDR                   (0x40000088)
#define UART_SRTS_ADDR                  (0x4000008C)
#define UART_SBCR_ADDR                  (0x40000090)
#define UART_SDMAM_ADDR                 (0x40000094)
#define UART_SFE_ADDR                   (0x40000098)
#define UART_SRT_ADDR                   (0x4000009C)
#define UART_STET_ADDR                  (0x400000A0)
#define UART_HTX_ADDR                   (0x400000A4)
#define UART_DMASA_ADDR                 (0x400000A8)
#define UART_CPR_ADDR                   (0x400000F4)
#define UART_UCV_ADDR                   (0x400000F8)
#define UART_CTR_ADDR                   (0x400000FC)


/*   divisor = ( freq / baudrate + 7 ) >> 4  freq = 16000000 */
#define UART_DIVISOR_115200             (9)
#define UART_DIVISOR_57600              (17)
#define UART_DIVISOR_38400              (26)
#define UART_DIVISOR_19200              (52)
#define UART_DIVISOR_9600               (104)

#define LSR_TRANS_EMPTY                 (0x40)
#define LSR_THR_EMPTP                   (0x20)
#define LSR_BREAK_INT                   (0x10)
#define LSR_FRAME_ERROR                 (0x08)
#define LSR_PARITY_ERROR                (0x04)
#define LSR_OVERRUN_ERROR               (0x02)
#define LSR_DATA_READY                  (0x01)


#define LCR_SEL_DLR                     (0x80)
#define LCR_BC_ENABLE                   (0x40)
#define LCR_STICK_PARITY_ENABLE         (0x20)
#define LCR_PARITY_ENABLE               (0x08)
#define LCR_PARITY_NONE                 (0x00) /* none parity */
#define LCR_PARITY_EVEN                 (0x18) /*even parity */
#define LCR_PARITY_ODD                  (0x08) /*odd parity */
#define LCR_WORD_SIZE_5                 (0x00)
#define LCR_WORD_SIZE_6                 (0x01)
#define LCR_WORD_SIZE_7                 (0x02)
#define LCR_WORD_SIZE_8                 (0x03)
#define LCR_STOP_BIT_1                  (0x00)  /* 1 stop bit */
#define LCR_STOP_BIT_1_5                (0x04)  /* 1.5 stop bit */
#define LCR_STOP_BIT_2                  (0x04)  /* 2 stop bit */

#define UART_IER_ERBFI_INT_MASK         (0x01)
#define UART_IER_ERBFI_QUERY_MASK       (0xFE)
#define UART_IER_ETBEI_INT_MASK         (0x02)
#define UART_IER_ETBEI_QUERY_MASK       (0xFD)
#define UART_IER_PTIME_MASK             (0x80)
#define UART_IER_ELSI_MASK              (0x04)
#define UART_IER_DLH_LCR7_MASK          (0x40)

#define IIR_IID_MODST                   (0x00)
#define IIR_IID_NO_INT                  (0x01)
#define IIR_IID_THR_EMPTY               (0x02)
#define IIR_IID_RX_AVAILABLE            (0x04)
#define IIR_IID_RX_RXLINE_ST            (0x06)
#define IIR_IID_DETECT_BUSY             (0x07)
#define IIR_IID_CHAR_TIMEOUT            (0x0c)

#define UART_LSR_DATA_READY_MASK        (0x01)

#endif	//#ifdef UART_CHERRY

typedef enum uart_baud_rate_e
{
    UART_BAUDRATE_1000000   = 1000000,
    UART_BAUDRATE_500000    = 500000,
    UART_BAUDRATE_115200    = 115200,
    UART_BAUDRATE_57600     = 57600,
    UART_BAUDRATE_38400     = 38400,
    UART_BAUDRATE_19200     = 19200,
    UART_BAUDRATE_9600      = 9600,
    UART_BAUDRATE_4800      = 4800,
    UART_BAUDRATE_2400      = 2400,
}uart_baud_rate_t;


/* Global Variables ---------------------------------------------*/

/* Extern -------------------------------------------------------*/

/* Function prototype -------------------------------------------*/

void 	uart_init( uart_baud_rate_t baud_rate_index);
void 	uart_set_baudrate( uart_baud_rate_t baud_rate_index);
void 	uart_set_recv_int( uint8_t enable_recv_int );
uint8_t uart_get_rx_int_status(void);
uint8_t uart_get_rx_status(void);
void 	uart_putc( uint8_t c);
uint8_t uart_getc(void);

#endif  // __UART_H_

