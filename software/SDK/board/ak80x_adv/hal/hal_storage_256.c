/*************************************************************************
	> File Name: hal_storage_256.c
	> Author: 
	> Mail: 
	> Created Time: Tue 26 Jan 2021 12:26:42 CST
 ************************************************************************/

#include "hal_storage_256.h"


//e2rom 256 byte


#define DATA_SIZE 				80
#define CRC_SIZE 				1
#define ERR_SIZE 				1

#define BUFFER_SIZE 			DATA_SIZE+CRC_SIZE+ERR_SIZE+7 //读写地址必须是8的倍数
#define BUFFER_CRC_OFFSET  		DATA_SIZE
#define BUFFER_ERR_OFFSET  		DATA_SIZE+CRC_SIZE+7



#define ADDR_1_OFFSET 			0 
#define CRC_1_OFFSET  			ADDR_1_OFFSET+DATA_SIZE
#define ERR_1_DATA_OFFSET  		ADDR_1_OFFSET+DATA_SIZE+CRC_SIZE+7  //读写地址必须是8的倍数

#define ADDR_2_OFFSET 			128 
#define CRC_2_OFFSET  			ADDR_2_OFFSET+DATA_SIZE
#define ERR_2_DATA_OFFSET  		ADDR_2_OFFSET+DATA_SIZE+CRC_SIZE+7  //读写地址必须是8的倍数



static bool write_f_flag = 0;//0 ---ADDR_1_OFFSET;  1---ADDR_2_OFFSET   

uint8_t g_virtual_buf[BUFFER_SIZE] ={//读写地址必须是8的倍数
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //beaconkey 
	
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,//key1
	
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,//download_sn and upload_sn
	
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,//u8 prov_state;u8 pair_srand[2];u8 mdev_test_flag;u32 srand_tick
	
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,//srand2
	
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,//srand3

	0xFF, // crc
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,//rev
	
	0xFF,// err cnt
};

uint8_t crc8_maxim(uint8_t *data, uint16_t length)
{
    uint8_t i;
    uint8_t crc = 0;         // Initial value
    while(length--)
    {
        crc ^= *data++;        // crc ^= *data; data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x8C;        // 0x8C = reverse 0x31
            else
                crc >>= 1;
        }
    }
    return crc;
}

u8 hal_storage_256_init(void){

}

