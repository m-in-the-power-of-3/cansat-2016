/*
 * sensor.c
 *
 *  Created on: 26 марта 2016 г.
 *      Author: developer
 */
#include <avr/io.h>
#include <stdbool.h>

#include "hal/defines.h"

void sensor_init (){
	SEPARATION_SENSOR_DDR &= ~(1<< SEPARATION_SENSOR_1_PIN);
	SEPARATION_SENSOR_DDR &= ~(1<< SEPARATION_SENSOR_2_PIN);
	SEPARATION_SENSOR_DDR &= ~(1<< SEPARATION_SENSOR_3_PIN);
}

bool separation_sensor (uint8_t sensor_number){
	switch (sensor_number){
	case 1:
		if ((SEPARATION_SENSOR_PPIN & (1<< SEPARATION_SENSOR_1_PIN)) == 0)
			return true;
		else return false;
		break;
	case 2:
		if ((SEPARATION_SENSOR_PPIN & (1<< SEPARATION_SENSOR_2_PIN)) == 0)
			return true;
		else return false;
		break;
	case 3:
		if ((SEPARATION_SENSOR_PPIN & (1<< SEPARATION_SENSOR_3_PIN)) == 0)
			return true;
		else return false;
		break;
	}
	return false;
}
