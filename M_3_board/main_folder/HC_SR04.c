/*
 * HC_SR04.c
 *
 *  Created on: 04 окт. 2016 г.
 *      Author: developer
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include <rscs/timeservice.h>

#include "hal/config.h"

void HC_SR04_init (){
	US_DDR &= ~(1 << US_pin_ECHO);
	US_PORT &= ~(1 << US_PIN_TRIG);
	US_DDR |= (1 << US_PIN_TRIG);
}

uint16_t HC_SR04_read (){
	uint32_t start_time;
	uint32_t stop_time;

	US_PORT |= (1 << US_PIN_TRIG);
	_delay_us (12);
	US_PORT &= ~(1 << US_PIN_TRIG);

	while ((US_PPIN & (1 << US_pin_ECHO)) == 0){}
	start_time = rscs_time_get();

	while ((US_PPIN & (1 << US_pin_ECHO)) != 0){}
	stop_time = rscs_time_get();

	float distance = (((stop_time - start_time) * 1000) * SONIC_SPEED)/2 ;
	distance *= 100;
	return (uint16_t)distance;
}
