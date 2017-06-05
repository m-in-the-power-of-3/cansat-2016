/*
 * mechanics.c
 *
 *  Created on: 05 июня 2017 г.
 *      Author: developer
 */
#include <avr/io.h>
#include <stdbool.h>

#include "hal/config.h"
#include "hal/structs.h"
#include "hal/time.h"

//============================================================================
//MOTOR
//============================================================================
void motor_off (uint8_t motor_number);

void motor_init() {
	MOTOR_DDR |= (1 << MOTOR_1_PIN);
	MOTOR_DDR |= (1 << MOTOR_2_PIN);
	MOTOR_DDR |= (1 << MOTOR_3_PIN);

	motor_off(1);
	motor_off(2);
	motor_off(3);
}

void motor_on (uint8_t motor_number){
	switch (motor_number){
	case 1:
		MOTOR_PORT |= (1 << MOTOR_1_PIN);
		break;
	case 2:
		MOTOR_PORT |= (1 << MOTOR_2_PIN);
		break;
	case 3:
		MOTOR_PORT |= (1 << MOTOR_3_PIN);
		break;
	}
}

void motor_off (uint8_t motor_number){
	switch (motor_number){
	case 1:
		MOTOR_PORT &= ~(1 << MOTOR_1_PIN);
		break;
	case 2:
		MOTOR_PORT &= ~(1 << MOTOR_2_PIN);
		break;
	case 3:
		MOTOR_PORT &= ~(1 << MOTOR_3_PIN);
		break;
	}
}

void porsh_check (state_porsh_t * state){
	if(state->end){
		if(time_compare(time_service_get(),state->time_krit)){
			motor_off(state->number);
			state->end = false;
		}
	}
}

//============================================================================
//SEPARATION SENSORS
//============================================================================
void separation_sensors_init (){
	SEPARATION_SENSOR_DDR &= ~(1<< SEPARATION_SENSOR_1_PIN);
	SEPARATION_SENSOR_DDR &= ~(1<< SEPARATION_SENSOR_2_PIN);
	SEPARATION_SENSOR_DDR &= ~(1<< SEPARATION_SENSOR_3_PIN);
}

bool separation_sensor (uint8_t sensor_number){
	switch (sensor_number){
	case 1:
		if ((SEPARATION_SENSOR_PPIN & (1 << SEPARATION_SENSOR_1_PIN)) == 0)
			return true;
		else return false;
		break;
	case 2:
		if ((SEPARATION_SENSOR_PPIN & (1 << SEPARATION_SENSOR_2_PIN)) == 0)
			return true;
		else return false;
		break;
	case 3:
		if ((SEPARATION_SENSOR_PPIN & (1 << SEPARATION_SENSOR_3_PIN)) == 0)
			return true;
		else return false;
		break;
	}
	return false;
}

bool separation_sensors_state (){
	bool separation_sensor_1 = separation_sensor(1);
	bool separation_sensor_2 = separation_sensor(2);
	bool separation_sensor_3 = separation_sensor(3);

	if ((separation_sensor_1 && separation_sensor_2) ||
		(separation_sensor_2 && separation_sensor_3) ||
		(separation_sensor_3 && separation_sensor_1))

		return true;
	else
		return false;
}

//============================================================================
//TRIGER
//============================================================================

void trigger_init (){
	TRIGGER_DDR &= ~(1<< TRIGGER_PIN);
}

bool trigger (){
	if ((TRIGGER_PPIN & (1<< TRIGGER_PIN)) == 0)
		return true;
	else return false;
}
