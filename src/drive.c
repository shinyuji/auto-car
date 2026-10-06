/*
 * drive.c
 */ 

#include "drive.h"

int func_state = MANUAL_MODE;
extern volatile uint8_t bt_data;
extern volatile Car car;

void BT_print_distance(void);

int limit = 20;
int max_speed = 450;
int min_speed = 300;

void (*pfunc[]) () =
{
	manual_mode,
	auto_mode,
	distance_check
};

void manual_mode(void)
{
	char cmd = bt_data;
	bt_data = 0;

	switch(cmd) {
		case 'F':
		case 'f':
		car.state = DRIVE;
		car.drive_state = FORWARD;
		forward(500, 500);
		break;
		
		case 'B':
		case 'b':
		car.state = DRIVE;
		if (car.drive_state != BACKWARD) {
			car.count_b++;
		}
		car.drive_state = BACKWARD;
		backward(500);
		break;
		
		case 'L':
		case 'l':
		car.state = DRIVE;
		if (car.drive_state != LEFT) {
			car.count_l++;
		}
		car.drive_state = LEFT;
		forward(0, 500);
		break;
		
		case 'R':
		case 'r':
		car.state = DRIVE;
		if (car.drive_state != RIGHT) {
			car.count_r++;
		}
		car.drive_state = RIGHT;
		forward(500, 0);
		break;
		
		case 'S':
		case 's':
		car.drive_state = STOP;
		stop();
		break;
		
		default:
		break;
	}
}

void auto_mode(void)
{
	int front = car.sensor[DIR_FRONT].distance;
	int left  = car.sensor[DIR_LEFT].distance;
	int right = car.sensor[DIR_RIGHT].distance;

	static int is_counted = 0;

	char cmd = bt_data;
	bt_data = 0;

	if(cmd == 'S' || cmd == 's')
	{
		car.drive_state = STOP;
	}
	
	if(cmd == 'F' || cmd == 'f')
	{
		car.drive_state = FORWARD;
	}

	if(front < 5 && left < 15 && right < 15)
	{
		car.drive_state = STOP;
	}
	
	int diff = abs(left-right);

	switch(car.drive_state)
	{
		case STOP:
		stop();
		return;
		
		case FORWARD:
		if (front >= 40 && left <= 40 && right <= 40) {
			is_counted = 0;
		}
		
		// 왼쪽이 비교적 넓으면
		if(left > 40 && left > right + 3)
		{
			car.drive_state = LEFT;	//왼쪽으로 돌기
			if(is_counted == 0) {
				car.count_l++;
				is_counted = 1;     // 증가했으므로 플래그 차단
			}
			car.turn_timer = 15;
			return;
		}
		// 오른쪽이 비교적 넓으면
		else if(right > 40 && right > left + 3)
		{
			car.drive_state = RIGHT;	//오른쪽으로 돌기
			if(is_counted == 0) {
				car.count_r++;
				is_counted = 1;
			}
			car.turn_timer = 15;
			return;
		}
		
		// 전방이 가까우면
		if(front < 40)
		{
			if(left > right)	//왼쪽이 더 넓으면
			{
				car.drive_state = LEFT;	//왼쪽으로 돌기
				if(is_counted == 0) {
					car.count_l++;
					is_counted = 1;
				}
				car.turn_timer = 15;
			}
			else
			{
				car.drive_state = RIGHT;	//오른쪽으로 돌기
				if(is_counted == 0) {
					car.count_r++;
					is_counted = 1;
				}
				car.turn_timer = 15;
			}
			return;
		}
		
		// 전방이 가깝지 않고 양쪽의 거리가 차이 나면 값 조정 (라인 유지 주행)
		else if(diff > 2)
		{
			int error = left - right;
			int correction = error * 7;
			forward(400 - correction, 400 + correction);
			break;
		}

		forward(400,400);
		break;
		
		// 왼쪽으로 돌기
		case LEFT:
		forward(0,400);
		if(car.turn_timer == 0) {
			car.drive_state = FORWARD;
		}
		break;
		
		// 오른쪽으로 돌기
		case RIGHT:
		forward(400,0);
		if(car.turn_timer == 0) {
			car.drive_state = FORWARD;
		}
		break;
	}
}

void distance_check(void)
{
	
}

void drive_processing()
{
	// 1. 모드 전환 예외 처리 (주행 모드가 바뀌면 초기화)
	if (car.prev_mode != car.mode)
	{
		stop();
		bt_data = 0;
		car.state = DRIVE;
		car.turn_timer = 0;
		car.prev_mode = car.mode;
	}
	
	// 2. 현재 최상위 상태(car.state)에 따른 분리
	switch (car.state)
	{
		case DRIVE:
		// 주행 상태일 때만 내부 주행 모드(MANUAL / AUTO)를 실행한다.
		switch (car.mode)
		{
			case MANUAL_MODE:
			manual_mode();
			break;
			case AUTO_MODE:
			auto_mode();
			break;
		}
		break;
		
		case DONE:
		// 주행이 끝난 상태라면 주행 관련 함수는 절대 호출하지 않고,
		// 오직 정지 상태 유지 및 기록 출력(FND용 데이터 유지 등)만 수행한다.
		stop();
		break;
		
		default:
		break;
	}
}


void BT_print_distance(void)
{
	char buf[60];

	sprintf(buf, "L:%d F:%d R:%d\r\n",
	car.sensor[DIR_LEFT].distance,
	car.sensor[DIR_FRONT].distance,
	car.sensor[DIR_RIGHT].distance);

	for(int i = 0; buf[i] != '\0'; i++)
	{
		UART1_transmit(buf[i]);
	}
}