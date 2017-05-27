/*
 * HC_SR04.c
 *
 *  Created on: 04 окт. 2016 г.
 *      Author: developer
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "hal/time.h"
#include "hal/config.h"
#include "hal/structs.h"

void HC_SR04_init (){
	US_DDR &= ~(1 << US_pin_ECHO);
	US_PORT &= ~(1 << US_PIN_TRIG);
	US_DDR |= (1 << US_PIN_TRIG);
}

rscs_e HC_SR04_read (uint16_t * value){
	time_data_t start_time;
	time_data_t stop_time;
	rscs_e error = RSCS_E_NONE;
	uint16_t timeout = 0;
	US_PORT |= (1 << US_PIN_TRIG);
	_delay_us (12);
	US_PORT &= ~(1 << US_PIN_TRIG);

	while ((US_PPIN & (1 << US_pin_ECHO)) == 0){
		if (timeout >= HC_SR04_TIME_OUT){
			error = RSCS_E_TIMEOUT;
			goto end;
		}
		timeout++;
		_delay_us(10);
	}

	start_time = time_service_get();

	while ((US_PPIN & (1 << US_pin_ECHO)) != 0){
		if (timeout >= HC_SR04_TIME_OUT){
			error = RSCS_E_TIMEOUT;
			goto end;
		}
		timeout++;
		_delay_us(10);
	}
	stop_time = time_service_get();

	float start_time_float = start_time.seconds + ((float)start_time.subseconds / MAX_SUBSECONDS);
	float stop_time_float = stop_time.seconds + ((float)stop_time.subseconds / MAX_SUBSECONDS);
	float distance = ((stop_time_float - start_time_float) * SONIC_SPEED)/2 ;
	distance *= 100;
	*(value) = (uint16_t)distance;
	end:
	return error;
}
