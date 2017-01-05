/*
 * sensor.h
 *
 *  Created on: 26 марта 2016 г.
 *      Author: developer
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <stdbool.h>

void sensor_init ();
bool separation_sensor (uint8_t sensor_number);
bool selector_switch ();

#endif /* SENSOR_H_ */
