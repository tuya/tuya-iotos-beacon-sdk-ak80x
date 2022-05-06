#include "app_dps.h"
#include "board.h"
#include "stdlib.h"
#include "pwm.h"
#include "gpio.h"
#include "app_select.h"
#include "print.h"
#include "ty_beacon.h"
#include "app_ir.h"

#include "ty_rtc.h"
#include "ty_timing.h"
#include "ty_light_driver_bp5758d.h"

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

// #define SCENE_MODEL0	0
// #define SCENE_MODEL3	3
// #define SCENE_MODEL4	4
// #define SCENE_MODEL7	7
// #define SCENE_MODEL24	24
// #define SCENE_MODEL28	28	
#define SCENE_MODEL0	0
#define SCENE_MODEL3	3
#define SCENE_MODEL4	4
#define SCENE_MODEL7	7
#define SCENE_MODEL24	24
#define SCENE_MODEL28	28

uint8_t sence_data1[24]={0x00,0x64,0x03,0xe8,0x03,0xe8,\
						0x00,0x6e,0x03,0xe8,0x03,0xe8,\
						0x00,0x78,0x03,0xe8,0x03,0xe8,\
						0x00,0x82,0x03,0xe8,0x03,0xe8};
							
uint8_t sence_data2[36]={0x00,0x00,0x03,0xe8,0x03,0xe8,\
						 0x00,0x78,0x03,0xe8,0x03,0xe8,\
						 0x00,0xf0,0x03,0xe8,0x03,0xe8,\
						 0x00,0x3d,0x03,0xe8,0x03,0xe8,\
						 0x00,0xae,0x03,0xe8,0x03,0xe8,\
						 0x01,0x13,0x03,0xe8,0x03,0xe8};
							
uint8_t sence_data3[18]={0x00,0x28,0x03,0x20,0x03,0xe8,\
						 0x00,0x1e,0x03,0x84,0x03,0xe8,\
					     0x00,0x14,0x03,0x84,0x03,0xe8};	

uint8_t sence_data4[18]={0x01,0x04,0x03,0x20,0x03,0xe8,\
						 0x01,0x18,0x02,0xbc,0x03,0xe8,\
					     0x01,0x13,0x03,0xe8,0x03,0xe8};

typedef enum {
    SCENE_WHITE = 0x00, // 白光模式
    SCENE_COLOR,        // 彩光模式
    SCENE_MODEL,        // 场景模式
    SCENE_MUSIC,        // 音乐律动模式
    SCENE_BLINK,        // 闪烁模式
    SCENE_MAX,
}DRIVER_SCENE_MODE_E;

#pragma pack (1)
typedef struct{//save[56:8]
	uint8_t onoff;//1
	uint8_t scene_mode;//0
	int16_t H;//0
	int16_t S;//0
	int16_t V;//1000
	int16_t B;//0
	int16_t T;//1000
	uint8_t light_disturb_flag;
	uint8_t sence_model;
	uint8_t data_right_flag;	
}light_value_s;


typedef struct 
{
	bool bFirstChange;
	uint8_t ucSceneUnit;
    uint8_t ucUnitCnt;
	uint8_t uiGainTemp;
    BP5758D_LIGHT_CONTRL_T tTargetVal;
	BP5758D_LIGHT_CONTRL_T tCurrVal;
}LIGHT_CTRL_HANDLE_T;


#pragma pack () 

light_value_s light_value;
LIGHT_CTRL_HANDLE_T sence_ctrl_data;
//reset_cnt   [64]
int16_t music_h = 0,music_s = 1000,music_v = 1000;//music mode not storage，default color is red
bool light_disturb_flag_temp = 0;
//uint8_t sence_model_unit = 0;
bool bSceneSetFirstFlag = TRUE;
//bool bSceneStopFlag = FALSE;

bool light_shade_flag;
// #define SCENE_COLOR 0
// #define SCENE_MODEL1 1
// #define SCENE_MODEL2 2
// #define SCENE_MODEL3 3
// #define SCENE_MODEL4 4
// #define SCENE_MODEL5 5
// #define SCENE_MODEL6 6
// #define SCENE_MODEL7 7
// #define SCENE_MODEL8 8
// #define SCENE_MODEL9 9
// #define SCENE_MUSIC  10
// #define SCENE_WHITE  11
// #define SCENE_BLINK  12

uint32_t countdown = 0;//s 1day=86400
uint8_t countdown_target_onoff = 0;

u32 store_T = 0;
bool store_flag = 1;

#define RESET_MAX_CNT 5

void app_dps_write_to_storage();

void app_light_write_to_storage_start(void){
	if(store_flag == 0){
		store_T = hal_clock_get_system_tick();
		store_flag = 1;
	}
}


