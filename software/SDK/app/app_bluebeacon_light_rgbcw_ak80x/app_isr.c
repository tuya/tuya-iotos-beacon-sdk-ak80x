/****************************************************************************
@FILENAME:	isr.c
@FUNCTION:	isr handler,stong isr function define
@AUTHOR:	flege.zhang
@DATE:		2020.04.18
*****************************************************************************/


#include "app_cfg.h"
#include "ble_adv.h"
#include "timer.h"
#include "timer_mgt.h"
#include "tccr.h"
#include "int.h"
#include "watchdog.h"
#include "uart.h"
#include "ty_beacon.h"
#include "lvd.h"
#include "gpio.h"

volatile uint32_t core_timer_count_ms = 0x0;
uint8_t rx_channel_offset=0;


void wtd_enable()
{
#if(ABNORMAL_PROTECTION_TYPE)
    asm("lrw         r2, 0x40120100");
	asm("movi      	r3, 0");
    asm("st.w        r3, (r2, 0x64)");
	
    asm("movi      	r3, 5");
    asm("st.w        r3, (r2, 0x68)");

    asm("movi      	r3, 0");
    asm("st.w        r3, (r2, 0x6c)");
	
    asm("movi      	r4, 1");
    asm("st.w        r4, (r2, 0x70)");

	asm("movi        r1, 4");
    asm("st.w        r1, (r2, 0x8c)");


	asm("rts");
#endif
}


void error_handler()
{
#if(ABNORMAL_PROTECTION_TYPE)

    uint16_t delay_counter =10000;

    asm("lrw   r3, 0x40120100");

    asm("movi  r2, 0x0");
    asm("st.w  r2, (r3, 0x8c)");

    asm("movi  r0, 1");
    asm("lsli  r0, r0, 5");
            
    asm("movi  r2, 0");
    asm("st.w  r2, (r3, 0x64)");
            
    asm("movi  r2, 0");
    asm("st.w  r2, (r3, 0x68)");
    
    asm("st.w  r0, (r3, 0x6c)");
            
    asm("movi  r2, 1");
    asm("st.w  r2, (r3, 0x70)");

    asm("movi  r2, 4");
    asm("st.w  r2, (r3, 0x8c)");

    while(delay_counter--)
    {
        asm("nop");
    }
    
#endif

}

ATTRIBUTE_ISR void tx_isr()
{
#if(RF_CHECK_TX_DONE_TYPE == RF_CHECK_TX_DONE_BY_IRQ)
#if LOG_DETAIL
	print("tx done\n");
#endif

	//send complete,switch rf to rx
    rf_mask_int(BB_INT_TX_MASK_MASK);

	//RF config as RX mode	
	rf_set_rx_tx_mode(RF_RX_MODE);
	rf_set_bbram_mode(BBRAM_FIFO_TXRX);
    rf_set_rx_sync_words( BB_CHANNEL_0, 0x8E, 0x89BED6AA );
    rf_soft_reset();
    rf_start();
#endif
}


ATTRIBUTE_ISR void rx_isr()
{
	uint32_t    reg_val = 0;
	uint8_t*	bb_buffer = (uint8_t*)BB_MEMEORY_ADDR;
	uint8_t		buffer_len = 0x0;
	uint8_t 	rx_rssi;
	uint8_t		rx_buffer[39] = {0};//from ble header to end
	
	reg_val = read_reg(BB_REG_INTERRUPT_READ_ONLY_REG);
	
	/*BLE ADV PACKET FORMAT*/
	/*Preamble[1],Access Address[4],Header[2],AdvA[6],AdvData[0 - 31]*/
	
	/*CAUTION:BB received data from BLE Header*/
    if(BB_CTRL2REG_RX_RAW_INT_STATE_MASK & reg_val)
    {
    	buffer_len = rf_get_rx_payload_len(0x0);

		//important:check rx len,base ble v4.0
		if(buffer_len > 39)//valid data length is[8,8 + 31]
		{
#if LOG_WARN
			print("ERR:rx invalid data,len:%x\n",buffer_len);
#endif
			//invalid rx buffer,don't handle it
			goto read_buff_end_label;
		}

		memcpy_byte(rx_buffer,bb_buffer,buffer_len);
		rx_rssi = rf_get_rssi( 0 );
		
		ty_beacon_download(rx_buffer,buffer_len,rx_rssi);//tuya advertising parse

read_buff_end_label:
        rf_read_finish();
    }
	
	rx_channel_offset++;
	if(rx_channel_offset>=3)
	{
		rx_channel_offset=0;
	}
	rf_set_channel_num(37+rx_channel_offset);
	rf_int_clear(BB_RX_INT_CLEAR_MASK);
	rf_soft_reset();
	rf_start();
}	

