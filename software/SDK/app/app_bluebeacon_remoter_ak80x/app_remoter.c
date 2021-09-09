#include "app_remoter.h"
#include "board.h"
#include "stdlib.h"
#include "gpio.h"
#include "app_select.h"
#include "print.h"
#include "ty_beacon_remoter.h"
#include "wakeup_timer.h"
#include "lvd.h"

#pragma pack (1)

#pragma pack () 

static u8 r_onoff = 0x00;
static u8 r_group_id = 0;
static u8 r_scene_id = 0;
static u8 r_timing_id = 0;
static u8 r_color_id = 0;

static u8 led_blink_times = 0;//指示灯闪烁次数

typedef struct
{
	//system used,1 words
	u32 ret_upload_sn;
	u8 ret_onoff;
	u8 ret_group_id;
	u8 ret_scene_id;
	u8 ret_timing_id;
	u8 ret_color_id;
}ty_retention_memory;
ty_retention_memory ty_ret_mem_data;


u8 app_led_blink_times_get(void)
{
	return led_blink_times;//指示灯闪烁次数
}

void app_led_blink_times_set(u8 blink_times)
{
	led_blink_times = blink_times;//指示灯闪烁次数
}



void app_remoter_ram_restore(void)
{
	//read sys reset cause
	uint32_t reg_value = read_reg(WAKEUP_SOURE_STATE);
	uint32_t* p_retmem = (uint32_t*)&ty_ret_mem_data;

	//reset by wakeup timer
	if(reg_value == WAKEUP_BY_WUT)
	{
		//get saved state by word from retention memory
		for(uint8_t i = 0; i < 3; i++)
		{
			p_retmem[i] = *((volatile uint32_t *)(RETENTION_MEMEORY_ADDR + (i<<2)));
		}
		print("RESET WT\n");
	}
	//first power on
	else if(reg_value == WAKEUP_BY_FPO)
	{
		memset(p_retmem,0x0,RETENTION_MEMEORY_SIZE);
		print("RESET 1ST\n");
	}

#if 1
	//log out reterntion memory by byte
	uint8_t* p_u8retmem = (uint8_t*)&ty_ret_mem_data;
	print("RETMEM:");
	for(uint8_t i = 0; i < 3; i++)
	{
		print("%x ",p_u8retmem[i]);
	}
	print("\r\n");
#endif
    
	ty_beacon_set_upload_sn(ty_ret_mem_data.ret_upload_sn);
	r_onoff = ty_ret_mem_data.ret_onoff;
	r_group_id = ty_ret_mem_data.ret_group_id;
	r_scene_id = ty_ret_mem_data.ret_scene_id;
	r_timing_id = ty_ret_mem_data.ret_timing_id;
	r_color_id = ty_ret_mem_data.ret_color_id;
}

void app_remoter_ram_save(void)
{
	ty_ret_mem_data.ret_upload_sn = ty_beacon_get_upload_sn();
	ty_ret_mem_data.ret_onoff = r_onoff;
	ty_ret_mem_data.ret_group_id = r_group_id;
	ty_ret_mem_data.ret_scene_id = r_scene_id;
	ty_ret_mem_data.ret_timing_id = r_timing_id;
	ty_ret_mem_data.ret_color_id = r_color_id;
	
	uint32_t* p_retmem = (uint32_t*)&ty_ret_mem_data;

	for(uint8_t i = 0; i < 3; i++)							//把休眠后需要保存的数据保存到Retation中
	{
		*((volatile uint32_t *)(RETENTION_MEMEORY_ADDR + (i<<2))) = p_retmem[i];
	}
}



/*
 命令枚举
总开关:         0x00,0xff,0x04,0x00(),0x00,0x00,0x00
夜灯:  一下两条命令在里面
定时:           0x00,0xff,0x06,0x00(),0x00(),0x00,0x00
亮度+:          0x00,0x01,0x09,0x01,0x00,0x14,0x00
亮度-:          0x00,0x01,0x09,0x02,0x00,0x14,0x00
亮度无极+:      0x00,0x01,0x0A,0x00(),0x00,0x14,0x64
亮度无极-:      0x00,0x01,0x0A,0x01(),0x00,0x14,0x00
色温+:          0x00,0x01,0x0B,0x01,0x14,0x00,0x00
色温-:          0x00,0x01,0x0B,0x02,0x14,0x00,0x00
色温无极+:      0x00,0x01,0x0C,0x00(),0x14,0x64,0x00
色温无极-:      0x00,0x01,0x0C,0x01(),0x14,0x00,0x00
喜好收藏:       0x00,0xff,0x05,0x01,0x00(),0x00,0x00
喜好执行:       0x00,0xff,0x05,0x02,0x00(),0x00,0x00
分组1:          0x00(),0xff,0x04,0x00,0x00,0x00,0x00
情景切换:       0x00,0x01,0x15,0x01,0x00(),0x00,0x00
彩色步进循环:   0x00,0x01,0x0D,0x01,0x00(),0x00,0x00
彩色无极循环:   0x00,0x01,0x0F,0x00(),0x14,0x00,0x00//只有连续加减,无法实现循环
色温步进循环:   0x00,0x01,0x0B,0x01,0x14,0x00,0x00
色温无极循环:   0x00,0x01,0x0C,0x00(),0x14,0x00,0x00
情景:           0x00,0x01,0x15,0x01,0x00(),0x00,0x00
 */