void app_light_reset_run(void){
	static u8 reset_judge_time = 3; 
	u8 cnt;
					
	if(ty_beacon_get_state() == DEVICE_STATE_PAIRED){
		if(reset_judge_time != 0){
			if(hal_clock_time_exceed(0,reset_judge_time*100000)){//300ms and 4s
				if(reset_judge_time == 3){
					reset_judge_time = 60;
					if(0 == hal_storage_256_read_bytes(112, &cnt,1)){//success
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
					hal_storage_256_write_bytes(112, &cnt,1);
				}else if(reset_judge_time == 60){
					reset_judge_time = 0;
					cnt = 0;
					hal_storage_256_write_bytes(112, &cnt,1);
				}
			}
		}
	}
}



static uint8_t reset_cnt;
uint8_t app_light_ctrl_data_reset_cnt_get(void)
{
	if(0 == hal_storage_256_read_bytes(112, &reset_cnt,1)){//success
		if(reset_cnt > RESET_MAX_CNT)reset_cnt = 0;
	}else{
		reset_cnt = 0;
	}
	return reset_cnt;
}

void app_light_ctrl_data_not_disturb_set(bool bNotDisturb)
{
    light_value.light_disturb_flag = bNotDisturb;
}


bool app_light_ctrl_data_not_disturb_get(void)
{
    return light_disturb_flag_temp;
}


void app_light_ctrl_data_onoff_set(bool state)
{
    light_value.onoff = state;
}



void app_light_def_state_init(void){
	light_value.onoff = 1;
	light_value.scene_mode = SCENE_COLOR;
	light_value.H = 0;
	light_value.S = 1000;
	light_value.V = 1000;
	light_value.B = 1000;
	light_value.T = 1000;
	light_value.light_disturb_flag = 0;
	light_disturb_flag_temp = 0;	
	light_value.sence_model = SCENE_MODEL0;
	light_value.data_right_flag = 0xA5;	

	mITimingFunc.init(); 
	countdown = 0;

	app_dps_write_to_storage();	
}



void app_dps_read_from_storage(void){
	if((0 == hal_storage_256_read_bytes(96, (u8 *)(&light_value),sizeof(light_value_s)))&&(light_value.data_right_flag == 0xA5)){//success
	
		if((light_value.light_disturb_flag == 1)&&(light_value.onoff == 0)){			
			light_disturb_flag_temp = 1;
		}
		
		if(light_value.light_disturb_flag > 1){
			light_value.light_disturb_flag = 0;
		}
		
		light_value.onoff = 1;//默认重启亮
		if(light_value.scene_mode >= SCENE_MAX){
			light_value.scene_mode = SCENE_COLOR;			
		}
		if(light_value.H > 360 || light_value.H < 0){
			light_value.H = 0;//360 1000 1000
		}
		if(light_value.S > 1000 || light_value.S < 0){
			light_value.S = 1000;//360 1000 1000
		}
		if(light_value.V > 1000 || light_value.V <= 0){
			light_value.V = 1000;//360 1000 1000
		}
		if(light_value.B > 1000 || light_value.B <= 0){
			light_value.B = 1000;//1000 1000
		}
		if(light_value.T > 1000 || light_value.T < 0){
			light_value.T = 1000;//1000 1000
		}	

	}else{
		app_light_def_state_init();			
	}
}

void app_dps_write_to_storage(){
	static light_value_s pre_light_value = {1,1,0,1000,1000,1000,1000,0,0,0xA5};
	if(0 == memcmp((u8 *)(&pre_light_value),(u8 *)(&light_value),sizeof(light_value_s)))return;
	memcpy((u8 *)(&pre_light_value),(u8 *)(&light_value),sizeof(light_value_s));
	hal_storage_256_write_bytes(96, (u8 *)(&pre_light_value),sizeof(light_value_s));
}

uint32_t ir_code = 0;
uint8_t ir_code_get = 0;



typedef struct _tagTColor{
    int R;
    int G;
    int B;
}TColor;

void light_adjust_RGBWC_hw(unsigned char red,unsigned char green,unsigned char blue,unsigned char warm,unsigned char white,unsigned char lum)
{
	static uint8_t r=0,g=0,b=0,c=0,w=0;
	if(r==red && g==green && b==blue && c==white && w==warm)
		return;
	
	//print("R=%d G=%d B=%d",red,green,blue);
	r=red,g=green,b=blue,c=white,w=warm;
	uint16_t pwm_section=PWM_FRQ_SELECTION/255;
	
	pwm_set_duty(PWM_CHANNEL_RGB_R, red*pwm_section);
	pwm_set_duty(PWM_CHANNEL_RGB_G, green*pwm_section);
	pwm_set_duty(PWM_CHANNEL_RGB_B, blue*pwm_section);

	pwm_set_duty(PWM_CHANNEL_RGB_C, white*pwm_section);
	pwm_set_duty(PWM_CHANNEL_RGB_W, warm*pwm_section);	
}

TColor HSV2RGB(int h,int s,int v)
{
	int i;
    float RGB_min, RGB_max;
    float RGB_Adj;
    int difs;
	uint16_t R,G,B;
	uint16_t *r=&R,*g=&G,*b=&B;

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
            *r = (uint16_t)RGB_max;
            *g = (uint16_t)(RGB_min + RGB_Adj);
            *b = (uint16_t)RGB_min;
            break;
            
        case 1:
            *r = (uint16_t)(RGB_max - RGB_Adj);
            *g = (uint16_t)RGB_max;
            *b = (uint16_t)RGB_min;
            break;
            
        case 2:
            *r = (uint16_t)RGB_min;
            *g = (uint16_t)RGB_max;
            *b = (uint16_t)(RGB_min + RGB_Adj);
            break;
            
        case 3:
            *r = (uint16_t)RGB_min;
            *g = (uint16_t)(RGB_max - RGB_Adj);
            *b = (uint16_t)RGB_max;
            break;
            
        case 4:
            *r = (uint16_t)(RGB_min + RGB_Adj);
            *g = (uint16_t)RGB_min;
            *b = (uint16_t)RGB_max;
            break;
            
        default:        // case 5:
            *r = (uint16_t)RGB_max;
            *g = (uint16_t)RGB_min;
            *b = (uint16_t)(RGB_max - RGB_Adj);
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
	
	static TColor color;
//	color.R = *r/10*255/100;
//	color.G = *g/10*255/100;
//	color.B = *b/10*255/100;
	color.R = *r;
	color.G = *g;
	color.B = *b;	
	return color;
}










void ty_light_gradual_ctrl_cw(IN u16 usBright, IN u16 usTemperatue)
{
	
	if(usBright < 10){
		sence_ctrl_data.tTargetVal.usRed = 0;
		sence_ctrl_data.tTargetVal.usGreen = 0;
		sence_ctrl_data.tTargetVal.usBlue = 0;
		
		sence_ctrl_data.tTargetVal.usCold = 0;
		sence_ctrl_data.tTargetVal.usWarm = 0;			
	}else{
		usBright = (usBright-10)*95/99 + 50;//5%——100%

		sence_ctrl_data.tTargetVal.usRed = 0;
		sence_ctrl_data.tTargetVal.usGreen = 0;
		sence_ctrl_data.tTargetVal.usBlue = 0;
		
		sence_ctrl_data.tTargetVal.usCold = (u16)((u32)usBright * usTemperatue / 1000);
		sence_ctrl_data.tTargetVal.usWarm = usBright - sence_ctrl_data.tTargetVal.usCold ;		
	}
	
	ty_light_driver_bp5758d_rgbcw(&sence_ctrl_data.tTargetVal); 	
}





//360 1000 1000
void light_adjust_HSV_hw(uint16_t hue,uint16_t saturation,uint16_t value){

	if(value < 10){
		sence_ctrl_data.tTargetVal.usRed = 0;
		sence_ctrl_data.tTargetVal.usGreen = 0;
		sence_ctrl_data.tTargetVal.usBlue = 0;
		
		sence_ctrl_data.tTargetVal.usCold = 0;
		sence_ctrl_data.tTargetVal.usWarm = 0;			
	}else{	
		value = (value-10)*90/99 + 100;//10%——100%
		
		TColor color = HSV2RGB(hue,saturation,value);

		sence_ctrl_data.tTargetVal.usWarm = 0;
		sence_ctrl_data.tTargetVal.usCold = 0;
		
		sence_ctrl_data.tTargetVal.usRed = color.R;
		sence_ctrl_data.tTargetVal.usGreen = color.G;
		sence_ctrl_data.tTargetVal.usBlue = color.B;
	}

//		u8 temmpppp[13]={0xAA,(u8)(hue>>8),(u8)hue,(u8)(saturation>>8),(u8)saturation,(u8)(value>>8),(u8)value,\
//		(u8)(sence_ctrl_data.tTargetVal.usRed>>8),(u8)sence_ctrl_data.tTargetVal.usRed,\
//		(u8)(sence_ctrl_data.tTargetVal.usGreen>>8),(u8)sence_ctrl_data.tTargetVal.usGreen,\
//		(u8)(sence_ctrl_data.tTargetVal.usBlue>>8),(u8)sence_ctrl_data.tTargetVal.usBlue,\
//		};
//		for(u8 j=0;j<13;j++){
//			uart_putc(temmpppp[j]);		
//		}
	
	ty_light_driver_bp5758d_rgbcw(&sence_ctrl_data.tTargetVal); 
}

void light_set_off(void){
	BP5758D_LIGHT_CONTRL_T light_temp;	
	light_temp.usWarm = 0;
	light_temp.usCold = 0;
	
	light_temp.usRed = 0;
	light_temp.usGreen = 0;
	light_temp.usBlue = 0;	
	ty_light_driver_bp5758d_rgbcw(&light_temp); 
}

static uint16_t uiLightToolGetABSValue(int16_t value)
{
    return (value > 0 ? value : -value);
}

static uint16_t uiLightToolGetMaxValue(uint16_t a, uint16_t b, uint16_t c)
{
    uint16_t x = a > b ? a : b;
    uint16_t y = b > c ? b : c;
    uint16_t z = x > y ? x : y;

    return z;
}

static void light_ctrl_shade_gradually(void)
{
//	uint8_t i =0;
//    uint16_t iError[3];
//	static uint16_t first_iError[3];
//    uint16_t usMaxError = 0;
//    uint16_t uiStep[3];
//
//	
//    if( memcmp(&sence_ctrl_data.tTargetVal, &sence_ctrl_data.tCurrVal, sizeof(BP5758D_LIGHT_CONTRL_T)) != 0 ) {  /* exist error */
//        
//        /* calulate the error between current value and target value */
//		if(sence_ctrl_data.tTargetVal.usRed > sence_ctrl_data.tCurrVal.usRed){
//			iError[0] = sence_ctrl_data.tTargetVal.usRed - sence_ctrl_data.tCurrVal.usRed;
//		}else{
//			iError[0] =sence_ctrl_data.tCurrVal.usRed - sence_ctrl_data.tTargetVal.usRed;
//		}
//
//		if(sence_ctrl_data.tTargetVal.usGreen > sence_ctrl_data.tCurrVal.usGreen){
//			iError[1] = sence_ctrl_data.tTargetVal.usGreen - sence_ctrl_data.tCurrVal.usGreen;
//		}else{
//			iError[1] =sence_ctrl_data.tCurrVal.usGreen - sence_ctrl_data.tTargetVal.usGreen;
//		}
//
//		if(sence_ctrl_data.tTargetVal.usBlue > sence_ctrl_data.tCurrVal.usBlue){
//			iError[2] = sence_ctrl_data.tTargetVal.usBlue - sence_ctrl_data.tCurrVal.usBlue;
//		}else{
//			iError[2] =sence_ctrl_data.tCurrVal.usBlue - sence_ctrl_data.tTargetVal.usBlue;
//		}
//
//		
//		//TUYA_APP_LOG_DEBUG("iError: %d %d %d",iError[0],iError[1],iError[2]);
//		
//        usMaxError = uiLightToolGetMaxValue(iError[0],iError[1],iError[2]);		
//
//        if( TRUE == sence_ctrl_data.bFirstChange) {    /* calculate change scale */
//			for(i=0;i<3;i++){
//				first_iError[i] = iError[i];
//			}
//            sence_ctrl_data.bFirstChange = FALSE;
//        }
//
//				
//		for(i=0;i<3;i++)
//		{
//			if( usMaxError == iError[i] ) {
//					uiStep[i] = sence_ctrl_data.uiGainTemp;
//			} else {
//					uiStep[i] = first_iError[i] * sence_ctrl_data.uiGainTemp / usMaxError;
//			}
//		}
//
//
//		if(sence_ctrl_data.tTargetVal.usRed > sence_ctrl_data.tCurrVal.usRed){
//			if( iError[0] < uiStep[0] ) {
//				sence_ctrl_data.tCurrVal.usRed += iError[0];
//			}else{
//				sence_ctrl_data.tCurrVal.usRed += uiStep[0];
//			}
//		}else{
//			if( iError[0] < uiStep[0] ) {
//				sence_ctrl_data.tCurrVal.usRed -= iError[0];
//			}else{
//				sence_ctrl_data.tCurrVal.usRed -= uiStep[0];
//			}
//		}
//
//		if(sence_ctrl_data.tTargetVal.usGreen > sence_ctrl_data.tCurrVal.usGreen){
//			if( iError[1] < uiStep[1] ) {
//				sence_ctrl_data.tCurrVal.usGreen += iError[1];
//			}else{
//				sence_ctrl_data.tCurrVal.usGreen += uiStep[1];
//			}
//		}else{
//			if( iError[1] < uiStep[1] ) {
//				sence_ctrl_data.tCurrVal.usGreen -= iError[1];
//			}else{
//				sence_ctrl_data.tCurrVal.usGreen -= uiStep[1];
//			}
//		}
//
//		if(sence_ctrl_data.tTargetVal.usBlue > sence_ctrl_data.tCurrVal.usBlue){
//			if( iError[2] < uiStep[2] ) {
//				sence_ctrl_data.tCurrVal.usBlue += iError[2];
//			}else{
//				sence_ctrl_data.tCurrVal.usBlue += uiStep[2];
//			}
//		}else{
//			if( iError[2] < uiStep[2] ) {
//				sence_ctrl_data.tCurrVal.usBlue -= iError[2];
//			}else{
//				sence_ctrl_data.tCurrVal.usBlue -= uiStep[2];
//			}
//		}
//
// 
//		ty_light_driver_bp5758d_rgbcw(&sence_ctrl_data.tCurrVal);      
//		//TUYA_APP_LOG_DEBUG("tCurrVal: %d %d %d",sence_ctrl_data.tCurrVal.usRed,sence_ctrl_data.tCurrVal.usGreen,sence_ctrl_data.tCurrVal.usBlue);
//
//    } else { /* no error between target and current */				
//				//TUYA_APP_LOG_DEBUG("shade change no need, stop");
//		light_shade_flag = FALSE;   /* just stop the change */
//
//		ty_light_driver_bp5758d_rgbcw(&sence_ctrl_data.tTargetVal); 		
//    }
}



//适用于渐变场景,只有彩光
static void opLightCtrlSceneChange(uint8_t sceneunit, uint8_t uispeed, uint8_t *sencedata)
{
	// if(TRUE == bSceneStopFlag){
	// 	return;
	// }

    if(bSceneSetFirstFlag) {
        sence_ctrl_data.ucUnitCnt = 0;		
    }

	uint16_t hue = ((sencedata[(sence_ctrl_data.ucUnitCnt*6)])<<8) | (sencedata[(sence_ctrl_data.ucUnitCnt*6+1)]);
	uint16_t saturation = ((sencedata[(sence_ctrl_data.ucUnitCnt*6+2)])<<8) | (sencedata[(sence_ctrl_data.ucUnitCnt*6+3)]);
	uint16_t value = ((sencedata[(sence_ctrl_data.ucUnitCnt*6+4)])<<8) | (sencedata[(sence_ctrl_data.ucUnitCnt*6+5)]);	
	
    sence_ctrl_data.ucUnitCnt++;        /* @attention:turn on/off on APP,will change the unit num++! */
    if(sence_ctrl_data.ucUnitCnt >= sceneunit) {
        sence_ctrl_data.ucUnitCnt = 0;
    }

	//sence_ctrl_data.uiGainTemp = 1000 / (uiSpeed / 10) + 0.5;//省代码
	sence_ctrl_data.uiGainTemp = 6;
	
//	if(bSceneSetFirstFlag == TRUE) {		
//		light_adjust_HSV_hw(hue, saturation, value);
//		memcpy(&sence_ctrl_data.tCurrVal, &sence_ctrl_data.tTargetVal, sizeof(sence_ctrl_data.tTargetVal));
//		bSceneSetFirstFlag = FALSE;
//	}else{
//		value = (value-10)*90/99 + 100;//10%——100%
//		TColor color = HSV2RGB(hue,saturation,value);
//		sence_ctrl_data.tTargetVal.usRed = color.R;
//		sence_ctrl_data.tTargetVal.usGreen = color.G;
//		sence_ctrl_data.tTargetVal.usBlue = color.B;
//		
//		light_shade_flag = TRUE;
//		sence_ctrl_data.bFirstChange = TRUE;		
//	}
//if(bSceneSetFirstFlag == TRUE) {
	light_adjust_HSV_hw(hue, saturation, value);
	bSceneSetFirstFlag = FALSE;
//}
}	







u32 sence_uint_cnt_time = 0;//场景单元切换4s计时


void scene_run(void){	

	if(light_value.onoff == 0){
        light_set_off();
		return;
    }

	switch(light_value.scene_mode){
		case SCENE_MUSIC:
			light_adjust_HSV_hw(music_h,music_s,music_v);
		break;
//		case SCENE_BLINK:{			
//		}break;
//		case SCENE_WHITE:
//			ty_light_gradual_ctrl_cw(light_value.B, light_value.T);
//		break;
		case SCENE_COLOR:
			light_adjust_HSV_hw(light_value.H,light_value.S,light_value.V);
		break;
#if SENCE_FUNCTION
		case SCENE_MODEL:{
			uint32_t compare_time = 4000000;
			if(sence_uint_cnt_time == 0){			
				compare_time = 10;						
			}
			if(hal_clock_time_exceed(sence_uint_cnt_time,compare_time)){
				sence_uint_cnt_time = hal_clock_get_system_tick();			

				switch(light_value.sence_model){
					case SCENE_MODEL0:{
						light_adjust_HSV_hw(46, 1000, 716);
					}break;
			
					case SCENE_MODEL3:{
						light_adjust_HSV_hw(232, 899, 796);
					}break;
					
					case SCENE_MODEL4:{				
						opLightCtrlSceneChange(4, 2000, sence_data1);					
					}break;

					case SCENE_MODEL7:{												
						opLightCtrlSceneChange(6, 2000, sence_data2);									
					}break;

					case SCENE_MODEL24:{					
						opLightCtrlSceneChange(3, 2000, sence_data3);									
					}break;

					case SCENE_MODEL28:{			
						opLightCtrlSceneChange(3, 2000, sence_data4);				
					}break;				
					default:
					break;
				}
			}

//			if(light_shade_flag == TRUE){
//				static u32 T_shade = 0;
//				if(hal_clock_time_exceed(T_shade,10000)){
//					T_shade = hal_clock_get_system_tick();
//					light_ctrl_shade_gradually();
//				}				
//			}			
		}break;
#endif		
		default:
		break;
	}	
}



void set_onoff(uint8_t state){
    light_value.onoff = state;
	if(countdown !=0 && countdown_target_onoff == state){
		countdown = 0;
	}
}

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


u16 light_time_setok_flag = 0;


u8 timing_end_cb(u8 index){
//    //PR_DEBUG("TIMING[%d] CB!\n",index);
	switch(mITime_Work[index].type){
		case 0x01://onoff
			light_value.onoff = mITime_Work[index].onoff;
			set_onoff(light_value.onoff);
			app_light_write_to_storage_start();
		break;
//		case 0x02://cw(b、t)
//			light_value.onoff = TRUE;
//			light_value.scene_mode = SCENE_WHITE;
//			light_value.B = mITime_Work[index].B * 10;//1000
//			light_value.T = mITime_Work[index].T * 10;//1000
//			set_onoff(light_value.onoff);
//			app_light_write_to_storage_start();
//		break;					
		case 0x03://sence id
			light_value.onoff = TRUE;
			light_value.scene_mode = SCENE_MODEL;
            light_value.sence_model = mITime_Work[index].senceID;
			bSceneSetFirstFlag = TRUE;
//			bSceneStopFlag = FALSE;
			light_shade_flag = FALSE;
			sence_uint_cnt_time = 0;
			set_onoff(light_value.onoff);
			app_light_write_to_storage_start();
		break;
		case 0x04://color(h、s、v)
			light_value.onoff = TRUE;
			light_value.scene_mode = SCENE_COLOR;
			light_value.H = mITime_Work[index].H * 10;//360
			light_value.S = mITime_Work[index].S * 10;//1000
			light_value.V = mITime_Work[index].V * 10;//1000
			set_onoff(light_value.onoff);
			app_light_write_to_storage_start();					
		break;
		default:
		break;
	}
//
////    PR_DEBUG("%04d/%02d/%02d %02d:%02d:%02d [星期:%d]\n",
////            date2.year,
////            date2.month,
////            date2.monthday,
////            date2.hour,
////            date2.minute,
////            date2.second,
////            date2.weekday);
    return 0;
}

void app_dps_download(unsigned char kind, unsigned char dpid, unsigned char dpty, unsigned char dplen, unsigned char *dpvalue){
//    print("dp:%d [%d]\r\n",dpid,dpvalue[0]);
//	uart_putc(dpid);
//	uint8_t i = 0;
//	for(i=0;i<dplen;i++){
//		uart_putc(dpvalue[i]);		
//	}
	u8 flag = 0;
	u16 remote_lightness;
	switch(dpid){
        case 0x01://onoff
			//light_value.scene_mode = SCENE_COLOR;
			light_value.onoff = dpvalue[0];
			countdown = 0; 
			flag = 1;
			sence_uint_cnt_time =0;
			//app_light_write_to_storage_start();
//			app_dps_upload(dpid,dpty,dplen,dpvalue);
            break;
        case 0x02://work_mode
			if(dpvalue[0] != SCENE_WHITE){		
				light_value.scene_mode = dpvalue[0];
//				app_dps_upload(dpid,dpty,dplen,dpvalue);
			flag = 1;
			//app_light_write_to_storage_start();

				if(light_value.scene_mode == SCENE_MODEL){
					bSceneSetFirstFlag = TRUE;
	//				bSceneStopFlag = FALSE;
					light_shade_flag = FALSE;
					sence_uint_cnt_time = 0;
				}else if(light_value.scene_mode == SCENE_MUSIC){
					music_h = 0;//360
					music_s = 1000;//1000
					music_v = 1000;//1000				
				}
			}			
            break;
#if WHITE_FUNCTION			
       case 0x03://bright_value
			light_value.scene_mode = SCENE_WHITE;
			light_value.B = ((dpvalue[2]<<8) | dpvalue[3]);
			flag = 1;
			//app_light_write_to_storage_start();
//			app_dps_upload(dpid,dpty,dplen,dpvalue);
           break;
       case 0x04://temp_value
			light_value.scene_mode = SCENE_WHITE;
			light_value.T = ((dpvalue[2]<<8) | dpvalue[3]);
			flag = 1;
			//app_light_write_to_storage_start();
//			app_dps_upload(dpid,dpty,dplen,dpvalue);		
           break;
#endif	
#if SENCE_FUNCTION	   			
        case 12:{//sence				
			light_value.scene_mode = SCENE_MODEL;
            light_value.sence_model = dpvalue[0];
//			app_dps_upload(dpid,dpty,dplen,dpvalue);
			bSceneSetFirstFlag = TRUE;
//			bSceneStopFlag = FALSE;
			light_shade_flag = FALSE;
			sence_uint_cnt_time = 0;
			flag = 1;
			//app_light_write_to_storage_start();
		}break;
#endif			
		case 11:{//HSV
			light_value.H = ((dpvalue[0]<<8) | dpvalue[1]);//360
			if(kind!=4)
			{
				light_value.S = dpvalue[2] * 10;//1000
				light_value.V = dpvalue[3] * 10;//1000
			}
			light_value.scene_mode = SCENE_COLOR;
//			app_dps_upload(dpid,dpty,dplen,dpvalue);
			flag = 1;
			//app_light_write_to_storage_start();
			}
			break;
#if DSITURB_FUNCTION			
       case 34:{//do_not_disturb  value = bool
			app_light_ctrl_data_not_disturb_set(dpvalue[0]);
//			app_dps_upload(dpid,dpty,dplen,dpvalue);
//			app_dps_write_to_storage();
			flag = 1;
			//app_light_write_to_storage_start();
           light_time_setok_flag = 1;
           }
           break;
#endif
#if TIME_WORK_FUNCTION		   
       case 17:{//time work set
			//mITimingWorkFunc.work_set(dpvalue);
//			app_dps_upload(dpid,dpty,dplen,dpvalue);
			u8 weekday;
			u16 time;
			int time_get = ty_rtc.get_date(&weekday, &time);				
			if(time_get != 0){
				return;					
			}
			uint8_t time_id = (dpvalue[0]>>4)&0x0F;
			if((time_id > MAX_TIMING_NUM)||(time_id < 1)){
				return;
			}
			time_id = time_id - 1;
			uint8_t time_enable_flag = (dpvalue[0]>>3)&0x01;
			uint16_t time_temp = ((dpvalue[1]<<8)&0x0F00)| dpvalue[2];
			uint8_t time_week = ((dpvalue[0]<<4)&0x70)|((dpvalue[1]>>4)&0x0F);
			uint8_t time_repet_flag = 1;
			if(time_week == 0){
				time_repet_flag = 0;

				
				if(time_temp <= time){
					weekday = (weekday+1)%7;
				}
				time_week = 0x01<<weekday;
			}
			
			mITime_Work[time_id].type = (dpvalue[3]>>4)&0x0F;
			switch(mITime_Work[time_id].type){
				case 0x01://onoff
					mITime_Work[time_id].onoff = dpvalue[4];
				break;
				case 0x02://cw(b、t)
					mITime_Work[time_id].B = dpvalue[4];//100
					mITime_Work[time_id].T = dpvalue[5];//100
				break;					
				case 0x03://sence id
					mITime_Work[time_id].senceID = dpvalue[4];
				break;
				case 0x04://color(h、s、v)
					mITime_Work[time_id].H = ((dpvalue[3]<<2)&0x3C) | ((dpvalue[4]>>6)&0x03);//36
					mITime_Work[time_id].S = ((dpvalue[4]<<1)&0x7E) | ((dpvalue[5]>>7)&0x01);//100
					mITime_Work[time_id].V = dpvalue[5]&0x7F;//100
				break;
				default:
				break;
			}			
			mITimingFunc.set(&mITiming[time_id],time_enable_flag,time_temp,time_repet_flag,time_week,&timing_end_cb);

			light_time_setok_flag = 1;			
			}
           break;
       case 18:{//time synchronization
			//mITimingWorkFunc.time_syn(dpvalue);
			
			u32 time_unix = 0;
			s16 time_zone = 0;
			time_unix = (dpvalue[2]<<24)|(dpvalue[3]<<16)|(dpvalue[4]<<8)| dpvalue[5];
			time_zone = (s16)((dpvalue[0]<<8)| dpvalue[1]);
			
			ty_rtc.set_date(time_unix, time_zone);
			
			app_dps_upload(dpid,dpty,dplen,dpvalue);
           }
           break;
#endif			
#if COUNT_DOWN_FUNCTION
		case 0x07:{//count down
			//uint32_t count
			countdown = (dpvalue[0]<<24) | (dpvalue[1]<<16) | (dpvalue[2]<<8) | (dpvalue[3]);  
			countdown_target_onoff = !light_value.onoff;
//			app_dps_upload(dpid,dpty,dplen,dpvalue);
			light_time_setok_flag = 1;
			}
			break;
#endif			
#if MUSIC_FUNCTION			
        case 13://music
    		music_h = ((dpvalue[1]<<8) | dpvalue[2]);//360
			music_s = dpvalue[3] * 10;//1000
			music_v = dpvalue[4] * 10;//1000
			light_value.scene_mode = SCENE_MUSIC;
			flag = 1;
			//app_light_write_to_storage_start();
            break;
#endif
        case 19://lightness
            remote_lightness = (dpvalue[1] << 8) | dpvalue[2];
            if(0x02== dpvalue[0])
            {
                if((SCENE_COLOR==light_value.scene_mode)){
					//if((SCENE_COLOR==light_value.scene_mode)||(LIGHT_RGB == ty_light_cfg_init_get_lightway())){
                    //判断是否有此功能，这里的亮度应该直接当做V用
                    if((remote_lightness)<10)
                    {
                        remote_lightness = 10;
                    }
                    light_value.V = remote_lightness;//1000
                    light_value.scene_mode = SCENE_COLOR;
                }else{
                    //判断是否有此功能
                    if((remote_lightness)<10)
                    {
                        remote_lightness = 10;
                    }
                    light_value.scene_mode = SCENE_COLOR;
                    light_value.B = remote_lightness;

                }
            }
//            else if(0x00== dpvalue[0])
//            {
//                if((remote_lightness)<10)
//                {
//                    remote_lightness = 10;
//                }
//                light_value.scene_mode = SCENE_WHITE;
//                light_value.B = remote_lightness;
//            }
//            else if(0x01== dpvalue[0])
//            {
//                if((remote_lightness)<10)
//                {
//                    remote_lightness = 10;
//                }
//                light_value.V = remote_lightness;//1000
//                light_value.scene_mode = SCENE_COLOR;
//            }
			flag = 1;
			//app_light_write_to_storage_start();
            break;
			
        default:
            break;
    }
	if(flag == 1)
	{
		app_light_write_to_storage_start();
	}
}

void app_ir_cmd(unsigned int code,uint8_t long_press_time_s){
	switch(code){
        case KEY_LUM_ADD:{
			light_value.V+=100;
			if(light_value.V > 1000) light_value.V = 1000;
                         }break;
        case KEY_LUM_DEC:{
            light_value.V-=100;
			if(light_value.V < 0) light_value.V = 0;
                         }break;
		case KEY_OFF:set_onoff(0);break;
		case KEY_ON:set_onoff(1);break;
		case KEY_R:light_value.H=0,light_value.S=1000,light_value.V=1000;light_value.scene_mode=0;break;
		case KEY_G:light_value.H=120,light_value.S=1000,light_value.V=1000;light_value.scene_mode=0;break;
		case KEY_B:light_value.H=240,light_value.S=1000,light_value.V=1000;light_value.scene_mode=0;break;
		case KEY_W:light_value.H=0,light_value.S=0,light_value.V=1000;light_value.scene_mode=0;break;
		case KEY_COLOR_1:light_value.H=338,light_value.S=883,light_value.V=1000;light_value.scene_mode=0;break;
		case KEY_COLOR_2:light_value.H=80,light_value.S=750,light_value.V=1000;light_value.scene_mode=0;break;break;
		case KEY_COLOR_3:light_value.H=223,light_value.S=1000,light_value.V=1000;light_value.scene_mode=0;break;
		case KEY_COLOR_4:light_value.H=23,light_value.S=1000,light_value.V=1000;light_value.scene_mode=0;break;
		case KEY_COLOR_5:light_value.H=158,light_value.S=47,light_value.V=1000;light_value.scene_mode=0;break;
		case KEY_COLOR_6:light_value.H=248,light_value.S=565,light_value.V=1000;light_value.scene_mode=0;break;
		case KEY_COLOR_7:light_value.H=56,light_value.S=502,light_value.V=1000;light_value.scene_mode=0;break;
		case KEY_COLOR_8:light_value.H=182,light_value.S=656,light_value.V=1000;light_value.scene_mode=0;break;
		case KEY_COLOR_9:light_value.H=280,light_value.S=755,light_value.V=1000;light_value.scene_mode=0;break;
		case KEY_COLOR_10:light_value.H=60,light_value.S=1000,light_value.V=1000;light_value.scene_mode=0;break;
		case KEY_COLOR_11:light_value.H=180,light_value.S=1000,light_value.V=1000;light_value.scene_mode=0;break;
		case KEY_COLOR_12:light_value.H=300,light_value.S=1000,light_value.V=1000;light_value.scene_mode=0;break;
		case KEY_SCENE_FLASH:light_value.S=1000,light_value.V=1000;light_value.scene_mode = 6;break;
		case KEY_SCENE_STROBE:light_value.S=1000,light_value.V=1000;light_value.scene_mode = 7;break;
		case KEY_SCENE_FADE:light_value.S=1000,light_value.V=1000;light_value.scene_mode = 8;break;
		case KEY_SCENE_SMOOTH:light_value.S=1000,light_value.V=1000;light_value.scene_mode = 9;break;
		default:break;
	}

	if(code == KEY_ON && long_press_time_s > 4){
		ty_beacon_start_pairing();
	}
}

void app_light_init(void){
	app_dps_read_from_storage();

//uart_putc(light_value.onoff);
//uart_putc(light_value.scene_mode);
//uart_putc(light_value.sence_model);
//
//uart_putc((light_value.H>>8));
//uart_putc(((u8)light_value.H));
//
//uart_putc((light_value.S>>8));
//uart_putc(((u8)light_value.S));
//
//uart_putc((light_value.V>>8));
//uart_putc(((u8)light_value.V));
//
//uart_putc((light_value.B>>8));
//uart_putc(((u8)light_value.B));
//
//uart_putc((light_value.T>>8));
//uart_putc(((u8)light_value.T));	
}





#ifdef FACTORY_FUNCTION
static uint8_t light_factory_test_flag = 0;
void ty_beacon_enter_test_mode_cb(u8 rssi){
	static uint32_t time_t = 0;
	if(light_factory_test_flag == 1){
		time_t = hal_clock_get_system_tick();
		light_factory_test_flag = 2;
		ty_light_gradual_ctrl_cw(1000, 1000);		
	}else if((light_factory_test_flag > 0)&&(light_factory_test_flag < 5)){
		if(hal_clock_time_exceed(time_t,1200000000)){
			time_t = hal_clock_get_system_tick();
			light_factory_test_flag ++;
			if(light_factory_test_flag == 3){
				ty_light_gradual_ctrl_cw(1000, 0);
			}else if(light_factory_test_flag == 4){
				light_adjust_HSV_hw(0, 0,1000);
			}
		}		
	}	
}
#else
void ty_beacon_enter_test_mode_cb(u8 rssi){
	
}
#endif


void ty_beacon_event_cb(ty_beacon_event_e e, void *params){
    switch(e){
        case BEACON_EVENT_PAIR_OK:
        case BEACON_EVENT_RESET:
            app_light_def_state_init();
            break;
        default:break;
    }
}

void tick_countdown_event_handle(void)
{
    if(countdown != 0){
		countdown --;
        if (countdown == 0){ // 倒计时结束关闭灯光
            set_onoff(countdown_target_onoff);
			app_light_write_to_storage_start();
        }
    }
}

void app_light_run(void){
	static u32 T = 0;
	static u32 T_countdown = 0;


#if FACTORY_FUNCTION
		if(light_factory_test_flag > 0){
		   ty_beacon_enter_test_mode_cb(45);
			return;	
		}
#endif
    if(light_time_setok_flag > 0){
		light_set_off();
		light_time_setok_flag ++;
		if(light_time_setok_flag < 500){
			return;			
		}else{
			light_time_setok_flag = 0;
			if(light_value.scene_mode == SCENE_MODEL){
				sence_uint_cnt_time = 0;
			}
			
		}
	}

	
	if(ty_beacon_get_state() == DEVICE_STATE_PAIRING){
		light_value.scene_mode = SCENE_BLINK;
		if(hal_clock_time_exceed_rtc(T,500000)){			
			light_value.onoff = 1;
			static uint16_t blink_R = 0;
			blink_R = blink_R == 0?1000:0; 
			light_adjust_HSV_hw(0, 1000, blink_R);
			T = hal_clock_get_system_tick_rtc();
		}
	}else{
		if(light_value.scene_mode == SCENE_BLINK){
			app_dps_read_from_storage();
			
			if(light_value.scene_mode == SCENE_MODEL){
				bSceneSetFirstFlag = TRUE;
//				bSceneStopFlag = FALSE;
				light_shade_flag = FALSE;
				sence_uint_cnt_time = 0;
			}else if(light_value.scene_mode == SCENE_MUSIC){
				music_h = 0;//360
				music_s = 1000;//1000
				music_v = 1000;//1000				
			}
			app_light_write_to_storage_start();			
		}
		
		if(store_flag == 1){
			if(hal_clock_time_exceed(store_T,5000000)){
				app_dps_write_to_storage();
				store_flag = 0;
			}
		}
	}

	scene_run();
	app_light_reset_run();

#if IR_FUNCTION
	static u32 pre_time;
	static u8 pre_ir_io_state;		
	if(ir_code_get){
		u32 now_time = hal_clock_get_system_tick()/HAL_CLOCK_1MS_TICKS;
		u8 now_state = gpio_get_bits(BIT6);
		if(ir_code_get == 1){
			pre_time = now_time;
			pre_ir_io_state = now_state;
			ir_code_get = 2;
		}else{
			if(now_time-pre_time > 110){
				#if LOG_MAJOR
					print("%x\r\n",ir_code);
				#endif
				app_ir_cmd(ir_code,ir_code_get/9);
				ir_code_get = 0;
			}else if(pre_ir_io_state != now_state){
				pre_ir_io_state = now_state;
				u32 x = now_time-pre_time;
				if(now_state == 0 && x > 90){//HIGH > 90 ms
					ir_code_get++;
					print("long press %d s\r\n",ir_code_get/9);
					
					if(ir_code_get/9 > 4){
						app_ir_cmd(ir_code,ir_code_get/9);
						ir_code_get = 0;
					}
				}
				pre_time = now_time;
			}
		}
	}
#endif
}



















