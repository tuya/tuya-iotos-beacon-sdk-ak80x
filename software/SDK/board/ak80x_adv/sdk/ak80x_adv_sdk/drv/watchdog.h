/* ---------------------------------------------------------------
** File name:		watchdog.h
** Brief:
** COPYRIGHT(C) ZXW 2020
** ---------------------------------------------------------------
** Module Information:
**
** Revision History ----------------------------------------------
** ---------------------------------------------------------------
**  Date        Version     Action                  Author
** ---------------------------------------------------------------
**  2019-6-20   1.0         Created
**
** ---------------------------------------------------------------
*/

#ifndef   __WATCHDOG_H_
#define   __WATCHDOG_H_


#define WDG_TIMEOUT		0xAAAA

#define WATCHDOG_COMP_VALUE_H_REG           (0x40120164)
#define WATCHDOG_COMP_VALUE_M_REG           (0x40120168)
#define WATCHDOG_COMP_VALUE_L_REG           (0x4012016C)

#define WATCHDOG_COMP_VLD_REG               (0x40120170)

#define WATCHDOG_FEED_DOG_REG               (0x40120174)
#define WATCHDOG_NTERRUPT_CLEAR_REG         (0x40120178)
#define WATCHDOG_STATE_REG                  (0x40120188)
#define WDG_RESET_FLAG                      (0x10)//BIT[4]
#define WDG_INT_FLAG                        (0x08)//BIT[3]
#define WDG_INT_en                          (0x04)//BIT[2]

#define WATCHDOG_MODULE_SELECT_REG          (0x4012018C)
#define WDG_EN                              (0x00000004)//BIT[2]

void set_wdg_comp_value(uint32_t value);
void set_wdg_comp_value_vld(void);
void wdg_feed_dog(void);
void wdg_int_clear(void);
uint32_t read_int_state(void);
void watchdog_enable();
void watchdog_disable();
void watchdog_init(uint32_t nms);
void wdt_isr(void);

#endif