u8 cmd_onoff[7] = {0x00,0xff,0x04,0x00,0x00,0x00,0x00};
u8 cmd_night_light[7] = {0x00,0x01,0x17,0x05,0x00,0x00,0x00};
u8 cmd_timing[7] = {0x00,0xff,0x06,0x00,0x00,0x00,0x00};
u8 cmd_lum_step[7] = {0x00,0x01,0x09,0x01,0x00,0x14,0x00};
u8 cmd_lum_stepless[7] = {0x00,0x01,0x0A,0x00,0x00,0x14,0x64};
u8 cmd_temp_step[7] = {0x00,0x01,0x0B,0x01,0x14,0x00,0x00};
u8 cmd_temp_stepless[7] = {0x00,0x01,0x0C,0x00,0x14,0x64,0x00};
u8 cmd_fav[7] = {0x00,0xff,0x05,0x01,0x00,0x00,0x00};
u8 cmd_group_onoff[7] = {0x00,0xff,0x04,0x00,0x00,0x00,0x00};
u8 cmd_color[7] = {0x00,0x01,0x0D,0x01,0x00,0x00,0x00};
u8 cmd_hue_stepless[7] = {0x00,0x01,0x0F,0x03,0x14,0x00,0x00};
u8 cmd_temp_step_cycle[7] = {0x00,0x01,0x0B,0x03,0x14,0x00,0x00};
u8 cmd_temp_stepless_cycle[7] = {0x00,0x01,0x0C,0x03,0x14,0x00,0x00};
u8 cmd_scene[7] = {0x00,0x01,0x15,0x01,0x00,0x00,0x00};

