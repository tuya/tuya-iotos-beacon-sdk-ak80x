/*************************************************************************
	> File Name: hal_irq.h
	> Author: 
	> Mail: 
	> Created Time: Thu 07 Mar 2019 18:15:57 CST
 ************************************************************************/

#ifndef _HAL_IRQ_H
#define _HAL_IRQ_H

#include "hal_sys.h"

extern u8 hal_irq_enable(void);
extern u8 hal_irq_disable(void);
extern void hal_irq_restore(u8 en);

#endif

