/*
 * BMP180.h
 *
 *  Created on: 25 мая 2016 г.
 *      Author: developer
 */
#ifndef ADDSENSORS_H_
#define ADDSENSORS_H_

#include <stdio.h>
#include "rscs/error.h"

rscs_e bmp180_init();

rscs_e bmp180_read_pressure (uint32_t * raw_pressure);
rscs_e bmp180_read_temperature (uint16_t * raw_temperature);

rscs_e bmp180_count_temperature(int16_t *temperature);
rscs_e bmp180_count_pressure(uint32_t * pressure);

rscs_e bmp180_count_all(uint32_t * pressure,int16_t * temperature);

#endif /* ADDSENSORS_H_ */
