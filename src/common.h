/*
 * common.h
 */ 

#ifndef COMMON_H_
#define COMMON_H_

#define F_CPU 16000000UL    // 16MHz

#include <avr/io.h>         
#include <util/delay.h>     
#include <avr/interrupt.h>  
#include <stdio.h>

typedef enum {
    MANUAL_MODE,
    AUTO_MODE
} DriveMode;

typedef enum {
    DONE,
    DRIVE
} State;

typedef enum {
	STOP,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
} DriveState;

typedef enum {
	RUNTIME_COUNT,
	BACKWARD_COUNT,
	LEFT_COUNT,
	RIGHT_COUNT
} DoneState;

typedef enum {
    DIR_LEFT,
    DIR_FRONT,
    DIR_RIGHT
} SensorDir;

// 초음파센서 구조체
typedef struct {
	int start_time;
	int distance;
	int  is_measuring;
} UltrasonicSensor;

//자동차 구조체
typedef struct {
    DriveMode mode;             //현재 모드
    DriveMode prev_mode;        //이전 모드

    UltrasonicSensor sensor[3]; // 초음파 데이터 (LEFT, FRONT, RIGHT 순서)
  
	State state;					//현재 상태 state 저장 - fnd에서도 사용
	DriveState drive_state;
	DoneState done_state;
	
	//도는데 걸리는 초를 재기 위한 타이머.
	int turn_timer;
	
	int count_d;	//주행 초
	int count_b;	//후진한 횟수
	int count_l;	//우측으로 꺾은 횟수
	int count_r;	//오른쪽으로 꺾은 횟수
	
} Car;

extern volatile Car car;
extern volatile UltrasonicSensor sensors[3];

#endif