/* ---------------------------------------------------------------
** File name:		gpio.h
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

#ifndef     __GPIO_H_
#define     __GPIO_H_


/* Includes -----------------------------------------------------*/

/* Macro --------------------------------------------------------*/


/* GPIO registers addr definition */
// base address 0x40060000

#define GPIO_SWPORT_A_DR        (0x40060000)
#define GPIO_SWPORT_A_DDR       (0x40060004)
#define GPIO_SWPORT_A_CTL		(0x40060008)

#define GPIO_SWPORT_B_DR        (0x4006000C)
#define GPIO_SWPORT_B_DDR       (0x40060010)
#define GPIO_SWPORT_B_CTL		(0x40060014)
#define GPIO_SWPORT_C_DR        (0x40060018)
#define GPIO_SWPORT_C_DDR       (0x4006001C)
#define GPIO_SWPORT_C_CTL		(0x40060020)
#define GPIO_SWPORT_D_DR        (0x40060024)
#define GPIO_SWPORT_D_DDR       (0x40060028)
#define GPIO_SWPORT_D_CTL		(0x4006002C)

#define GPIO_INT_EN             (0x40060030)
#define GPIO_INT_MASK           (0x40060034)
#define GPIO_INT_TYPE_LEVEL     (0x40060038)
#define GPIO_INT_POLORITY		(0x4006003C)
#define GPIO_INT_STATUS			(0x40060040)
#define GPIO_RAW_INT_STATUS		(0x40060044)
#define GPIO_DEBOUNCE			(0x40060048)
#define GPIO_EOI                (0x4006004C)

#define GPIO_EXT_PORT_A			(0x40060050)
#define GPIO_EXT_PORT_B			(0x40060054)
#define GPIO_EXT_PORT_C			(0x40060058)
#define GPIO_EXT_PORT_D			(0x4006005C)

#define GPIO_IS_SYNC			(0x40060060)
#define GPIO_ID_CODE			(0x40060064)
#define GPIO_EDGE_INT_REG       (0x40060068)
#define GPIO_VER_ID_CODE		(0x4006006C)
#define GPIO_CONFIG_REG1		(0x40060074)
#define GPIO_CONFIG_REG2		(0x40060070)




typedef enum gpio_direction
{
    GPIO_INPUT      = 0,
    GPIO_OUTPUT     = 1

} gpio_dir_t;

typedef enum gpio_bits
{
    BIT0    = 0x00000001,
    BIT1    = 0x00000002,
    BIT2    = 0x00000004,
    BIT3    = 0x00000008,
    BIT4    = 0x00000010,
    BIT5    = 0x00000020,
    BIT6    = 0x00000040,
    BIT7    = 0x00000080,
    BIT8    = 0x00000100,
    BIT9    = 0x00000200,
    BIT10   = 0x00000400,
    BIT11   = 0x00000800,
    BIT12   = 0x00001000,
    BIT13   = 0x00002000,
    BIT14   = 0x00004000,
    BIT15   = 0x00008000,
    BIT16   = 0x00010000,
    BIT17   = 0x00020000,
    BIT18   = 0x00040000,
    BIT19   = 0x00080000

} gpio_bits_t;

typedef enum gpio_irq_type
{
    GPIO_LEVEL_SENSITIVE    = 0x0,
    GPIO_EDGE_SENSITIVE     = 0x1
} gpio_irq_type_t;

typedef enum gpio_irq_polarity_type
{
    GPIO_LOW_LEVEL_OR_FALLING_EDGE    = 0x0,
    GPIO_HIGH_LEVEL_OR_RISING_EDGE    = 0x1
} gpio_polarity_type_t;
/* Global Variables ---------------------------------------------*/

/* Extern -------------------------------------------------------*/

/* Function prototype -------------------------------------------*/

void gpio_set_port_direction(  gpio_dir_t direction);
void gpio_set_bit_direction( uint32_t bit, gpio_dir_t direction);
void gpio_set_port( uint32_t data);
void gpio_set_port( uint32_t data);
void gpio_set_bit(uint32_t bit, uint8_t data);
uint32_t gpio_get_bits(uint32_t bit);
void gpio_double_edge_trigger_irq(uint32_t bits);
void gpio_enable_irq(uint32_t bits);
void gpio_disable_irq(uint32_t bits);
void gpio_mask_irq(uint32_t bits);
void gpio_unmask_irq(uint32_t bits);
void gpio_clear_irq( uint32_t bits);
error_t gpio_set_irq_type(gpio_irq_type_t type, uint32_t bits);
void set_int_polarity(gpio_polarity_type_t polarity, uint32_t bits);
//__attribute__((isr)) void gpio_isr();




#endif  // __GPIO_H_

