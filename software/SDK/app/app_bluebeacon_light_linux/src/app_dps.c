#include "app_dps.h"
#include "ty_beacon.h"

/// RED gamma 0.8 ~ 100%
//static uint8_t ucGammaRed[] = {
//						0,  0,  1,  1,  1,  2,  2,  3,  3,  4,  4,  5,  6,  6,  7,  7,  8,  9,  9,  10,
//						11, 11, 12, 13, 13, 14, 15, 15, 16, 17, 18, 18, 19, 20, 21, 21, 22, 23, 24, 24,
//						25, 26, 27, 28, 28, 29, 30, 31, 32, 32, 33, 34, 35, 36, 37, 37, 38, 39, 40, 41,
//						42, 43, 44, 44, 45, 46, 47, 48, 49, 50, 51, 52, 52, 53, 54, 55, 56, 57, 58, 59,
//						60, 61, 62, 63, 64, 65, 66, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
//						79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
//						99, 100,101,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
//						121,122,123,124,125,126,127,128,129,130,131,132,134,135,136,137,138,139,140,141,
//						142,144,145,146,147,148,149,150,151,152,154,155,156,157,158,159,160,162,163,164,
//						165,166,167,168,170,171,172,173,174,175,177,178,179,180,181,182,184,185,186,187,
//						188,189,191,192,193,194,195,196,198,199,200,201,202,204,205,206,207,208,210,211,
//						212,213,214,216,217,218,219,220,222,223,224,225,227,228,229,230,231,233,234,235,
//						236,238,239,240,241,243,244,245,246,248,249,250,251,253,254,255
//};
//
///// GREEN gamma 0.6 ~ 70%
//static uint8_t ucGammaGreen[] = {
//						0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,
//						3,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  6,  6,  6,  7,  7,  7,  7,  8,
//						8,  8,  9,  9, 10, 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16,
//						16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25,
//						26, 26, 27, 27, 28, 29, 29, 30, 30, 31, 31, 32, 33, 33, 34, 34, 35, 36, 36, 37,
//						38, 38, 39, 39, 40, 41, 41, 42, 43, 43, 44, 45, 45, 46, 47, 47, 48, 49, 49, 50,
//						51, 52, 52, 53, 54, 54, 55, 56, 57, 57, 58, 59, 60, 60, 61, 62, 63, 63, 64, 65,
//						66, 66, 67, 68, 69, 70, 70, 71, 72, 73, 74, 75, 75, 76, 77, 78, 79, 80, 80, 81,
//						82, 83, 84, 85, 86, 86, 87, 88, 89, 90, 91, 92, 93, 94, 94, 95, 96, 97, 98, 99,
//						100,101,102,103,104,105,106,106,107,108,109,110,111,112,113,114,115,116,117,118,
//						119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,139,
//						140,141,142,143,144,145,146,147,148,149,150,151,152,154,155,156,157,158,159,160,
//						161,162,164,165,166,167,168,169,170,172,173,174,175,176,177,179
//};
//
////BLUE gama 0.6-75%
//static uint8_t ucGammaBlue[] = {
//						0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  3,
//						3,  3,  3,  3,  4,  4,  4,  5,  5,  5,  5,  6,  6,  6,  7,  7,  7,  8,  8,  8,
//						9,  9,  9, 10, 10, 11, 11, 11, 12, 12, 13, 13, 14, 14, 14, 15, 15, 16, 16, 17,
//						17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 27, 27,
//						28, 28, 29, 29, 30, 31, 31, 32, 32, 33, 34, 34, 35, 36, 36, 37, 38, 38, 39, 40,
//						40, 41, 42, 42, 43, 44, 44, 45, 46, 46, 47, 48, 49, 49, 50, 51, 51, 52, 53, 54,
//						54, 55, 56, 57, 58, 58, 59, 60, 61, 61, 62, 63, 64, 65, 65, 66, 67, 68, 69, 70,
//						70, 71, 72, 73, 74, 75, 76, 76, 77, 78, 79, 80, 81, 82, 83, 83, 84, 85, 86, 87,
//						88, 89, 90, 91, 92, 93, 94, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,
//						107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,127,
//						128,129,130,131,132,133,134,135,136,137,138,139,141,142,143,144,145,146,147,148,
//						150,151,152,153,154,155,156,158,159,160,161,162,163,165,166,167,168,169,170,172,
//						173,174,175,176,178,179,180,181,183,184,185,186,188,189,190,191
//};
#pragma pack (1)
typedef struct{//svae[56:8]
	u8 onoff;//1
	u8 scene_mode;//0
	u16 H;//0
	u16 S;//0
	u16 V;//1000
}light_value_s;
#pragma pack () 
light_value_s light_value;
//reset_cnt   [64]
int16_t music_h,music_s,music_v;//music mode not storage
#define SCENE_NORMAL 0
#define SCENE_MODEL1 1
#define SCENE_MODEL2 2
#define SCENE_MODEL3 3
#define SCENE_MODEL4 4
#define SCENE_MODEL5 5
#define SCENE_MODEL6 6
#define SCENE_MODEL7 7
#define SCENE_MODEL8 8
#define SCENE_MODEL9 9
#define SCENE_MUSIC  10
#define SCENE_BLINK  11

