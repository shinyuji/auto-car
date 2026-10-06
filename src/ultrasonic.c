/*
 * ultrasonic.c
 */

#include "ultrasonic.h"

extern volatile int ultrasonic_check_time;
extern volatile Car car;

volatile UltrasonicSensor sensors[3];

ISR(INT4_vect)
{
    if(ECHO_PORT & (1 << ECHO_PIN_LEFT))
    {
        sensors[0].start_time = TCNT3;
        sensors[0].is_measuring = 1;
    }
    else if(sensors[0].is_measuring)
    {
        car.sensor[DIR_LEFT].distance = ((TCNT3 - sensors[0].start_time) * 4) / 58;
        sensors[0].is_measuring = 0;
    }
}

ISR(INT5_vect)
{
    if(ECHO_PORT & (1 << ECHO_PIN_FRONT))
    {
        sensors[1].start_time = TCNT3;
        sensors[1].is_measuring = 1;
    }
    else if(sensors[1].is_measuring)
    {
		car.sensor[DIR_FRONT].distance = ((TCNT3 - sensors[1].start_time) * 4) / 58;
		sensors[1].is_measuring = 0;
    }
}

ISR(INT6_vect)
{
    if(ECHO_PORT & (1 << ECHO_PIN_RIGHT))
    {
        sensors[2].start_time = TCNT3;
        sensors[2].is_measuring = 1;
    }
    else if(sensors[2].is_measuring)
    {
		car.sensor[DIR_RIGHT].distance = ((TCNT3 - sensors[2].start_time) * 4) / 58;
		sensors[2].is_measuring = 0;
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

    EICRB |= (1 << ISC40) | (1 << ISC50) | (1 << ISC60);

    EIMSK |= (1 << INT4) | (1 << INT5) | (1 << INT6);

    TCCR3A = 0;
    TCCR3B = 0;

    TCCR3B |= (1 << CS31) | (1 << CS30);
    TCNT3 = 0;
}

void make_trigger(uint8_t sensor)
{
    TRIG_PORT &= ~(1 << sensor);
    _delay_us(2);
    TRIG_PORT |= (1 << sensor);
    _delay_us(15);
    TRIG_PORT &= ~(1 << sensor);
}

void ultrasonic_processsing(void)
{
    static uint32_t sensor_index = 0;

    if(ultrasonic_check_time >= 60)
    {
        ultrasonic_check_time = 0;
        make_trigger(sensor_index);
        sensor_index = (sensor_index + 1) % 3;
    }
}

