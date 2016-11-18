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
//INIT
	DDRG |= (1<<3);
	//adc_init();
	//BMP180_init();
	motor_init();
	//one_wire_init();
	sensor_init();
	//spi_init();
	time_service_init();
	//twi_init();
	uart_init();
	uart_stdio_init();
	HC_SR04_init();
//CONST
	const time_data_t time_porsh = {2,0};//<<========================== add value
//VARIABLE
	typedef enum {
		STATE_IN_FIRST_MEASURE,
		STATE_IN_SECOND_MEASURE,
		STATE_IN_THIRD_MEASURE,
		STATE_AFTER_THIRD_MEASURE
	} state;
	state state_now = STATE_IN_FIRST_MEASURE;
	packet_t main_packet = {0,0,0,0,0,0,0,0,0,0};
	porsh_state_t porsh_1 = {{0,0},false,1};
	porsh_state_t porsh_2 = {{0,0},false,2};
	porsh_state_t porsh_3 = {{0,0},false,3};
	float hight = 0;
	uint32_t pressure_at_start;
//CHECK
	/*описание ошибок (дописать)
	 * 0x00 - all right*/
	/*uint8_t check_state;
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
	} else PORTG |= (1<<3);*/
//BEFORE START
	//pressure_at_start = //----------------------------- add function
//BEFORE SEPARATION
	while(1){}
//AFTER SEPARATION
	//const float hight_at_separation = 2;
	const float hight_1 = 97000;//(3 * hight_at_separation) / 4;
	const float hight_2 = 98000;//hight_at_separation / 2;
	const float hight_3 = 99000;//hight_at_separation / 4;

	while (1){
//MAKE DATA
		HC_SR04_trip_check();
		adc_read(&main_packet.MPX5100_pressure);

		printf("%f\n",hight);
//OTHER ACTIONS
//PISTONS
		switch (state_now) {
		//FIRST PISTON
		case STATE_IN_FIRST_MEASURE:
			if (hight >= hight_1){
				motor_on (1);
				printf("motor 1 on \n");//-------------------------------------------
				porsh_1.time_krit = time_sum(time_service_get(), time_porsh);
				porsh_1.end = true;
				state_now = STATE_IN_SECOND_MEASURE;
				}
			break;
		//SECOND PISTON
		case STATE_IN_SECOND_MEASURE:
			if (hight >= hight_2){
				motor_on (2);
				printf("motor 2 on \n");//-------------------------------------------
				porsh_2.time_krit = time_sum(time_service_get(), time_porsh);
				porsh_2.end = true;
				state_now = STATE_IN_THIRD_MEASURE;
			}
	//THIRD PISTON
		break;
			break;
		case STATE_IN_THIRD_MEASURE:
			if (hight >= hight_3) {
				motor_on (3);
				printf("motor 3 on \n");//-------------------------------------------
				porsh_3.time_krit = time_sum(time_service_get(), time_porsh);
				porsh_3.end = true;
				state_now = STATE_AFTER_THIRD_MEASURE;
			}
			break;
		case STATE_AFTER_THIRD_MEASURE:
			break;
		};
		porsh_check(&porsh_1);
		porsh_check(&porsh_2);
		porsh_check(&porsh_3);
//SEND DATA
	}
}
