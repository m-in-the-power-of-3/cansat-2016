/*
 * data.h
 *
 *  Created on: 29 окт. 2016 г.
 *      Author: developer
 */

#ifndef DATA_H_
#define DATA_H_

uint32_t count_average_pressure(packet_t * ptr,bmp280_t * bmp280);

void count_height (float * height, packet_t * ptr,bmp280_t * bmp280, const uint32_t pressure_at_start);

#endif /* DATA_H_ */
