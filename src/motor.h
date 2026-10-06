/*
 * pwm.h
 */ 

#ifndef MOTOR_H_
#define MOTOR_H_

#include "common.h"

#define MOTOR_PWM_DDR  DDRB
#define MOTOR_LEFT_PORT_DDR  5    // OC1A
#define MOTOR_RIGHT_PORT_DDR  6   // OC1B

#define MOTOR_DIRECTION_PORT  PORTF
#define MOTOR_DIRECTION_PORT_DDR  DDRF

void init_motor_driver(void);
void init_timer1_pwm(void);

void forward(int left_speed, int right_speed);
void backward(int speed);
void turn_left(int speed);
void turn_right(int speed);
void stop(void);

#endif