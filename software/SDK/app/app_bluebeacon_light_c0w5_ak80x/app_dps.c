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

static uint16_t seg_temperature[16] = {0,100,200,300,400,500,600,700,800,900,1000};
static uint16_t seg_brightness[16]  = {10,100,200,300,400,500,600,700,800,900,1000};

volatile static uint32_t power_outage_detect_flag;
volatile uint32_t gpio_level = 0;
#define SCENE_MODEL0	0
#define SCENE_NIGHT		0
#define SCENE_NEUTRAL	1
#define SCENE_COLDW		2
#define SCENE_WARMW		3

typedef enum {
	SCENE_WHITE = 0x00,
    SCENE_MODEL = 0x02,		
	SCENE_BLINK = 0x03,	
    SCENE_SEGLIGHT = 0x04,
	SCENE_REMIND = 0x05,        // 渐变模式
	SCENE_MAX = 0x06,
}DRIVER_SCENE_MODE_E;



#pragma pack (1)
typedef struct{//save[56:8]
	uint8_t seg_onoff;//1
	uint8_t scene_mode;//0
	uint8_t seg_nums;//0
	uint8_t sence_model;
	int16_t seg_B1;//1000
	int16_t seg_B2;//1000
	int16_t seg_B3;//1000
	int16_t seg_B4;//1000
	int16_t seg_B5;//1000
	int16_t seg_T1;//1000
	int16_t seg_T2;//1000
	int16_t seg_T3;//1000
	int16_t seg_T4;//1000
	int16_t seg_T5;//1000
	int16_t B;//0
	int16_t T;//1000
	uint8_t cnt;
	uint8_t ac_cnt;
	uint8_t data_right_flag;
}light_value_s;


typedef struct 
{
    u16 usCold;       ///< C value,rang from 0 to 1000
    u16 usWarm;       ///< W value,rang from 0 to 1000
}PWM_LIGHT_CONTRL_T;
typedef struct 
{
	bool bFirstChange;
	//uint8_t ucSceneUnit;
    //uint8_t ucUnitCnt;
	uint8_t uiGainTemp;
    PWM_LIGHT_CONTRL_T tTargetVal;
	PWM_LIGHT_CONTRL_T tCurrVal;
}LIGHT_CTRL_HANDLE_T;

bool light_ctrl_amplify_flag = 1;
#pragma pack () 

light_value_s light_value;
LIGHT_CTRL_HANDLE_T light_ctrl_data;

bool bSceneSetFirstFlag = TRUE;
//bool bSceneStopFlag = FALSE;

volatile bool light_value_onoff = 1;
bool light_shade_flag;


uint32_t countdown = 0;//s 1day=86400
uint8_t countdown_target_onoff = 0;
uint8_t breathe_scene_flag = 0;
u16 light_time_setok_flag = 0;
u32 store_T = 0;
bool store_flag = 0;

#define RESET_MAX_CNT (light_value.seg_onoff > 0 ? 12 : 5)

void app_dps_write_to_storage();
static void light_ctrl_shade_start(u8 step);

void app_light_write_to_storage_start(void){
	if(store_flag == 0){
		store_T = hal_clock_get_system_tick();
		store_flag = 1;
	}
}
uint8_t app_light_ctrl_data_reset_cnt_get();