u8 read_data_from_flash(u16 offset, u8* pdata, u8 len){
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

u8 write_data_to_flash(u16 offset, u8* pdata, u8 len){
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

u8 hal_storage_256_read_bytes(u16 offset, u8* pdata, u8 len)
{
	if((offset >= DATA_SIZE) || ((offset+len) > DATA_SIZE)){
		print("read offset addr or len error\r\n");
		return 1;
	}	
	
	write_f_flag = 0;//0 ---ADDR_1_OFFSET;  1---ADDR_2_OFFSET 	
	
	uint8_t check_crc = 0;
    if(read_data_from_flash(ADDR_1_OFFSET, g_virtual_buf, BUFFER_SIZE) != 0) {
        print("-------------R_1 fail------------\r\n");
		
		if(read_data_from_flash(ADDR_2_OFFSET, g_virtual_buf, BUFFER_SIZE) != 0) {
			print("-------------R_2 fail------------\r\n");
			return 1;
			
		}else{
			check_crc = crc8_maxim(g_virtual_buf, DATA_SIZE);
			
			if (check_crc != g_virtual_buf[BUFFER_CRC_OFFSET]){
				g_virtual_buf[BUFFER_ERR_OFFSET] += 1;
				write_data_to_flash(ERR_2_DATA_OFFSET, &g_virtual_buf[BUFFER_ERR_OFFSET], 1);
				print("-------------R_2 fail------------: %d\r\n", g_virtual_buf[BUFFER_ERR_OFFSET]);
				return 1;
			}			
		}		
    }else{
		check_crc = crc8_maxim(g_virtual_buf, DATA_SIZE);
		
		if(check_crc != g_virtual_buf[BUFFER_CRC_OFFSET]){			
			g_virtual_buf[BUFFER_ERR_OFFSET] += 1;
			write_data_to_flash(ERR_1_DATA_OFFSET, &g_virtual_buf[BUFFER_ERR_OFFSET], 1);
			print("-------------R_1 fail------------: %d\r\n", g_virtual_buf[BUFFER_ERR_OFFSET]);
			
			if(read_data_from_flash(ADDR_2_OFFSET, g_virtual_buf, BUFFER_SIZE) != 0) {
				print("-------------R_2 fail------------\r\n");
				return 1;
				
			}else{
				check_crc = crc8_maxim(g_virtual_buf, DATA_SIZE);
				
				if (check_crc != g_virtual_buf[BUFFER_CRC_OFFSET]){
					g_virtual_buf[BUFFER_ERR_OFFSET] += 1;
					write_data_to_flash(ERR_2_DATA_OFFSET, &g_virtual_buf[BUFFER_ERR_OFFSET], 1);
					print("-------------R_2 fail------------: %d\r\n", g_virtual_buf[BUFFER_ERR_OFFSET]);
					return 1;
				}			
			}
		}else{
			write_f_flag = 1;//0 ---ADDR_1_OFFSET;  1---ADDR_2_OFFSET
		}		
		
	}

    memcpy(pdata, &g_virtual_buf[offset], len);
    print("read success\r\n");

    return 0;
}








u8 hal_storage_256_write_bytes(u16 offset, u8* pdata, u8 len){
	
	if((offset >= DATA_SIZE) || ((offset+len) > DATA_SIZE)){
		print("write offset addr or len error\r\n");
		return 1;
	}
	
    memcpy(&g_virtual_buf[offset], pdata, len);
	
    uint8_t return_value = 1;
	
    g_virtual_buf[BUFFER_CRC_OFFSET] = crc8_maxim(g_virtual_buf, DATA_SIZE);
	
	
	if(write_f_flag == 1){//  1---ADDR_2_OFFSET  wirte ADDR_2 firstly

		if (write_data_to_flash(ADDR_2_OFFSET, g_virtual_buf, BUFFER_SIZE) == 0) {
			print("w_2_success\r\n");
			return_value = 0;
		}else{
			return_value = 2;

			g_virtual_buf[BUFFER_ERR_OFFSET] += 1;
			write_data_to_flash(ERR_2_DATA_OFFSET, &g_virtual_buf[BUFFER_ERR_OFFSET], 1);
			print("----------W_2 fail----------: %d\r\n", g_virtual_buf[BUFFER_ERR_OFFSET]);	
			
		}

		u8 read_buf[BUFFER_SIZE];
		uint8_t check_crc = 0;
		read_data_from_flash(ADDR_2_OFFSET, read_buf, BUFFER_SIZE);
		check_crc = crc8_maxim(read_buf, DATA_SIZE);
		if (check_crc != g_virtual_buf[BUFFER_CRC_OFFSET]){
			if (write_data_to_flash(ADDR_2_OFFSET, g_virtual_buf, BUFFER_SIZE) != 0) {//error
				return 2;
			}else{
				read_data_from_flash(ADDR_2_OFFSET, read_buf, BUFFER_SIZE);
				check_crc = crc8_maxim(read_buf, DATA_SIZE);
				if (check_crc != g_virtual_buf[BUFFER_CRC_OFFSET]){	
					return 2;
				}			
			}
		}
		

		if (write_data_to_flash(ADDR_1_OFFSET, g_virtual_buf, BUFFER_SIZE) == 0) {
			print("w_1_success\r\n");
			return_value = 0;
		}
		else{
			g_virtual_buf[BUFFER_ERR_OFFSET] += 1;
			write_data_to_flash(ERR_1_DATA_OFFSET, &g_virtual_buf[BUFFER_ERR_OFFSET], 1);
			print("----------W_1 fail----------: %d\r\n", g_virtual_buf[BUFFER_ERR_OFFSET]);	
		}
		
	}else{	//  0---ADDR_1_OFFSET  wirte ADDR_1 firstly

		if (write_data_to_flash(ADDR_1_OFFSET, g_virtual_buf, BUFFER_SIZE) == 0) {
			print("w_1_success\r\n");
			return_value = 0;
		}
		else{
			g_virtual_buf[BUFFER_ERR_OFFSET] += 1;
			write_data_to_flash(ERR_1_DATA_OFFSET, &g_virtual_buf[BUFFER_ERR_OFFSET], 1);
			print("----------W_1 fail----------: %d\r\n", g_virtual_buf[BUFFER_ERR_OFFSET]);	
		}

		u8 read_buf[BUFFER_SIZE];
		uint8_t check_crc = 0;
		read_data_from_flash(ADDR_1_OFFSET, read_buf, BUFFER_SIZE);
		check_crc = crc8_maxim(read_buf, DATA_SIZE);
		if (check_crc != g_virtual_buf[BUFFER_CRC_OFFSET]){
			if (write_data_to_flash(ADDR_1_OFFSET, g_virtual_buf, BUFFER_SIZE) != 0) {//error
				return 1;
			}else{
				read_data_from_flash(ADDR_1_OFFSET, read_buf, BUFFER_SIZE);
				check_crc = crc8_maxim(read_buf, DATA_SIZE);
				if (check_crc != g_virtual_buf[BUFFER_CRC_OFFSET]){
					return 1;
				}			
			}
		}

		if (write_data_to_flash(ADDR_2_OFFSET, g_virtual_buf, BUFFER_SIZE) == 0) {
			print("w_2_success\r\n");
			return_value = 0;
		}else{
			return_value = 2;

			g_virtual_buf[BUFFER_ERR_OFFSET] += 1;
			write_data_to_flash(ERR_2_DATA_OFFSET, &g_virtual_buf[BUFFER_ERR_OFFSET], 1);
			print("----------W_2 fail----------: %d\r\n", g_virtual_buf[BUFFER_ERR_OFFSET]);	
			
		}		
	}

    return return_value;
}



