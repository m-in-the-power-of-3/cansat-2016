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

#include <rscs/error.h>
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


#define CHECK_PRESSURE_MIN 75000
#define CHECK_PRESSURE_MAX 120000
#define CHECK_TEMPERATURE_MIN -25
#define CHECK_TEMPERATURE_MAX 45
#define ERROR_BMP180 (1 << 8)
#define ERROR_DS18B20 (1 << 7)
#define ALL_RIGHT  0x00

#define CHECK_PRESSURE(PRESSURE,ERROR) \
	if((PRESSURE > CHECK_PRESSURE_MAX) || (PRESSURE < CHECK_PRESSURE_MIN))\
		ERROR = false;\
	  else\
		ERROR = true;
#define CHECK_TEMPERATURE(TEMPERATURE,ERROR) \
	if((TEMPERATURE > CHECK_TEMPERATURE_MAX) || (TEMPERATURE < CHECK_TEMPERATURE_MIN))\
		ERROR = false;\
	  else\
		ERROR = true;

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

  //TIME
	rscs_time_init();

  //OTHER
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
	packet_t main_packet = {0,0,0,0,0,0,0,0,0,0};
	uint8_t check_state = 0;
//============================================================================
//CHECK
//============================================================================
	{
		rscs_e error_function;
		bool error_value;

	  //DS180B20
		if(rscs_ds18b20_start_conversion(ds18b20_1) == 0){
			_delay_ms (1100);

			error_function = rscs_ds18b20_read_temperature(ds18b20_1,&main_packet.DS18B20_temperature) != 0;
			CHECK_TEMPERATURE(main_packet.DS18B20_temperature,error_value)

			if((error_function != 0) || !error_value)
				check_state |= ERROR_DS18B20;
		}
		else
			check_state |= ERROR_DS18B20;

	  //BMP180
		error_function = bmp180_count_all(&main_packet.BMP180_pressure,&main_packet.BMP180_temperature);
		CHECK_PRESSURE(main_packet.BMP180_pressure,error_value)

		if((error_function != 0) || !error_value)
			check_state |= ERROR_BMP180;

	}
//============================================================================
//BEFORE SEPARATION
//============================================================================
	while(1){}
}