static u8 first_pairing_flag = 0;
void gpio_read_flag(void)
{
	static u32 T1 = 0;
	if (hal_clock_time_power_exceed(T1,5*1000))
	{
		T1 = hal_clock_get_system_tick();
		gpio_level = gpio_get_bits(BIT3);
		
		if (gpio_level > 0)
		{
			
			power_outage_detect_flag = 0;
		}else{
			
			power_outage_detect_flag++;	
		}
		if (power_outage_detect_flag > 9)
		{
			
			light_value_onoff = 0;
			ty_light_ctrl_amplify_cw(0 ,light_value.T);
		}
	
	}
	
	
}
#define NO_POWER 	 0
#define POWER_SUPPLY 1
#define STANDBY		 2
static u8 paring_result = 0;
void power_outage_state(void)
{
	static u8 power_state = 1;
	static u8 gpio4_level = 0;
	static u8 seg_dimmer = 1;
	static u8 power_flag = 0;
	static uint32_t count = 1;
	static uint32_t T1 = 0;
	static uint32_t T2 = 0;

	if (paring_result == 1)
	{
		light_value.ac_cnt = 12;
		app_dps_write_to_storage();
		//hal_storage_256_write_bytes(96, (u8 *)(&pre_light_value),sizeof(light_value_s));
		paring_result = 0;
		power_flag = 1;
	}

	if (power_outage_detect_flag < 10)
	{
		
		power_state = POWER_SUPPLY;
	}else if(power_flag ==0){
		
		power_state = NO_POWER;
	}else{
		
		power_state = STANDBY;
	}
	switch(power_state){
				case NO_POWER:{
					if (light_value.ac_cnt == 0)
					{
						ty_beacon_paring_stop();
					}
						countdown = 0;
						mITimingFunc.init(); 
						light_value_onoff = 0;
						power_flag = 1;
						light_time_setok_flag = 0;
						light_value.ac_cnt++;
						uart_putc(0xc0);
						app_dps_write_to_storage();
						T2 = hal_clock_get_system_tick();
						power_state = STANDBY;
					}break;	
				case POWER_SUPPLY:{
						if (power_flag == 1 || seg_dimmer == 1)
						{
							store_T = 0;
							store_flag = 0;
							gpio4_level = gpio_get_bits(BIT4);
							if (gpio4_level > 0)
							{
								light_ctrl_amplify_flag = 1;
							}else
							{
								light_ctrl_amplify_flag = 0;
							}
							light_value_onoff = 1;
							uart_putc(0xc1);
							uart_putc(light_value.ac_cnt);
							uart_putc(light_ctrl_amplify_flag);
							light_ctrl_Segmented_dimmer_handle();
							if(light_value.ac_cnt>= RESET_MAX_CNT){
								ty_beacon_start_pairing();	
								app_dps_write_to_storage();
								//hal_storage_256_write_bytes(96, (u8 *)(&pre_light_value),sizeof(light_value_s));
							}
							if (ty_beacon_get_state() == DEVICE_STATE_PAIRING)
							{
								first_pairing_flag = 0;
								light_time_setok_flag = 7;
								uart_putc(0xc5);
							}else{
								beacon_remote_pair_windowns_pramer_set(TRUE,30);
							}
							seg_dimmer = 0;
							T2 = hal_clock_get_system_tick();
						}
						if (light_value.ac_cnt != 0)
						{	
							if (hal_clock_time_exceed(T2,6*1000*1000))
							{
								light_value.ac_cnt = 0;
								app_dps_write_to_storage();
								//hal_storage_256_write_bytes(96, (u8 *)(&pre_light_value),sizeof(light_value_s));
							}
						}
						power_flag = 0;
					}break;	
				case STANDBY:{
					}break;		
				default:
				break;
				}

	
	hal_storage_256_write_bytes(96, (u8 *)(&light_value),sizeof(light_value_s));
}

static void light_ctrl_Segmented_dimmer(uint8_t seg_cnt)
{
	if(ty_beacon_get_state() == DEVICE_STATE_PAIRED){
	if (light_value.seg_onoff > 0)
	{
			light_value.scene_mode = SCENE_SEGLIGHT;
			light_value.sence_model++;
			if (light_value.sence_model > light_value.seg_nums)
			{
				light_value.sence_model = 1;
			}
			if (seg_cnt ==  2)
			{
			switch(light_value.sence_model){
					case 1:{
						if (light_value.B == light_value.seg_B1 && light_value.T == light_value.seg_T1)
						{
							light_value.sence_model++;
						}
					}break;	
					case 2:{
						if (light_value.B == light_value.seg_B2 && light_value.T == light_value.seg_T2)
						{
							light_value.sence_model++;
						}
					}break;	
					case 3:{
						if (light_value.B == light_value.seg_B3 && light_value.T == light_value.seg_T3)
						{
							light_value.sence_model++;
						}
					}break;	
					case 4:{
						if (light_value.B == light_value.seg_B4 && light_value.T == light_value.seg_T4)
						{
							light_value.sence_model++;
						}
					}break;	
					case 5:{
						if (light_value.B == light_value.seg_B5 && light_value.T == light_value.seg_T5)
						{
							light_value.sence_model++;
						}
					}break;			
					default:
					break;
				}
			}
			if (light_value.sence_model > light_value.seg_nums)
			{
				light_value.sence_model = 1;
			}
			hal_storage_256_write_bytes(96, (u8 *)(&light_value),sizeof(light_value_s));
	 }
	}		
}
void light_ctrl_Segmented_dimmer_handle()
{
	if(ty_beacon_get_state() == DEVICE_STATE_PAIRED){
	if(0 == hal_storage_256_read_bytes(96, (u8 *)(&light_value),sizeof(light_value_s))){//success
		if (light_value.ac_cnt >1 )
		{
			light_ctrl_Segmented_dimmer(light_value.ac_cnt);
		}else{
			if (light_value.scene_mode == SCENE_BLINK)
			{
				light_value.scene_mode = SCENE_BLINK;
			}else{
				light_value.scene_mode = SCENE_WHITE;
				
				
			}
			
			
		}
	}else
	{
		light_value.cnt = 0;
	}
	}
}

