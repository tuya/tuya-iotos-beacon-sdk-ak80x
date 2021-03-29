/* ---------------------------------------------------------------
** File name:		wakeup_timer.h
** Brief:
** COPYRIGHT(C) ZXW 2020
** ---------------------------------------------------------------
** Module Information:
**
** Revision History ----------------------------------------------
** ---------------------------------------------------------------
**  Date        Version     Action                  Author
** ---------------------------------------------------------------
**  2019-6-27   1.0         Created
**
** ---------------------------------------------------------------
*/

#ifndef   __WAKEUP_TIMER_H_
#define   __WAKEUP_TIMER_H_


#define DEBOUNCE_TIME	(0xA)
#define EVENT_COUNT		(0xA)

#define WAKWUP_TIMER_BASE_ADDR				(0x40100000)
#define WT_CTL 								(0x40100000)
#define WT_COMPARE							(0x40100004)
#define WT_COUNTER							(0x40100008)
#define WT_INPUT_EN							(0x4010000C)
#define WT_POL								(0x40100010)
#define WT_INT_STS							(0x40100020)
#define WT_INT_EN 							(0x40100024)
#define WT_INT_MASK 						(0x40100028)
#define WT_INT_CLR 							(0x4010002C)

#define TOP_SOFT_RESET2                     (0x40120110)
#define WT_SOFT_RESET                       0x01//BIT[0]
#define WT_DEB_VAL							0x000FFF00//0x0000//BIT[19:8]//bug fixed,flege add at 20200602
#define WT_EN								0x00001//enable wakeup_timer//BIT[0]

void set_debounce(uint8_t deb_ms);
void wakeup_timer_enable(void);
void wakeup_timer_disable();
void set_wt_compare_val(uint8_t wt_compare_val);
void wt_input_enable(uint32_t reg_bits);
void wt_input_disable();
void wt_pol_set(uint32_t reg_bits);
void wt_int_enable();
void wt_int_disable();
void wt_int_mask_enable(void);
void wt_int_mask_disable(void);
void wt_int_clear(void);
void wt_soft_reset(void);
void wakeup_timer_init(uint32_t reg_bits,uint8_t pull_en,uint8_t wakeup_level,uint8_t debounce_ms);
void clear_sleep_status(void);
#endif
