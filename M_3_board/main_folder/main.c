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

#define CHECK_PRESSURE_MIN 0
#define CHECK_PRESSURE_MAX 0
#define CHECK_TEMPERATURE_MIN 0
#define CHECK_TEMPERATURE_MAX 0
#define ERROR_BMP180_PRESSURE 0x01
#define ERROR_BMP180_TEMPERATURE 0x02
#define ALL_RIGHT  0x00

int main() {
//INIT
	DDRG |= (1<<3);
	adc_init();
	BMP180_init();
	motor_init();
	one_wire_init();
	sensor_init();
	spi_init();
	time_service_init();
	twi_init();
	uart_init();
	uart_stdio_init();
	HC_SR04_init();
//VARIABLE
	packet_t main_packet = {0,0,0,0,0,0,0,0,0,0};
//CHECK
	uint8_t check_state;
	main_packet.BMP180_temperature = BMP180_count_temperature();
	if((main_packet.BMP180_temperature < CHECK_TEMPERATURE_MIN) && (main_packet.BMP180_temperature > CHECK_TEMPERATURE_MAX)){
		check_state = ERROR_BMP180_TEMPERATURE;
	}
	main_packet.BMP180_pressure = BMP180_count_pressure();
	if((main_packet.BMP180_pressure < CHECK_PRESSURE_MIN) && (main_packet.BMP180_pressure > CHECK_PRESSURE_MAX)){
		check_state = ERROR_BMP180_PRESSURE;
	}
	if(check_state != ALL_RIGHT){
		PORTG |= (1<<3);
		_delay_ms(250);
		PORTG &= ~(1<<3);
		_delay_ms(250);
		PORTG |= (1<<3);
		_delay_ms(500);
		PORTG &= ~(1<<3);
		_delay_ms(250);
		PORTG |= (1<<3);
		_delay_ms(250);
		PORTG &= ~(1<<3);
	} else PORTG |= (1<<3);
//BEFORE SEPARATION
	while(1){}
}
