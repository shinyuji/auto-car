/*
 * timer.h
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "common.h"

void init_timer0(void);
void time_calc(void);
uint32_t get_sec_count(void);
void reset_sec_count(void);

#endif /* TIMER_H_ */
