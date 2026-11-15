/*
 * timer.c
 *
 * Timer0 1ms 인터럽트: 시간 계산, 초음파 측정 주기, 회전 시간
 */

#include "timer.h"
#include "ultrasonic.h"
#include <util/atomic.h>

static volatile uint16_t ms_count = 0;	// 1ms마다 증가
static uint32_t sec_count = 0;			// 주행 시작 후 경과 초

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6;	// 6~256: 250 count = 1ms
	ms_count++;
	ultrasonic_tick();

	if(car.turn_timer)
		car.turn_timer--;
}

void init_timer0(void)
{
	TCNT0 = 6;	// 6~256: 250개의 펄스를 count하기 위해 6으로 세팅

	TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));	// 분주비 bit clear
	TCCR0 |= 1 << CS02;		// 64분주 (16MHz / 64 = 250kHz)

	TIMSK |= 1 << TOIE0;	// TIMER0 Overflow INT(256이 되는 순간 인터럽트)
}

uint32_t get_sec_count(void)
{
	return sec_count;
}

void reset_sec_count(void)
{
	sec_count = 0;
}

void time_calc(void)
{
	static uint32_t last_sec = 0;
	uint16_t ms;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { ms = ms_count; }
	if(ms >= 1000)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { ms_count = 0; }
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