#define RESET_MAX_CNT 3

void app_light_reset_run(void){
	static u8 reset_judge_time = 1; 
    static u32 old_tick = 0;
	u8 cnt;
					
	if(ty_beacon_get_state() == DEVICE_STATE_PAIRED){
		if(reset_judge_time != 0){//300ms~6S
			if(hal_clock_time_exceed(old_tick,reset_judge_time*300000)){//300MS
                old_tick = hal_clock_get_system_tick();

				if(reset_judge_time == 1){
					reset_judge_time = 20;
					if(0 == hal_storage_256_read_bytes(64, &cnt,1)){//success
						if(cnt > RESET_MAX_CNT)cnt = 0;
					}else{
						cnt = 0;
					}
					
					cnt++;
					if(cnt >= RESET_MAX_CNT){
						cnt = 0;
						ty_beacon_start_pairing();
						reset_judge_time = 0;
					}
					hal_storage_256_write_bytes(64, &cnt,1);
				}else if(reset_judge_time == 20){
					reset_judge_time = 0;
					cnt = 0;
					hal_storage_256_write_bytes(64, &cnt,1);
				}
			}
		}
	}
}

void app_dps_read_from_storage(void){
	if(0 == hal_storage_256_read_bytes(56, (u8 *)(&light_value),sizeof(light_value_s))){//success
		light_value.onoff = 1;//默认重启亮
		if(light_value.scene_mode > SCENE_MUSIC)
			light_value.scene_mode = SCENE_NORMAL;
		if(light_value.H > 360 || light_value.H < 0){
			light_value.H = 0;//360 1000 1000
		}
		if(light_value.S > 1000 || light_value.S < 0){
			light_value.S = 0;//360 1000 1000
		}
		if(light_value.V > 1000 || light_value.V <= 0){
			light_value.V = 1000;//360 1000 1000
		}
	}else{
		light_value.onoff = 1;
		light_value.scene_mode = SCENE_NORMAL;
		light_value.H = 0;
		light_value.S = 0;
		light_value.V = 1000;
	}
}

void app_dps_write_to_storage(){
	static light_value_s pre_light_value = {1,0,0,0,1000};
	if(0 == memcmp((u8 *)(&pre_light_value),(u8 *)(&light_value),sizeof(light_value_s)))return;
	memcpy((u8 *)(&pre_light_value),(u8 *)(&light_value),sizeof(light_value_s));
	hal_storage_256_write_bytes(56, (u8 *)(&pre_light_value),sizeof(light_value_s));
}

uint32_t ir_code = 0;
uint8_t ir_code_get = 0;

uint32_t countdown = 0;//s 1day=86400
uint8_t countdown_target_onoff = 0;

typedef struct _tagTColor{
    int R;
    int G;
    int B;
}TColor;


