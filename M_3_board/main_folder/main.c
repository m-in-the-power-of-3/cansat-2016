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
	twi_init();
	uart_init();
	uart_stdio_init();
	DDRG |= (1<<3);

	BMP180_init();
	while (1){
	PORTG |= (1<<3);
	_delay_ms (300);
	PORTG &= ~(1<<3);
	_delay_ms (300);
		printf("pressure = %li Pa\n",BMP180_count_pressure());
		printf("temperature = %f C\n", BMP180_count_temperature());
		_delay_ms (100);
	}
}
