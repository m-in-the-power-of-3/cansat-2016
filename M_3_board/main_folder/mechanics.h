/*
 * mechanics.h
 *
 *  Created on: 05 июня 2017 г.
 *      Author: developer
 */

#ifndef MECHANICS_H_
#define MECHANICS_H_

#include <stdbool.h>
#include <hal/structs.h>

void motor_init();
void motor_on (uint8_t motor_number);
void porsh_check (state_porsh_t * state);

void separation_sensors_init ();
bool separation_sensors_state ();

void trigger_init ();
bool trigger ();

#endif /* MECHANICS_H_ */
