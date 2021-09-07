/*************************************************************************
	> File Name: ty_beacon.h
	> Author: 
	> Mail: 
	> Created Time: Mon 30 Nov 2020 11:37:50 CST
 ************************************************************************/

#ifndef _TY_BEACON_H
#define _TY_BEACON_H

#include "board.h"


#define TY_BEACON_CMD_UPLOAD_PAIRING                   0x01
#define TY_BEACON_CMD_DOWNLOAD_BEACONKEY	           0x02
#define TY_BEACON_CMD_DOWNLOAD_COMM_CHECK              0x03
#define TY_BEACON_CMD_UPLOAD_COMM_CHECK_RSP 		   0x04
#define TY_BEACON_CMD_UPLOAD_DP                        0x06
#define TY_BEACON_CMD_DOWNLOAD_DP                      0x07
#define TY_BEACON_CMD_DOWNLOAD_GROUP_MANAGE			   0x08
#define TY_BEACON_CMD_UPLOAD_GROUP_MANAGE_RSP 		   0x09
#define TY_BEACON_CMD_NULL                             0x0A

typedef enum{
    DEVICE_STATE_PAIRING = 0x00,
    DEVICE_STATE_PAIRED,
	DEVICE_STATE_NOT_PAIRED,
	DEVICE_STATE_REP_GET_BEACONKEY,
	DEVICE_STATE_REP_GET_PAIRING_OK	
}state_machine_e;

typedef enum{
    BEACON_EVENT_PAIR_OK = 0x00,
    BEACON_EVENT_RESET,
    BEACON_EVENT_GROUP_ADD_SUCCESS,
    BEACON_EVENT_GROUP_ADD_ERR,
    BEACON_EVENT_GROUP_DEL_SUCCESS,
}ty_beacon_event_e;

//mac 6 bytes
//pid 8 bytes
//authkey 16 bytes (authkey[0~16],authkey total 32 bytes)
//version 1 byte(0.0~9.9)
//kind(same as sig mesh, big end)
u8 ty_beacon_init(u8 *pmac, u8 *pauthkey, u8 *ppid, u8 version, u16 kind);
u8 ty_beacon_download(u8 *rx_buf, u8 len_pdu, u8 rssi);
u8 ty_beacon_send(u32 cycle, u8 send_times, u8 cmd, u8* params, u8 params_len, u8 tp);
u8 ty_beacon_start_pairing(void);
state_machine_e ty_beacon_get_state(void);
u8 ty_beacon_run(void);

void __attribute__((weak)) ty_beacon_event_cb(ty_beacon_event_e e, void *params);
void __attribute__((weak)) ty_beacon_enter_test_mode_cb(u8 rssi);

#endif
