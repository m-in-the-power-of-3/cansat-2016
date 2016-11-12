/*
 * BMP180.h
 *
 *  Created on: 25 мая 2016 г.
 *      Author: developer
 */
#ifndef ADDSENSORS_H_
#define ADDSENSORS_H_

#include <stdio.h>

void BMP180_init();
uint32_t BMP180_read_pressure();
uint16_t BMP180_read_temperature();
int32_t BMP180_count_pressure();
float BMP180_count_temperature();

#endif /* ADDSENSORS_H_ */
