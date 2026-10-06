/*
 * button.c
 */ 

#include "button.h"
extern uint32_t sec_count;
// 버튼 초기화
void init_button(void)
{
	BUTTON_DDR &= ~(1 << BUTTON0PIN);
	BUTTON_DDR &= ~(1 << BUTTON1PIN);
}

// 버튼 이벤트 체크
int get_button(int button_num, int button_pin)
{
	static unsigned char button_status[BUTTON_NUMBER] = { BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE };

	int current_state;
	
	// 1. 버튼 읽기
	current_state = BUTTON_PIN & (1 << button_pin);
	
	// 2. 버튼 상태 check 
	if (current_state  && button_status[button_num] == BUTTON_RELEASE)  // 버튼이 처음 눌려진 상태 
	{
		_delay_ms(15);  // noise가 지나가기를 기다린다. 
		button_status[button_num] = BUTTON_PRESS;  
		return 0;     // 아직은  완전히 눌렀다 뗀 상태가 아니다. 
	}
	else if (button_status[button_num] == BUTTON_PRESS && current_state == BUTTON_RELEASE)
	{  // 버튼이 이전에 눌려진 상태였으며 지금은 떼어진 상태 
		_delay_ms(15);  // noise가 지나가기를 기다린다. 
		button_status[button_num] = BUTTON_RELEASE;   // 다음 버튼을 체크 하기 위하여 초기화
		return 1;   // 완전히 1번 눌렀다 뗀 상태로 인정 한다. 
	}
	
	return 0;   // 버튼이 open 상태 이다. 
}

void button_event(){
	//버튼 0이 눌렸을 때
	if (get_button(BUTTON0,BUTTON0PIN))
	{
		//주행모드로 변경해주고, 주행관련기록 전부삭제.
		car.state = DRIVE;
		car.count_d = 0;
		car.count_b = 0;
		car.count_l = 0;
		car.count_r = 0;
		sec_count = 0;
		
		switch(car.mode){
			case MANUAL_MODE:					//매뉴얼 모드면
				car.mode = AUTO_MODE;
				break;
			case AUTO_MODE:						//자율주행 모드면
				car.mode = MANUAL_MODE;		//메뉴얼 모드로 변경
				break;
		}
	}
	
	//버튼 1이 눌렸을 때 (주행중이면, 종료로 진입)
	if (get_button(BUTTON1,BUTTON1PIN))
	{
		if(car.state == DRIVE){
			car.state = DONE;
			car.drive_state = STOP;
			car.done_state = RUNTIME_COUNT;
			stop();
		}
		//주행 종료면 버튼을.. 누를때마다 모드가 변경이 되어야 한다. 
		else if(car.state == DONE){
			switch(car.done_state){
				case RUNTIME_COUNT:
					car.done_state = BACKWARD_COUNT;
					break;
				case BACKWARD_COUNT:
					car.done_state = LEFT_COUNT;
					break;
				case LEFT_COUNT:
					car.done_state = RIGHT_COUNT;
					break;
				case RIGHT_COUNT:
					car.state = DRIVE;
					break;
			}
		}	
	}
}