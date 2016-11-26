/*
 * motor.c
 *
 *  Created on: 26 марта 2016 г.
 *      Author: developer
 */

#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>

#include "hal/config.h"
#include "hal/structs.h"
#include "hal/time.h"

void motor_init() {
	MOTOR_DDR |= (1 << MOTOR_1_PIN);
	MOTOR_DDR |= (1 << MOTOR_2_PIN);
	MOTOR_DDR |= (1 << MOTOR_3_PIN);
	MOTOR_PORT |= (1 << MOTOR_1_PIN);
	MOTOR_PORT |= (1 << MOTOR_2_PIN);
	MOTOR_PORT |= (1 << MOTOR_3_PIN);
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

void porsh_check (porsh_state_t * state){
	if(state->end){
		if(time_compare(time_service_get(),state->time_krit)){
			motor_off(state->number);
			state->end = false;
			printf("motor %u off \n",state->number);//----------------------------------for the test
		}
	}
}

