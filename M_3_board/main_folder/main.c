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
	DDRG |= (1<<3);
	motor_init();
	sensor_init();
	time_service_init();
	uart_init();
	uart_stdio_init();

	time_data_t time_krit_value = {3,0};
	porsh_state_t motor_1 = {{0,0},0,1};
	porsh_state_t motor_2 = {{0,0},0,2};
	porsh_state_t motor_3 = {{0,0},0,3};

	while (1){
		_delay_ms(5000);
		motor_on(1);
		motor_1.end = true;
		motor_1.time_krit = time_sum(time_service_get(),time_krit_value);
		porsh_check(motor_1);
		_delay_ms(5000);
		motor_on(3);
		motor_2.end = true;
		motor_2.time_krit = time_sum(time_service_get(),time_krit_value);
		porsh_check(motor_2);
		_delay_ms(5000);
		motor_on(3);
		motor_3.end = true;
		motor_3.time_krit = time_sum(time_service_get(),time_krit_value);
		porsh_check(motor_3);
	}
}
