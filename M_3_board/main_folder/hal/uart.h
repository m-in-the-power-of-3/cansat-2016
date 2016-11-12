/*
 * uart.h
 *
 *  Created on: 27 февр. 2016 г.
 *      Author: developer
 */
#ifndef UART_H_
#define UART_H_

void uart_init ();
void uart_write_byte (char value);
char uart_read_byte ();
inline void uart_write_string (char *p) {
	while (*p ) {
	uart_write_byte(*p);
	p++;
	}
}
void uart_stdio_init ();

#endif /* UART_H_ */
