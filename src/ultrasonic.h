/*
 * ultrasonic.h
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "common.h"

#define TRIG_DDR     DDRA
#define TRIG_PORT    PORTA
#define ECHO_DDR     DDRE
#define ECHO_PORT    PINE

#define TRIG_PIN_LEFT   0
#define TRIG_PIN_FRONT  1
#define TRIG_PIN_RIGHT  2

#define ECHO_PIN_LEFT   4
#define ECHO_PIN_FRONT  5
#define ECHO_PIN_RIGHT  6

void init_ultrasonic(void);
void ultrasonic_processing(void);
void ultrasonic_tick(void);	// Timer0 1ms 인터럽트에서 호출

#endif /* ULTRASONIC_H_ */
