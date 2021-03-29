/****************************************************************************
@FILENAME:	e2prom_access.h
@FUNCTION:	handle e2prom access functon
@AUTHOR:	flege.zhang
@DATE:		2020.01.08
@COPYRIGHT(C) ZXW 2020
*****************************************************************************/

#ifndef _E2PROM_ACCESS__H_
#define _E2PROM_ACCESS__H_

#include "app_cfg.h"
#include "iic.h"


#define E2PROM_ACCESS_TRY_MAX_TIMES									(0x03)

#define E2PROM_PAGE_SIZE											(0x08)

/*E2PROM ADDR MANAGE*/
#define E2PROM_ADDR_BASE											(0x00)

#define E2PROM_ADDR_SETUP_INFO										(E2PROM_ADDR_BASE + 0x30)


void    e2prom_init(void);
uint8_t e2prom_write_bytes(uint16_t sub_addr, uint8_t* data, uint8_t data_len, uint8_t try_times);
uint8_t e2prom_read_bytes(uint16_t sub_addr, uint8_t* data, uint8_t data_len, uint8_t try_times);

#endif
