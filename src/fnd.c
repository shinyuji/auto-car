#include "fnd.h"

uint32_t ms_count = 0;
uint32_t sec_count = 0;

void init_fnd(void)
{
	FND_DATA_DDR = 0xFF;     // PC0~7 출력

	FND1_DIGIT_DDR |= 0xF0;   // PA4~7 출력
	FND2_DIGIT_DDR |= 0x0F;   // PB0~3 출력

	// 자릿수는 0일 때 OFF, 데이터는 1일 때 OFF
	FND1_DIGIT_PORT &= 0x0F;  // PA4~7 OFF (0으로 클리어)
	FND2_DIGIT_PORT &= 0xF0;  // PB0~3 OFF (0으로 클리어)
	
	FND_DATA_PORT = 0xFF;     // 데이터 초기화 (1을 주어 LED 모두 끔)
}

void fnd_display(){
	static int digit_select = 0;    // 자릿수 선택 (0~7)
	int index = 0;
	uint8_t *data = NULL;
	uint8_t digit;
	
	FND_DATA_PORT = 0xFF;
	FND1_DIGIT_PORT &= 0x0F;
	FND2_DIGIT_PORT &= 0xF0;

	if(digit_select < 4){
		set_fnd1_data_port(digit_select, &index, &data);
		set_fnd1_digit_port(digit_select, &digit);
		
		if(data != NULL) {
			FND_DATA_PORT = data[index];
			FND1_DIGIT_PORT = (FND1_DIGIT_PORT & 0x0F) | digit;
		}
		} else {
		set_fnd2_data_port(digit_select-4, &index, &data);
		set_fnd2_digit_port(digit_select-4, &digit);
		
		if(data != NULL) {
			FND_DATA_PORT = data[index];
			FND2_DIGIT_PORT = (FND2_DIGIT_PORT & 0xF0) | digit;
		}
	}
	
	digit_select = (digit_select + 1) % 8;
}

void set_fnd2_data_port(int digit_select, int *index, uint8_t **data)
{
	// 테스트용 원하는 숫자 고정 (나중에 주행 연산값 쓸 때는 car.count_d 등으로 변경)
	int number = 4321;
	if (car.state == DONE) {
		switch(car.done_state) {
			case RUNTIME_COUNT:
			number = car.count_d;       // 전체 주행 시간 (초)
			break;
			case BACKWARD_COUNT:
			number = car.count_b;     // 후진 횟수
			break;
			case LEFT_COUNT:
			number = car.count_l;     // 좌회전 횟수
			break;
			case RIGHT_COUNT:
			number = car.count_r;     // 우회전 횟수
			break;
			default:
			number = 0;
			break;
		}
	}
	
	static uint8_t fnd_font_num[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f};
	static uint8_t fnd_font_ani[] = {0xFE, 0xBF, 0xF7};
	
	// 자릿수 순서를 digit_select(0, 1, 2, 3)에 일치하도록 배열 배치
	int digit_num[4] = {
		number / 1000,          // [3] : 1000의 자리
		(number / 100) % 10,   // [2] : 100의 자리
		(number / 10) % 10,    // [1] : 10의 자리
		number % 10           // [0] : 1의 자리
	};
	
	if (car.state == DRIVE){
		*data = fnd_font_ani;
		switch(car.drive_state){
			case FORWARD:  *index = sec_count % 3; break;
			case BACKWARD: *index = (2 - (sec_count % 3)); break;
			case LEFT:     *index = sec_count % 3; break;
			case RIGHT:    *index = sec_count % 3; break;
			case STOP:     *index = 1; break;
		}
	}
	else if(car.state == DONE){
		*data = fnd_font_num;
		
		*index = digit_num[digit_select];
	}
}

void set_fnd1_data_port(int digit_select, int *index, uint8_t **data)
{
	static uint8_t fnd_font_dir[] = {0xfe, 0xCF, 0xF9, 0xf7, 0xBF};
	static uint8_t fnd_font_num[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f};
	static uint8_t fnd_font_eng[] = {0x8E, 0x83, 0xC7, 0xAF};
	
	if (car.state == DRIVE){
		*data = fnd_font_dir;
		switch(car.drive_state){
			case FORWARD:   *index = 0; break;
			case BACKWARD:  *index = 3; break;
			case LEFT:      *index = 1; break;
			case RIGHT:     *index = 2; break;
			case STOP:      *index = 4; break;
		}
	}
	else if(car.state == DONE){
		*data = fnd_font_eng;
		switch(car.done_state){
			case RUNTIME_COUNT:   *index = 0; break;
			case BACKWARD_COUNT:  *index = 1; break;
			case LEFT_COUNT:      *index = 2; break;
			case RIGHT_COUNT:     *index = 3; break;
		}
	}
}

void set_fnd1_digit_port(int digit_select, uint8_t *digit){
	switch(digit_select)
	{
		case 0:  *digit = 0x10; break; // PA4
		case 1:  *digit = 0x20; break; // PA5
		case 2:  *digit = 0x40; break; // PA6
		case 3:  *digit = 0x80; break; // PA7
		default: *digit = 0x00; break;
	}
}

void set_fnd2_digit_port(int digit_select, uint8_t *digit){
	switch(digit_select)
	{
		case 0:  *digit = 0x01; break; // PG0 (1의 자리)
		case 1:  *digit = 0x02; break; // PG1 (10의 자리)
		case 2:  *digit = 0x04; break; // PG2 (100의 자리)
		case 3:  *digit = 0x08; break; // PG3 (1000의 자리)
		default: *digit = 0x00; break;
	}
}