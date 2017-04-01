/*
 * data.c
 *
 *  Created on: 29 окт. 2016 г.
 *      Author: developer
 */
#include <stdint.h>

#include <rscs/error.h>
#include <rscs/bmp280.h>

#include "BMP180.h"
#include "hal/config.h"
#include "hal/structs.h"

uint32_t count_average_pressure(packet_t * ptr,bmp280_t * bmp280){
	uint8_t n = 0;
	rscs_e error = bmp180_count_all(&ptr->BMP180_pressure,&ptr->BMP180_temperature);
	if((error != 0 ) || (CHECK_MAX_PRESSURE < ptr->BMP180_pressure) || (CHECK_MIN_PRESSURE > ptr->BMP180_pressure)){
		ptr->BMP180_pressure = 0;
	}
	else {
		n += 1;
	}

	rscs_bmp280_read(bmp280->descriptor,&bmp280->raw_press,&bmp280->raw_temp);
	rscs_bmp280_calculate(bmp280->calibration_values,bmp280->raw_press,bmp280->raw_temp,&ptr->BMP280_pressure,&ptr->BMP280_temperature);

	if((CHECK_MAX_PRESSURE < ptr->BMP280_pressure) || (CHECK_MIN_PRESSURE > ptr->BMP280_pressure)){
		ptr->BMP280_pressure = 0;
	}
	else {
		n += 1;
	}

	if (n != 0){
		printf("a_p = %lu)\n",(ptr->BMP180_pressure + ptr->BMP280_pressure));
		printf("av_pressure = %lu\n",((ptr->BMP180_pressure + ptr->BMP280_pressure)/n));
		return (ptr->BMP180_pressure + ptr->BMP280_pressure)/n;
	}
	return 0;
}

void count_height (float * height, packet_t * ptr,bmp280_t * bmp280, uint32_t pressure_at_start){
	uint32_t average_pressure = count_average_pressure(ptr,bmp280);
	if (average_pressure != 0){
		float X = average_pressure /pressure_at_start;
		*height = 44330 * (1.0 - pow(X,0.1903));
	}
	return;
}

