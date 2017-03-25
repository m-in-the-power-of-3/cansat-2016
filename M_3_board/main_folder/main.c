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
#include "packet.h"
#include "sensor.h"
#include "hal/spi.h"
#include "hal/time.h"
#include "HC_SR04.h"
#include "hal/structs.h"
/*uint32_t count_average_pressure(packet_t * ptr){
	if(bmp_180_count_all(ptr->BMP180_pressure,ptr->BMP180_temperature) == 0){

	}
}*/
int main() {
//============================================================================
//INIT
//============================================================================
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

  //TIME
	time_service_init();

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
	int32_t raw_press  = 19;
	int32_t raw_temp = 19;
	packet_t main_packet = {0,0,0,0,0,0,0,0,0,0,0};
	rscs_ds18b20_start_conversion(ds18b20_1);
	while(1){
		if (rscs_ds18b20_check_ready()){
			rscs_ds18b20_read_temperature(ds18b20_1,&main_packet.DS18B20_temperature);
			rscs_ds18b20_start_conversion(ds18b20_1);
		}
		bmp_180_count_all(&main_packet.BMP180_pressure,&main_packet.BMP180_temperature);
		printf("bmp180 - t = %f C\n",main_packet.BMP180_temperature/10.0);
		printf("bmp180 - p = %lu\n",main_packet.BMP180_pressure);
		printf("ds18b20 -t = %f\n",main_packet.DS18B20_temperature/16.0);
}
