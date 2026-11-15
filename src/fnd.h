/*
 * fnd.h
 */

#ifndef FND_H_
#define FND_H_

#include "common.h"

//세그먼트 데이터는 PC0~7
#define FND_DATA_DDR DDRC
#define FND_DATA_PORT PORTC

//FND 1번(방향·상태 표시) 자리 선택: PA4~7
#define FND1_DIGIT_DDR DDRA
#define FND1_DIGIT_PORT PORTA

#define FND1_DIGIT_D1 4
#define FND1_DIGIT_D2 5
#define FND1_DIGIT_D3 6
#define FND1_DIGIT_D4 7

//FND 2번(숫자 표시) 자리 선택: PB0~3
#define FND2_DIGIT_DDR DDRB
#define FND2_DIGIT_PORT PORTB

#define FND2_DIGIT_D1 0
#define FND2_DIGIT_D2 1
#define FND2_DIGIT_D3 2
#define FND2_DIGIT_D4 3

void init_fnd(void);
void fnd_display(void);

#endif /* FND_H_ */
