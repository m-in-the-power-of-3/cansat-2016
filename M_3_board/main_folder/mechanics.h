/*
 * mechanics.h
 *
 *  Created on: 05 июня 2017 г.
 *      Author: developer
 */

#ifndef MECHANICS_H_
#define MECHANICS_H_

#include <stdbool.h>

#include "hal/structs.h"

void motor_init ();
void motor_on (uint8_t motor_number);
void porsh_check (state_porsh_t * state);

void separation_sensors_init ();
bool separation_sensors_state ();

void trigger_init ();
bool trigger ();

void intake (uint8_t number);

void led_red (bool trigger);
void led_blue (bool trigger);
void led_init ();

void signal_fatal_error ();
void signal_actions ();
void signal_wait_trigger ();

#endif /* MECHANICS_H_ */
