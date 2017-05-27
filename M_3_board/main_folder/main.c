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
#include <rscs/sdcard.h>

#include "BMP180.h"
#include "motor.h"
#include "packet.h"
#include "sensor.h"
#include "HC_SR04.h"
#include "hal/structs.h"
#include "hal/config.h"

#define SD_DDR DDRA
#define SD_PORT PORTA
#define SD_PIN (1 << 6)

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
	rscs_spi_set_pol(RSCS_SPI_POL_SAMPLE_RISE_SETUP_FALL);
	rscs_spi_set_clk(400);

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
  //SD
	_delay_ms (100);
	rscs_sdcard_t * sd = rscs_sd_init(&SD_DDR,&SD_PORT,SD_PIN);
	rscs_sd_set_timeout(sd,50000);
	rscs_sd_spi_setup_slow();
	rscs_e error1;
	rscs_e error;
	while(1){
		error1 = rscs_sd_startup(sd);
		if(error1 == RSCS_E_NONE){
			break;
		}
		else {
			printf("error1 == %i\n",error1);
		}
	}

	rscs_sd_spi_setup();
	uint8_t arr[512];
	for (int i = 0; i < 128;i++){
		arr[i*4] = 12;
		arr[(i*4)+1] = 61;
		arr[(i*4)+2] = 89;
		arr[(i*4)+3] = 74;
	}

	while(1){
		error =  rscs_sd_block_write(sd,2,&arr[0],1);
		if(error == RSCS_E_NONE){
			break;
		}
		else {
			printf("error == %i\n",error);
		}
	}
	while(1){
		printf("second:error = %i, error1 == %i\n",error, error1);
		_delay_ms (100);
	}
}
