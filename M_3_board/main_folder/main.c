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

//============================================================================
//TEST
//============================================================================
	while(1){
		take_data_for_packet();
		if (trigger())
			STATUS_BECOME_ALL_RIGHT(STATUS_BMP180)
		else STATUS_BECOME_ERROR(STATUS_BMP180)

		if (separation_sensor(1))
			STATUS_BECOME_ALL_RIGHT(STATUS_BMP280)
		else STATUS_BECOME_ERROR(STATUS_BMP280)

		if (separation_sensor(2))
			STATUS_BECOME_ALL_RIGHT(STATUS_ADXL345_INIT)
		else STATUS_BECOME_ERROR(STATUS_ADXL345_INIT)

		if (separation_sensor(3))
			STATUS_BECOME_ALL_RIGHT(STATUS_SD)
		else STATUS_BECOME_ERROR(STATUS_SD)

		signal_wait_trigger();
	}
}
