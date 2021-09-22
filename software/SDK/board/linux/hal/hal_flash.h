/*************************************************************************
	> File Name: hal_linux_flash.h
	> Author: 
	> Mail: 
	> Created Time: Thu 21 Feb 2019 14:57:34 CST
 ************************************************************************/

#ifndef _HAL_FLASH_H
#define _HAL_FLASH_H

#include "hal_sys.h"

extern void hal_flash_init(void);
extern u8 hal_flash_erase_sector(u32 addr);
extern u8 hal_flash_write_page(u32 addr, u32 len, const u8 *buf);
extern u8 hal_flash_read_page(u32 addr, u32 len, u8 *buf);

#endif
