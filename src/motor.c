/*
 * motor.c
 */ 

#include "motor.h"

void init_timer1_pwm(void)
{
	TCCR1B |= 1 << CS11 | 1 << CS10;   // 분주비 64

	// 모드 14 :  고속 PWM 모드 사용 하겠다. timer1 (P327 표14-5)
	TCCR1A |= 1 << WGM11;     // TOP --> ICR1으로 설정 
	TCCR1B |= 1 << WGM13 | 1 << WGM12;
	
	TCCR1A |= 1 << COM1A1;  
	TCCR1A |= 1 << COM1B1;  
	
	ICR1 = 0x3ff;  // 1023 * 4us ==> 4ms TOP 값 
}

void init_motor_driver(void)
{
	init_timer1_pwm();
	
	// 1. 출력 모드로 설정
	MOTOR_PWM_DDR &= ~(1 << 5 | 1 << 6);   // 0 으로 초기화 하고 시작 하자 
	MOTOR_PWM_DDR |= 1 << 5 | 1 << 6;
	MOTOR_DIRECTION_PORT_DDR &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DIRECTION_PORT_DDR |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;   // 출력 모드로 설정
	
	// 2. 모터를 전진 모드로 
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);  // 0 으로 초기화 하고 시작 
	MOTOR_DIRECTION_PORT |=  1 << 2 | 1 << 0;   // 모터를 전진 모드로 IN4IN3IN2IN1
	                                            //                    0  1 0  1
}

void backward(int speed)
{
	//  모터를 후진 모드로
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);  // 0 으로 초기화 하고 시작
	MOTOR_DIRECTION_PORT |=  1 << 3 | 1 << 1;   // 모터를 후진 모드로 IN4IN3IN2IN1
	                                            //                    1 0  1  0
	OCR1A=OCR1B=speed;   // OCR1A: PWM left OCR1B: PWM right
}

void forward(int left_speed, int right_speed)
{
	// 모터를 전진 모드로
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);  // 0으로 초기화
	MOTOR_DIRECTION_PORT |=  1 << 2 | 1 << 0;   // 모터를 전진 모드로 IN4 IN3 IN2 IN1
	OCR1A = left_speed;
	OCR1B = right_speed;   // OCR1A: PWM left OCR1B: PWM right
}

void turn_left(int speed)
{
	MOTOR_DIRECTION_PORT &= ~((1<<0)|(1<<1)|(1<<2)|(1<<3));

	// 왼쪽 후진, 오른쪽 전진
	MOTOR_DIRECTION_PORT |= (1<<3) | (1<<0);

	OCR1A = speed;
	OCR1B = speed;
}

void turn_right(int speed)
{
	MOTOR_DIRECTION_PORT &= ~((1<<0)|(1<<1)|(1<<2)|(1<<3));

	// 왼쪽 전진, 오른쪽 후진
	MOTOR_DIRECTION_PORT |= (1<<2) | (1<<1);

	OCR1A = speed;
	OCR1B = speed;
}
void stop(void)
{
	//  모터를 stop 모드로
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);  // 0 으로 초기화 하고 시작
	MOTOR_DIRECTION_PORT |=  1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;   // 모터를 stop 모드로 IN4IN3IN2IN1
	//                    0  1 0  1
	OCR1A=0;
	OCR1B=0;   // OCR1A: PWM left OCR1B: PWM right
}

