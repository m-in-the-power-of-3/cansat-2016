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
#include <rscs/spi.h>
#include <rscs/bmp280.h>
#include <rscs/timeservice.h>

#include "BMP180.h"
#include "motor.h"
#include "packet.h"
#include "sensor.h"
#include "HC_SR04.h"
#include "hal/structs.h"
#include "hal/config.h"

int main (){
//============================================================================
//INIT
//============================================================================
  //ONE WIRE
	rscs_ow_init_bus();

  //UART 1
	rscs_uart_bus_t * uart_1 = rscs_uart_init(RSCS_UART_ID_UART1, RSCS_UART_FLAG_ENABLE_TX);
	rscs_uart_set_baudrate(uart_1, 9600);
	rscs_uart_set_character_size(uart_1, 8);
	rscs_uart_set_parity(uart_1, RSCS_UART_PARITY_NONE);
	rscs_uart_set_stop_bits(uart_1, RSCS_UART_STOP_BITS_ONE);

  //UART 0
	rscs_uart_bus_t * uart_0 = rscs_uart_init(RSCS_UART_ID_UART0, RSCS_UART_FLAG_ENABLE_TX);
	rscs_uart_set_baudrate(uart_0, 9600);
	rscs_uart_set_character_size(uart_0, 8);
	rscs_uart_set_parity(uart_0, RSCS_UART_PARITY_NONE);
	rscs_uart_set_stop_bits(uart_0, RSCS_UART_STOP_BITS_ONE);

  //PRINTF
	FILE * f = rscs_make_uart_stream(uart_1);
	stdout = f;

  //TWI
	rscs_i2c_init();
	rscs_i2c_set_scl_rate(800);

  //SPI
	rscs_spi_init();
	rscs_spi_set_clk(RSCS_BMP280_SPI_FREQ_kHz);

  //TIME
	rscs_time_init();

  //OTHER
	DDRG |= (1<<3);
	motor_init();
	sensor_init();

  //DS18B20
	rscs_ds18b20_t * ds18b20_1 = rscs_ds18b20_init(0);

  //BMP180
	bmp180_init();

  //HC_SR04
	HC_SR04_init();

  //BMP280
	DDRC |= (1<<2);//delete
	PORTC |= (1<<2);//delete

	rscs_bmp280_descriptor_t * bmp280_descriptor = rscs_bmp280_init();
	rscs_bmp280_parameters_t bmp280_parametrs = {RSCS_BMP280_OVERSAMPLING_X16,RSCS_BMP280_OVERSAMPLING_X2,RSCS_BMP280_STANDBYTIME_500MS,RSCS_BMP280_FILTER_X16};
	rscs_bmp280_setup(bmp280_descriptor,&bmp280_parametrs);
	rscs_bmp280_changemode (bmp280_descriptor,RSCS_BMP280_MODE_NORMAL);

//============================================================================
//CONST
//============================================================================
	const rscs_bmp280_calibration_values_t * bmp280_calibration_values = rscs_bmp280_get_calibration_values (bmp280_descriptor);
//============================================================================
//VARIABLE
//============================================================================
	typedef enum {
		STATE_IN_FIRST_MEASURE,
		STATE_IN_SECOND_MEASURE,
		STATE_IN_THIRD_MEASURE,
		STATE_AFTER_THIRD_MEASURE
	} state;
	state state_now = STATE_IN_FIRST_MEASURE;

	packet_t main_packet = {0,0,0,0,0,0,0,0,0};

	porsh_state_t porsh_1 = {0,false,1};
	porsh_state_t porsh_2 = {0,false,2};
	porsh_state_t porsh_3 = {0,false,3};

	float hight = 0;

	uint32_t pressure_at_start = 0;//----------------------------- add function

//============================================================================
//CHECK
//============================================================================
	//-----------------------------------------------------------finish writing
//============================================================================
//BEFORE START
//============================================================================
	//pressure_at_start = //----------------------------- add function
//============================================================================
//BEFORE SEPARATION
//============================================================================
	//-----------------------------------------------------------finish writing
	while(1){}
//============================================================================
//AFTER SEPARATION
//============================================================================
	const float hight_at_separation = 2;//----------------------------add function
	const float hight_1 = (3 * hight_at_separation) / 4;
	const float hight_2 = hight_at_separation / 2;
	const float hight_3 = hight_at_separation / 4;

	while (1){
	//============================================================================
	//MAKE DATA
	//============================================================================
	//============================================================================
	//OTHER ACTIONS
	//============================================================================
	//============================================================================
	//PISTONS
	//============================================================================
		switch (state_now) {
	  //FIRST PISTON
		case STATE_IN_FIRST_MEASURE:
			if (hight >= hight_1){
				motor_on (1);
				porsh_1.time_krit = rscs_time_get() + TIME_FOR_PORSH;
				porsh_1.end = true;
				state_now = STATE_IN_SECOND_MEASURE;
				}
			break;
	  //SECOND PISTON
		case STATE_IN_SECOND_MEASURE:
			if (hight >= hight_2){
				motor_on (2);
				porsh_2.time_krit = rscs_time_get() + TIME_FOR_PORSH;
				porsh_2.end = true;
				state_now = STATE_IN_THIRD_MEASURE;
			}
	  //THIRD PISTON
		break;
			break;
		case STATE_IN_THIRD_MEASURE:
			if (hight >= hight_3) {
				motor_on (3);
				porsh_3.time_krit = rscs_time_get() + TIME_FOR_PORSH;
				porsh_3.end = true;
				state_now = STATE_AFTER_THIRD_MEASURE;
			}
			break;
		case STATE_AFTER_THIRD_MEASURE:
			break;
		};
	  //DEACTIVATION
		porsh_check(&porsh_1);
		porsh_check(&porsh_2);
		porsh_check(&porsh_3);
	//============================================================================
	//SEND DATA
	//============================================================================
	}
}
