/*************************************************************************
	> File Name: ty_timing.h
	> Author: 
	> Mail: 
	> Created Time: Thu 01 Aug 2019 21:36:49 CST
 ************************************************************************/

#ifndef _TY_TIMING_H
#define _TY_TIMING_H

#include "board.h"

//
//enum{
//    TIMING_STATE_NO = 0x00,
//	TIMING_STATE_IN = 0x01,
//	TIMING_STATE_END = 0x02,
//};
#define MAX_TIMING_NUM 10 //time work MAX
//
struct ITiming{
    u8 enable:1;                    //是否使能该定时动作
    u8 repet:1;                     //是否重复定时
    u8 timing_state:4;              //当前定时的状态
    u8 day;                         //一周中有哪些天是重复定时
    u16 time;                       //分钟来计算的一天的时间   
    u8 (*timing_end_cb)(u8 index);  //定时触发回调函数 
};

struct ITime_Work{
    u8 type;       //类型：onoff、cw(b、t)、color(h、s、v)、sence
	u8 onoff;	   //开关
    u8 B;          //bright
    u8 T;          //temperture
    u8 H;          //hue
    u8 S;          //sat
    u8 V;          //value
    u8 senceID;    //sence id
};


struct ITimingFunc{
    void (*init)(void);
    void (*set)(struct ITiming *pITiming,u8 enable,u16 time,u8 repet,u8 day,u8 (*cb)(u8 index));
    void (*kill)(struct ITiming *pITiming);
    void (*run)(struct ITiming *pITiming, u8 num);
};


struct ITimingWorkFunc{
    void (*work_set)(u8 *workdata);
    void (*time_syn)(u8 *timedata);
};


extern struct ITiming mITiming[MAX_TIMING_NUM];
extern struct ITime_Work mITime_Work[MAX_TIMING_NUM];
extern struct ITimingFunc       mITimingFunc;
extern struct ITimingWorkFunc   mITimingWorkFunc;

#endif
