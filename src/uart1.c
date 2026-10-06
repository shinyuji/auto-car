/*
 * uart1.c
 */ 

#include "uart1.h"
#include "uart0.h"

volatile uint8_t bt_data;

ISR(USART1_RX_vect)
{
	bt_data = UDR1;
}

void init_uart1(void)
{
	//전송속도 9600bps
	UBRR1H = 0x00;
	UBRR1L = 207;
	
	//2배속
	UCSR1A |= 1 << U2X1;
	
	//송신, 수신, RX INT 가능
	UCSR1B |= 1 << RXEN1 | 1 << TXEN1 | 1 << RXCIE1;
}

// UART1 로 1byte를 전송 하는 함수
void UART1_transmit(uint8_t data)
{
	while ( !(UCSR1A & 1 << UDRE1))   //송신 버퍼가 비워질 때 까지 기다리기
		;
	UDR1 = data;   // HW 전송 register에 data를 송신 한다.
}

