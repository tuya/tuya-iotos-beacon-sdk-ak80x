/*************************************************************************
	> File Name: ty_rtc.h
	> Author: 
	> Mail: 
	> Created Time: 2019年08月03日 星期六 10时48分44秒
 ************************************************************************/

#ifndef _TY_RTC_H
#define _TY_RTC_H

#include "board.h"

typedef struct{ 
}ty_rtc_params_s;

// //from 2000/01/01/00/00/00 epoch time
// typedef struct{ 
//   // u8 second;//0~59              
//   // u8 minute;//0~59                   
//   // u8 hour;//0~23                    
//   // u8 weekday;//1~7(7:星期天)          
//   // u8 monthday;//1~31                        
//   // u8 month;//1~12             
//   // u16 year;//  

// }ty_rtc_date_s;

// #define TY_RTC_Month_January                ((u8)0x01)
// #define TY_RTC_Month_February               ((u8)0x02)
// #define TY_RTC_Month_March                  ((u8)0x03)
// #define TY_RTC_Month_April                  ((u8)0x04)
// #define TY_RTC_Month_May                    ((u8)0x05)
// #define TY_RTC_Month_June                   ((u8)0x06)
// #define TY_RTC_Month_July                   ((u8)0x07)
// #define TY_RTC_Month_August                 ((u8)0x08)
// #define TY_RTC_Month_September              ((u8)0x09)
// #define TY_RTC_Month_October                ((u8)0x10)
// #define TY_RTC_Month_November               ((u8)0x11)
// #define TY_RTC_Month_December               ((u8)0x12)

// #define TY_RTC_Weekday_Monday               ((u8)0x01)
// #define TY_RTC_Weekday_Tuesday              ((u8)0x02)
// #define TY_RTC_Weekday_Wednesday            ((u8)0x03)
// #define TY_RTC_Weekday_Thursday             ((u8)0x04)
// #define TY_RTC_Weekday_Friday               ((u8)0x05)
// #define TY_RTC_Weekday_Saturday             ((u8)0x06)
// #define TY_RTC_Weekday_Sunday               ((u8)0x07)

// #define TY_RTC_IS_SECOND(N) ((N) < 60)
// #define TY_RTC_IS_MINUTE(N) ((N) < 60)
// #define TY_RTC_IS_HOUR(N) ((N) < 24)
// #define TY_RTC_IS_WEEKDAY(N) (((N) > 0) && ((N) < 8))
// #define TY_RTC_IS_MONTHDAY(N) (((N) > 0) && ((N) < 32))
// #define TY_RTC_IS_MONTH(N) (((N) > 0) && ((N) < 13))
// #define TY_RTC_IS_YEAR(N) (((N) > 0) && ((N) < 4000))

// #define TY_RTC_IS_MATCH_WEEKDAY(N) ((N) < 8)
// #define TY_RTC_IS_MATCH_MONTHDAY(N) ((N) < 32)
// #define TY_RTC_IS_MATCH_MONTH(N) ((N) < 13)
// #define TY_RTC_IS_MATCH_YEAR(N) ((N) < 4000)


// #define ty_rtc_cmp_kind_s                   u8
// #define TY_RTC_IS_SAME_SECOND               ((u8)0x00)
// #define TY_RTC_IS_SAME_MINUTE               ((u8)0x02)
// #define TY_RTC_IS_SAME_HOUR                 ((u8)0x03)
// #define TY_RTC_IS_SAME_DAY                  ((u8)0x04)
// #define TY_RTC_IS_SAME_MONTH                ((u8)0x05)
// #define TY_RTC_IS_SAME_YEAR                 ((u8)0x06)

typedef struct{
    u8 (*init)(ty_rtc_params_s *param);
    void (*set_date)(u32 time_unix, s16 time_zone);
    int (*get_date)(u8 *weekday, u16 *daytime);//OUT u8 *weekday, OUT u16 *daytime
    // void (*date_to_utc)(ty_rtc_date_s date, u32 *utc_time);//utc - second 
    // void (*utc_to_date)(u32 utc_time, ty_rtc_date_s *date);
    // u8 (*is_same)(ty_rtc_date_s *time_stamp1, ty_rtc_date_s *time_stamp2, ty_rtc_cmp_kind_s kind);
    void (*run)(void);
}ty_rtc_s;

extern ty_rtc_s ty_rtc;

#endif
