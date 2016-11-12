/*
 * motor.c
 *
 *  Created on: 26 марта 2016 г.
 *      Author: developer
 */

#include <avr/io.h>
#include <stdbool.h>

#define m_port PORTC
#define m_ppin PINC
#define m_ddr DDRC
#define m_pin_1 1
#define m_pin_2 2
#define m_pin_3 3

#include "hal/structs.h"

void motor_init() {
	m_ddr |= (1 << m_pin_1);
	m_ddr |= (1 << m_pin_2);
	m_ddr |= (1 << m_pin_3);
	m_port |= (1 << m_pin_1);
	m_port |= (1 << m_pin_2);
	m_port |= (1 << m_pin_3);
}

void motor_on (uint8_t motor_number){
	switch (motor_number){
	case 1:
		m_port |= (1 << m_pin_1);
		break;
	case 2:
		m_port |= (1 << m_pin_2);
		break;
	case 3:
		m_port |= (1 << m_pin_3);
		break;
	}
}

void motor_off (uint8_t motor_number){
	switch (motor_number){
	case 1:
		m_port &= ~(1 << m_pin_1);
		break;
	case 2:
		m_port &= ~(1 << m_pin_2);
		break;
	case 3:
		m_port &= ~(1 << m_pin_3);
		break;
	}
}

void porsh_check (porsh_state_t * state){
	if(state->end){
		if(time_compare(time_service_get(),state->time_krit)){
			motor_off(state->number);
			state->end = false;
			printf("motor %u off \n",state->number);
		}
	}
}

