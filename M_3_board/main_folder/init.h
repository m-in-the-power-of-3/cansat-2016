/*
 * init.h
 *
 *  Created on: 03 июня 2017 г.
 *      Author: developer
 */

#ifndef INIT_H_
#define INIT_H_

#include <rscs/adxl345.h>
#include <rscs/ds18b20.h>
#include <rscs/sdcard.h>

#include "hal/structs.h"

extern bmp280_t bmp280;
extern gps_t gps;

extern float RO;

extern rscs_ds18b20_t * ds18b20;
extern rscs_adxl345_t * adxl345;

extern packet_t main_packet;
extern packet_extra_t packet_extra;

extern rscs_uart_bus_t * uart_1;

extern 	rscs_sdcard_t * sd;

extern 	state_porsh_t porsh_1;
extern 	state_porsh_t porsh_2;
extern 	state_porsh_t porsh_3;

extern const time_data_t time_for_porsh;

extern uint8_t blink;

void init_status ();
void init_low_hardware ();
void init_hardware ();
void init_sensors ();

#endif /* INIT_H_ */
