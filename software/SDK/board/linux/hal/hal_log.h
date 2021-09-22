/*************************************************************************
	> File Name: hal_log.h
	> Author: 
	> Mail: 
	> Created Time: Wed 03 Jul 2019 22:05:09 CST
 ************************************************************************/

#ifndef _HAL_LOG_H
#define _HAL_LOG_H


#define DEBUG 1

#if DEBUG
#define PR_DEBUG(_fmt_, ...) \
        printf("[dbg]%-30s:%d "_fmt_,__FUNCTION__,__LINE__,##__VA_ARGS__)
#define PR_DEBUG_RAW(_fmt_, ...) \
        printf(_fmt_,##__VA_ARGS__)
#define PR_DEBUG_HEX_ARRAY(array,len) do{ \
    if((len) == 0)break; \
    printf("%02X",(array)[0]); \
    for(u8 i=1;i<(len);i++){\
        printf(" %02X",(array)[i]); \
    } \
}while(0)

#else
#define PR_DEBUG(...)
#define PR_DEBUG_RAW(_fmt_, ...)
#define PR_DEBUG_ARRAY(data,len)
#endif

#define PR_NOTICE(_fmt_, ...) \
        printf("[notice]%-30s:%d "_fmt_, __FUNCTION____,__LINE__,##__VA_ARGS__)
#define PR_ERR(_fmt_, ...) \
        printf("[err]%-30s:%d "_fmt_, __FUNCTION____,__LINE__,##__VA_ARGS__)


#define print printf
#endif
