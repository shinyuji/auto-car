/*
 * drive.h
 */ 

#ifndef DRIVE_H_
#define DRIVE_H_

#include "common.h"
#include "motor.h"	//모터 초기화 사용

void manual_mode(void);
void auto_mode();
void distance_check(void);
void drive_processing();

#endif