/*************************************************************************
	> File Name: app_remoter.h
	> Author: 
	> Mail: 
	> Created Time: Thu 22 Oct 2020 15:21:03 CST
 ************************************************************************/

#ifndef _APP_REMOTER_H
#define _APP_REMOTER_H

#include "app_matrix_key.h"

void app_remoter_ram_restore(void);
void app_remoter_ram_save(void);

void ty_beacon_key_event(PressEvent key_state, kb_data_t* kb_data);

u8 app_led_blink_times_get(void);
void app_led_blink_times_set(u8 blink_times);
#endif
