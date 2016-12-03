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

#define CHECK_PRESSURE_MIN 0 //<<========================== add value
#define CHECK_PRESSURE_MAX 0 //<<========================== add value
#define CHECK_TEMPERATURE_MIN 0 //<<========================== add value
#define CHECK_TEMPERATURE_MAX 0 //<<========================== add value
#define ERROR_BMP180_PRESSURE 0x01
#define ERROR_BMP180_TEMPERATURE 0x02
#define ALL_RIGHT  0x00

int main() {
	twi_init();
	uart_init();
	uart_stdio_init();
	BMP180_init();
	int32_t pressure_at_start;
	int32_t average_pressure;
	pressure_at_start = BMP180_count_pressure();
	while(1){
		average_pressure = BMP180_count_pressure();
		float X = average_pressure /pressure_at_start;
		float height = 44330 * (1.0 - pow(X,0.1903));
		printf("%f\n",height);
	}
}
