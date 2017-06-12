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

int main (){
//============================================================================
//INIT
//============================================================================
	buffer_for_sd_init();
	init_low_hardware();
	init_hardware();
	init_sensors();
	DDRC |= (1 << 2);
	DDRC |= (1 << 1);

	PORTC |= (1 << 2);
	PORTC |= (1 << 1);

//============================================================================
//TEST
//============================================================================
	while(1){
		LED_MK_BLINK(100);
		take_data_for_packet();
		take_data_for_packet_extra();
		update_packet_extra();
		update_packet();
		send_packet(&main_packet.control,sizeof(main_packet));
		send_packet(&packet_extra.control,sizeof(packet_extra));
	}
}
