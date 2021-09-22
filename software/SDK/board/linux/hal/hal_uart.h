/*************************************************************************
	> File Name: hal_uart.h
	> Author: 
	> Mail: 
	> Created Time: Thu 07 Jun 2018 08:32:09 PM CST
 ************************************************************************/

#ifndef _HAL_UART_H
#define _HAL_UART_H

#include "hal_sys.h"

extern int hal_uart_init(void);
extern int hal_uart_send(u8 *datas, u8 length);
extern int hal_uart_read(u8 *datas, u8 length); 

#endif
