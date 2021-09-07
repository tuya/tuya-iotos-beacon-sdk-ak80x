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


void btn_init(void){
  	sys_set_port_mux(PAD_GPIO_06, PAD_MUX_FUNCTION_0|0x03); //设置IO的功能,并使能上拉
	gpio_set_bit_direction(BIT6, GPIO_INPUT);   			//设置GPIO为输入引脚
    gpio_set_irq_type(GPIO_EDGE_SENSITIVE,BIT6);			//配置为边沿产生中断
    set_int_polarity(GPIO_LOW_LEVEL_OR_FALLING_EDGE,BIT6);	//配置为下降沿产生中断
    gpio_enable_irq(BIT6);									//使能GPIO6中断功能
    gpio_clear_irq(BIT6);									//为防止使能后马上进入中断，应先清除GPIO6的中断标志
 
    int_enable_irq(INT_GPIO_EN_MASK);	
}

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
	#define AUTHKEY_X 1
#else
	#define AUTHKEY_12 1
#endif
	
#if AUTHKEY_X//EEPROM
	u8 * authkey = 0x3FC4;
	u8 * mac = 0x3FE4;
#endif
#if AUTHKEY_6//IOS  TEST
	u8 mac[6] = {0xdc,0x23,0x4d,0x26,0x5c,0xe6};
	u8 authkey[17] = "jmF7Ju8J9AASqR9M";
#endif
#if AUTHKEY_7//IOS  TEST
	u8 mac[6] = {0xdc,0x23,0x4d,0x26,0x5c,0xe7};
	u8 authkey[17] = "jBDuUoCU3EUmrsXp";
#endif
#if AUTHKEY_12//IOS  TEST
	u8 mac[6] = {0xdc,0x23,0x4d,0x26,0x5c,0xec};
	u8 authkey[17] = "0A8vbTWPkxHYr0dE";
#endif
#if AUTHKEY_8//TEST
	u8 mac[6] = {0xdc,0x23,0x4d,0x26,0x5c,0xe8};
	u8 authkey[17] = "OaCTgth3DhP7Cql6";
#endif
#if AUTHKEY_9//TEST
	u8 mac[6] = {0xdc,0x23,0x4d,0x26,0x5c,0xe9};
	u8 authkey[17] = "PX1ATmHOCHYe0XAi";
#endif
#if AUTHKEY_10//yifeng
	u8 mac[6] = {0xdc,0x23,0x4d,0x26,0x5c,0xea};
	u8 authkey[17] = "UX9QZW5ZVqgtmbDx";
#endif
#if AUTHKEY_11//yifeng
	u8 mac[6] = {0xdc,0x23,0x4d,0x26,0x5c,0xeb};
	u8 authkey[17] = "NrGki6PGsygV10vH";
#endif

	print("authkey:");
	for(u8 i=0;i<16;i++)
		print("%c",authkey[i]);
	print("\r\n");
	
	print("mac:");
	for(u8 i=0;i<6;i++)
		print("%x ",mac[i]);
	print("\r\n");

	
	u8 pid[9] = "chltwaay";//rc0ieijt";//"3tiavcvp";//"y0jtgfql";//"xniu6ibw";
	u8 version = 0x11;//1.1
	u16 kind = 0x0103;
	
    app_light_init();
	ty_beacon_init(mac,authkey,pid,version,kind);
	btn_init();
	
	if(ty_beacon_get_state() == DEVICE_STATE_NOT_PAIRED)
		ty_beacon_start_pairing();
//	ty_beacon_start_pairing();
	while(1)
	{
		app_light_run();
        ty_beacon_run();
	}
}




