/*
 * defines.h
 *
 *  Created on: 18 нояб. 2016 г.
 *      Author: developer
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#include <util/delay.h>

#include "structs.h"
#include "../init.h"

//============================================================================
//ACTIONS
//============================================================================
#define GO_TO_END_IF_ERROR(ERROR)      \
	error = ERROR;                     \
	if(error != RSCS_E_NONE)           \
		goto end;

//============================================================================
//BMP180
//============================================================================
#define BMP180_ADRESS 0x77
#define BMP180_MODE_ADRESS 0xF4
#define BMP180_ADRESS_TO_READ 0xF6

#define BMP180_ADRESS_AC1 0xAA
#define BMP180_ADRESS_AC2 0xAC
#define BMP180_ADRESS_AC3 0xAE
#define BMP180_ADRESS_AC4 0xB0
#define BMP180_ADRESS_AC5 0xB2
#define BMP180_ADRESS_AC6 0xB4
#define BMP180_ADRESS_B1 0xB6
#define BMP180_ADRESS_B2 0xB8
#define BMP180_ADRESS_MB 0xBA
#define BMP180_ADRESS_MC 0xBC
#define BMP180_ADRESS_MD 0xBE

//settings
#define BMP180_TEMPERATURE_COD 0x2E
#define BMP180_TIME_TEMPERATURE 5

#define BMP180_TIME_PRESSURE 8
#define BMP180_PRESSURE_COD 0x74

#define BMP180_OSS 1
/*look at the table
Temperature | OSS = 00 | CSO = 1 | 01110 | 00101110 = 0x2E | 4,5 mc  | 3 mA | 0,5 °C
Pressure    | OSS = 00 | CSO = 1 | 10100 | 00110100 = 0x34 | 4,5 mc  | 3 mA | 0,6 GPa
Pressure    | OSS = 01 | CSO = 1 | 10100 | 01110100 = 0x74 | 7,5 mc  | 5 mA | 0,5 GPa
Pressure    | OSS = 10 | CSO = 1 | 10100 | 10110100 = 0xB4 | 13,5 mc | 7 mA | 0,4 GPa
Pressure    | OSS = 11 | CSO = 1 | 10100 | 11110100 = 0xF4 | 25,5 mc |12 mA | 0,3 GPa*/

//Only for test
#define BMP180_TEST_AC1 408
#define BMP180_TEST_AC2 -71
#define BMP180_TEST_AC3 -14383
#define BMP180_TEST_AC4 32741
#define BMP180_TEST_AC5 32757
#define BMP180_TEST_AC6 23153
#define BMP180_TEST_B1 6190
#define BMP180_TEST_B2 4
#define BMP180_TEST_MB -32768
#define BMP180_TEST_MC -8711
#define BMP180_TEST_MD 2868

// ========================================================
//MOTOR
// ========================================================
#define MOTOR_PORT PORTC
#define MOTOR_DDR DDRC
#define MOTOR_1_PIN 6
#define MOTOR_2_PIN 4
#define MOTOR_3_PIN 5

// ========================================================
//SENSORS
// ========================================================
#define SEPARATION_SENSOR_PPIN PINA
#define SEPARATION_SENSOR_DDR DDRA
#define SEPARATION_SENSOR_1_PIN 3
#define SEPARATION_SENSOR_2_PIN 4
#define SEPARATION_SENSOR_3_PIN 5

// ========================================================
//TRIGGER
// ========================================================
#define TRIGGER_DDR DDRC
#define TRIGGER_PPIN PINC
#define TRIGGER_PIN 3

// ========================================================
//TIME
// ========================================================
#define MAX_SUBSECONDS 31250

// ========================================================
//HC_SR04
// ========================================================
#define SONIC_SPEED 340

#define US_PORT PORTA
#define US_PPIN PINA
#define US_DDR DDRA
#define US_PIN_TRIG 1
#define US_pin_ECHO 0

#define HC_SR04_TIME_OUT 5000

#define HC_SR04_DATA_MIN 5
#define HC_SR04_DATA_MAX 2000

// ========================================================
//MQ7
// ========================================================
#define MQ7_DDR DDRA
#define MQ7_PORT PORTA
#define MQ7_PIN 0

// ========================================================
//DS18B20
// ========================================================
#define DS18B20_TIMEOUT 15000
#define DS18B20_DEFECTIVE_VALUE -2200

// ========================================================
//SD
// ========================================================
#define SD_DDR DDRA
#define SD_PORT PORTA
#define SD_PIN (1 << 6)

// ========================================================
//INIT_TRY
// ========================================================
#define INIT_TRY_BMP180 10
#define INIT_TRY_BMP280 10
#define INIT_TRY_ADXL345 10
#define INIT_TRY_DS18B20 10
#define INIT_TRY_CO 5
#define INIT_TRY_SD 50

// ========================================================
//PORSH
// ========================================================
#define TIME_FOR_PORSH {5,0}

// ========================================================
//CHECK
// ========================================================
#define CHECK_MAX_PRESSURE 105000
#define CHECK_MIN_PRESSURE 80000

// ========================================================
//STATUS
// ========================================================
#define STATUS_BMP180 1
#define STATUS_BMP280 2
#define STATUS_ADXL345_INIT 3
#define STATUS_ADXL345_DATA 4
#define STATUS_DS18B20 5
#define STATUS_CO_INIT 6
#define STATUS_CO_DATA 7
#define STATUS_SD 8
#define STATUS_GPS 9
#define STATUS_INTAKE_1 10
#define STATUS_INTAKE_2 11
#define STATUS_INTAKE_3 12
#define STATUS_INTAKECO_1 13
#define STATUS_INTAKECO_2 14

#define STATUS_BECOME_ERROR(STATUS) main_packet.status |= (1 << STATUS);
#define STATUS_BECOME_ALL_RIGHT(STATUS) main_packet.status &= ~(1 << STATUS);

#define STATUS_IS_ALL_RIGHT(STATUS) ((main_packet.status & (1 << STATUS)) == 0)

// ========================================================
//LED
// ========================================================
#define LED_INIT DDRG |= (1 << 3);
#define LED_ON PORTG |= (1 << 3);
#define LED_OFF PORTG &= ~(1 << 3);
#define                   \
	LED_BLINK(TIME)       \
	LED_ON                \
	_delay_ms (TIME);     \
	LED_OFF               \
	_delay_ms (TIME);

#endif /* DEFINES_H_ */
