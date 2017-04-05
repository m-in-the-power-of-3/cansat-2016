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

#define US_PORT PORTC
#define US_PPIN PINC
#define US_DDR DDRC
#define US_PIN_TRIG 1
#define US_pin_ECHO 2

#define MAX_SUBSECONDS 31250
#define SONIC_SPEED 340//======================сделать функцию
#define DISTANCE_FOR_CHECK 1

/*//tests
#include "motor.h"
#include "stdio.h"

#define TEST_DISTANCE_FOR_CHECK_1 1.2
#define TEST_DISTANCE_FOR_CHECK_2 0.8
#define TEST_DISTANCE_FOR_CHECK_3 0.4
//----------*/


/*float count_sonic_speed (){
	return sonic_speed;
}*/


void HC_SR04_init (){
	US_DDR &= ~(1 << US_pin_ECHO);
	US_PORT &= ~(1 << US_PIN_TRIG);
	US_DDR |= (1 << US_PIN_TRIG);
}

uint16_t HC_SR04_read (){
	time_data_t start_time;
	time_data_t stop_time;
	US_PORT |= (1 << US_PIN_TRIG);
	_delay_us (12);
	US_PORT &= ~(1 << US_PIN_TRIG);
	while ((US_PPIN & (1 << US_pin_ECHO)) == 0){}
	start_time = time_service_get();
	while ((US_PPIN & (1 << US_pin_ECHO)) != 0){}
	stop_time = time_service_get();
	float start_time_float = start_time.seconds + ((float)start_time.subseconds / MAX_SUBSECONDS);
	float stop_time_float = stop_time.seconds + ((float)stop_time.subseconds / MAX_SUBSECONDS);
	float distance = ((stop_time_float - start_time_float) * SONIC_SPEED)/2 ;
	distance *= 100;
	return distance;
}

void HC_SR04_trip_check (){
	uint16_t distance[3];
	distance[0] = HC_SR04_read();
	_delay_ms (5);
	distance[1] = HC_SR04_read();
	_delay_ms (5);
	distance[2] = HC_SR04_read();
	if ((distance[0] >= distance[1]) && (distance[1] >= distance[2]) && (distance[0] <= DISTANCE_FOR_CHECK)){
		//-----------------------------------------------------------дописать
	}

}
