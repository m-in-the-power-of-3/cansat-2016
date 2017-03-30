/*
 * structs.h
 *
 *  Created on: 29 окт. 2016 г.
 *      Author: developer
 */
#ifndef STRUCT_H_
#define STRUCT_H_

#include <stdbool.h>

typedef struct {
	uint8_t control;
	uint16_t number;
	int16_t DS18B20_temperature;
	uint32_t BMP180_pressure;
	int16_t BMP180_temperature;
	int32_t BMP280_pressure;
	int32_t BMP280_temperature;
	float GPS_lon;
 	float GPS_lat;
 	float GPS_height;
	uint16_t state;
	uint32_t time;
	uint16_t sum;
} packet_t;

typedef struct {
	int16_t ac1, ac2, ac3;
	uint16_t ac4, ac5, ac6;
	int16_t b1, b2;
	int16_t mb, mc, md;
} bmp180_calibration_t;

typedef struct {
	uint16_t seconds;
	uint16_t subseconds;
} time_data_t ;

typedef struct {
	uint32_t time_krit;
	bool end;
	uint8_t number;
} porsh_state_t;

#endif /* STRUCT_H_ */
