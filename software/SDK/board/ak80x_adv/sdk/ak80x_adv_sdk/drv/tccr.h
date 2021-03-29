/* ---------------------------------------------------------------
** File name:		tccr.h
** Brief:
** COPYRIGHT(C) ZXW 2020
** ---------------------------------------------------------------
** Module Information:
**
** Revision History ----------------------------------------------
** ---------------------------------------------------------------
**  Date        Version     Action                  Author
** ---------------------------------------------------------------
**  2019-6-11   1.0         Created
**
** ---------------------------------------------------------------
*/

#ifndef   __TCCR_H_
#define   __TCCR_H_


//tccr addr define
#define TCCR_GPIO_PORT						(0x40120000 + 0x1C)//GPIO7

#define TCCR_BASE_ADDR						(0x40010000)
#define TCCR_EN_ADDR						(TCCR_BASE_ADDR)
#define TCCR_TIMER_RELOAD_ADDR				(TCCR_BASE_ADDR + 0x04)
#define TCCR_CC0C_ADDR						(TCCR_BASE_ADDR + 0x18)//ÉÏÉýÑØtimer countÖµ
#define TCCR_CC1C_ADDR						(TCCR_BASE_ADDR + 0x1C)//ÏÂ½µÑØtimer countÖµ
#define TCCR_INT_EN_ADDR					(TCCR_BASE_ADDR + 0x28)
#define TCCR_INT_MASK_ADDR					(TCCR_BASE_ADDR + 0x2C)
#define TCCR_INT_CLEAR_ADDR					(TCCR_BASE_ADDR + 0x30)
#define TCCR_INT_STATUS_ADDR				(TCCR_BASE_ADDR + 0x34)


//function define
#define TCCR_EN_ENABLE						(0x1)//BIT[0]
#define TCCR_EN_DISABLE						(0x0)

#define TCCR_EX_RELOAD_EN_ENABLE			(0x2)//BIT[1]
#define TCCR_EX_RELOAD_EN_DISABLE			(0x0)

#define TIMER_RELOAD_SEL_STOP				(0x0)//BIT[3:2]
#define TIMER_RELOAD_SEL_SOFT				(0x4)
#define TIMER_RELOAD_SEL_AUTO				(0x8)
#define TIMER_RELOAD_SEL_FALL_EDG			(0xC)

#define TIMER_CLK_SEL_STOP					(0x0)//BIT[5:4]
#define TIMER_CLK_SEL_NORMAL				(0x10)
#define TIMER_CLK_SEL_ADD_FALL_EGD			(0x20)
#define TIMER_CLK_SEL_ADD_HIGH				(0x30)

#define TCCR_INPUT_REVERSE					(0x40)//BIT[6]


enum en_tccr_int_type
{
	TCCR_INT_NEGEDGE_EN 					= 0x01,
	TCCR_INT_TIMEOUT_EN 					= 0x02,
	TCCR_INT_ALL_EN 						= 0x03,
};


#define CHECK_AC_TIMEOUT					(0xFFFFFF - 0x4E200)//16M/2, unit 0.0625us??/2 = 0.125us, 60ms:0x75300,40ms:0x4E200


void tccr_init(uint32_t check_count,enum en_tccr_int_type enable_int_type);

#endif
