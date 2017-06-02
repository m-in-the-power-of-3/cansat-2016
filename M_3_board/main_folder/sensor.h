/*
 * sensor.h
 *
 *  Created on: 26 марта 2016 г.
 *      Author: developer
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <stdbool.h>

void separation_sensors_init ();
void trigger_init ();

bool trigger ();

bool separation_sensor (uint8_t sensor_number);

#endif /* SENSOR_H_ */