void app_remoter_single_click(kb_data_t* kb_data)
{
    if(kb_data->cnt > 1)
        return;
    u8 cmd_type = DEVICE_CMD_TYPE_CTRL;
    u8 *params;
    u8 params_len = 7;

    cmd_type = DEVICE_CMD_TYPE_CTRL;
    
    switch(kb_data->keycode[0]){
        case 1:{
                r_group_id = 0; 
                params = &cmd_onoff[0];
                *params = r_group_id;
                *(params+3) = r_onoff;
				print("------device onoff:%d\r\n",r_onoff);
				r_onoff++;
                r_onoff = r_onoff%2;
               }
            break;
        case 2:
				params = &cmd_night_light[0];
				*params = r_group_id;
            break;
        case 3:{
                params = &cmd_timing[0];
                *params = r_group_id;
                u16 timing = 0;
                if(0 == r_timing_id)
                {
                    timing = 900;
					led_blink_times = 2;//指示灯闪烁次数-1
                }else if(1 == r_timing_id){
                    timing = 1800;
					led_blink_times = 3;//指示灯闪烁次数-1
                }else{
                    timing = 3600;
					led_blink_times = 4;//指示灯闪烁次数-1
                }
                *(params+3) = (timing>>8)&0xFF;
                *(params+4) = timing&0xFF;
                r_timing_id++; 
                r_timing_id = r_timing_id%3;
						
                print("------device countdown\r\n");
               }
            break;
        case 4:{
                params = &cmd_lum_step[0];
                *params = r_group_id;
                *(params+3) = 0x01;

				print("------device lum up\r\n");
               }
            break;
        case 5:{
                params = &cmd_lum_step[0];
                *params = r_group_id;
                *(params+3) = 0x02;
				
                print("------device lum down\r\n");
               }
            break;
        case 6:{
                params = &cmd_temp_step[0];
                *params = r_group_id;
                *(params+3) = 0x01;
				
                print("------device temperature up\r\n");
               }
            break;
        case 7:{
                params = &cmd_temp_step[0];
                *params = r_group_id;
                *(params+3) = 0x02;
				
                print("------device remperature down\r\n");
               }
            break;
        case 8:{
                params = &cmd_fav[0];
                *params = r_group_id;
				*(params+3) = 0x02;
                *(params+4) = 0x00;
               }
            break;
        case 9:{
                params = &cmd_fav[0];
                *params = r_group_id;
				*(params+3) = 0x02;
                *(params+4) = 0x01;
               }
            break;
        case 10:{
                params = &cmd_fav[0];
                *params = r_group_id;
				*(params+3) = 0x02;
                *(params+4) = 0x02;
               }
            break;
        case 11:{
                params = &cmd_fav[0];
                *params = r_group_id;
				*(params+3) = 0x02;
                *(params+4) = 0x03;
               }
            break;
        case 12:{
                r_group_id = 1;
                params = &cmd_onoff[0];
                *params = r_group_id;
                *(params+3) = r_onoff;
				r_onoff++;
                r_onoff = r_onoff%2;
                }
            break;
        case 13:{
                r_group_id = 2; 
                params = &cmd_onoff[0];
                *params = r_group_id;
                *(params+3) = r_onoff;
				r_onoff++;
                r_onoff = r_onoff%2;    
                }
            break;
        case 14:{
                r_group_id = 3;
                params = &cmd_onoff[0];
                *params = r_group_id;
                *(params+3) = r_onoff;
				r_onoff++;
                r_onoff = r_onoff%2;
                }
            break;
        case 15:{
                r_group_id = 4;
                params = &cmd_onoff[0];
                *params = r_group_id;
                *(params+3) = r_onoff;
				r_onoff++;
                r_onoff = r_onoff%2;
                }
            break;
        case 16:{
                params = &cmd_scene[0];
                *params = r_group_id;
                *(params+4) = r_scene_id;
				r_scene_id++;
                r_scene_id = r_scene_id%8;
                }
            break;
        case 17:{
				params = &cmd_color[0];
				*params = r_group_id;
				*(params+4) = r_color_id;
				r_color_id++;
				r_color_id = r_color_id%7;
				}
            break;
        case 18:{
				params = &cmd_temp_step_cycle[0];
				*params = r_group_id;
				}
            break;
        case 19:{
                r_scene_id = 2;
                params = &cmd_scene[0];
                *params = r_group_id;
                *(params+4) = r_scene_id;
                }
            break;
        case 20:{
                r_scene_id = 6;
                params = &cmd_scene[0];
                *params = r_group_id;
                *(params+4) = r_scene_id;
                }
            break;
        case 21:{
                r_scene_id = 7;
                params = &cmd_scene[0];
                *params = r_group_id;
                *(params+4) = r_scene_id;
                }
            break;
        case 22:{
                r_scene_id = 1;
                params = &cmd_scene[0];
                *params = r_group_id;
                *(params+4) = r_scene_id;
                }
            break;
        case 23:{
                r_scene_id = 3;
                params = &cmd_scene[0];
                *params = r_group_id;
                *(params+4) = r_scene_id;
                }
            break;
        case 24:{
                r_scene_id = 4;
                params = &cmd_scene[0];
                *params = r_group_id;
                *(params+4) = r_scene_id;
                }
            break;
        case 25:{
                r_scene_id = 5;
                params = &cmd_scene[0];
                *params = r_group_id;
                *(params+4) = r_scene_id;
                }
            break;
        default:
            return;
            break;
    }

	print("ctrl------------------:");
	for(u8 i=0;i<params_len;i++)
		print("%x ",params[i]);
	print("\r\n");
	
    ty_beacon_ctrl(cmd_type, params, params_len);
}

