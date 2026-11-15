/*
 * uart1.h
 */

#ifndef UART1_H_
#define UART1_H_

#include "common.h"

extern volatile uint8_t bt_data;	// 블루투스로 마지막에 받은 문자

void init_uart1(void);
void UART1_transmit(uint8_t data);

#endif /* UART1_H_ */
