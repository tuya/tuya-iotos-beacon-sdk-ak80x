/*************************************************************************
	> File Name: ty_beacon_remoter.h
	> Author: 
	> Mail: 
	> Created Time: Wed 20 Apr 2021 11:37:50 CST
 ************************************************************************/

#ifndef _TY_BEACON_REMOTER_H
#define _TY_BEACON_REMOTER_H

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
    DEVICE_STATE_PROVING = 0x00,
    DEVICE_STATE_PROVED,
	DEVICE_STATE_NOT_PROVED,
	DEVICE_STATE_REP_GET_BEACONKEY,
	DEVICE_STATE_REP_GET_PROVING_OK	
}state_machine_e;

#define BEACON_DP_ID            11
#define BEACON_DP_TYPE_DP_LEN   0x07

#define BEACON_DP_CMD_PAIR      0x02
#define BEACON_DP_CMD_UNPAIR    0x03
typedef enum{
	DEVICE_CMD_TYPE_CTRL = 0x00,
    DEVICE_CMD_TYPE_PROV,
    DEVICE_CMD_TYPE_PAIR,
	DEVICE_CMD_TYPE_UNPROV_RESET,
	DEVICE_CMD_TYPE_PROV_RESET
}cmd_type_e;

typedef struct{
    u8 kind_id;
    u8 cmd;
    u8 para[4];
}beacon_dp_data_s;

typedef struct{
    u8 dp_id;
    u8 dp_type_len;
    u8 dp_group_id;
    beacon_dp_data_s dp_data;
}beacon_dp_s;

//mac 6 bytes
//pid 8 bytes
//authkey 16 bytes (authkey[0~16],authkey total 32 bytes)
//version 1 byte(0.0~9.9)
//kind(same as sig mesh, big end)
u8 ty_beacon_init(u8 *pmac, u8 *pauthkey, u8 *ppid, u8 version, u16 kind);
u8 ty_beacon_receive(u8 *rx_buf, u8 len_pdu, u8 rssi);
u8 ty_beacon_send(u32 cycle, u8 send_times, u8 cmd, u8* params, u8 params_len, u8 tp);
u8 ty_beacon_start_pairing(void);
state_machine_e ty_beacon_get_state(void);

u32 ty_beacon_get_upload_sn(void);

void ty_beacon_set_upload_sn(u32 sn);

u8 ty_beacon_run(void);

u8 ty_beacon_pair_init(void);

void ty_beacon_ctrl(cmd_type_e cmd_type, u8* params, u8 params_len);

void __attribute__((weak)) ty_beacon_enter_test_mode_cb(u8 rssi);

#endif
