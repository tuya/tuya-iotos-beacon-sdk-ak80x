#include "app_matrix_key.h"
#include "board.h"
#include "stdlib.h"
#include "gpio.h"
#include "app_select.h"
#include "print.h"
#include "hal_gpio.h"
#include "int.h"
#include "app_remoter.h"

#pragma pack (1)

#pragma pack () 

kb_data_t kb_data;
kb_data_t kb_data_record;

u8 key_press_flag = 0;

//long press time 3000ms
//double press time max interval 300ms

#define KEY_LONG_PRESS_TIME 3000
#define KEY_PRESS_INTERVAL_TIME  300

const u32 RowArray[5] = {BIT3,BIT14,BIT15,BIT4,BIT5};
const u32 LineArray[5] = {BIT19,BIT7,BIT8,BIT6,BIT0};

#define KEY_ROW_NUM           5   
#define KEY_LINE_NUM          5   

u8 key_state = NONE_PRESS;
void app_matrix_key_init(void)
{
    //RowArray
    sys_set_port_mux(PAD_GPIO_03, (PAD_MUX_FUNCTION_0 | 0x02));
    sys_set_port_mux(PAD_GPIO_14, (PAD_MUX_FUNCTION_0 | 0x02));
    sys_set_port_mux(PAD_GPIO_15, (PAD_MUX_FUNCTION_0 | 0x02));
    sys_set_port_mux(PAD_GPIO_04, (PAD_MUX_FUNCTION_0 | 0x02));
    sys_set_port_mux(PAD_GPIO_05, (PAD_MUX_FUNCTION_0 | 0x02));
 
   //LineArray
	hal_gpios_set(TY_TRIGER_IO,GPIO_INPUT,GPIO_LOW);
	
	hal_gpios_set(TY_LED,GPIO_OUTPUT,GPIO_LOW);
    sys_set_port_mux(TY_LED, PAD_MUX_FUNCTION_0 | 0x03);
	gpio_set_bit(TY_LED, 0);
}
static void app_matrix_key_scan(void)
{
    kb_data.cnt = 0;

	delay_ms(10);
    for(u8 i=0; i<KEY_LINE_NUM; i++)
	{
		hal_gpios_set(LineArray[i],GPIO_OUTPUT,GPIO_LOW);
		delay_ms(5);
		for(u8 j=0; j<KEY_ROW_NUM; j++)
		{
			if(0 == gpio_get_bits(RowArray[j])){
				kb_data.cnt %= KB_RETURN_KEY_MAX;
				kb_data.keycode[kb_data.cnt++] = j * KEY_ROW_NUM + i + 1;
			}
		}
		hal_gpios_set(LineArray[i],GPIO_INPUT,GPIO_LOW);
	}
}

u32 ty_record_time = 0;

static app_matrix_key_temp_clear(void)
{
    key_press_flag = 0;
    memset(&kb_data_record,0,sizeof(kb_data_t));
    key_state = NONE_PRESS;
}

void app_matrix_key_run(void)
{
    u32 now_time = hal_clock_get_system_tick()/HAL_CLOCK_1MS_TICKS;
    
    app_matrix_key_scan();

    if(0 != kb_data.cnt)
    {
        key_press_flag = 1;
        if(kb_data.cnt > kb_data_record.cnt)
        {
            key_state = SINGLE_CLICK;
            memcpy(&kb_data_record,&kb_data,sizeof(kb_data_t));
        }
    }else{
		ty_record_time = now_time;
	}

    if((LONG_PRESS_START == key_state)&&(0 == kb_data.cnt))
    {
        key_state = LONG_PRESS_RELEASE;
        ty_beacon_key_event(key_state, &kb_data_record);

	    app_matrix_key_temp_clear();
		return;
    }
        
    if((0 == kb_data.cnt)&&(1 == key_press_flag)&&(key_state < LONG_PRESS_START))
    {
        key_state = SINGLE_CLICK;
        ty_beacon_key_event(key_state, &kb_data_record);
     
        app_matrix_key_temp_clear();
    }

    //reserved for double and triple click
#if 0
    if((0 == kb_data.cnt)&&(now_time > KEY_PRESS_INTERVAL_TIME)){
        
        ty_beacon_key_event(key_state, &kb_data_record);
        
        app_matrix_key_temp_clear();
    }
#endif 

    if((key_state < LONG_PRESS_START)&&((now_time - ty_record_time) > KEY_LONG_PRESS_TIME)){
        key_state = LONG_PRESS_START;
        
        ty_beacon_key_event(key_state, &kb_data_record);
    }
}
