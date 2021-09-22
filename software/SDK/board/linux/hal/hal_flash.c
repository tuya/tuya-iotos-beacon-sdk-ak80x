/*************************************************************************
	> File Name: hal_flash.c
	> Author: 
	> Mail: 
	> Created Time: Thu 21 Feb 2019 14:57:38 CST
 ************************************************************************/
#include "hal_flash.h"

#define FLASH_BUF_SIZE      1000000
#define FLASH_SECTOR_SIZE   0x1000     
u8 flash_buf[FLASH_BUF_SIZE];

void hal_flash_init(void){
    memset(flash_buf,0xFF,FLASH_BUF_SIZE);
}

u8 hal_flash_erase_sector(u32 addr){

    memset(flash_buf+addr,0xFF,FLASH_SECTOR_SIZE);
    printf("erase: addr = %lld\r\n",addr);
    return 0;
}

u8 hal_flash_write_page(u32 addr, u32 len, const u8 *buf){
    memcpy(flash_buf+addr,buf,len); 
    printf("write: addr = %lld, len = %lld\r\n",addr,len);
    for(u32 i = 0;i < len;i++){
        printf("%02X ",buf[i]);
    }
    printf("\r\n");
    return 0;
}

u8 hal_flash_read_page(u32 addr, u32 len, u8 *buf){
    memcpy(buf,flash_buf+addr,len); 
    printf("read: addr = %lld, len = %lld\r\n",addr,len);
    return 0;
}
