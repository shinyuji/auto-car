/*
 * ultrasonic.c
 */

#include "ultrasonic.h"

static volatile uint8_t ultrasonic_check_time = 0;	// 1ms마다 증가, 60ms마다 다음 센서 측정

// Echo 펄스 폭(Timer3, 4us 단위) -> 거리(cm). 16bit 카운터가 넘쳐도 뺄셈 결과는 올바름
#define ECHO_TO_CM(start) (((uint32_t)(uint16_t)(TCNT3 - (start)) * 4) / 58)

void ultrasonic_tick(void)
{
	if(ultrasonic_check_time < 255)
		ultrasonic_check_time++;
}

ISR(INT4_vect)
{
    if(ECHO_PORT & (1 << ECHO_PIN_LEFT))
    {
        car.sensor[DIR_LEFT].start_time = TCNT3;
        car.sensor[DIR_LEFT].is_measuring = 1;
    }
    else if(car.sensor[DIR_LEFT].is_measuring)
    {
        car.sensor[DIR_LEFT].distance = ECHO_TO_CM(car.sensor[DIR_LEFT].start_time);
        car.sensor[DIR_LEFT].is_measuring = 0;
    }
}

ISR(INT5_vect)
{
    if(ECHO_PORT & (1 << ECHO_PIN_FRONT))
    {
        car.sensor[DIR_FRONT].start_time = TCNT3;
        car.sensor[DIR_FRONT].is_measuring = 1;
    }
    else if(car.sensor[DIR_FRONT].is_measuring)
    {
		car.sensor[DIR_FRONT].distance = ECHO_TO_CM(car.sensor[DIR_FRONT].start_time);
		car.sensor[DIR_FRONT].is_measuring = 0;
    }
}

ISR(INT6_vect)
{
    if(ECHO_PORT & (1 << ECHO_PIN_RIGHT))
    {
        car.sensor[DIR_RIGHT].start_time = TCNT3;
        car.sensor[DIR_RIGHT].is_measuring = 1;
    }
    else if(car.sensor[DIR_RIGHT].is_measuring)
    {
		car.sensor[DIR_RIGHT].distance = ECHO_TO_CM(car.sensor[DIR_RIGHT].start_time);
		car.sensor[DIR_RIGHT].is_measuring = 0;
    }
}

void init_ultrasonic(void)
{
    //Trigger 출력
    TRIG_DDR |= (1 << TRIG_PIN_LEFT) | (1 << TRIG_PIN_FRONT) | (1 << TRIG_PIN_RIGHT);

    //Echo 입력
    ECHO_DDR &= ~((1 << ECHO_PIN_LEFT) | (1 << ECHO_PIN_FRONT) | (1 << ECHO_PIN_RIGHT));

    //Trigger 초기값 LOW
    TRIG_PORT &= ~((1 << TRIG_PIN_LEFT) | (1 << TRIG_PIN_FRONT) | (1 << TRIG_PIN_RIGHT));

    //INT4~6: Echo 상승·하강 에지 모두에서 인터럽트
    EICRB |= (1 << ISC40) | (1 << ISC50) | (1 << ISC60);
    EIMSK |= (1 << INT4) | (1 << INT5) | (1 << INT6);

    //Timer3: 64분주, 1 count = 4us (펄스 폭 측정용)
    TCCR3A = 0;
    TCCR3B = 0;
    TCCR3B |= (1 << CS31) | (1 << CS30);
    TCNT3 = 0;
}

//해당 센서 Trig 핀에 10us 이상 HIGH 펄스
static void make_trigger(uint8_t sensor)
{
    TRIG_PORT &= ~(1 << sensor);
    _delay_us(2);
    TRIG_PORT |= (1 << sensor);
    _delay_us(15);
    TRIG_PORT &= ~(1 << sensor);
}

//60ms마다 센서를 하나씩 순환 측정 (센서 간 간섭 방지)
void ultrasonic_processing(void)
{
    static uint8_t sensor_index = 0;	// TRIG_PIN_LEFT/FRONT/RIGHT = 0/1/2

    if(ultrasonic_check_time >= 60)
    {
        ultrasonic_check_time = 0;
        make_trigger(sensor_index);
        sensor_index = (sensor_index + 1) % 3;
    }
}
