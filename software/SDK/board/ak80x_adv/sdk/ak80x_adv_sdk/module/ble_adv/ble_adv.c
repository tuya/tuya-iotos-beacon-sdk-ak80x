/****************************************************************************
@FILENAME:	ble_adv.c
@FUNCTION:	config the rf to send adv
@AUTHOR:	flege.zhang
@DATE:		2019.11.27
@COPYRIGHT(C) ZXW 2020
*****************************************************************************/


#include "ble_adv.h"
#include "timer_mgt.h"
#include "delay.h"
#include "int.h"

#include "hal_clock.h"

volatile uint8_t	tx_channel = 37;
volatile uint8_t	tx_channel_offset = 0;
uint8_t 			*DEFAULT_BLE_MAC = (uint8_t *)0x3FE4;
str_adv_fifo 		adv_fifo[ADV_FIFO_MAX_NUM];

volatile uint32_t 	send_adv_timer_count = 0x0;


void ble_adv_start()
{
	/*[EFFICIENCY]disable uart log,send 2 advs spend 2.14ms,send 1 adv spend 1.08ms,test at 20200412*/
	uint8_t i,send_end_flag = 0x0;
	uint8_t* send_buff = (uint8_t*)HW_TX_BUFFER_ADDR;

	//print("%d channel adv start:%d\r\n",tx_channel_offset+37,hal_clock_get_system_tick());
	

	//update current timer count
	send_adv_timer_count += SEND_ADV_TIMER_IRQ_MIN_INTERVAL;

	//check current timer count is send moment
	for(i = 0; i < ADV_FIFO_MAX_NUM; i++)
	{
		//[CAUTION] need check interval, if interval is 0,sys crash,A%B(B != 0)
		if((adv_fifo[i].interval) && (send_adv_timer_count % (adv_fifo[i].interval) == 0x0))
		{
			goto send_adv_label;
		}
	}

	if(i == ADV_FIFO_MAX_NUM)
	{
#if LOG_DETAIL
		print("NOT SEND MOMENT\n");
#endif
		return;
	}

send_adv_label:
	//rf config as tx mode
	rf_int_clear(BB_TX_INT_CLEAR_MASK);
    rf_unmask_int(BB_INT_TX_MASK_MASK);
	rf_set_tx_preamble(BB_TX_PRE_MODE_1_BYTE,0xAA);
	rf_set_tx_aa(BB_TX_AA_MODE_4_BYTE,0x00,0x8E89BED6);
	rf_set_rx_tx_mode(RF_TX_MODE);
	rf_set_channel_num(tx_channel+tx_channel_offset);//by huangjun alter 2020/11/05

	for(i = 0; i < ADV_FIFO_MAX_NUM; i++)
	{
		//check left count and send moment
		//[CAUTION] need check interval, if interval is 0,sys crash,A%B(B != 0)
		if((!adv_fifo[i].tx_left_count) || ((adv_fifo[i].interval) && (send_adv_timer_count % (adv_fifo[i].interval) != 0x0)))
		{
			continue;
		}

		//update left count
		adv_fifo[i].tx_left_count--;

		//update tx length
		rf_set_tx_len(adv_fifo[i].adv_payload_len);

		//update tx buffer
		memcpy_byte(send_buff,&adv_fifo[i].adv_payload,adv_fifo[i].adv_payload_len);
#if LOG_DETAIL
		//print("[%d]LEN:%d,HWBUFFER:",i,adv_fifo[i].adv_payload_len);
		for(uint8_t j = 0; j < adv_fifo[i].adv_payload_len; j++)
		{
			print("%x ",send_buff[j]);
		}
		print("\n");
#endif

		rf_soft_reset();
		rf_write_finish();
		rf_start();
		
		//by huangjun alter 2020/11/05
#if LOG_DETAIL
		//print("%d channel adv start:%d\r\n",tx_channel_offset+37,hal_clock_get_system_tick());
#endif
		//print("%d channel adv start:%d\r\n",tx_channel_offset+37,hal_clock_get_system_tick());
		tx_channel_offset+=1;
		if(tx_channel_offset >= 0x03)
		{
			tx_channel_offset=0x00;
		}
			
#if(RF_CHECK_TX_DONE_TYPE == RF_CHECK_TX_DONE_BY_INQUIRY)
		delay_ms(1);

		//check rf send done?
		if(0 == (BB_CTRL2REG_TX_RAW_INT_STATE_MASK&rf_get_int_status()))
		{
#if LOG_WARN
			print("tx undone\n");
#endif
		}
#endif
	}


#if(RF_CHECK_TX_DONE_TYPE == RF_CHECK_TX_DONE_BY_INQUIRY)
	//rf config as rx mode
	rf_set_rx_tx_mode(RF_RX_MODE);
    rf_set_rx_sync_words(BB_CHANNEL_0,0x8E,0x89BED6AA);
    rf_soft_reset();
    rf_start();
#endif

#if LOG_SECONDARY
	print("TIMER:%d\n",send_adv_timer_count);

	for(i = 0; i < ADV_FIFO_MAX_NUM; i++)
	{
		print("TP[%d],LEFT[%d]\n",i,adv_fifo[i].tx_left_count);
	}
#endif

	//check all advs is send over?
	for(i = 0; i < ADV_FIFO_MAX_NUM; i++)
	{
		if(0x0 == adv_fifo[i].tx_left_count)
		{
			send_end_flag++;
		}
	}
	
	if(ADV_FIFO_MAX_NUM == send_end_flag)
	{
#if LOG_DETAIL
		print("STOP TIMER\n");
#endif
		//stop send adv timer
		int_disable_irq(INT_TIM1_EN_MASK);
	}
}

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
void ble_adv_send(uint8_t type,const uint8_t* data, uint8_t data_len, uint16_t tx_num, uint16_t interval_ms, uint8_t tx_chmap)
{
	//parameters check
	if(!tx_num)
	{
		return;
	}

	//check type is valid
	if(type >= ADV_FIFO_MAX_NUM)
	{
		return;
	}

	uint8_t* send_buff = (uint8_t*)&(adv_fifo[type].adv_payload);

	//stop timer first
	int_disable_irq(INT_TIM1_EN_MASK);

	//refill the same type adv
	adv_fifo[type].adv_payload_len = data_len + 8;
	adv_fifo[type].interval = interval_ms;
	adv_fifo[type].tx_left_count = tx_num;
	
	//tx_channel = 37 + (tx_chmap>>1);//send suport with single channel only!!!

	/*BLE ADV PACKET FORMAT*/
	/*Preamble[1],Access Address[4],Header[2],AdvA[6],AdvData[0 - 31],CRC[3]*/

	/*1.Build BLE Header*/
	send_buff[0] = BLE_ADV_RX_ADDR_FLAG|BLE_ADV_TX_ADDR_FLAG|BLE_ADV_PDU_TYPE;
	send_buff[1] = 6 + data_len;//length:BLE4.1[5:0];BLE4.2/BLE5.x[7:0]

	/*2.Build BLE Mac*/
	for(int i = 5; i >= 0; i--){
		send_buff[2+5-i] = DEFAULT_BLE_MAC[i];
	}
#if LOG_SECONDARY
	print("MAC_ADDR:");
	for(int i = 5; i >= 0; i--){
	{
		print("0x%x ",DEFAULT_BLE_MAC[i]);
	}
	print("\n");
#endif

	/*3.Build BLE AdvData*/
	//save the adv payload to fifo
	memcpy(send_buff + 8,data,data_len);

#if LOG_SECONDARY
	print("build ble[%d]:",adv_fifo[type].tx_left_count);
	for(uint8_t i = 0; i < adv_fifo[type].adv_payload_len; i++)
	{
		print("%x ",adv_fifo[type].adv_payload[i]);
	}
	print("\n");
#endif
	
	//config timer
	int_set_priority(IRQN_TIME1,IRQ_PRI_1);
	timer1_init(TIMER_CLOCK_4M,4000*SEND_ADV_TIMER_IRQ_MIN_INTERVAL); 
}

/****************************************************************************
 * @function config stop adv of any type 
 *
 * @param[in] type          the adv type,define in enum en_adv_type,0xFF is disable all and disable timer
 ****************************************************************************/
void ble_adv_stop(uint8_t adv_type)
{
	//special flag to stop timer
	if(adv_type == 0xFF)
	{
		for(uint8_t i = 0; i < ADV_FIFO_MAX_NUM; i++)
		{
			adv_fifo[i].tx_left_count = 0x0;
		}
		
		//stop timer
		core_timer_disable();

		return;
	}

	if(adv_type < ADV_FIFO_MAX_NUM)
	{
		adv_fifo[adv_type].tx_left_count = 0x0;
	}
}

/****************************************************************************
 * @function copy src to dest by byte
 *
 * @param[out] dest		the pointer of dest
 * @param[in]  src		the pointer of src
 * @param[in]  len		the copy lenth
 *
 * @return     the pointer of dest
****************************************************************************/
uint8_t *memcpy_byte(void* dst, void* src, uint32_t len)
{
	if((0x0 == dst) || (0x0 == src) || (0x0 == len))
	{
		return 0x0;
	}

	uint8_t* psrc = (uint8_t*)src;
	uint8_t* pdst = (uint8_t*)dst;

	while((len--) > 0)
	{
		*pdst++ = *psrc++;
	}

	return dst;
}

