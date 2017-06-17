/*
 * mechanics.c
 *
 *  Created on: 05 июня 2017 г.
 *      Author: developer
 */
#include <stdbool.h>

#include "hal/config.h"
#include "hal/structs.h"
#include "hal/time.h"
#include "init.h"

//============================================================================
//MOTOR
//============================================================================
void motor_off (uint8_t motor_number);

void motor_init (){
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
	else return false;
}

//============================================================================
//TRIGER
//============================================================================

void trigger_init (){
	TRIGGER_DDR &= ~(1<< TRIGGER_PIN);
}

bool trigger (){
	if ((TRIGGER_PPIN & (1<< TRIGGER_PIN)) != 0)
		return true;
	else return false;
}

//============================================================================
//INTAKE
//============================================================================
void intake (uint8_t number){
	motor_on (number);
	switch (number) {
	case 1:
		porsh_1.time_krit = time_sum(time_service_get(),time_for_porsh);
		porsh_1.end = true;
		STATUS_BECOME_ALL_RIGHT(STATUS_INTAKE_1)
		break;
	case 2:
		porsh_2.time_krit = time_sum(time_service_get(),time_for_porsh);
		porsh_2.end = true;
		STATUS_BECOME_ALL_RIGHT(STATUS_INTAKE_2)
		break;
	case 3:
		porsh_3.time_krit = time_sum(time_service_get(),time_for_porsh);
		porsh_3.end = true;
		STATUS_BECOME_ALL_RIGHT(STATUS_INTAKE_3)
		break;
	};


}

//============================================================================
//LED
//============================================================================
void led_red (bool trigger){
	if (trigger)
		LED_RED_PORT |= (1 << LED_RED_PIN);
	else LED_RED_PORT &= ~(1 << LED_RED_PIN);
}

void led_blue (bool trigger){
	if (trigger)
		LED_BLUE_PORT |= (1 << LED_BLUE_PIN);
	else LED_BLUE_PORT &= ~(1 << LED_BLUE_PIN);
}

void led_init (){
	LED_RED_DDR |= (1 << LED_RED_PIN);
	LED_BLUE_DDR |= (1 << LED_BLUE_PIN);

	led_red(false);
	led_blue(false);
}

//============================================================================
//SIGNALS
//============================================================================
void signal_fatal_error (){
	led_red(true);
	led_blue(false);
}

void signal_actions (){
	led_red(false);
	led_blue(true);
}

void signal_wait_trigger (){
	if (((blink / 5) % 2) == 0)
		led_blue(true);
	else led_blue (false);

	switch(blink){
	case 0:
		led_red(true);
		break;
	case 20:
		if (!(STATUS_IS_ALL_RIGHT(STATUS_BMP180)))
			led_red(true);
		break;
	case 30:
		led_blue(true);
		if (!(STATUS_IS_ALL_RIGHT(STATUS_BMP280)))
			led_red(true);
		break;
	case 40:
		if (!(STATUS_IS_ALL_RIGHT(STATUS_ADXL345_INIT)))
			led_red(true);
		break;
	case 50:
		if (!(STATUS_IS_ALL_RIGHT(STATUS_SD)))
			led_red(true);
		break;
	case 15:
	case 25:
	case 35:
	case 45:
	case 55:
		led_red(false);
		break;
	default:
		break;
	};

	blink++;
	if (blink >= 60)
		blink = 0;
}