void app_remoter_long_press_start(kb_data_t* kb_data)
{
    u8 cmd_type = DEVICE_CMD_TYPE_CTRL;
    u8 *params;
    u8 params_len = 7;
    u8 pair0[3] = {0x02,0x00,0xff};
    u8 pair1[3] = {0x02,0x01,0xff};
    u8 pair2[3] = {0x02,0x02,0xff};
    u8 pair3[3] = {0x02,0x03,0xff};
    u8 pair4[3] = {0x02,0x04,0xff};
    switch(kb_data->cnt){
        case 1:{
                switch(kb_data->keycode[0]){
					case 3:{
							params = &cmd_timing[0];
							*params = r_group_id;
							u16 timing = 0;
							*(params+3) = (timing>>8)&0xFF;
							*(params+4) = timing&0xFF;
							
							r_timing_id = 0;
							led_blink_times = 5;//指示灯闪烁次数-1
							print("------device countdown\r\n");
							}
						break;
					case 4:
							{
							params = &cmd_lum_stepless[0];
							*params = r_group_id;
							*(params+3) = 0x00;
							*(params+6) = 0x64;
							print("------Brightness increase stepless adjustment\r\n");
							}
                        break;
					case 5:{
							params = &cmd_lum_stepless[0];
							*params = r_group_id;
							*(params+3) = 0x01;
							*(params+6) = 0x01;
							print("------Brightness decrease stepless adjustment\r\n");
							}
                        break;
					case 6:{
							params = &cmd_temp_stepless[0];
							*params = r_group_id;
							*(params+3) = 0x00;
							*(params+5) = 0x64;
							print("------Temperature increase stepless adjustment\r\n");
							}
                        break;
					case 7:{
							params = &cmd_temp_stepless[0];
							*params = r_group_id;
							*(params+3) = 0x01;
							*(params+5) = 0x00;
							print("------Temperature decrease stepless adjustment\r\n");
							}
                        break;
					case 8:{
							params = &cmd_fav[0];
							*params = r_group_id;
							*(params+3) = 0x01;
							*(params+4) = 0x00;
							led_blink_times = 3;//指示灯闪烁次数-1
						   }
						break;
					case 9:{
							params = &cmd_fav[0];
							*params = r_group_id;
							*(params+3) = 0x01;
							*(params+4) = 0x01;
							led_blink_times = 3;//指示灯闪烁次数-1
						   }
						break;
					case 10:{
							params = &cmd_fav[0];
							*params = r_group_id;
							*(params+3) = 0x01;
							*(params+4) = 0x02;
							led_blink_times = 3;//指示灯闪烁次数-1
						   }
						break;
					case 11:{
							params = &cmd_fav[0];
							*params = r_group_id;
							*(params+3) = 0x01;
							*(params+4) = 0x03;
							led_blink_times = 3;//指示灯闪烁次数-1
						   }
						break;
                    case 12:{
							cmd_type = DEVICE_CMD_TYPE_PAIR;
							params = &pair1[0];
							params_len = 3;
						
							print("------device 1 group pair\r\n");
							}
                        break;
                    case 13:{
							cmd_type = DEVICE_CMD_TYPE_PAIR;
							params = &pair2[0];
							params_len = 3;
						
							print("------device 2 group pair\r\n");
							}
                        break;
                    case 14:{
							cmd_type = DEVICE_CMD_TYPE_PAIR;
							params = &pair3[0];
							params_len = 3;
							
							print("------device 3 group pair\r\n");
							}
                        break;
                    case 15:{
							cmd_type = DEVICE_CMD_TYPE_PAIR;
							params = &pair4[0];
							params_len = 3;
						
							print("------device 4 group pair\r\n");
							}
                        break;
					case 17:{
							params = &cmd_hue_stepless[0];
							*params = r_group_id;
							*(params+3) = 0x03;
							print("------Hue stepless adjustment\r\n");
							}
                        break;
					case 18:{
							params = &cmd_temp_stepless_cycle[0];
							*params = r_group_id;
							*(params+3) = 0x03;
							print("------Temperature cycle stepless adjustment\r\n");
							}
                        break;
                    default:
                        return;
                        break;
                }
               }
            break;
        case 2:
            if(((4 == kb_data->keycode[0])&&(5 == kb_data->keycode[1]))||((5 == kb_data->keycode[0])&&(4 == kb_data->keycode[1]))){
                cmd_type = DEVICE_CMD_TYPE_UNPROV_RESET;
                params_len = 0;
				led_blink_times = 6;//指示灯闪烁次数-1
				
				print("------device proving\r\n");
            }else if(((1 == kb_data->keycode[0])&&(5 == kb_data->keycode[1]))||((5 == kb_data->keycode[0])&&(1 == kb_data->keycode[1]))){
                cmd_type = DEVICE_CMD_TYPE_PAIR;
                params = &pair0[0];
                *params = 0x03;
                params_len = 3;
				led_blink_times = 4;//指示灯闪烁次数-1
				print("------device all unpair\r\n");
            }else if(((12 == kb_data->keycode[0])&&(5 == kb_data->keycode[1]))||((5 == kb_data->keycode[0])&&(12 == kb_data->keycode[1]))){
                cmd_type = DEVICE_CMD_TYPE_PAIR;
                params = &pair1[0];
                *params = 0x03;
                params_len = 3;
				led_blink_times = 4;//指示灯闪烁次数-1
				print("------device 1 group unpair\r\n");
            }else if(((13 == kb_data->keycode[0])&&(5 == kb_data->keycode[1]))||((5 == kb_data->keycode[0])&&(13 == kb_data->keycode[1]))){
                cmd_type = DEVICE_CMD_TYPE_PAIR;
                params = &pair2[0];
                *params = 0x03;
                params_len = 3;
				led_blink_times = 4;//指示灯闪烁次数-1
				print("------device 2 group unpair\r\n");
            }else if(((14 == kb_data->keycode[0])&&(5 == kb_data->keycode[1]))||((5 == kb_data->keycode[0])&&(14 == kb_data->keycode[1]))){
                cmd_type = DEVICE_CMD_TYPE_PAIR;
                params = &pair3[0];
                *params = 0x03;
                params_len = 3;
				led_blink_times = 4;//指示灯闪烁次数-1
				print("------device 3 group unpair\r\n");
            }else if(((15 == kb_data->keycode[0])&&(5 == kb_data->keycode[1]))||((5 == kb_data->keycode[0])&&(15 == kb_data->keycode[1]))){
                cmd_type = DEVICE_CMD_TYPE_PAIR;
                params = &pair4[0];
                *params = 0x03;
                params_len = 3;
				led_blink_times = 4;//指示灯闪烁次数-1
				print("------device 4 group unpair\r\n");
            }else{
                return;
            }
            break;
        default:
            return;
            break;
    }
	print("ctrl------------------:");
	for(u8 i=0;i<params_len;i++)
		print("%x ",params[i]);
	print("\r\n");
    ty_beacon_ctrl(cmd_type, params, params_len);
}