void app_light_reset_run(void){
	static u8 reset_judge_time = 3; 
	static uint32_t T = 0; 
	
		if(reset_judge_time != 0){
			if(hal_clock_time_exceed(T,reset_judge_time*100000)){//300ms and 4s
				if(reset_judge_time == 3){
					reset_judge_time = 50;
					//if(0 == hal_storage_256_read_bytes(72, &cnt,1)){//success
					if(0 == hal_storage_256_read_bytes(96, (u8 *)(&light_value),sizeof(light_value_s))){//succes
						light_value.cnt++;	
						//uart_putc(0x14);
						
						if(light_value.cnt >= RESET_MAX_CNT){
							ty_beacon_start_pairing();
						}
					}
					app_dps_write_to_storage();
				}else if(reset_judge_time == 50){
						reset_judge_time = 0;
						light_value.cnt = 0;
						
						app_dps_write_to_storage();
					}
			}
		
		}
}



static uint8_t reset_cnt;
uint8_t app_light_ctrl_data_reset_cnt_get(void)
{
	//if(0 == hal_storage_256_read_bytes(72, &reset_cnt,1)){//success
	if(0 == hal_storage_256_read_bytes(96, (u8 *)(&light_value),sizeof(light_value_s))){
		if (light_value.ac_cnt > light_value.cnt)
		{
			reset_cnt = light_value.ac_cnt;
		}else{
			reset_cnt = light_value.cnt;
		}
	}
	return reset_cnt;

}
void app_light_ctrl_data_not_disturb_set(bool bNotDisturb)
{
    //light_value.light_disturb_flag = bNotDisturb;
}


// bool app_light_ctrl_data_not_disturb_get(void)
// {
//     return light_disturb_flag_temp;
// }


void app_light_ctrl_data_onoff_set(bool state)
{
	light_value_onoff = state;
}



void app_light_def_state_init(void){
	light_value_onoff = 1;
	light_value.seg_onoff = 1;
	light_value.cnt = 0;
	light_value.ac_cnt = 0;
	light_value.scene_mode = SCENE_WHITE;
	
	light_value.seg_nums = 4;
	light_value.B = 1000;
	light_value.T = 500;
	light_value.seg_B1 = 1000;
	light_value.seg_T1 = 500;
	light_value.seg_B2 = 1000;
	light_value.seg_T2 = 1000;
	light_value.seg_B3 = 1000;
	light_value.seg_T3 = 0;
	light_value.seg_B4 = 10;
	light_value.seg_T4 = 500;
	light_value.seg_B5 = 0;
	light_value.seg_T5 = 0;
	//light_value.light_disturb_flag = 0;
	light_value.seg_nums = 4;
	light_value.sence_model = SCENE_MODEL0;
	light_value.data_right_flag = 0xA5;	

	mITimingFunc.init(); 
	countdown = 0;

	app_dps_write_to_storage();	
}

void app_light_pairing_state_init(void){
	light_value_onoff = 1;
	light_value.seg_onoff = 1;
	light_value.scene_mode = SCENE_WHITE;
	light_value.seg_nums = 4;
	light_value.B = 1000;
	light_value.T = 500;
	light_value.seg_B1 = 1000;
	light_value.seg_T1 = 500;
	light_value.seg_B2 = 1000;
	light_value.seg_T2 = 1000;
	light_value.seg_B3 = 1000;
	light_value.seg_T3 = 0;
	light_value.seg_B4 = 10;
	light_value.seg_T4 = 500;
	light_value.seg_B5 = 0;
	light_value.seg_T5 = 0;
	//light_value.light_disturb_flag = 0;
	light_value.sence_model = SCENE_MODEL0;
	light_value.data_right_flag = 0xA5;	
	mITimingFunc.init(); 
	countdown = 0;
	app_dps_write_to_storage();	
}



