/*************************************************************************
	> File Name: hal_storage_256.h
	> Author: 
	> Mail: 
	> Created Time: Tue 26 Jan 2021 12:26:46 CST
    > 256 bytes fast storage(eeprom or flash ...)
 ************************************************************************/

#ifndef _HAL_STORAGE_256_H
#define _HAL_STORAGE_256_H

#include "hal_sys.h"

u8 hal_storage_256_init(void);
u8 hal_storage_256_read_bytes(u16 offset, u8* pdata, u8 len);
u8 hal_storage_256_write_bytes(u16 offset, u8* pdata, u8 len);
u8 hal_storage_256_read_all(void);
u8 hal_storage_256_write_all(u8 state);

#endif
