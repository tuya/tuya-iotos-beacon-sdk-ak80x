/*************************************************************************
	> File Name: bal_beacon.c
	> Author: 
	> Mail: 
	> Created Time: Sat 19 Dec 2020 23:48:08 CST
 ************************************************************************/

#include "bal_beacon.h"


uint8_t tx_buf[31] = {30,0xff,
    0x07, 0xd0,//company id
    0x00,//head
    0xDC,0x23,0x4C,0x00, 0x00,0x01,//mac
    0x00,0x00,0x00,0x01,//sn
    0x00,0x00,0x00,0x01,//sn'
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,//params'
    0x00,//crc-8
    0x00
};//transmit-params



void bal_beacon_txrx(u32 tx_cnt, u32 rx_cnt){
	if(tx_cnt > 0)
		ble_adv_send(0,tx_buf,31,tx_cnt*10*3,10,ADV_CHANNEL_BIT_37|ADV_CHANNEL_BIT_38|ADV_CHANNEL_BIT_39);
}


