/****************************************************************************
@FILENAME:	ble_adv.h
@FUNCTION:	config the rf to send adv
@AUTHOR:	flege.zhang
@DATE:		2019.11.27
@COPYRIGHT(C) ZXW 2020
*****************************************************************************/


#ifndef _BLE_ADV__H_
#define _BLE_ADV__H_

#include "rf.h"
#include "app_cfg.h"


#ifdef RF_SEND_DIFF_PACKET_NUM
#define ADV_FIFO_MAX_NUM										(RF_SEND_DIFF_PACKET_NUM)
#else
#define ADV_FIFO_MAX_NUM										(0x01)
#endif

#define BLE_ADV_PAYLOAD_MAX_LENGTH								(39)//include ble header[2],adva[6],advdata[31]

#define SEND_ADV_TIMER_IRQ_MIN_INTERVAL							(5)//unit:ms
#define HW_TX_BUFFER_ADDR										(BB_MEMEORY_ADDR + (BB_MEMEORY_SIZE>>1))

#define RF_CHECK_TX_DONE_BY_IRQ									(0x01)
#define RF_CHECK_TX_DONE_BY_INQUIRY								(0x02)
#define RF_CHECK_TX_DONE_TYPE									RF_CHECK_TX_DONE_BY_INQUIRY

#define ADV_CHANNEL_BIT_37										(0x01)
#define ADV_CHANNEL_BIT_38										(0x02)
#define ADV_CHANNEL_BIT_39										(0x04)

/*BLE ADV PDU TYPE(BIT[3:0])*/
#define BLE_ADV_IND												(0x00)
#define BLE_ADV_DIRECT_IND										(0x01)
#define BLE_ADV_NONCONN_IND										(0x02)
#define BLE_SCAN_REQ											(0x03)
#define BLE_SCAN_RSP											(0x04)
#define BLE_CONNECT_REQ											(0x05)
#define BLE_ADV_SCAN_IND										(0x06)
#define BLE_ADV_PDU_TYPE										BLE_ADV_NONCONN_IND

/*BLE ADV ADDR FLAG(BIT[7:6])*/
#define ADDR_PUBLIC_FLAG										(0x00)
#define ADDR_RANDOM_FLAG										(0x01)
#define BLE_ADV_TX_ADDR_FLAG									(ADDR_RANDOM_FLAG<<6) //public address need to apply from SIG.
#define BLE_ADV_RX_ADDR_FLAG									(ADDR_PUBLIC_FLAG<<7)


typedef struct
{
	uint8_t 	adv_payload_len;
	uint16_t 	interval;
	uint16_t 	tx_left_count;
	uint8_t 	adv_payload[BLE_ADV_PAYLOAD_MAX_LENGTH];
}str_adv_fifo;


void 	ble_adv_start(void);

/****************************************************************************
 * @function config rf as tx,and start send adv 
 *
 * @param[in] type          the adv type,define in enum en_adv_type
 * @param[in] data          adv data which need be sent.
 * @param[in] data_len      length of adv data.
 * @param[in] tx_num        number of transmission per advertising channel to perform,must be an integral multiple of SEND_ADV_TIMER_IRQ_MIN_INTERVAL
 * @param[in] interval_ms   interval of the advertising (unit:ms)
 * @param[in] tx_chmap      advertising channel map
 ****************************************************************************/
void 	ble_adv_send(uint8_t type,const uint8_t* data, uint8_t data_len, uint16_t tx_num, uint16_t interval_ms, uint8_t tx_chmap);

/****************************************************************************
 * @function config stop adv of any type 
 *
 * @param[in] type          the adv type,define in enum en_adv_type,0xFF is disable all and disable timer
 ****************************************************************************/
void 	ble_adv_stop(uint8_t adv_type);

/****************************************************************************
 * @function copy src to dest by byte
 *
 * @param[out] dest		the pointer of dest
 * @param[in]  src		the pointer of src
 * @param[in]  len		the copy lenth
 *
 * @return     the pointer of dest
****************************************************************************/
uint8_t *memcpy_byte(void* dst, void* src, uint32_t len);

#endif

