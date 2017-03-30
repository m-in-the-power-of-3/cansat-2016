/*
 * main.c
 *
 *  Created on: 27 февр. 2016 г.
 *      Author: developer
 */
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <math.h>

#include <rscs/onewire.h>
#include <rscs/ds18b20.h>
#include <rscs/uart.h>
#include <rscs/i2c.h>
#include <rscs/stdext/stdio.h>

#include "BMP180.h"
#include "motor.h"
#include "packet.h"
#include "sensor.h"
#include "HC_SR04.h"
#include "hal/structs.h"

int main() {
//INIT
	//ONE WIRE
	rscs_ow_init_bus();
	rscs_ds18b20_t * ds18b20_1 = rscs_ds18b20_init(0);
	//UART 1
	rscs_uart_bus_t * uart = rscs_uart_init(RSCS_UART_ID_UART1, RSCS_UART_FLAG_ENABLE_TX);
	rscs_uart_set_baudrate(uart, 9600);
	rscs_uart_set_character_size(uart, 8);
	rscs_uart_set_parity(uart, RSCS_UART_PARITY_NONE);
	rscs_uart_set_stop_bits(uart, RSCS_UART_STOP_BITS_ONE);
	FILE * f = rscs_make_uart_stream(uart);
	stdout = f;
	//TWI
	rscs_i2c_init();
	rscs_i2c_set_scl_rate(400);

	int16_t temp;
	uint32_t press;

	DDRG |= (1<<3);
	bmp180_init();
	//bmp180_test();

	while (1){
	//printf("test");
	PORTG |= (1<<3);
	_delay_ms (300);
	PORTG &= ~(1<<3);
	_delay_ms (300);
	bmp180_count_pressure(&press);
	bmp180_count_temperature(&temp);
	printf("pressure = %lu Pa\n",press);
	printf("temperature = %i C\n",temp);
	}
}


