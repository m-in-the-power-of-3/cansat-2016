/*
 * structs.h
 *
 *  Created on: 29 окт. 2016 г.
 *      Author: developer
 */
#ifndef STRUCT_H_
#define STRUCT_H_

#include <stdbool.h>

#include <rscs/bmp280.h>

typedef struct {
	const uint8_t control;
	uint16_t number;
	int16_t DS18B20_temperature;
	uint32_t BMP180_pressure;
	int16_t BMP180_temperature;
	int32_t BMP280_pressure;
	int32_t BMP280_temperature;
	int16_t ADXL345_x;
	int16_t ADXL345_y;
	int16_t ADXL345_z;
	float GPS_lon;
 	float GPS_lat;
 	float GPS_height;
 	float CO;
	uint16_t state;
	uint16_t time_h;
	uint16_t time_l;
	uint16_t sum;
} packet_t;

typedef struct {
	uint8_t control;
	uint16_t number;
	uint16_t HC_SR04;
	uint16_t time_h;
	uint16_t time_l;
	uint16_t sum;
} packet_extra_t;

typedef struct {
	int16_t ac1, ac2, ac3;
	uint16_t ac4, ac5, ac6;
	int16_t b1, b2;
	int16_t mb, mc, md;
} bmp180_calibration_t;

typedef struct {
	int32_t raw_press;
	int32_t raw_temp;
	rscs_bmp280_descriptor_t * descriptor;
	const rscs_bmp280_calibration_values_t * calibration_values;
} bmp280_t;

typedef struct {
	uint16_t seconds;
	uint16_t subseconds;
} time_data_t ;

typedef struct {
	time_data_t time_krit;
	bool end;
	uint8_t number;
} porsh_state_t;

#endif /* STRUCT_H_ */
