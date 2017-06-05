/*
 * sensor.c
 *
 *  Created on: 26 марта 2016 г.
 *      Author: developer
 */
#include <avr/io.h>
#include <stdbool.h>

#include "hal/config.h"

void separation_sensors_init (){
	SEPARATION_SENSOR_DDR &= ~(1<< SEPARATION_SENSOR_1_PIN);
	SEPARATION_SENSOR_DDR &= ~(1<< SEPARATION_SENSOR_2_PIN);
	SEPARATION_SENSOR_DDR &= ~(1<< SEPARATION_SENSOR_3_PIN);
}

void trigger_init (){
	TRIGGER_DDR &= ~(1<< TRIGGER_PIN);
}

bool trigger (){
	if ((TRIGGER_PPIN & (1<< TRIGGER_PIN)) == 0)
		return true;
	else return false;
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
