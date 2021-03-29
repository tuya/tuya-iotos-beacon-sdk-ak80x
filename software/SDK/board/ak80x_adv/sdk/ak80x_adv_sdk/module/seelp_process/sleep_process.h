/* ---------------------------------------------------------------
** File name:		seelp_process.h
** Brief:
** COPYRIGHT(C) 
** ---------------------------------------------------------------
** Module Information:
**
** Revision History ----------------------------------------------
** ---------------------------------------------------------------
**  Date        Version     Action                  Author
** ---------------------------------------------------------------
**  2020-9-23   1.0         Created
**
** ---------------------------------------------------------------
*/
#ifndef _SEELP_PROCESS_H_
#define _SEELP_PROCESS_H_
#include "app_cfg.h"
void sys_reset_state_check(void);
void sys_sleep_process(void);
void clear_sleep_count(void);
#endif