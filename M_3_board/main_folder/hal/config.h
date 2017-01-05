/*
 * defines.h
 *
 *  Created on: 18 нояб. 2016 г.
 *      Author: developer
 */

#ifndef DEFINES_H_
#define DEFINES_H_

//BMP180
#define ADRESS_SENSORS 0x77
#define ADRESS_MODE 0xF4
#define ADRESS_READ 0xF6
#define PRESSURE_COD 0x74/*look at the table
Temperature | OSS = 00 | CSO = 1 | 01110 | 00101110 = 0x2E | 4,5 mc  | 3 mA | 0,5 °C
Pressure    | OSS = 00 | CSO = 1 | 10100 | 00110100 = 0x34 | 4,5 mc  | 3 mA | 0,6 GPa
Pressure    | OSS = 01 | CSO = 1 | 10100 | 01110100 = 0x74 | 7,5 mc  | 5 mA | 0,5 GPa
Pressure    | OSS = 10 | CSO = 1 | 10100 | 10110100 = 0xB4 | 13,5 mc | 7 mA | 0,4 GPa
Pressure    | OSS = 11 | CSO = 1 | 10100 | 11110100 = 0xF4 | 25,5 mc |12 mA | 0,3 GPa*/
#define TEMPERATURE_COD 0x2E
#define BMP180_TIME_TEMPERATURE 5
#define BMP180_TIME_PRESSURE 8
#define OSS 1
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

//SELECTOR SWITCH
#define SELECTOR_SWITCH_DDR DDRC
#define SELECTOR_SWITCH_PPIN PINC
#define SELECTOR_SWITCH_PORT PORTC
#define SELECTOR_SWITCH_PIN 4

#endif /* DEFINES_H_ */
