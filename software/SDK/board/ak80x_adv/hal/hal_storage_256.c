/*************************************************************************
	> File Name: hal_storage_256.c
	> Author: 
	> Mail: 
	> Created Time: Tue 26 Jan 2021 12:26:42 CST
 ************************************************************************/

#include "hal_storage_256.h"

u8 hal_storage_256_init(void){

}

u8 hal_storage_256_read_bytes(u16 offset, u8* pdata, u8 len){
    if(offset % E2PROM_PAGE_SIZE != 0)return 1;//error,  0=success 
    uint8_t i,page_num = len >> 3;
	uint8_t left_size = len;
    for(i=0;i<page_num;i++){
		left_size -= E2PROM_PAGE_SIZE;
        if(e2prom_read_bytes(offset+i*E2PROM_PAGE_SIZE,&pdata[i*E2PROM_PAGE_SIZE],E2PROM_PAGE_SIZE,E2PROM_ACCESS_TRY_MAX_TIMES) != 0){
            return 1;
        }
    }
    
    if(left_size !=0 && e2prom_read_bytes(offset+i*E2PROM_PAGE_SIZE,&pdata[i*E2PROM_PAGE_SIZE],left_size,E2PROM_ACCESS_TRY_MAX_TIMES) != 0){
        return 1;
    }

    return 0; 
}

u8 hal_storage_256_write_bytes(u16 offset, u8* pdata, u8 len){
    if(offset % E2PROM_PAGE_SIZE != 0)return 1;//error,  0=success 
    uint8_t i,page_num = len >> 3;
	uint8_t left_size = len;
    for(i=0;i<page_num;i++){
		left_size -= E2PROM_PAGE_SIZE;
        if(e2prom_write_bytes(offset+i*E2PROM_PAGE_SIZE,&pdata[i*E2PROM_PAGE_SIZE],E2PROM_PAGE_SIZE,E2PROM_ACCESS_TRY_MAX_TIMES) != 0){
            return 1;
        }
    }
    
    if(left_size !=0 && e2prom_write_bytes(offset+i*E2PROM_PAGE_SIZE,&pdata[i*E2PROM_PAGE_SIZE],left_size,E2PROM_ACCESS_TRY_MAX_TIMES) != 0){
        return 1;
    }

    return 0; 
}



