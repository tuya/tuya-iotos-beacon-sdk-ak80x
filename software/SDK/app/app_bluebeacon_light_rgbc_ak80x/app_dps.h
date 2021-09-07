/*************************************************************************
	> File Name: app_dps.h
	> Author: 
	> Mail: 
	> Created Time: Thu 22 Oct 2020 15:21:03 CST
 ************************************************************************/

#ifndef _APP_DPS_H
#define _APP_DPS_H


void app_dps_download(unsigned char dpid, unsigned char dpty, unsigned char dplen, unsigned char *dpvalue);
void light_adjust_RGBWC_hw(unsigned char red,unsigned char green,unsigned char blue,unsigned char warm,unsigned char white,unsigned char lum);

#endif
