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
	/*
//INIT
	//ONE WIRE
	rscs_ow_init_bus();
	rscs_ds18b20_t * ds18b20_1 = rscs_ds18b20_init(0);
	*/
	//UART 1
	rscs_uart_bus_t * uart = rscs_uart_init(RSCS_UART_ID_UART1, RSCS_UART_FLAG_ENABLE_TX);
	rscs_uart_set_baudrate(uart, 9600);
	rscs_uart_set_character_size(uart, 8);
	rscs_uart_set_parity(uart, RSCS_UART_PARITY_NONE);
	rscs_uart_set_stop_bits(uart, RSCS_UART_STOP_BITS_ONE);
	FILE * f = rscs_make_uart_stream(uart);
	stdout = f;
	printf("hello /n");
	/*
	//TWI
	rscs_i2c_init();
	rscs_i2c_set_scl_rate(800);

	DDRG |= (1<<3);
	adc_init();
	BMP180_init();
	motor_init();
	sensor_init();
	spi_init();
	time_service_init();
	twi_init();
	uart_init();
	uart_stdio_init();
	HC_SR04_init();
//CONST
	const time_data_t time_porsh = {2,0};
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
	//-----------------------------------------------------------finish writing
//BEFORE START
	//pressure_at_start = //----------------------------- add function
//BEFORE SEPARATION
	//-----------------------------------------------------------finish writing
	while(1){}
//AFTER SEPARATION
	const float hight_at_separation = 2;//----------------------------add function
	const float hight_1 = (3 * hight_at_separation) / 4;
	const float hight_2 = hight_at_separation / 2;
	const float hight_3 = hight_at_separation / 4;
	while (1){
//MAKE DATA
		//-----------------------------------------finish writing
		HC_SR04_trip_check();
		adc_read(&main_packet.MPX5100_pressure);
//OTHER ACTIONS
//PISTONS
		switch (state_now) {
		//FIRST PISTON
		case STATE_IN_FIRST_MEASURE:
			if (hight >= hight_1){
				motor_on (1);
				porsh_1.time_krit = time_sum(time_service_get(), time_porsh);
				porsh_1.end = true;
				state_now = STATE_IN_SECOND_MEASURE;
				}
			break;
		//SECOND PISTON
		case STATE_IN_SECOND_MEASURE:
			if (hight >= hight_2){
				motor_on (2);
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
	*/
	rscs_adc_init();
	rscs_adc_set_refrence(RSCS_ADC_REF_EXTERNAL_VCC);
	rscs_adc_set_prescaler(RSCS_ADC_PRESCALER_64);

	double RO = calibrate();

	while(1){
		if(mq7_digital_read()) printf("YES\n");
		else printf("NO\n");
		// printf( "%f \n " ,readCO());
		printf( "%f \n " , readCO(RO));
		_delay_ms(700);
	}

	return 0;
}
