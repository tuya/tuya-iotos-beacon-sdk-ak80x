/****************************************************************************
@FILENAME:	app_select.h
@FUNCTION:	selcet the build project
@AUTHOR:	flege.zhang
@DATE:		2020.05.09
@COPYRIGHT(C) ZXW 2020
*****************************************************************************/

#include "app_common.h"
/*********************************    PWM    ********************************/
#define PWM_FRQ_SELECTION							PWM_FRQ_1KHZ

#define PWM_CHANNEL_RGB_R					        PWM_CHANNEL_4//GPIO7
#define PWM_CHANNEL_RGB_G					        PWM_CHANNEL_1//GPIO4
#define PWM_CHANNEL_RGB_B					        PWM_CHANNEL_0//GPIO3
//#define PWM_CHANNEL_RGB_C					        PWM_CHANNEL_3//GPIO6
//#define PWM_CHANNEL_RGB_W					        PWM_CHANNEL_2//GPIO5
//driver_bp5758d_pin
#define BP5758D_SCL					        BIT3//GPIO3		
#define BP5758D_SDA					        BIT4//GPIO4
/*********************************    ADV    ********************************/
#define SEND_BEACON_INTERVAL							(20)//20ms
#define SEND_BEACON_CHANNEL								ADV_CHANNEL_BIT_37









