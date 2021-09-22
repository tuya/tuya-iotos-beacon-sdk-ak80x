/*************************************************************************
	> File Name: hal_irq.c
	> Author: 
	> Mail: 
	> Created Time: Thu 07 Mar 2019 18:16:01 CST
 ************************************************************************/
#include "hal_irq.h"

static u8 reg_irq_en = 0;

u8 hal_irq_enable(void){
	u8 r = reg_irq_en;		// don't worry,  the compiler will optimize the return value if not used
	reg_irq_en = 1;
	return r;
}

u8 hal_irq_disable(void){
	u8 r = reg_irq_en;		// don't worry,  the compiler will optimize the return value if not used
	reg_irq_en = 0;
	return r;
}

void hal_irq_restore(u8 en){
	reg_irq_en = en;
}

