/*
 * fnd.h
 */ 


#ifndef FND_H_
#define FND_H_

#include "common.h"

//디지트는 PC0~7
#define FND_DATA_DDR DDRC
#define FND_DATA_PORT PORTC

//fnd 디지트4개 중 1번은 PA 4,5,6,7
#define FND1_DIGIT_DDR DDRA
#define FND1_DIGIT_PORT PORTA

#define FND1_DIGIT_D1 4
#define FND1_DIGIT_D2 5
#define FND1_DIGIT_D3 6
#define FND1_DIGIT_D4 7

//fnd 디지트4개 2번은 Pf 0,1,2,3
#define FND2_DIGIT_DDR DDRB
#define FND2_DIGIT_PORT PORTB

#define FND2_DIGIT_D1 0
#define FND2_DIGIT_D2 1
#define FND2_DIGIT_D3 2
#define FND2_DIGIT_D4 3

#endif /* FND_H_ */