/*
 * uart0.c
 */

#include "uart0.h"

void init_uart0(void)
{
	//전송속도 9600bps
	UBRR0H = 0x00;
	UBRR0L = 207;

	//2배속
	UCSR0A |= 1 << U2X0;

	//송신만 사용
	UCSR0B |= 1 << TXEN0;
}

// UART0 로 1byte를 전송
void UART0_transmit(uint8_t data)
{
	while ( !(UCSR0A & 1 << UDRE0))   // data가 송신중이면 송신이 끝날때 까지 기다림
		;     // No operation
	UDR0 = data;   // HW 전송 register에 data를 송신 한다.
}
