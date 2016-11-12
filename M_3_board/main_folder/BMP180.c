/*
 * BMP180.c
 *
 *  Created on: 22 мая 2016 г.
 *      Author: developer
 */
#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>

#include "hal/twi.h"
#include "hal/structs.h"

#define ADRESS_SENSORS 0x77
#define ADRESS_MODE 0xF4
#define ADRESS_READ 0xF6
#define PRESSURE_COD 0xB4/*См. таблицу
Температура | OSS = 00 | CSO = 1 | 01110 | 00101110 = 0x2E | до 4,5 мс  | 3 мкА | 0,5 °C
Давление    | OSS = 00 | CSO = 1 | 10100 | 00110100 = 0x34 | до 4,5 мс  | 3 мкА | 0,6 гПа
Давление    | OSS = 01 | CSO = 1 | 10100 | 01110100 = 0x74 | до 7,5 мс  | 5 мкА | 0,5 гПа
Давление    | OSS = 10 | CSO = 1 | 10100 | 10110100 = 0xB4 | до 13,5 мс | 7 мкА | 0,4 гПа
Давление    | OSS = 11 | CSO = 1 | 10100 | 11110100 = 0xF4 | до 25,5 мс |12 мкА | 0,3 гПа*/
#define TEMPERATURE_COD 0x2E
#define OSS 2

#define AC1 0xAA
#define AC2 0xAC
#define AC3 0xAE
#define AC4 0xB0
#define AC5 0xB2
#define AC6 0xB4
#define B1 0xB6
#define B2 0xB8
#define MB 0xBA
#define MC 0xBC
#define MD 0xBE

BMP180calibration_t calibration;

void BMP180_init(){
	twi_read_2_bytes(AC1,&calibration.ac1);
	twi_read_2_bytes(AC2,&calibration.ac2);
	twi_read_2_bytes(AC3,&calibration.ac3);
	twi_read_2_bytes(AC4,&calibration.ac4);
	twi_read_2_bytes(AC5,&calibration.ac5);
	twi_read_2_bytes(AC6,&calibration.ac6);
	twi_read_2_bytes(B1,&calibration.b1);
	twi_read_2_bytes(B2,&calibration.b2);
	twi_read_2_bytes(MB,&calibration.mb);
	twi_read_2_bytes(MC,&calibration.mc);
	twi_read_2_bytes(MD,&calibration.md);
}

uint32_t BMP180_read_pressure (){
	twi_start();
	twi_write_adress(ADRESS_SENSORS,0);
	twi_write_byte(ADRESS_MODE);
	twi_write_byte(PRESSURE_COD);
	twi_stop();
	twi_start();
	twi_write_adress(ADRESS_SENSORS,0);
	twi_write_byte(ADRESS_READ);
	twi_start();
	twi_write_adress(ADRESS_SENSORS,1);
	uint8_t MSB = twi_read_byte(true);
	uint8_t LSB = twi_read_byte(true);
	uint8_t xLSB = twi_read_byte(false);
	twi_stop();
	uint32_t pressure = (uint32_t)MSB << 16;
	pressure |= (uint16_t)LSB << 8;
	pressure |= xLSB;
	pressure = pressure >> (8-OSS);
	return pressure;
}

uint16_t BMP180_read_temperature (){
	twi_start();
	twi_write_adress(ADRESS_SENSORS,0);
	twi_write_byte(ADRESS_MODE);
	twi_write_byte(TEMPERATURE_COD);
	twi_stop();
	twi_start();
	twi_write_adress(ADRESS_SENSORS,0);
	twi_write_byte(ADRESS_READ);
	twi_start();
	twi_write_adress(ADRESS_SENSORS,1);
	uint8_t MSB = twi_read_byte(true);
	uint8_t LSB = twi_read_byte(false);
	twi_stop();
	uint16_t temperature = ((uint16_t)MSB << 8) | LSB;
	return temperature;
}

int32_t BMP180_count_B5 (int32_t UT){
  int32_t X1 = (UT - (int32_t)calibration.ac6) * ((int32_t)calibration.ac5) >> 15;
  int32_t X2 = ((int32_t)calibration.mc << 11) / (X1+(int32_t)calibration.md);
  return X1 + X2;
}

float BMP180_count_temperature(){
	int32_t B5 = BMP180_count_B5 (BMP180_read_temperature());
	float temperature = (B5 + 8) >> 4;
	temperature = temperature / 10.0;
	return temperature;
}


int32_t BMP180_count_pressure(){
	uint16_t UT = BMP180_read_temperature();
	uint32_t  UP = BMP180_read_pressure();
	int32_t B5 = BMP180_count_B5(UT);
	int32_t B6 = B5 - 4000;
	int32_t X1 = ((int32_t)calibration.b2 * ( (B6 * B6)>>12 )) >> 11;
	int32_t X2 = ((int32_t)calibration.ac2 * B6) >> 11;
	int32_t X3 = X1 + X2;
	int32_t B3 = ((((int32_t)calibration.ac1*4 + X3) << OSS) + 2) / 4;
	X1 = ((int32_t)calibration.ac3 * B6) >> 13;
	X2 = ((int32_t)calibration.b1 * ((B6 * B6) >> 12)) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;
	uint32_t B4 = ((uint32_t)calibration.ac4 * (uint32_t)(X3 + 32768))>>15;
	uint32_t B7 = ((uint32_t)UP - B3) * (uint32_t)(50000UL >> OSS);
	int32_t p;
	if (B7 < 0x80000000) {
	p = (B7 * 2) / B4;
	} else {
	p = (B7 * 2) / B3;
	}
	X1 = (p >> 8) * (p >> 8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * p) >> 16;
	p = p + ((X1 + X2 + (int32_t)3791)>>4);
	return p;

}
