/*
 * AUTO_CAR
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>    // sei 등 함수 
#include <stdio.h>

#include "common.h"

void init_timer0(void);
void time_calc();
void init_all(void);

extern volatile uint32_t turn_timer;
volatile Car car = {0};
volatile int ultrasonic_check_time = 0;

extern volatile uint32_t ms_count;
extern volatile uint32_t sec_count;

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6;
	ms_count++;
	ultrasonic_check_time++;
	
	if(car.turn_timer)
		car.turn_timer--;
}


int main(void)
{


   init_all();
   sei();

    while (1) 
    {

		button_event();
		ultrasonic_processsing();
		drive_processing();
		time_calc();
		fnd_display();

	}
}

void init_all(void){
	init_timer0();
	init_button();
	init_motor_driver();
	init_uart0();
	init_uart1();
	init_ultrasonic();
	init_fnd();
}

void init_timer0(void)
{
	TCNT0 = 6;	// 6~256: 250개의 펄스를 count하기 위해 6으로 세팅
	
	TCCR0 &= ~((1 << CS02) | (0 << CS01) | (0 << CS00));	//0분주 - reset
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;		//64분주
	
	TIMSK |= 1 << TOIE0;	// TIMER0 Overflow INT(256이 되는 순간 인터럽트)

}

void time_calc(void){
	
	static uint32_t last_sec = 0;
		
	if(ms_count >= 1000)
	{
		ms_count = 0;
		sec_count++;
	}
	
	//1초가 지날 때 마다
	if (sec_count != last_sec)
	{
		//주행 시간 증가
		if(car.state != DONE && car.drive_state != STOP)
		{
			car.count_d++;
		}
		//last_sec 업데이트
		last_sec = sec_count;
	}
	
}