void app_dps_read_from_storage(void){
	//if((0 == hal_storage_256_read_bytes(56, (u8 *)(&light_value),sizeof(light_value_s)))&&(light_value.data_right_flag == 0xA5)){//success
	if((0 == hal_storage_256_read_bytes(96, (u8 *)(&light_value),sizeof(light_value_s)))&&(light_value.data_right_flag == 0xA5)){//success
		

		light_value_onoff = 1;
		
		if(light_value.scene_mode >= SCENE_MAX){
			light_value.scene_mode = SCENE_WHITE;	
		
		
		}
		if(light_value.seg_B1> 1000 || light_value.seg_B1 < 0){
			light_value.seg_B1 = 0;//360 1000 1000
		}
		if(light_value.seg_B2> 1000 || light_value.seg_B2 < 0){
			light_value.seg_B2 = 0;//360 1000 1000
		}
		if(light_value.seg_B3> 1000 || light_value.seg_B3 < 0){
			light_value.seg_B3 = 0;//360 1000 1000
		}
		if(light_value.seg_B4> 1000 || light_value.seg_B4 < 0){
			light_value.seg_B4 = 0;//360 1000 1000
		}
		if(light_value.seg_B5> 1000 || light_value.seg_B5 < 0){
			light_value.seg_B5 = 0;//360 1000 1000
		}
		if(light_value.seg_T2> 1000 || light_value.seg_T2 < 0){
			light_value.seg_T2 = 0;//360 1000 1000
		}
		if(light_value.seg_T3> 1000 || light_value.seg_T3 < 0){
			light_value.seg_T3 = 0;//360 1000 1000
		}
		if(light_value.seg_T4> 1000 || light_value.seg_T4 < 0){
			light_value.seg_T4 = 0;//360 1000 1000
		}
		if(light_value.seg_T5> 1000 || light_value.seg_T5 < 0){
			light_value.seg_T5 = 0;//360 1000 1000
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
	static light_value_s pre_light_value = {0};
	if(0 == memcmp((u8 *)(&pre_light_value),(u8 *)(&light_value),sizeof(light_value_s)))return;
	memcpy((u8 *)(&pre_light_value),(u8 *)(&light_value),sizeof(light_value_s));
	//hal_storage_256_write_bytes(56, (u8 *)(&pre_light_value),sizeof(light_value_s));
	hal_storage_256_write_bytes(96, (u8 *)(&pre_light_value),sizeof(light_value_s));
	uart_putc(0x99);
	hal_storage_256_write_all(1);
	
}

uint32_t ir_code = 0;
uint8_t ir_code_get = 0;



void light_adjust_WC_hw(PWM_LIGHT_CONTRL_T *data)
{
	
	static uint16_t c=0,w=0;
	uint16_t white = data->usCold;
	uint16_t warm = data->usWarm;
	if(c==white && w==warm)
		return;
	
	//print("R=%d G=%d B=%d",red,green,blue);
	c=white,w=warm;
	uint16_t pwm_section=PWM_FRQ_SELECTION/1000;
	pwm_set_duty(PWM_CHANNEL_RGB_W, (1000-warm)*pwm_section);
	pwm_set_duty(PWM_CHANNEL_RGB_C, white*pwm_section);
}


#define BRIGHT_MAX 1000
#define BRIGHT_MIN 100
void ty_light_bt_cw(IN u16 usBright, IN u16 usTemperatue, OUT PWM_LIGHT_CONTRL_T *data)
{
	if(usBright < CTRL_CW_BRIGHT_VALUE_MIN){
    
		data->usCold = 0;
		data->usWarm = 0;			
	}else{
    /* RGB adjustment amount by hue */
		usBright = (usBright - 10) * (1000 - 100) / (1000 - 10) + 100;//10%——100%
        
		data->usCold = (u16)((u32)usBright * (usTemperatue) / 1000);
		data->usWarm = usBright - data->usCold;
    }

	
//	color.R = *r/10*255/100;
//	color.G = *g/10*255/100;
//	color.B = *b/10*255/100;
}










void ty_light_ctrl_cw(IN u16 usBright, IN u16 usTemperatue)
{
	PWM_LIGHT_CONTRL_T light_C_W;
	
	if(usBright < 10){		
		light_C_W.usCold = 0;
		light_C_W.usWarm = 0;			
	}else{
		usBright = (usBright-10)*95/99 + 50;//5%——100%		
		light_C_W.usCold = (u16)((u32)usBright * usTemperatue / 1000);
		light_C_W.usWarm = usBright - light_C_W.usCold ;		
	}
	light_ctrl_data.tCurrVal.usCold = light_C_W.usCold;
	light_ctrl_data.tCurrVal.usWarm = light_C_W.usWarm;
	light_adjust_WC_hw(&light_C_W);
	
		
}

void ty_light_ctrl_amplify_cw(IN u16 usBright, IN u16 usTemperatue)
{
	PWM_LIGHT_CONTRL_T light_C_W;
	if(usBright < 10){		
		light_C_W.usCold = 0;
		light_C_W.usWarm = 0;			
	}else{
		usBright = (usBright-10)*95/99 + 50;//5%——100%	
		if (light_ctrl_amplify_flag == 0)
		{
			light_C_W.usCold = (u16)((u32)usBright * usTemperatue / 1000);
			light_C_W.usWarm = usBright - light_C_W.usCold;
		}else{
			if(usTemperatue > CTRL_CW_TEMPER_VALUE_MAX/2){               
			light_C_W.usCold= usBright;
			light_C_W.usWarm = (u16)(((CTRL_CW_TEMPER_VALUE_MAX-usTemperatue)*1000/usTemperatue)*(light_C_W.usCold)/1000);
			}else{
				light_C_W.usWarm  = usBright;
				light_C_W.usCold = (u16)((usTemperatue*1000/(CTRL_CW_TEMPER_VALUE_MAX-usTemperatue))*(light_C_W.usWarm)/1000);
			}	
		}		
	}
	light_adjust_WC_hw(&light_C_W);
}



// void light_set_off(void){
// 	ty_light_ctrl_amplify_cw(0, light_value.T);
// }

void remind_scene(u8 value)
{
	
	light_time_setok_flag = value;	
	light_value.scene_mode = SCENE_BLINK;
	
	hal_storage_256_write_bytes(96, (u8 *)(&light_value),sizeof(light_value_s));
	
}

void scene_run(void){	
	
	static u32 T = 0;
	static u16 bright_data=0;
	if (light_value_onoff == 0)
	{
		
		light_time_setok_flag = 0;
		ty_light_ctrl_amplify_cw(0 ,light_value.T);
		return;
	}
	
	switch(light_value.scene_mode){
#if SEGMENTED_DIMMER_FUNCTION
		case SCENE_SEGLIGHT:{
			if (ty_beacon_get_state() == DEVICE_STATE_PAIRED)
				{
				switch(light_value.sence_model){
						case 1:{
							light_value.B = light_value.seg_B1;
							light_value.T = light_value.seg_T1;
						}break;	
						case 2:{
							light_value.B = light_value.seg_B2;
							light_value.T = light_value.seg_T2;
						}break;	
						case 3:{
							light_value.B = light_value.seg_B3;
							light_value.T = light_value.seg_T3;
						}break;	
						case 4:{
							light_value.B = light_value.seg_B4;
							light_value.T = light_value.seg_T4;
						}break;	
						case 5:{
							light_value.B = light_value.seg_B5;
							light_value.T = light_value.seg_T5;
						}break;			
						default:
						break;
					}
				ty_light_ctrl_amplify_cw(light_value.B, light_value.T);
				}
			app_light_write_to_storage_start();
		}break;
#endif	
		case SCENE_WHITE:
			ty_light_ctrl_amplify_cw(light_value.B, light_value.T);
		break;
		case SCENE_BLINK:
				if (light_time_setok_flag && power_outage_detect_flag < 10){
					if(hal_clock_time_exceed_rtc(T,500000)){
						light_time_setok_flag--;
						uart_putc(light_time_setok_flag);		
						static uint16_t blink_B = 0;
						blink_B = blink_B == 0 ? light_value.B : 0; 
						ty_light_ctrl_amplify_cw(blink_B, light_value.T);
						
						T = hal_clock_get_system_tick_rtc();
					}
				}else{
					ty_light_ctrl_amplify_cw(light_value.B, light_value.T);
					light_time_setok_flag = 0;
					light_value.scene_mode = SCENE_WHITE;
					
				
				}
		break;
#if SENCE_FUNCTION
		case SCENE_MODEL:{
			ty_light_ctrl_amplify_cw(light_value.B, light_value.T);
			breathe_scene_flag = SCENE_MODEL;
			//ty_light_ctrl_amplify_cw(light_value.B, light_value.T);
				switch(light_value.sence_model){
					case SCENE_NIGHT:{
						light_value.B = 10;
						light_value.T = 500;
					}break;	
					case SCENE_NEUTRAL:{
						light_value.B = 1000;
						light_value.T = 500;
					}break;	
					case SCENE_COLDW:{
						light_value.B = 1000;
						light_value.T = 1000;
					}break;	
					case SCENE_WARMW:{
						light_value.B = 1000;
						light_value.T = 0;
					}break;			
					default:
					break;
				}			
			}			
		break;
#endif		
		default:
		break;
	}	
}



void set_onoff(uint8_t state){
	light_value_onoff = state;
	uart_putc(0xa5);
	uart_putc(light_value_onoff);
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





u8 timing_end_cb(u8 index){
//    //PR_DEBUG("TIMING[%d] CB!\n",index);
	//uart_putc(mITime_Work[index].type);
	
	switch(mITime_Work[index].type){
		case 0x01://onoff
			light_value_onoff = mITime_Work[index].onoff;
			set_onoff(light_value_onoff);
			app_light_write_to_storage_start();
		break;
		case 0x02://cw(b、t)
			light_value_onoff = TRUE;
			light_value.scene_mode = SCENE_WHITE;
			
			light_value.B = mITime_Work[index].B * 10;//1000
			light_value.T = mITime_Work[index].T * 10;//1000
			set_onoff(light_value_onoff);
			app_light_write_to_storage_start();
		break;					
		case 0x03://sence id
			light_value_onoff = TRUE;
			light_value.scene_mode = SCENE_MODEL;
           	light_value.sence_model = mITime_Work[index].senceID;
			bSceneSetFirstFlag = TRUE;
//			bSceneStopFlag = FALSE;
			light_shade_flag = FALSE;
			set_onoff(light_value_onoff);
			app_light_write_to_storage_start();
		break;
//		case 0x04://color(h、s、v)
//			light_value.onoff = TRUE;
//			light_value.scene_mode = SCENE_COLOR;
//			light_value.H = mITime_Work[index].H * 10;//360
//			light_value.S = mITime_Work[index].S * 10;//1000
//			light_value.V = mITime_Work[index].V * 10;//1000
//			set_onoff(light_value.onoff);					
//			app_light_write_to_storage_start();					
//		break;
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
#if BEACON_REMOTE_FUNCTION
void app_dps_download(unsigned char kind, unsigned char dpid, unsigned char dpty, unsigned char dplen, unsigned char *dpvalue){
//void app_dps_download(unsigned char dpid, unsigned char dpty, unsigned char dplen, unsigned char *dpvalue){
//    print("dp:%d [%d]\r\n",dpid,dpvalue[0]);
//    uart_putc(0x12);uart_putc(0xEF);
	uart_putc(dpid);
	uint8_t i = 0;
	for(i=0;i<dplen;i++){
		uart_putc(dpvalue[i]);		
	}
	u8 flag = 0;
	u16 remote_lightness;
	switch(dpid){
        case 0x01://onoff
			light_value_onoff= dpvalue[0];
			countdown = 0; 
			flag = 1;
			//app_light_write_to_storage_start();
//			app_dps_upload(dpid,dpty,dplen,dpvalue);
            break;
        case 0x02://work_mode
			if(dpvalue[0] == SCENE_WHITE){
				light_value.scene_mode = dpvalue[0];
//				app_dps_upload(dpid,dpty,dplen,dpvalue);				
			flag = 1;
			//app_light_write_to_storage_start();
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
			uart_putc(dplen);			
			light_value.scene_mode = SCENE_MODEL;
            light_value.sence_model = dpvalue[0];
			if (dpvalue[0] == 0)
			{
				
				light_value_onoff = 1;
				
			}else if (dpvalue[0] == 0x07)
			{
				light_value.sence_model = 2;
			}else if (dpvalue[0] == 0x1c)
			{
				light_value.sence_model = 1;
			}else if (dpvalue[0] == 0x18)
			{
				light_value.sence_model = 2;
			}else if (dpvalue[0] == 0x04)
			{
				light_value.sence_model = 3;
			}
//			app_dps_upload(dpid,dpty,dplen,dpvalue);
			//bSceneSetFirstFlag = TRUE;
//			bSceneStopFlag = FALSE;
			//light_shade_flag = FALSE;
			//sence_uint_cnt_time = 0;
			flag = 1;
			//app_light_write_to_storage_start();
		}break;
#endif			
//		case 11:{//HSV
//			light_value.H = ((dpvalue[0]<<8) | dpvalue[1]);//360
//			light_value.S = dpvalue[2] * 10;//1000
//			light_value.V = dpvalue[3] * 10;//1000
//			light_value.scene_mode = SCENE_COLOR;
//			app_dps_upload(dpid,dpty,dplen,dpvalue);
//			app_light_write_to_storage_start();
//			}
//			break;
#if DSITURB_FUNCTION			
       case 34:{//do_not_disturb  value = bool
			app_light_ctrl_data_not_disturb_set(dpvalue[0]);
//			app_dps_upload(dpid,dpty,dplen,dpvalue);
//			app_dps_write_to_storage();
			flag = 1;
			//app_light_write_to_storage_start();

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
			remind_scene(3);
			//light_value.scene_mode = SCENE_REMIND;			
			}
           break;
       case 18:{//time synchronization
			//mITimingWorkFunc.time_syn(dpvalue);
			
			u32 time_unix = 0;
			s16 time_zone = 0;
			time_unix = (dpvalue[2]<<24)|(dpvalue[3]<<16)|(dpvalue[4]<<8)| dpvalue[5];
			time_zone = (s16)((dpvalue[0]<<8)| dpvalue[1]);
			
			ty_rtc.set_date(time_unix, time_zone);
			
			//app_dps_upload(dpid,dpty,dplen,dpvalue);
           }
           break;
#endif			
#if COUNT_DOWN_FUNCTION
		case 0x07:{//count down
			//uint32_t count
			countdown = (dpvalue[0]<<24) | (dpvalue[1]<<16) | (dpvalue[2]<<8) | (dpvalue[3]);  
			countdown_target_onoff = !light_value_onoff;
//			app_dps_upload(dpid,dpty,dplen,dpvalue);
			if (countdown != 0) {
			remind_scene(3);
				//light_value.scene_mode = SCENE_REMIND;
			} else {
				remind_scene(5);
				//light_value.scene_mode = SCENE_REMIND;
			}
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
            break;
#endif
        case 19://lightness
            remote_lightness = (dpvalue[1] << 8) | dpvalue[2];
            if(0x02== dpvalue[0])
            {

				//判断是否有此功能
				if((remote_lightness)<10)
				{
					remote_lightness = 10;
				}
				light_value.scene_mode = SCENE_WHITE;
				
				light_value.B = remote_lightness;
            }

			flag = 1;
            break;
#if SEGMENTED_DIMMER_FUNCTION		
       case 53:{
		   light_value.scene_mode = SCENE_WHITE;
		 
		   light_value.seg_onoff = ((dpvalue[0] >> 4) & 0x01);
		   light_value.seg_nums = (dpvalue[0] & 0x0F);
		   light_value.seg_B1 =  seg_brightness[(dpvalue[1]  >> 4)];
		   light_value.seg_T1 =  seg_temperature[(dpvalue[1]  & 0x0F)];
		   light_value.seg_B2 =  seg_brightness[(dpvalue[2]  >> 4)];
		   light_value.seg_T2 =  seg_temperature[(dpvalue[2]  & 0x0F)];
		   light_value.seg_B3 =  seg_brightness[(dpvalue[3]  >> 4)];
		   light_value.seg_T3 =  seg_temperature[(dpvalue[3] & 0x0F)];
		   light_value.seg_B4 =  seg_brightness[(dpvalue[4]  >> 4)];
		   light_value.seg_T4 =  seg_temperature[(dpvalue[4]  & 0x0F)];
		   light_value.seg_B5 =  seg_brightness[(dpvalue[5]  >> 4)];
		   light_value.seg_T5 =  seg_temperature[(dpvalue[5]  & 0x0F)];
		   flag = 1;
		   breathe_scene_flag = SCENE_WHITE;
		   remind_scene(3);
		   //light_value.scene_mode = SCENE_REMIND;
           } 
		   break;
#endif	
        default:
            break;
    }
	if(flag == 1)
	{
		app_light_write_to_storage_start();
	}
}
#else
void app_dps_download(unsigned char dpid, unsigned char dpty, unsigned char dplen, unsigned char *dpvalue){
	switch(dpid){
        case 0x01://onoff
			light_value_onoff = dpvalue[0];
			countdown = 0; 
			app_light_write_to_storage_start();
            break;
        case 0x02://work_mode
			light_value.scene_mode = dpvalue[0];
			app_light_write_to_storage_start();
			if(light_value.scene_mode == SCENE_MODEL){
				bSceneSetFirstFlag = TRUE;
				light_shade_flag = FALSE;
			}else if(light_value.scene_mode == SCENE_MUSIC){
				// music_h = 0;//360
				// music_s = 1000;//1000
				// music_v = 1000;//1000				
			}			
            break;
#if WHITE_FUNCTION			
       case 0x03://bright_value
			light_value.scene_mode = SCENE_WHITE;
			light_value.B = ((dpvalue[2]<<8) | dpvalue[3]);
			app_light_write_to_storage_start();
           break;
       case 0x04://temp_value
			light_value.scene_mode = SCENE_WHITE;
			light_value.T = ((dpvalue[2]<<8) | dpvalue[3]);
			app_light_write_to_storage_start();
           break;
#endif	
#if SENCE_FUNCTION	   			
        case 12:{//sence				
			light_value.scene_mode = SCENE_MODEL;
            light_value.sence_model = dpvalue[0];
			bSceneSetFirstFlag = TRUE;
			light_shade_flag = FALSE;
			app_light_write_to_storage_start();
		}break;
#endif			
		case 11:{//HSV
			light_value.H = ((dpvalue[0]<<8) | dpvalue[1]);//360
			light_value.S = dpvalue[2] * 10;//1000
			light_value.V = dpvalue[3] * 10;//1000
			light_value.scene_mode = SCENE_COLOR;
			app_light_write_to_storage_start();
			}
			break;
#if DSITURB_FUNCTION			
       case 34:{//do_not_disturb  value = bool
			app_light_ctrl_data_not_disturb_set(dpvalue[0]);
			app_light_write_to_storage_start();
    
           }
           break;
#endif
#if TIME_WORK_FUNCTION		   
       case 17:{//time work set
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
			remind_scene(3);		
			}
           break;
       case 18:{//time synchronization
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
			countdown = (dpvalue[0]<<24) | (dpvalue[1]<<16) | (dpvalue[2]<<8) | (dpvalue[3]);  
			countdown_target_onoff = !light_value_onoff;
			remind_scene(3);
			}
			break;
#endif			
#if MUSIC_FUNCTION			
        case 13://music
    		music_h = ((dpvalue[1]<<8) | dpvalue[2]);//360
			music_s = dpvalue[3] * 10;//1000
			music_v = dpvalue[4] * 10;//1000
			light_value.scene_mode = SCENE_MUSIC;
			app_light_write_to_storage_start();
            break;
#endif
        default:
            break;
    }
}
#endif






void app_light_init(void){
	light_value_onoff = 1;
	hal_storage_256_read_all();
	app_dps_read_from_storage();

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
		ty_light_ctrl_amplify_cw(1000, 1000);		
	}else if((light_factory_test_flag > 0)&&(light_factory_test_flag < 5)){
		if(hal_clock_time_exceed(time_t,1200000000)){
			time_t = hal_clock_get_system_tick();
			light_factory_test_flag ++;
			if(light_factory_test_flag == 3){
				ty_light_ctrl_amplify_cw(1000, 0);
			}else if(light_factory_test_flag == 4){
				// light_adjust_HSV_hw(0, 0,1000);
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
			
			app_light_def_state_init();
			remind_scene(7);
			uart_putc(0xc4);
			//light_value.scene_mode = SCENE_REMIND;
            break;
        case BEACON_EVENT_RESET:
            app_light_def_state_init();
			paring_result = 1;
            break;
		case BEACON_EVENT_PAIRING:
            app_light_pairing_state_init();
			uart_putc(0xa8);
            break;
        default:break;
    }
}

bool is_system_time_exceed_1s(void)
{
    static u32 pre_time = 0;
    if(hal_clock_time_exceed(pre_time, 1000000)){
        pre_time = hal_clock_get_system_tick();
        return true;
    }

    return false;

	
}

void tick_countdown_event_handle(void)
{
	static u32 T = 0;
	if (countdown > 0 && power_outage_detect_flag < 10)
	{

		if(hal_clock_time_exceed(T, 1000000)){
			countdown--;
			uart_putc(countdown);
       	 	T = hal_clock_get_system_tick();
			if ((countdown) == 0){
            set_onoff(countdown_target_onoff);
        	}	
    	}
		
	}
    return;
}

void app_light_run(void){
	static u32 T = 0;
	static u8 blink_cnt = 0;
	static u32 T_countdown = 0;
#if FACTORY_FUNCTION
		if(light_factory_test_flag > 0){
		   ty_beacon_enter_test_mode_cb(45);
			return;	
		}
#endif

#if SEGMENTED_DIMMER_FUNCTION	
	gpio_read_flag();
	power_outage_state();
#endif	


	if(ty_beacon_get_state() == DEVICE_STATE_PAIRING){
		if (first_pairing_flag == 0){
			light_value.B = 1000;
			light_value.T = 500;
			ty_light_ctrl_amplify_cw(1000, 500);
			first_pairing_flag = 1;
			remind_scene(7);
			uart_putc(0xc3);
		}
	}else{
		if(first_pairing_flag == 1){
			first_pairing_flag = 0;			
			app_dps_read_from_storage();
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

}

















