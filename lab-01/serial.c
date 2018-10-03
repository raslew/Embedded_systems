#define FOSC 16000000UL
#define BAUD 38400
#define MYUBRR FOSC/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>

#include "serial.h"

void uart_init(void){
//Setting baud rate
	unsigned int ubrr = MYUBRR;
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;

//Setting TX to enable.
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
//Setting frame format: 8N1 (1 stop bit is default)
	UCSR0C = (3<<UCSZ00);
}

void buffer(void){while ( !( UCSR0A & (1<<UDRE0)) );}

void uart_putchar(char chr){
//while arg is not true. Exit loop.
	buffer();
	if (chr == '\n'){
		UDR0 = '\r';
		buffer();
		UDR0 = '\n';
        }
	UDR0 = chr;
}

void uart_putstr(const char *str){
	//while *str is true run function and whne done increment the point with one
	while(*str){
	uart_putchar(*str++);
	}
}

char uart_getchar(void){
	while ( !(UCSR0A & (1<<RXC0)) );
	return UDR0;
}

void uart_echo(void){
	char rx_chr = uart_getchar();
        uart_putchar(rx_chr);
}

