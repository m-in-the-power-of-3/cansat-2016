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

#include <rscs/adc.h>
#include <rscs/adxl345.h>
#include <rscs/bmp280.h>
#include <rscs/ds18b20.h>
#include <rscs/error.h>
#include <rscs/gps_nmea.h>
#include <rscs/i2c.h>
#include <rscs/onewire.h>
#include <rscs/sdcard.h>
#include <rscs/spi.h>

#include <rscs/stdext/stdio.h>

#include <rscs/uart.h>

#include "BMP180.h"
#include "data.h"
#include "HC_SR04.h"
#include "init.h"
#include "mechanics.h"
#include "mq7.h"
#include "send_packets.h"

#include "hal/structs.h"
#include "hal/config.h"
#include "hal/time.h"



int main (){
//============================================================================
//BEFORE INIT
//============================================================================
  //CONST
	const time_data_t time_for_porsh = TIME_FOR_PORSH;

  //VARIABLE
	state_mission_t state_mission_now = STATE_IN_FIRST_MEASURE;
	state_t state_now = STATE_FATAL_ERROR;

	state_porsh_t porsh_1 = {{0,0},false,1};
	state_porsh_t porsh_2 = {{0,0},false,2};
	state_porsh_t porsh_3 = {{0,0},false,3};

	uint32_t pressure_at_start = 0;

	important_heights_t heights = {0,0,0,0};

//============================================================================
//INIT
//============================================================================
	init_low_hardware();
	init_hardware();
	init_sensors();
//============================================================================
//FIRST DATA
//============================================================================
	take_data_for_packet();
	take_data_for_packet_extra();
	pressure_at_start = count_average_pressure();
	if (pressure_at_start == 0)
		state_now = STATE_FATAL_ERROR;
	else
		state_now = STATE_WAIT_SIGNAL;

while (1){
	switch(state_now) {
	//============================================================================
	//WAIT SIGNAL
	//============================================================================
	case STATE_WAIT_SIGNAL:
		if (!trigger()){
			take_data_for_packet();
			//TODO: Добавить функцию для отправки пакета по uart.
			//TODO: Добавить функцию для отправки пакета на sd.
			//TODO: Чем-нибуть помигать. Показать, что мы ждем.
		}
		else state_now = STATE_WAIT_SEPARATION;
		break;
	//============================================================================
	//WAIT SEPARATION
	//============================================================================
	case STATE_WAIT_SEPARATION:
		if (!separation_sensors_state()){
			take_data_for_packet();
			//TODO: Добавить функцию для отправки пакета по uart.
			//TODO: Добавить функцию для отправки пакета на sd.
			//TODO: Чем-нибуть помигать. Показать, что мы ждем, но подругому.
		}
		else state_now = STATE_AFTER_SEPARATION;
	//============================================================================
	//AFTER SEPARATION
	//============================================================================
	case STATE_AFTER_SEPARATION:
		if (count_height(&heights.height_separation,pressure_at_start) == RSCS_E_NONE){
			count_height_points(&heights);
			state_now = STATE_MAIN_PART;
		}
		else state_now = STATE_FATAL_ERROR;
	//============================================================================
	//MAIN PART
	//============================================================================
	case STATE_MAIN_PART:
		//TODO: Собрать всю волю в кулак и доделать.
	//============================================================================
	//FTAL ERROR
	//============================================================================
	case STATE_FATAL_ERROR:
		take_data_for_packet();
		//TODO: Добавить функцию для отправки пакета по uart.
		//TODO: Добавить функцию для отправки пакета на sd.
		//TODO: Чем-нибуть помигать. В общем, показать, что все плохо.
		break;
	};
}

	/*


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
				porsh_1.time_krit = time_sum(time_service_get(),time_for_porsh);
				porsh_1.end = true;
				state_now = STATE_IN_SECOND_MEASURE;
				}
			break;
	  //SECOND PISTON
		case STATE_IN_SECOND_MEASURE:
			if (hight >= hight_2){
				motor_on (2);
				porsh_2.time_krit = time_sum(time_service_get(),time_for_porsh);
				porsh_2.end = true;
				state_now = STATE_IN_THIRD_MEASURE;
			}
	  //THIRD PISTON
		break;
			break;
		case STATE_IN_THIRD_MEASURE:
			if (hight >= hight_3) {
				motor_on (3);
				porsh_3.time_krit = time_sum(time_service_get(),time_for_porsh);
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
		//send_packet (uart_1,&main_packet,sizeof(main_packet));
	}*/
}
