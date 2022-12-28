/****************************************************************************
@FILENAME:	app_common.h
@FUNCTION:	app common define
@AUTHOR:	flege.zhang
@DATE:		2019.08.20
@COPYRIGHT(C) ZXW 2020
*****************************************************************************/

#include "string.h"
#include "uart.h"
#include "type.h"


/******************************PWM FRQ DEFINE********************************/
#define PWM_FRQ_NA							(0x00)
#define PWM_FRQ_250HZ						(64000)				//16BIT
#define PWM_FRQ_500HZ						(32000)				//15BIT
#define PWM_FRQ_580HZ						(27586)				//15BIT
#define PWM_FRQ_1KHZ						(16000)				//14BIT
#define PWM_FRQ_2KHZ						(PWM_FRQ_1KHZ>>1)
#define PWM_FRQ_3KHZ						(PWM_FRQ_1KHZ/3)
#define PWM_FRQ_4KHZ						(PWM_FRQ_1KHZ>>2)
#define PWM_FRQ_5KHZ						(PWM_FRQ_1KHZ/5)
#define PWM_FRQ_8KHZ						(PWM_FRQ_1KHZ>>3)
#define PWM_FRQ_16KHZ						(PWM_FRQ_1KHZ>>4)
#define PWM_FRQ_20KHZ						(PWM_FRQ_1KHZ/20)	//10BIT,800

/******************************COMMON DEFINE*********************************/
#define DISABLE								(0x00)
#define ENABLE								(0x01)


/****************************LOG LEVEL DEFINE********************************/
#define LOG_LEVEL_OFF						(0x00)
#define LOG_LEVEL_FATAL						(0x01)
#define LOG_LEVEL_ERROR						(0x02)
#define LOG_LEVEL_WARN						(0x04)
#define LOG_LEVEL_MAJOR						(0x10)
#define LOG_LEVEL_SECONDARY					(0x20)
#define LOG_LEVEL_DETAIL					(0x80)
#define LOG_LEVEL_ALL						(0xFF)

/***************************WAKEUP SOURCE DEFINE*****************************/
#define WAKEUP_SOURCE_NA					(0x00)
#define WAKEUP_SOURCE_AC					(0x01)
#define WAKEUP_SOURCE_LVD					(0x02)


/***************************HW DETECT TYPE DEFINE****************************/
#define DECODE_V8T04A_TYPE					(0x0101)
#define DECODE_V8T04A_16_TYPE				(0x0102)
#define DECODE_UART_TYPE					(0x0201)
#define DECODE_NEC_TYPE						(0x0301)
#define DECODE_433_TYPE						(0x0401)
/*******************************PROJECT DEFINE*******************************/
//BIT[31:28]:PROTOCOL TYPE
#define BLE									(0x30000000)//sig ble packet
#define PRT									(0x40000000)//private 2.4g packet

//BIT[27:24]:DETECT TYPE
#define RCSCAN								(0x01000000)//row and column scanning

//BIT[23:16]:FOR DEVICE TYPE
#define LMP									(0x00010000)//lamp

