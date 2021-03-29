/****************************************************************************
@FILENAME:	app_cfg.h
@FUNCTION:	app config
@AUTHOR:	flege.zhang
@DATE:		2019.08.14
*****************************************************************************/

#ifndef _APP_CFG__H_
#define _APP_CFG__H_

#include "config.h"
#include "uart.h"
#include "print.h"
#include "app_select.h"


/****************************PROJECT SPECIAL DEFINE****************************/


/******************************BLE ENCODER VERSION*****************************/
#define RF_SEND_DIFF_PACKET_NUM						(0x01)


/*****************************RETENTION MEMORY MAP*****************************/


/****************************ABNORMAL PROTEC FUNCTION**************************/
#ifndef DEBUG
    #define ABNORMAL_PROTECTION_TYPE                    ENABLE
#else
    #define ABNORMAL_PROTECTION_TYPE                    DISABLE
#endif

#define SYS_CRASH_WTD_RESET_ENABLE					DISABLE
#if(SYS_CRASH_WTD_RESET_ENABLE)
#define WATCH_DOG_RESET_TIME						2000//unit:ms
#endif

/*******************************E2PROM DEFINE**********************************/
#define E2PROM_ENABLE								DISABLE


/*********************************TEST FUNCTION********************************/


/******************************LOG DEBUG FUNCTION******************************/
#ifndef DEBUG
    #define LOG_ENABLE								    DISABLE    //default disable log  XXX
#else
    #define LOG_ENABLE                                  ENABLE
#endif

#if(LOG_ENABLE)
#define LOG_LEVEL									LOG_LEVEL_MAJOR	//selection the log level
#else
#define LOG_LEVEL									LOG_LEVEL_OFF	//off log
#endif

#define LOG_FATAL									(LOG_LEVEL >= LOG_LEVEL_FATAL)
#define LOG_ERROR									(LOG_LEVEL >= LOG_LEVEL_ERROR)
#define LOG_WARN									(LOG_LEVEL >= LOG_LEVEL_WARN)
#define LOG_MAJOR									(LOG_LEVEL >= LOG_LEVEL_MAJOR)
#define LOG_SECONDARY								(LOG_LEVEL >= LOG_LEVEL_SECONDARY)
#define LOG_DETAIL									(LOG_LEVEL >= LOG_LEVEL_DETAIL)

#define GROUP_INDEX_MAX							(3)

#endif
