/*************************************************************************
	> File Name: app_dps.h
	> Author: 
	> Mail: 
	> Created Time: Thu 22 Oct 2020 15:21:03 CST
 ************************************************************************/

#ifndef _APP_DPS_H
#define _APP_DPS_H


void app_dps_download(unsigned char kind, unsigned char dpid, unsigned char dpty, unsigned char dplen, unsigned char *dpvalue);
void light_adjust_WC(unsigned short warm,unsigned short white);

#endif
