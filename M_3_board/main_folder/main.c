/*
 * main.c
 *
 *  Created on: 27 февр. 2016 г.
 *      Author: developer
 */
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include <avr/interrupt.h>

#include <rscs/onewire.h>
#include <rscs/ds18b20.h>
#include <rscs/uart.h>
#include <rscs/i2c.h>
#include <rscs/stdext/stdio.h>
#include <rscs/spi.h>
#include <rscs/bmp280.h>
#include <rscs/adxl345.h>
#include <rscs/adc.h>
#include <rscs/sdcard.h>

#include "BMP180.h"
#include "mechanics.h"
#include "send_packets.h"
#include "HC_SR04.h"
#include "init.h"
#include "hal/structs.h"
#include "hal/config.h"
#include "data.h"
#include "hal/time.h"
#include "mq7.h"

#include <rscs/uart.h>
#include <rscs/stdext/stdio.h>

#define SD_DDR DDRA
#define SD_PORT PORTA
#define SD_PIN (1 << 6)

int main (){
//============================================================================
//INIT
//============================================================================
	buffer_for_sd_init();
	init_low_hardware();
	init_hardware();
	init_sensors();
	//printf("%u\n",main_packet.status);
//============================================================================
//VARIABLE
//============================================================================
	//SD
	rscs_sdcard_t * sd = rscs_sd_init(&SD_DDR,&SD_PORT,SD_PIN);
	rscs_sd_set_timeout(sd,4000);
	rscs_sd_spi_setup_slow();

	for (uint8_t i = 1;i <= INIT_TRY_SD;i++) {
		if (rscs_sd_startup(sd) == RSCS_E_NONE) {
			STATUS_BECOME_ALL_RIGHT(STATUS_SD)
			break;
		}
		else STATUS_BECOME_ERROR(STATUS_SD)
	}

	rscs_sd_spi_setup();
	rscs_e error;
	while(1){
		LED_BLINK(600);
		take_data_for_packet();
		take_data_for_packet_extra();
		//printf("test\n");
		update_packet_extra();
		update_packet();
		send_packet_uart (uart_1,&main_packet.control,sizeof(main_packet));
		error = send_packet_sd (sd,&main_packet.control,sizeof(main_packet));
		printf("%i",error);
	}
}
