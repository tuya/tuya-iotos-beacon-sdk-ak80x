/* ---------------------------------------------------------------
** File name:		int.h
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

#ifndef   __INT_H_
#define   __INT_H_

/* Includes -----------------------------------------------------*/

/* Macro --------------------------------------------------------*/

/* Registers ----------------------------------------------------*/

#define INT_VIC_ISER_REG        (0xE000E100)  // int enable set
#define INT_VIC_ICER_REG        (0xE000E180)  // int enable clear

#define INT_VIC_ISPR_REG        (0xE000E200)  // int pending set
#define INT_VIC_ICPR_REG        (0xE000E280)  // int pending clear

#define INT_VIC_IABR_REG        (0xE000E300)  // int ACK status

#define INT_VIC_ISR_REG         (0xE000EC00)  // int status

#define INT_VIC_IPR_REG			(0xE000E400)  // int priority

#define INT_VIC_VIC_IPTR_REG	(0xE000EC04)  // int priority threshold


#define INT_WDT_EN_MASK         (0x00000001)
#define INT_TIM0_EN_MASK        (0x00000002)
#define INT_TIM1_EN_MASK        (0x00000004)
#define INT_TIM2_EN_MASK        (0x00000008)
#define INT_UART_EN_MASK        (0x00000010)
#define INT_GPIO_EN_MASK        (0x00000020)
#define INT_RX_EN_MASK          (0x00000040)
#define INT_TX_EN_MASK          (0x00000080)
#define INT_TCCR_EN_MASK        (0x00000100)
#define INT_WT_EN_MASK          (0x00000200)
#define INT_LV_EN_MASK          (0x00000400)
#define INT_TICK_TIMER_EN_MASK  (0x00000800)
#define INT_LV_L_EN_MASK        (0x00001000)
#define INT_LV_H_EN_MASK        (0x00002000)
#define INT_ALL_EN_MASK         (0x00003FFF)


enum en_irqn
{
	//IPR0
	IRQN_WDT				= 0,
	IRQN_TIME0				= 1,
	IRQN_TIME1				= 2,
	IRQN_TIME2				= 3,

	//IPR1
	IRQN_UART				= 4,
	IRQN_GPIO				= 5,
	IRQN_RX					= 6,
	IRQN_TX					= 7,

	//IPR2
	IRQN_TCCR				= 8,
	IRQN_WAKEUP_TIMER		= 9,
	IRQN_LVD				= 10,
	IRQN_TICK_TIMER			= 11,
	
	IRQN_MAX				= 11,
};

enum en_irq_pri
{
	//BIT[7:6]
	IRQ_PRI_0				= 0x00,
	IRQ_PRI_1				= 0x40,
	IRQ_PRI_2				= 0x80,
	IRQ_PRI_3				= 0xC0,
};

/* Functions prototype ------------------------------------------*/

void int_init(void);
void int_enable_irq(uint32_t irq_bit);
void int_disable_irq(uint32_t irq_bit);
void int_set_priority(enum en_irqn irqn,enum en_irq_pri priority);
void int_config_global_irq(uint8_t en_global_irq);
void GLOBAL_INT_DISABLE(void);
void GLOBAL_INT_RESTORE(void);

#endif  // #ifndef   __INT_H_
