/*
 * data.h
 *
 *  Created on: 29 окт. 2016 г.
 *      Author: developer
 */

#ifndef DATA_H_
#define DATA_H_

#include "hal/structs.h"

void take_data_for_packet ();
void take_data_for_packet_extra ();

uint32_t count_average_pressure();
rscs_e count_height (float * height,const uint32_t pressure_at_start);


#endif /* DATA_H_ */
