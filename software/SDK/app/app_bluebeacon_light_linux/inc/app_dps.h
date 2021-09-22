/*************************************************************************
	> File Name: app_dps.h
	> Author: 
	> Mail: 
	> Created Time: Thu 22 Oct 2020 15:21:03 CST
 ************************************************************************/

#ifndef _APP_DPS_H
#define _APP_DPS_H

void app_light_run(void);
void app_dps_download(unsigned char dpid, unsigned char dpty, unsigned char dplen, unsigned char *dpvalue);

#endif
