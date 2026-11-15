/*
 * AUTO_CAR
 */

#include "common.h"
#include "button.h"
#include "motor.h"
#include "drive.h"
#include "ultrasonic.h"
#include "fnd.h"
#include "timer.h"
#include "uart0.h"
#include "uart1.h"

volatile Car car = {0};

static void init_all(void);

int main(void)
{
	init_all();
	sei();

	while (1)
	{
		button_event();
		ultrasonic_processing();
		drive_processing();
		time_calc();
		fnd_display();
	}
}

static void init_all(void)
{
	init_timer0();
	init_button();
	init_motor_driver();
	init_uart0();
	init_uart1();
	init_ultrasonic();
	init_fnd();
}
