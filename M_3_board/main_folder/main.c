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
	//ONE WIRE
	rscs_ow_init_bus();
	rscs_ds18b20_t * ds18b20_1 = rscs_ds18b20_init(0);
	//UART 1
	rscs_uart_bus_t * uart = rscs_uart_init(RSCS_UART_ID_UART1, RSCS_UART_FLAG_ENABLE_TX);
	rscs_uart_set_baudrate(uart, 9600);
	rscs_uart_set_character_size(uart, 8);
	rscs_uart_set_parity(uart, RSCS_UART_PARITY_NONE);
	rscs_uart_set_stop_bits(uart, RSCS_UART_STOP_BITS_ONE);
	FILE * f = rscs_make_uart_stream(uart);
	stdout = f;
	//TWI
	rscs_i2c_init();
	rscs_i2c_set_scl_rate(800);
	//SPI
	rscs_spi_init();
	rscs_spi_set_clk(RSCS_BMP280_SPI_FREQ_kHz);
	//BMP280
	DDRC |= (1<<2);
	PORTC |= (1<<2);
	rscs_bmp280_descriptor_t * bmp280_descriptor = rscs_bmp280_init();
	rscs_bmp280_parameters_t bmp280_parametrs = {RSCS_BMP280_OVERSAMPLING_X16,RSCS_BMP280_OVERSAMPLING_X2,RSCS_BMP280_STANDBYTIME_500MS,RSCS_BMP280_FILTER_X16};
	rscs_bmp280_setup(bmp280_descriptor,&bmp280_parametrs);
	const rscs_bmp280_calibration_values_t * bmp280_calibration_values = rscs_bmp280_get_calibration_values (bmp280_descriptor);
	rscs_bmp280_changemode (bmp280_descriptor,RSCS_BMP280_MODE_NORMAL);
	DDRG |= (1<<3);
	int32_t raw_press  = 19;
	int32_t raw_temp = 19;
	int32_t pressure;
	int32_t temperature;
	while(1){
		rscs_bmp280_read(bmp280_descriptor,&raw_press,&raw_temp);
		rscs_bmp280_calculate(bmp280_calibration_values,raw_press,raw_temp,&pressure,&temperature);
		printf("temperature = %li\n",temperature);
		printf("pressure = %li\n",pressure);
	}
}
