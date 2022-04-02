/*************************************************************************
	> File Name: ty_xxtea.h
	> Author: 
	> Mail: 
	> Created Time: Tue 27 Oct 2020 10:51:49 CST
 ************************************************************************/

#ifndef _TY_XXTEA_H
#define _TY_XXTEA_H
	
#include "board.h"

//abs(n)<=4*100, abs(n) % 4 == 0, abs(n) = sizeof(v), left date fill 0x00
int ty_xxtea_u8(u8 *v, int n, u8 const k[16]);

#endif