TColor HSV2RGB(int h,int s,int v)
{
	int i;
    float RGB_min, RGB_max;
    float RGB_Adj;
    int difs;
	u16 R,G,B;
	u16 *r=&R,*g=&G,*b=&B;

    if(h >= 360) {
        h = 0;
    }
    
    RGB_max = v * 1.0f;
    RGB_min = RGB_max*(1000 - s) / 1000.0f;

    i = h / 60;
    difs = h % 60; /* factorial part of h */


    /* RGB adjustment amount by hue */
    RGB_Adj = (RGB_max - RGB_min)*difs / 60.0f;

    switch (i) {
        case 0:
            *r = (u16)RGB_max;
            *g = (u16)(RGB_min + RGB_Adj);
            *b = (u16)RGB_min;
            break;
            
        case 1:
            *r = (u16)(RGB_max - RGB_Adj);
            *g = (u16)RGB_max;
            *b = (u16)RGB_min;
            break;
            
        case 2:
            *r = (u16)RGB_min;
            *g = (u16)RGB_max;
            *b = (u16)(RGB_min + RGB_Adj);
            break;
            
        case 3:
            *r = (u16)RGB_min;
            *g = (u16)(RGB_max - RGB_Adj);
            *b = (u16)RGB_max;
            break;
            
        case 4:
            *r = (u16)(RGB_min + RGB_Adj);
            *g = (u16)RGB_min;
            *b = (u16)RGB_max;
            break;
            
        default:        // case 5:
            *r = (u16)RGB_max;
            *g = (u16)RGB_min;
            *b = (u16)(RGB_max - RGB_Adj);
            break;
    }

    if(*r > 1000) {
        *r = 1000;
    }

    if(*g > 1000) {
        *g = 1000;
    }

    if(*b > 1000) {
        *b = 1000;
    }
	
	TColor color;
	color.R = *r/10*255/100;
	color.G = *g/10*255/100;
	color.B = *b/10*255/100;
	return color;
}

void light_adjust_RGBWC_hw(u8 R, u8 G,u8 B,u8 W, u8 C, u8 L){
    print("light_adjust_rgb_hw:r=%d,g=%d,b=%d,w=%d,c=%d,l=%d\r\n",R,G,B,W,C,L);
}
//360 1000 1000
void light_adjust_HSV_hw(u16 hue,u16 saturation,u16 value){
	static u16 pre_h=0,pre_s=0,pre_v=0;
	if(pre_h == hue && pre_s == saturation && pre_v == value)return;
	pre_h = hue;
	pre_s = saturation;
	pre_v = value;
			
	TColor color = HSV2RGB(hue,saturation,value);
	light_adjust_RGBWC_hw(color.R,color.G,color.B,0,0,0);
}


void scene_run(void){
	static u8 mode = 0;
	static u32 T = 0;
	u8 scene = light_value.scene_mode;

	switch(scene){
		case SCENE_MODEL1:
		light_value.H=0,light_value.S=0,light_value.V=100;
		break;
		case SCENE_MODEL2:
		light_value.H=0,light_value.S=0,light_value.V=500;
		break;
		case SCENE_MODEL3:
		light_value.H=0,light_value.S=0,light_value.V=700;
		break;
		case SCENE_MODEL4:
		light_value.H=0,light_value.S=0,light_value.V=1000;
		break;
		case SCENE_MODEL5:
			if(hal_clock_time_exceed(T,20000)){
				mode++;
				if(mode > 62)mode = 0;
				T = hal_clock_get_system_tick();
				if(mode > 31)
					light_value.V=(62-mode)*(62-mode);
				else
					light_value.V=mode*mode;
					
			}
			light_value.H=120,light_value.S=1000;
		break;
		case SCENE_MODEL6:
			if(hal_clock_time_exceed(T,400000)){
				T = hal_clock_get_system_tick();
				if(mode >2)mode = 0;
				light_value.H=mode*120;
				mode++;
			}
			break;
		case SCENE_MODEL7:
			if(hal_clock_time_exceed(T,20000)){
				T = hal_clock_get_system_tick();
				light_value.H++;
				if(light_value.H > 359)light_value.H=0;
			}
		break;
		case SCENE_MODEL8:
			if(hal_clock_time_exceed(T,20000)){
				T = hal_clock_get_system_tick();
				light_value.H++;
				if(light_value.H > 359)light_value.H=0;
			}
		break;
		case SCENE_MODEL9:
			if(hal_clock_time_exceed(T,400000)){
				T = hal_clock_get_system_tick();
				
				if(mode>5)mode = 0;
				light_value.H=mode*60;
				mode++;
				//0,120,240,60,300,180
			}
		break;
		default:
		break;
	}

	if(light_value.onoff == 0){
        light_adjust_HSV_hw(0,0,0);
    }else{
        if(scene == SCENE_MUSIC || scene == SCENE_BLINK)
			light_adjust_HSV_hw(music_h,music_s,music_v);
		else
			light_adjust_HSV_hw(light_value.H,light_value.S,light_value.V);       
    }
}

