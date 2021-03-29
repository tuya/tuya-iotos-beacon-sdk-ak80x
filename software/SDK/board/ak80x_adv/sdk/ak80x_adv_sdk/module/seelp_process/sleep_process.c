#include "sleep_process.h"
#include "ble_packet_send.h"
#include "codec.h"
#include "hal_gpio.h"
#include "wakeup_timer.h"
#include "type.h"
#include "sys.h"
#include "lvd.h"

extern str_retention_memory ret_mem_data;
#ifndef SYSTEM_NOT_SLEEP
volatile uint8_t sys_sleep_enable = 0x0;
volatile uint32_t sys_sleep_count = 0x0;
void clear_sleep_count()
{
	sys_sleep_enable = 0x0;
	sys_sleep_count = 0x0;
}

void sys_reset_state_check()
{
	//read sys reset cause
	uint32_t reg_value = read_reg(WAKEUP_SOURE_STATE);
	uint32_t* p_retmem = (uint32_t*)&ret_mem_data;
	
	//reset by wakeup timer
	if(reg_value == WAKEUP_BY_WUT)
	{
		//get saved state by word from retention memory
		for(uint8_t i = 0; i < 4; i++)
		{
			p_retmem[i] = *((volatile uint32_t *)(RETENTION_MEMEORY_ADDR + (i<<2)));
		}
		
#if LOG_MAJOR
		print("RESET WT\n");
#endif
	}
	//first power on
	else if(reg_value == WAKEUP_BY_FPO)
	{
		memset(p_retmem,0x0,RETENTION_MEMEORY_SIZE);
		ret_mem_data.current_group_index = 0x01;//default group index is 0x01,master using 0x00
#if LOG_MAJOR
		print("RESET 1ST\n");
#endif
	}

#if LOG_SECONDARY
	//log out reterntion memory by byte
	uint8_t* p_u8retmem = (uint8_t*)&ret_mem_data;
	print("RETMEM:");
	for(uint8_t i = 0; i < 16; i++)
	{
		print("%x ",p_u8retmem[i]);
	}
	print("\r\n");
#endif
}

void sys_sleep_process()
{
		/*sleep when conditions are met*/
		if(sys_sleep_enable)
		{
			sys_sleep_count = 0;
			sys_sleep_enable = 0;
#if LOG_MAJOR
			print("enter sleep\r\n");
#endif
			//config not wakeup io as output low,for power saving
			hal_gpio_cfg_before_sleep(UNUSED_IO);

			//save state to retention memory before sleep,because the state will be reset when next wakeup by io
			//save state to retention memory by word
			uint32_t* p_retmem = (uint32_t*)&ret_mem_data;
			for(uint8_t i = 0; i < 4; i++)
			{
				*((volatile uint32_t *)(RETENTION_MEMEORY_ADDR + (i<<2))) = p_retmem[i];
			}
			
			//config wakeup io:low wakeup,and io enable pull
#ifdef HIGH_LEVEL_WAKEUP
			wakeup_timer_init(WAKEUP_IO,0x1,0x1,0x0);
#else
			wakeup_timer_init(WAKEUP_IO,0x1,0x0,0x0);
#endif

#if(SYS_CRASH_WTD_RESET_ENABLE)
			//disable wtd
			wdg_int_clear();
			wdg_feed_dog();
			watchdog_disable();
#endif
			
			//goto sleep,wake up from begain of main()
			sys_sleep_down();

#if LOG_ERROR
			print("unsleep\n");
#endif
		}
}
#endif
