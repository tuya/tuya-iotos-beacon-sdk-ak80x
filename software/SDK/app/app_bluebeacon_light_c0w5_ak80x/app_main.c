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

#include "ty_rtc.h"
#include "ty_timing.h"
#include "ty_light_driver_bp5758d.h"







void btn_init(void){
  	sys_set_port_mux(PAD_GPIO_03, PAD_MUX_FUNCTION_0|0x03); //设置IO的功能,并使能上拉
	gpio_set_bit_direction(BIT3, GPIO_INPUT);   			//设置GPIO为输入引脚
    gpio_set_irq_type(GPIO_LEVEL_SENSITIVE,BIT3);			//配置为边沿产生中断
    set_int_polarity(GPIO_HIGH_LEVEL_OR_RISING_EDGE,BIT3);	//配置为下降沿产生中断
    gpio_enable_irq(BIT3);									//使能GPIO6中断功能
    gpio_clear_irq(BIT3);									//为防止使能后马上进入中断，应先清除GPIO6的中断标志
 
    int_enable_irq(INT_GPIO_EN_MASK);	
}
void gpio_init(void){
  	sys_set_port_mux(PAD_GPIO_03, PAD_MUX_FUNCTION_0|0x03); //设置IO的功能,并使能上拉
	gpio_set_bit_direction(BIT3, GPIO_INPUT);   			//设置GPIO为输入引脚
	sys_set_port_mux(PAD_GPIO_04, PAD_MUX_FUNCTION_0|0x03); //设置IO的功能,并使能上拉
	gpio_set_bit_direction(BIT4, GPIO_INPUT);   			//设置GPIO为输入引脚
}

int main(void)
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

#if PWM_CONTROL_FUNCTION
	//init pwm
	pwm_init();
	pwm_set_cycle(PWM_FRQ_SELECTION);
	pwm_set_reverse_en(PWM2_RVSE_EN);
	pwm_enable(PWM2_EN|PWM3_EN);
	
	pwm_set_duty(PWM_CHANNEL_RGB_C, 0);
	pwm_set_duty(PWM_CHANNEL_RGB_W, PWM_FRQ_SELECTION);
//	light_adjust_WC(0,0);
//	pwm_set_reverse_en(PWM0_RVSE_EN|PWM1_RVSE_EN|PWM2_RVSE_EN|PWM3_RVSE_EN|PWM5_RVSE_EN);
//	pwm_enable(PWM0_EN|PWM1_EN|PWM2_EN|PWM3_EN|PWM5_EN);
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
	rf_set_power(3);
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
	u8 * authkey = (u8 *)0x3FC4;
	u8 * mac = (u8 *)0x3FE4;
#endif

#if AUTHKEY_12//IOS  TEST 2 
	u8 mac[6] = {0xdc,0x23,0x4d,0x3e,0xa3,0x33};
	u8 authkey[17] = "V535C1E5DAmLyUJsMCApluADPwVzMsv9";
#endif
#if AUTHKEY_1//IOS  TEST  5
	u8 mac[6] = {0xdc,0x23,0x4d,0x30,0x92,0xa6};
	u8 authkey[17] = "y77xv7EdbjW2SWE4UPvNFXUcAue8BJAF";
#endif
	 
	u8 pid[9] = "        ";//rc0ieijt";//"3tiavcvp";//"y0jtgfql";//"xniu6ibw";mjy40xws
	u8 version = 0x03;//0.3
	u16 kind = 0x1012;
	
    app_light_init();
	ty_beacon_init(mac,authkey,pid,version,kind);
	//btn_init();
	gpio_init();

#if TIME_WORK_FUNCTION
	//rtc init
   ty_rtc_params_s ty_rtc_params;
   ty_rtc.init(&ty_rtc_params);
 
   //timing init
    mITimingFunc.init(); 
/*eg
//    mITimingFunc.set(&mITiming[0],1,360,1,0x01,&timing_end_cb);//06:00:00 [星期:7] 重复
//    mITimingFunc.set(&mITiming[1],1,360,0,0x02,&timing_end_cb);//06:00:00 [星期:6] 不重复
//    mITimingFunc.set(&mITiming[2],1,1020,1,0x0F,&timing_end_cb);//17:00:00 [星期:4\5\6\7] 重复
*/
#endif
extern uint8_t app_light_ctrl_data_reset_cnt_get(void);
extern void ty_beacon_paring_stop(void);
#if BP5758D_CONTROL_FUNCTION
	//bp5758d_init
	ty_light_driver_bp5758d_init(6, 6, 6, 27, 27); // R G B C W 电流
#endif


beacon_remote_pair_windowns_pramer_set(TRUE,30);

	if(ty_beacon_get_state() == DEVICE_STATE_NOT_PAIRED){
		if (app_light_ctrl_data_reset_cnt_get() == 0)
		{
			ty_beacon_paring_stop();
		}
	}
	else{
#if DSITURB_FUNCTION		
		if((TRUE == app_light_ctrl_data_not_disturb_get())&&(app_light_ctrl_data_reset_cnt_get()<1)){
			app_light_ctrl_data_onoff_set(0);
		}
#endif		
	}

extern void power_outage_state(void);

	while(1)
	{
#if COUNT_DOWN_FUNCTION
		
        tick_countdown_event_handle(); // 倒计时事件处理
#endif
		static u32 T_2ms = 0;
		if(hal_clock_time_exceed(T_2ms,2000)){
			T_2ms = hal_clock_get_system_tick();
			GLOBAL_INT_DISABLE();
			app_light_run();
			
#if TIME_WORK_FUNCTION		
		   mITimingFunc.run(mITiming,MAX_TIMING_NUM);
		   ty_rtc.run();
#endif
			
			GLOBAL_INT_RESTORE();
		}

        ty_beacon_run();
#if BEACON_REMOTE_FUNCTION		
		tick_remote_pair_windowns_countdown_event_handle();
		tick_remote_countdown_event_handle();
#endif
	}
}


