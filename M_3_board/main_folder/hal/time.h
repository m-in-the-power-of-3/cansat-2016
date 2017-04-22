/*
 * Time.h
 *
 *  Created on: 12 марта 2016 г.
 *      Author: developer
 */

#ifndef TIME_H_
#define TIME_H_
#include <stdbool.h>
#include <stdint.h>

#include "structs.h"

void time_service_init ();
time_data_t time_service_get ();
time_data_t time_sum (time_data_t aa, time_data_t bb);
bool time_compare (time_data_t aa, time_data_t bb);

#endif /* TIME_H_ */