void set_onoff(u8 state){
    light_value.onoff = state;
	if(countdown !=0 && countdown_target_onoff == state){
		countdown = 0;
	}
}

void ty_beacon_event_cb(ty_beacon_event_e e, void *params){}
void ty_beacon_enter_test_mode_cb(u8 rssi){}


u8 app_dps_upload(unsigned char dpid, unsigned char dpty, unsigned char dplen, unsigned char *dpvalue){
	if(dplen > 7)return 0;
	u8 params[10];
	params[0] = dpid;
	params[1] = dpty;
	params[2] = dplen;
	memcpy(&params[3],dpvalue,dplen);
	ty_beacon_send(300000, 20,  TY_BEACON_CMD_UPLOAD_DP, params, dplen+3, 0);//300ms sned 20 times,again use ty_beacon_send will break pre send
	return 1;
}

void app_dps_download(unsigned char dpid, unsigned char dpty, unsigned char dplen, unsigned char *dpvalue){
    print("dp:%d [%d]\r\n",dpid,dpvalue[0]);
	switch(dpid){
        case 0x01://onoff
			light_value.scene_mode = SCENE_NORMAL;
			light_value.onoff = dpvalue[0];
			app_dps_upload(dpid,dpty,dplen,dpvalue);
            break;
        case 12:
			light_value.S=1000,light_value.V=1000;
            light_value.scene_mode = dpvalue[0]+1;
			app_dps_upload(dpid,dpty,dplen,dpvalue);
            break;
		case 11:{//HSV
			light_value.H = ((dpvalue[0]<<8) | dpvalue[1]);//360
			light_value.S = dpvalue[2] * 10;//1000
			light_value.V = dpvalue[3] * 10;//1000
			light_value.scene_mode = SCENE_NORMAL;
			app_dps_upload(dpid,dpty,dplen,dpvalue);
			}
			break;
		case 0x07:{//count down
			//uint32_t count
			countdown = (dpvalue[0]<<24) | (dpvalue[1]<<16) | (dpvalue[2]<<8) | (dpvalue[3]);  
			countdown_target_onoff = !light_value.onoff;
			app_dps_upload(dpid,dpty,dplen,dpvalue);
			}
			break;
        case 13://music
    		music_h = ((dpvalue[1]<<8) | dpvalue[2]);//360
			music_s = dpvalue[3] * 10;//1000
			music_v = dpvalue[4] * 10;//1000
			light_value.scene_mode = SCENE_MUSIC;
            break;
        default:
            break;
    }
}

void app_ir_cmd(unsigned int code){
}

void app_light_init(void){
	app_dps_read_from_storage();
}

void app_light_run(void){
	static u32 T = 0;
	
	if(ty_beacon_get_state() == DEVICE_STATE_PAIRING){
		if(hal_clock_time_exceed(T,1000000)){
			light_value.scene_mode = SCENE_BLINK;
			light_value.onoff = 1;
			music_h = 0;
			music_s = 0;
			music_v = music_v == 0?1000:0;
			T = hal_clock_get_system_tick();
		}
	}else{
		if(light_value.scene_mode == SCENE_BLINK)
			light_value.scene_mode = SCENE_NORMAL;
			
		if(hal_clock_time_exceed(T,8000000)){
			app_dps_write_to_storage();
			T = hal_clock_get_system_tick();
		}
	
		if(countdown != 0){
			if(hal_clock_time_exceed(T,1000000)){
				countdown--;
				print("countdown=%d\r\n",countdown);
				if(countdown == 0){
					set_onoff(countdown_target_onoff);
				}
				if(countdown%60 == 0){//1min upload
					u8 dpvalue[4];
					dpvalue[0] = (countdown >> 24) & 0xFF;
					dpvalue[1] = (countdown >> 16) & 0xFF;
					dpvalue[2] = (countdown >> 8) & 0xFF;
					dpvalue[3] = (countdown >> 0) & 0xFF;
					app_dps_upload(0x07,0x02,4,dpvalue);
				}
				T = hal_clock_get_system_tick();
			}
		}
	}
	scene_run();
    app_light_reset_run();
}
