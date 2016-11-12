/*
 * uart.c
 *
 *  Created on: 27 февр. 2016 г.
 *      Author: developer
 */
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void uart_init (){
	UBRR0L = 51;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
}

void uart_write_byte (char value){
	while ((UCSR0A & (1<<UDRE0)) == 0){}
	UDR0 = value;
}

char uart_read_byte (){
	while ((UCSR0A & (1<<RXC0)) == 0){}
	return UDR0;
}
int my_put_char(char value, FILE * stream){
	while ((UCSR1A & (1<<UDRE1)) == 0){}
	UDR1 = value;
	return 0;
}

FILE mystdout = FDEV_SETUP_STREAM(my_put_char,NULL,_FDEV_SETUP_WRITE);
void uart_stdio_init (){
	UBRR1L = 51;
	UCSR1B = (1<<TXEN1);
	stdout = &mystdout;
}
