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
//#include "hal_gpio.h"
#include "stdlib.h"
#include "pwm.h"
#include "watchdog.h"
#include "ble_adv.h"
#include "rf.h"
#include "ty_beacon.h"
#include "app_dps.h"
#include "board.h"
#include "hal_clock.h"
#include "e2prom_access.h"


void main(void)
{
	sys_init();
	sys_set_clock(CPU_CLOCK_16M);
	
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

	//init pwm
	pwm_init();
	pwm_set_cycle(PWM_FRQ_SELECTION);
	pwm_set_reverse_en(PWM0_RVSE_EN|PWM1_RVSE_EN|PWM2_RVSE_EN|PWM3_RVSE_EN|PWM5_RVSE_EN);
	pwm_enable(PWM0_EN|PWM1_EN|PWM2_EN|PWM3_EN|PWM5_EN);
	
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
	#define AUTHKEY_RELEASE 1
#else
	#define AUTHKEY_TEST 1
#endif
	
#if AUTHKEY_RELEASE//EEPROM
	u8 * authkey = 0x3FC4;
	u8 * mac = 0x3FE4;
#endif
#if AUTHKEY_TEST//IOS  TEST
	u8 mac[6] = {0xdc,0x23,0x4d,0x26,0x5c,0xec};
	u8 authkey[17] = "0A8vbTWPkxHYr0dE";
#endif

	print("authkey:");
	for(u8 i=0;i<16;i++)
		print("%c",authkey[i]);
	print("\r\n");
	
	print("mac:");
	for(u8 i=0;i<6;i++)
		print("%x ",mac[i]);
	print("\r\n");

	
	u8 pid[9] = "chltwaay";
	u8 version = 0x11;//1.1
	u16 kind = 0x0103;
	
    app_light_init();
	ty_beacon_init(mac,authkey,pid,version,kind);
	
	if(ty_beacon_get_state() == DEVICE_STATE_NOT_PAIRED)
		ty_beacon_start_pairing();
//	ty_beacon_start_pairing();
	while(1)
	{
		app_light_run();
        ty_beacon_run();
	}
}




