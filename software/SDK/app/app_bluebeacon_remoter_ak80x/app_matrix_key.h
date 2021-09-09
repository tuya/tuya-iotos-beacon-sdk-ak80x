/*************************************************************************
	> File Name: app_key.h
	> Author: 
	> Mail: 
	> Created Time: Thu 22 Oct 2020 15:21:03 CST
 ************************************************************************/

#ifndef _APP_MATRIX_KEY_H
#define _APP_MATRIX_KEY_H

#include "board.h"

#define KB_RETURN_KEY_MAX	6

#pragma pack (1)
typedef enum {
	NONE_PRESS = 0,

	SINGLE_CLICK,
	DOUBLE_CLICK,
	TRIPLE_CLICK,

	LONG_PRESS_START,
	LONG_PRESS_RELEASE,

    COMB_PRESS

}PressEvent;

typedef struct{
	u8 cnt;
	u8 keycode[KB_RETURN_KEY_MAX];
}kb_data_t;

#pragma pack () 

#define TY_LED	BIT2

#define TY_WAKEUP_IO (BIT3|BIT14|BIT15|BIT4|BIT5)
#define TY_TRIGER_IO (BIT19|BIT7|BIT8|BIT6|BIT0)
//#define UNUSED_IO	(~(TY_WAKEUP_IO|TY_LED))
#define UNUSED_IO	(~(TY_WAKEUP_IO))

extern u8 key_state;

void app_matrix_key_init(void);
void app_matrix_key_run(void);

#endif
