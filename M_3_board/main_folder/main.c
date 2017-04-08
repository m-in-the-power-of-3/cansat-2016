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
#include <rscs/adc.h>
#include "hal/adc.h"
#include "BMP180.h"
#include "motor.h"
#include "hal/one_wire.h"
#include "packet.h"

#include "hal/spi.h"
#include "DS18B20.h"
#include "hal/time.h"
#include "hal/twi.h"
#include "rscs/uart.h"
#include "HC_SR04.h"
#include "hal/structs.h"
#include "mq7.h"

int main() {
//INIT
	//UART 1
	rscs_uart_bus_t * uart = rscs_uart_init(RSCS_UART_ID_UART1, RSCS_UART_FLAG_ENABLE_TX);
	rscs_uart_set_baudrate(uart, 9600);
	rscs_uart_set_character_size(uart, 8);
	rscs_uart_set_parity(uart, RSCS_UART_PARITY_NONE);
	rscs_uart_set_stop_bits(uart, RSCS_UART_STOP_BITS_ONE);
	FILE * f = rscs_make_uart_stream(uart);
	stdout = f;
	printf("hello /n");
	rscs_adc_init();
	rscs_adc_set_refrence(RSCS_ADC_REF_EXTERNAL_VCC);
	rscs_adc_set_prescaler(RSCS_ADC_PRESCALER_64);

	float RO = calibrate();

	while(1){
		if(mq7_digital_read()) printf("YES\n");
		else printf("NO\n");
		// printf( "%f \n " ,readCO());
		printf( "%f \n " , readCO(RO));
		_delay_ms(1000);
	}

	return 0;
}
