/*
 * uart0.h
 */ 

#ifndef UART0_H_
#define UART0_H_

#include "common.h"

void init_uart0(void);
void UART0_transmit(uint8_t data);

#endif