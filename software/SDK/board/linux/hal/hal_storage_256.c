/*************************************************************************
	> File Name: hal_storage_256.c
	> Author: 
	> Mail: 
	> Created Time: Tue 26 Jan 2021 12:26:42 CST
 ************************************************************************/

#include "hal_storage_256.h"

#define WRITE_TO_FILE(file_name,data,data_size) do{ \
   	FILE *p = fopen((file_name), "w"); \
	fwrite((data), (data_size), 1, p); \
    fclose(p);\
}while(0)

#define READ_FROM_FILE(file_name,data,data_size) do{ \
   	FILE *p = fopen((file_name), "r"); \
    if(p != NULL){\
	    fread((data), (data_size), 1, p); \
        fclose(p);\
    }\
}while(0)


#define VIRTUAL_E2PROM 1
#if VIRTUAL_E2PROM
//typedef struct{
//	u8 beaconkey[16];//save[0-15]
//	u8 key1[16];//save[16-31]
//	u32 upload_sn;//save[32-35]
//	int download_sn;//save[36-39]
//	u8 group_id[GROUP_MAX_NUM];//save[40-47]
//	state_machine_e device_state;//save[48]--DEVICE_STATE_PAIRING=0;DEVICE_STATE_PAIRED=1
//}dev_info_s;
u8 virtual_e2_buf[256] ={
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //beaconkey 
	0x35, 0x30, 0x38, 0x38, 0x41, 0x39, 0x34, 0x31, 
	0x45, 0x33, 0x35, 0x30, 0x44, 0x35, 0x31, 0x44,//key1
	0x00, 0x00, 0x00, 0x00,//upload_sn
	0xFF, 0xFF, 0xF0, 0x00,//download_sn
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//group_id
	0x00,//device_state
    0x00,0x00,0x00,//rev
    0x00,0x00,0x00,0x00//old_srand_tick
	};
		
u8 hal_storage_256_init(void){
    READ_FROM_FILE("hal_storage_256.db",virtual_e2_buf,256);
	return 0;
}

u8 hal_storage_256_read_bytes(u16 offset, u8* pdata, u8 len){
	memcpy(pdata,&virtual_e2_buf[offset],len);
	
	print("->[r]:offset = %d, len = %d, data:[",offset,len);
	for(int i=0;i<len;i++)
		print("0x%x, ",pdata[i]);
	print("]\r\n");
	
    return 0; 
}

u8 hal_storage_256_write_bytes(u16 offset, u8* pdata, u8 len){
  	memcpy(&virtual_e2_buf[offset],pdata,len);
	
	print("->[w]:offset = %d, len = %d, data:[",offset,len);
	for(int i=0;i<len;i++)
		print("0x%x, ",pdata[i]);
	print("]\r\n");

    WRITE_TO_FILE("hal_storage_256.db",virtual_e2_buf,256);
    return 0; 
}
#else
u8 hal_storage_256_init(void){return 0;}
u8 hal_storage_256_read_bytes(u16 offset, u8* pdata, u8 len){return 0;}
u8 hal_storage_256_write_bytes(u16 offset, u8* pdata, u8 len){return 0;}
#endif
