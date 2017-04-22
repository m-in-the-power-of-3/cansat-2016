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
	rscs_spi_set_clk(100);

  //TIME

	packet_t packet = {0xff,1,24,99000,25,100000,25,4,1,2};

	send_packet(uart_1,&packet,sizeof (packet));
	while(1){
		//printf("%u\n",0xFF);
	}
}
