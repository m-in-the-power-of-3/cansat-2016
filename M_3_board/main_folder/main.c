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
#include <rscs/uart.h>
#include <rscs/stdext/stdio.h>
#include <rscs/ds18b20.h>

#include "hal/adc.h"
#include "BMP180.h"
#include "motor.h"
#include "hal/one_wire.h"
#include "packet.h"
#include "sensor.h"
#include "hal/spi.h"
#include "DS18B20.h"
#include "hal/time.h"
#include "hal/twi.h"
#include "hal/uart.h"
#include "HC_SR04.h"
#include "hal/structs.h"

int main() {
//INIT
	//one_wire_init();
	rscs_ow_init_bus();
	rscs_ds18b20_t * ds18b20_1 = rscs_ds18b20_init(0);
	rscs_uart_bus_t * uart = rscs_uart_init(RSCS_UART_ID_UART1, RSCS_UART_FLAG_ENABLE_TX);
	rscs_uart_set_baudrate(uart, 9600);
	rscs_uart_set_character_size(uart, 8);
	rscs_uart_set_parity(uart, RSCS_UART_PARITY_NONE);
	rscs_uart_set_stop_bits(uart, RSCS_UART_STOP_BITS_ONE);
	FILE * f = rscs_make_uart_stream(uart);
	stdout = f;

	DDRG |= (1 << 3);
	int16_t temperature = 8;
	rscs_e e,e1 = 0;
	printf("test\n");
	float tem = 0;
	while(1){
		//printf("pinc = %d", PINC & 1);
		e1 = rscs_ds18b20_start_conversion(ds18b20_1);
		PORTG |= (1 << 3);
		_delay_ms (500);
		PORTG &= ~(1 << 3);
		_delay_ms (600);
		//temperature = DS18B20_read();
		e = rscs_ds18b20_read_temperature(ds18b20_1,&temperature);
		tem = temperature / 16.0f;
		printf("temp = %i, %f\n",temperature,tem);
		printf("test == %d %d\n",e,e1);


	}
}