extern uint32_t ir_code;
extern uint8_t ir_code_get;
ATTRIBUTE_ISR void gpio_isr(){
	static uint32_t pre_time = 0;
	static uint8_t ir_num = 0xFF;
	uint32_t reg = 0;
		
	uint32_t now_time = get_core_timer_value();
	reg = read_reg(GPIO_INT_STATUS);
	if(reg&BIT6){							//判断是否由GPIO6产生的中断
		gpio_clear_irq(BIT6);				//清除该位的中断标志
		uint32_t spend_time = (pre_time - now_time)/16;
		pre_time = now_time;
		
		if(ir_code_get != 0)return;
		if(ir_num == 0xFF){
			if(spend_time > 10000  && spend_time < 15000){
				ir_num = 0;
				ir_code = 0;
				ir_code_get = 0;
			}
		}else{
			if(spend_time > 2500 || spend_time< 800){
				ir_num = 0xFF;
				return;
			}
				
	
			if(spend_time > 1600){
				ir_code |= (1<<ir_num);
			}
			
			ir_num++;
			
			if(ir_num > 31){
				ir_num = 0xFF;
				ir_code_get = 1;
			}
		}
	}
}

ATTRIBUTE_ISR void timer0_isr()
{
    if(read_reg(TIMER0_INT_STATUS))
    {
        read_reg(TIMER0_EOI);//read clear

    }
}

ATTRIBUTE_ISR void timer1_isr()
{
    if(read_reg(TIMER1_INT_STATUS))
    {
        read_reg(TIMER1_EOI);//read clear
        ble_adv_start();
    }
}

ATTRIBUTE_ISR void timer2_isr()
{
    if(read_reg(TIMER2_INT_STATUS))
    {
        read_reg(TIMER2_EOI);//read clear
    }
}

ATTRIBUTE_ISR void core_timer_isr()
{
	read_reg(CORE_TIMER_CORET_CSR_REG);//read clear
	core_timer_count_ms += 1000;//increace 1000ms
}

ATTRIBUTE_ISR void tccr_isr()
{
    /*
	__      ______
      |     |     |
      |_____|     |_________________________
      A     B     A       C
      A:irq,tccr timer reload,record current timer count as TCCR_CC1C_ADDR
      B:tccr record current timer count as TCCR_CC0C_ADDR
      C:irq, tccr timeout
    */
}

ATTRIBUTE_ISR void lvd_isr()
{

}
ATTRIBUTE_ISR void lvd_l_isr()
{
#if(ABNORMAL_PROTECTION_TYPE)
	write_reg(INT_VIC_ICPR_REG, INT_LV_L_EN_MASK);//clear lvd_l int status
    write_reg(TOP_POWER_CTRL_REG, 0x00);//clear sleep flag
    lvd_threshold_set(LVD_THR_0);
    lvd_powerup_enable();
	write_reg(TOP_POWER_CTRL_REG, 0x01);//set sleep flag
#endif
}
ATTRIBUTE_ISR void wdt_isr()
{
    if(WDG_INT_FLAG & read_reg(WATCHDOG_STATE_REG))
    {
        wdg_int_clear();
        wdg_feed_dog();
    }
}

ATTRIBUTE_ISR void uart_isr()
{

}

