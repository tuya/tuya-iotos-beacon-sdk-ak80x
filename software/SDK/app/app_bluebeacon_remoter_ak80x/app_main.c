/* ---------------------------------------------------------------
** File name:		main.c
** Brief:
** COPYRIGHT(C) ZXW 2020
** ---------------------------------------------------------------
** Module Information:
**
** Revision History ----------------------------------------------
** ---------------------------------------------------------------
**  Date        Version     Action                  Author
** ---------------------------------------------------------------
**  2019-5-17   1.0         Created
**
** ---------------------------------------------------------------
*/
/*

 * 本示例演示休眠，并通过IO唤醒

 * 且把数据保存到Retation 中，确保休眠后需要记录的数据不会丢失

 * ps:芯片休眠后电流为2ua，每次唤醒都会重新跑main函数，并非从休眠前的地方继续跑，

 * 从休眠到唤醒的时间需要花费5ms时间

 */

/* Includes -----------------------------------------------------*/
#include "type.h"
#include "sys.h"
#include "int.h"
#include "timer.h"
#include "delay.h"
#include "timer_mgt.h"
#include "wakeup_timer.h"
#include "rf.h"
#include "uart.h"
#include "gpio.h"
#include "lvd.h"

#include "hal_gpio.h"

#include "stdlib.h"
#include "pwm.h"
#include "watchdog.h"
#include "ble_adv.h"
#include "rf.h"
#include "ty_beacon_remoter.h"
#include "app_remoter.h"
#include "board.h"
#include "hal_clock.h"
#include "e2prom_access.h"
#include "app_matrix_key.h"



void main(void)
{
	sys_init();
	sys_set_clock(CPU_CLOCK_16M);
	
    //config unsleep
	write_reg(TOP_POWER_CTRL_REG, 0x00);							//清除休眠标志位，必须！
	wakeup_timer_disable();
	wt_int_clear();
	
    //enable gpio clock
    hal_gpio_init();
	e2prom_init();													//初始化e2prom
	delay_ms(5);

#if (ABNORMAL_PROTECTION_TYPE)
    //disable wtd,wtd enabled at ctr0.s
    watchdog_disable();

    write_reg(TOP_POWER_CTRL_REG, 0x00);//clear sleep flag
    //using lvd to detect vdd voltage
    lvd_init();
    lvd_threshold_set(LVD_THR_2);
#endif

	//init core timer
	core_timer_init(CLOCK_SYS_CLOCK_HZ);//interrupt frequency:1000ms
	core_timer_set_int(ON);//enable core timer int
	core_timer_enable();//enable core timer

#if(LOG_ENABLE)
	uart_init(UART_BAUDRATE_115200);
#endif
	//init rf
#if(PLATFORM == FPGA_801)
	rf_bb_mode_config();
#endif
	rf_init();
    rf_set_bbram_mode(BBRAM_FIFO_TXRX);
	rf_set_whit_and_crc(ON,ON);

    //mcu int of bb
    int_enable_irq(INT_RX_EN_MASK);
	int_set_priority(IRQN_RX,IRQ_PRI_1);//modified by flege,20200603

    //rf start
    rf_set_rx_tx_mode(RF_RX_MODE);
    rf_soft_reset();
    rf_start();
	
#if LOG_MAJOR
	print("tuya test!\n");
#endif

#ifndef DEBUG
	#define AUTHKEY_X 1
#else
	#define AUTHKEY_1 1
#endif
	
#if AUTHKEY_X//EEPROM
	u8 * authkey = 0x3FC4;
	u8 * mac = 0x3FE4;
#endif
#if AUTHKEY_1//IOS  TEST
	u8 mac[6] = {0xdc,0x23,0x4d,0x3C,0xCF,0x7A};
	u8 authkey[17] = "h2Mu1AkfNAwhg5Su";
#endif

	print("authkey:");
	for(u8 i=0;i<16;i++)
		print("%c",authkey[i]);
	print("\r\n");
	
	print("mac:");
	for(u8 i=0;i<6;i++)
		print("%x ",mac[i]);
	print("\r\n");

	
	//u8 pid[9] = "key55wy8";
	//u8 pid[9] = "ykuagamo";
	u8 pid[9] = "        ";
	u8 version = 0x13;//1.3
	u16 kind = 0x1054;
	
	ty_beacon_init(mac,authkey,pid,version,kind);

	ty_beacon_pair_init();
    
//	int_init();                     //中断初始化函数

    app_matrix_key_init();
    
    delay_ms(50);
	
	app_remoter_ram_restore();
	
	u8 ty_sleep_time = 0;
	u8 led_state = 1;
	u32 T_time = 0;
	u8 led_off = 1;//1 off;0 on
    while(1)
    {
        ty_beacon_run();
		
		if(((DEVICE_STATE_PROVED == ty_beacon_get_state())||(DEVICE_STATE_NOT_PROVED == ty_beacon_get_state())))
        {
		
			app_matrix_key_run();
			
			delay_ms(50);

			//if((NONE_PRESS == key_state)&&((DEVICE_STATE_PROVED == ty_beacon_get_state())||(DEVICE_STATE_NOT_PROVED == ty_beacon_get_state())))
			if(NONE_PRESS == key_state)
			{
				led_off = 1;//1 stop
//				u8 led_blink_times_temp = app_led_blink_times_get();
//
//				if(led_blink_times_temp > 0){
//					if(hal_clock_time_exceed(T_time,500)){
//						T_time = hal_clock_get_system_tick();
//						
//						led_state = led_state == 0?1:0;
//						gpio_set_bit(TY_LED, led_state);
//						
//						if(led_state == 0){
//							led_blink_times_temp--;
//							app_led_blink_times_set(led_blink_times_temp);
//						}
//						
//					}					
//				}else{
					ty_sleep_time++;
//				}
				
				
				if(ty_sleep_time>60)
				{
					gpio_set_bit(TY_LED, 0);
					print("system sleep!\r\n");
					hal_gpio_cfg_before_sleep(UNUSED_IO);					//把其它不作为唤醒IO进入一个debug模式，防止耗电
					app_remoter_ram_save();
					wakeup_timer_init(TY_WAKEUP_IO,0x1,0x0,0x0);				//配置唤醒引脚，低电平唤醒
					sys_sleep_down();
					delay_ms(10);
				}
			}else{
				ty_sleep_time = 0;
				led_off = 0;//on
			}

			/*****     灯闪烁提醒      *****/
//			if((ty_sleep_time<=60)&&(ty_sleep_time>0))
//			{			
				u8 led_blink_times_temp = app_led_blink_times_get();

				if(led_blink_times_temp > 0){
					if(hal_clock_time_exceed(T_time,500)){
						T_time = hal_clock_get_system_tick();
						
						led_state = led_state == 0?1:0;
						gpio_set_bit(TY_LED, led_state);
						
						if(led_state == 0){
							led_blink_times_temp--;
							app_led_blink_times_set(led_blink_times_temp);
							
							if(led_blink_times_temp == 0){
								led_off = 1;//off
							}
						}
						
					}					
				}else{
					if(led_off == 0){//on
						led_state = 1;
						gpio_set_bit(TY_LED, GPIO_HIGH);
					}else{
						gpio_set_bit(TY_LED, 0);
					}
					T_time = 0;					
				}
//			}
			
		}else{
			delay_ms(10);
		}
	}
}
