/*************************************************************************
	> File Name: bal_beacon.h
	> Author: 
	> Mail: 
	> Created Time: Sat 19 Dec 2020 23:48:05 CST
 ************************************************************************/

#ifndef _BAL_BEACON_H
#define _BAL_BEACON_H

#include "board.h"

/*
 * tx_cnt：（发送次数）37,38,39轮流发送的总次
 * rx_cnt：（接收次数）37,38,39轮流接收的总次
 * 调用该函数，先执行tx_cnt次发送，再执行rx_cnt次接
 */
extern void bal_beacon_txrx(u32 tx_cnt, u32 rx_cnt);


#endif
