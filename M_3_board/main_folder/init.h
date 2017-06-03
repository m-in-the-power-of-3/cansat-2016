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

#include "hal/structs.h"

bmp280_t bmp280;
gps_t gps;

float RO;

rscs_ds18b20_t * ds18b20;
rscs_adxl345_t * adxl345;

packet_t main_packet = {0xFF,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
packet_extra_t packet_extra = {0xFE,0,0,0,0,0};

void init_low_hardware ();
void init_hardware ();
void init_sensors ();

#endif /* INIT_H_ */
