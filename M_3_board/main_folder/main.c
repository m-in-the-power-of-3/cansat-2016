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
  //VARIABLE
	state_mission_t state_mission_now = STATE_IN_FIRST_MEASURE;

	float height_now;

//============================================================================
//INIT
//============================================================================
	init_status ();
	init_low_hardware();
	init_hardware();
	init_sensors();

//============================================================================
//FIRST DATA
//============================================================================
	take_data_for_packet();
	take_data_for_packet_extra();

	packet_mission.pressure_at_start = count_average_pressure();

	update_packet();
	update_packet_extra();

	send_packet(&main_packet.control,sizeof(main_packet));
	send_packet(&packet_extra.control,sizeof(packet_extra));

	if (packet_mission.pressure_at_start == 0)
		change_state(STATE_FATAL_ERROR);
	else
		change_state(STATE_WAIT_SIGNAL);

	while (1){
		take_data_for_packet();
		switch(state_now) {
		//============================================================================
		//WAIT SIGNAL
		//============================================================================
		case STATE_WAIT_SIGNAL:
			if (!trigger())
				signal_wait_trigger();
			else
				change_state(STATE_WAIT_SEPARATION);
			break;
		//============================================================================
		//WAIT SEPARATION
		//============================================================================
		case STATE_WAIT_SEPARATION:
			if (separation_sensors_state())
				change_state(STATE_AFTER_SEPARATION);
			break;
		//============================================================================
		//AFTER SEPARATION
		//============================================================================
		case STATE_AFTER_SEPARATION:
			if (count_height(&packet_mission.height_separation,packet_mission.pressure_at_start) == RSCS_E_NONE){
				count_height_points();
				update_packet_mission();
				change_state(STATE_MAIN_PART);
			}
			else change_state(STATE_FATAL_ERROR);
			break;
		//============================================================================
		//MAIN PART
		//============================================================================
		case STATE_MAIN_PART:
		  //INTAKE
			if (count_height(&height_now,packet_mission.pressure_at_start) == RSCS_E_NONE){
				switch (state_mission_now) {
			  //FIRST INTAKE
				case STATE_IN_FIRST_MEASURE:
					if (height_now <= packet_mission.height_1){
						intake(1);
						state_mission_now = STATE_IN_SECOND_MEASURE;
						}
					break;
			  //SECOND INTAKE
				case STATE_IN_SECOND_MEASURE:
					if (height_now <= packet_mission.height_2){
						intake(2);
						state_mission_now = STATE_IN_THIRD_MEASURE;
					}
					else {
						if (main_packet.CO >= CO_INTAKE_VALUE){
							intake(2);
							STATUS_BECOME_ALL_RIGHT(STATUS_INTAKECO_1)
							state_mission_now = STATE_IN_THIRD_MEASURE;
						}
					}
			  //THIRD INTAKE
				break;
					break;
				case STATE_IN_THIRD_MEASURE:
					if (height_now <= packet_mission.height_3) {
						intake(3);
						state_mission_now = STATE_AFTER_THIRD_MEASURE;
					}
					else {
						if ((main_packet.CO >= CO_INTAKE_VALUE) && (!(STATUS_IS_ALL_RIGHT(STATUS_INTAKECO_1)))){
							intake(3);
							STATUS_BECOME_ALL_RIGHT(STATUS_INTAKECO_2)
							state_mission_now = STATE_AFTER_THIRD_MEASURE;
						}
					}
					break;
				case STATE_AFTER_THIRD_MEASURE:
					take_data_for_packet_extra();
					if (packet_extra.HC_SR04 != 0){
						update_packet_extra();
						send_packet(&packet_extra.control,sizeof(packet_extra));
					}
					else send_packet_mission ();
					break;
				};
			  //DEACTIVATION
				porsh_check(&porsh_1);
				porsh_check(&porsh_2);
				porsh_check(&porsh_3);
			}
			break;
		//============================================================================
		//FATAL ERROR
		//============================================================================
		case STATE_FATAL_ERROR:
			if (count_height(&packet_mission.height_separation,packet_mission.pressure_at_start) == RSCS_E_NONE){
				count_height_points();
				update_packet_mission();
				change_state(STATE_MAIN_PART);
			}
			else {
				take_data_for_packet_extra();
				update_packet_extra();
				send_packet(&packet_extra.control,sizeof(packet_extra));
			}
			break;
		};
		update_packet();
		send_packet(&main_packet.control,sizeof(main_packet));
	}
}
