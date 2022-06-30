/****************************************************************************
@FILENAME:	e2prom_access.c
@FUNCTION:	handle e2prom access functon
@AUTHOR:	flege.zhang
@DATE:		2020.01.08
@COPYRIGHT(C) ZXW 2020
*****************************************************************************/


#include "e2prom_access.h"
#include "delay.h"


void e2prom_init(void)
{
	iic_init();
}


#ifndef DEBUG
	#define VIRTUAL_E2PROM 0
#else
	#define VIRTUAL_E2PROM 1
#endif

#if VIRTUAL_E2PROM
//typedef struct{
//	u8 beaconkey[16];//save[0-15]
//	u8 key1[16];//save[16-31]
//	u32 upload_sn;//save[32-35]
//	int download_sn;//save[36-39]
//	u8 group_id[GROUP_MAX_NUM];//save[40-47]
//	state_machine_e device_state;//save[48]--DEVICE_STATE_PAIRING=0;DEVICE_STATE_PAIRED=1
//}dev_info_s;
uint8_t virtual_e2_buf[256] ={//读写地址必须是8的倍数
	0xf4, 0x37, 0xcf, 0x93, 0xba, 0x7d, 0x4c, 0x15,
	0x6e, 0x1e, 0xfc, 0xab, 0xbe, 0xae, 0xfe, 0x75, //beaconkey 
	0x35, 0x30, 0x38, 0x38, 0x41, 0x39, 0x34, 0x31, 
	0x45, 0x33, 0x35, 0x30, 0x44, 0x35, 0x31, 0x44,//key1
	0x00, 0x00, 0x00, 0x00,//upload_sn
	0xFF, 0xFF, 0xF0, 0x00,//download_sn
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//group_id
	0xff,//device_state
	0xFF,0xFF,0xFF,//rev------------------------
    0x00,0x00,0x00,0x00,//srand_tick
	
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//remote_mac
	0xFF,0xFF,               //s1
	0xFF,                    //group_id
	0xFF,                    //approve_flag
	0xFF,0xFF,0xFF,          //sn
	
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//remote_mac
	0xFF,0xFF,               //s1
	0xFF,                    //group_id
	0xFF,                    //approve_flag
	0xFF,0xFF,0xFF,          //sn
	
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//remote_mac
	0xFF,0xFF,               //s1
	0xFF,                    //group_id
	0xFF,                    //approve_flag
	0xFF,0xFF,0xFF,          //sn
	
	0x00,                     //resv
	0x01,//onoff	[offset 56开始，读写必须是8的倍数]
	0x01,//scene_mode
	0x00,0x00,//H
	0xe8,0x03,//S
	0xF4,0x01,//V
	0x00,//SCENE_MODEL0
	0x32,//music_sensitivity_percent 50%
	0x00,//music_onoff:  off
	0x00,//music_model: 0摇滚，1爵士
	0xA5,//data flag
	0xFF,0xFF,0xFF,//rev------------------------
	0x00//reset_cnt
};

uint8_t e2prom_write_bytes(uint16_t sub_addr, uint8_t* data, uint8_t data_len, uint8_t try_times)
{
	memcpy(&virtual_e2_buf[sub_addr],data,data_len);
	
	print("->[w]:sub_addr = %d, len = %d, data:[",sub_addr,data_len);
	for(int i=0;i<data_len;i++)
		print("0x%x, ",data[i]);
	print("]\r\n");
	
	return 0;
}

uint8_t e2prom_read_bytes(uint16_t sub_addr, uint8_t* data, uint8_t data_len, uint8_t try_times)
{
	memcpy(data,&virtual_e2_buf[sub_addr],data_len);
		
	print("->[r]:sub_addr = %d, len = %d, data:[",sub_addr,data_len);
	for(int i=0;i<data_len;i++)
		print("0x%x, ",data[i]);
	print("]\r\n");

	return 0;
}
#else

uint8_t e2prom_write_bytes(uint16_t sub_addr, uint8_t* data, uint8_t data_len, uint8_t try_times)
{
	uint8_t status;

	while(try_times--)
	{
		status = iic_write_bytes(sub_addr, data, data_len);

		delay_ms(5);

		if(status == 0x0)//SUCCESS
		{
			break;
		}
	}

	return status;
}

uint8_t e2prom_read_bytes(uint16_t sub_addr, uint8_t* data, uint8_t data_len, uint8_t try_times)
{
	uint8_t status;

	while(try_times--)
	{
		status = iic_read_bytes(sub_addr, data, data_len);

		delay_ms(5);
		
		if(status == 0x0)//SUCCESS
		{
			break;
		}
	}

	return status;
}
#endif
