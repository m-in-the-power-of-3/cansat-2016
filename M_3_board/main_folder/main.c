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
#include <rscs/adxl345.h>

#include "BMP180.h"
#include "motor.h"
#include "packet.h"
#include "sensor.h"
#include "HC_SR04.h"
#include "hal/structs.h"
#include "hal/config.h"
#include "hal/time.h"

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
	rscs_i2c_set_scl_rate(100);

  //TIME
	time_service_init();

  //OTHER
	DDRG |= (1<<3);
	motor_init();
	sensor_init();

  //ADXL345
	rscs_adxl345_t * adxl345 = rscs_adxl345_initi2c (RSCS_ADXL345_ADDR_ALT);
	rscs_adxl345_set_range(adxl345,RSCS_ADXL345_RANGE_2G);
	rscs_adxl345_set_rate(adxl345,RSCS_ADXL345_RATE_200HZ);
	int16_t x = 0;
	int16_t y = 0;
	int16_t z = 0;
	float x_g = 0;
	float y_g = 0;
	float z_g = 0;

	rscs_e error = 123;
	while (1){
		error = rscs_adxl345_read (adxl345,&x,&y,&z);
		printf("error_r = %i\n",error);
		rscs_adxl345_cast_to_G (adxl345,x,y,z,&x_g,&y_g,&z_g);
		printf("===========================================\n");
		printf("x = %i\n",x);
		printf("y = %i\n",y);
		printf("z = %i\n",z);
		printf("--------------------------------------------\n");
		printf("x_g = %f\n",x_g);
		printf("y_g = %f\n",y_g);
		printf("z_g = %f\n",z_g);
		printf("===========================================\n");
	}
}