void app_remoter_long_press_release(kb_data_t* kb_data)
{
    u8 cmd_type = DEVICE_CMD_TYPE_CTRL;
    u8 *params;
    u8 params_len = 7;
    switch(kb_data->cnt){
        case 1:{
                switch(kb_data->keycode[0]){
                    case 4:{
							params = &cmd_lum_stepless[0];
							*params = r_group_id;
							*(params+3) = 0x02;
							*(params+6) = 0x64;
							print("------Brightness increase stepless adjustment\r\n");
							}
                        break;
					case 5:{
							params = &cmd_lum_stepless[0];
							*params = r_group_id;
							*(params+3) = 0x02;
							*(params+6) = 0x01;
							print("------Brightness decrease stepless adjustment\r\n");
							}
                        break;
					case 6:{
							params = &cmd_temp_stepless[0];
							*params = r_group_id;
							*(params+3) = 0x02;
							*(params+5) = 0x64;
							print("------Temperature increase stepless adjustment\r\n");
							}
                        break;
					case 7:{
							params = &cmd_temp_stepless[0];
							*params = r_group_id;
							*(params+3) = 0x02;
							*(params+5) = 0x00;
							print("------Temperature decrease stepless adjustment\r\n");
							}
                        break;
					case 17:{
							params = &cmd_hue_stepless[0];
							*params = r_group_id;
							*(params+3) = 0x02;
							print("------Hue stepless adjustment\r\n");
							}
                        break;
					case 18:{
							params = &cmd_temp_stepless_cycle[0];
							*params = r_group_id;
							*(params+3) = 0x02;
							print("------Temperature cycle stepless adjustment\r\n");
							}
                        break;
                    default:
                        return;
                        break;
                }
               }
            break;
        default:
            return;
            break;
    }
	print("ctrl------------------:");
	for(u8 i=0;i<params_len;i++)
		print("%x ",params[i]);
	print("\r\n");
    ty_beacon_ctrl(cmd_type, params, params_len);
}


void ty_beacon_key_event(PressEvent key_state, kb_data_t* kb_data)
{
    print("key state=%d \r\n",key_state);
    print("key cnt=%d \r\n",kb_data->cnt);
    print("keycode:");
	for(u8 i=0;i<5;i++)
		print("%x ",kb_data->keycode[i]);
	print("\r\n");

    switch(key_state){
        case SINGLE_CLICK:
			led_blink_times = 1;//指示灯闪烁次数
            app_remoter_single_click(kb_data);
            break;
        case DOUBLE_CLICK:
            break;
        case LONG_PRESS_START:
			led_blink_times = 1;//指示灯闪烁次数		
            app_remoter_long_press_start(kb_data);
            break;
        case LONG_PRESS_RELEASE:
            app_remoter_long_press_release(kb_data);
            break;
        default:
            break;
    }
}

