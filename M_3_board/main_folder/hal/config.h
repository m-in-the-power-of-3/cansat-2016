/*
 * defines.h
 *
 *  Created on: 18 нояб. 2016 г.
 *      Author: developer
 */

#ifndef DEFINES_H_
#define DEFINES_H_

//BMP180
#define BMP180_ADRESS 0x77
#define BMP180_MODE_ADRESS 0xF4
#define BMP180_ADRESS_TO_READ 0xF6
#define BMP180_PRESSURE_COD 0x74/*look at the table
Temperature | OSS = 00 | CSO = 1 | 01110 | 00101110 = 0x2E | 4,5 mc  | 3 mA | 0,5 °C
Pressure    | OSS = 00 | CSO = 1 | 10100 | 00110100 = 0x34 | 4,5 mc  | 3 mA | 0,6 GPa
Pressure    | OSS = 01 | CSO = 1 | 10100 | 01110100 = 0x74 | 7,5 mc  | 5 mA | 0,5 GPa
Pressure    | OSS = 10 | CSO = 1 | 10100 | 10110100 = 0xB4 | 13,5 mc | 7 mA | 0,4 GPa
Pressure    | OSS = 11 | CSO = 1 | 10100 | 11110100 = 0xF4 | 25,5 mc |12 mA | 0,3 GPa*/
#define BMP180_TEMPERATURE_COD 0x2E
#define BMP180_TIME_TEMPERATURE 5
#define BMP180_TIME_PRESSURE 8
#define BMP180_OSS 1
#define BMP180_AC1 0xAA
#define BMP180_AC2 0xAC
#define BMP180_AC3 0xAE
#define BMP180_AC4 0xB0
#define BMP180_AC5 0xB2
#define BMP180_AC6 0xB4
#define BMP180_B1 0xB6
#define BMP180_B2 0xB8
#define BMP180_MB 0xBA
#define BMP180_MC 0xBC
#define BMP180_MD 0xBE

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

//MOTOR
#define MOTOR_PORT PORTC
#define MOTOR_DDR DDRC
#define MOTOR_1_PIN 1
#define MOTOR_2_PIN 2
#define MOTOR_3_PIN 3

//ONE WIRE
#define ONE_WIRE_PORT PORTC
#define ONE_WIRE_PPIN PINC
#define ONE_WIRE_DDR DDRC
#define ONE_WIRE_PIN 0

//SENSORS
#define SEPARATION_SENSOR_PPIN PINA
#define SEPARATION_SENSOR_DDR DDRA
#define SEPARATION_SENSOR_1_PIN 3
#define SEPARATION_SENSOR_2_PIN 4
#define SEPARATION_SENSOR_3_PIN 5

//TIME
#define MAX_SUBSECONDS 31250

#endif /* DEFINES_H_ */
