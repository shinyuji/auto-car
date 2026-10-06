/*
 * button.h
 */ 

#ifndef BUTTON_H_
#define BUTTON_H_

#include "common.h"

#define  BUTTON_DDR  DDRD
#define  BUTTON_PIN  PIND   // PORTD를 읽는 register 5v:1  0v:0 

#define  BUTTON0PIN 7		// PORTD.7
#define  BUTTON0   0		// PORTD.7의 가상 index (sw번호)

#define  BUTTON1PIN 6		// PORTD.6
#define  BUTTON1   1		// PORTD.6의 가상 index (sw번호)

#define  BUTTON_NUMBER  2   // 버튼 갯수 

#define  BUTTON_PRESS  1      // 버튼을 누르면 high (active-high)
#define  BUTTON_RELEASE 0     // 버튼을 뗀 상태 (low)

void init_button(void);
int get_button(int button_num, int button_pin);

#endif