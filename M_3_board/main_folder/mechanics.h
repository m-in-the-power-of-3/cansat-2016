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
bool separation_sensor (uint8_t sensor_number);
bool separation_sensors_state ();

void trigger_init ();
bool trigger ();

void intake (uint8_t number);

void led_init ();
void led_on (uint8_t number);
void led_off (uint8_t number);

void signal_fatal_error ();
void signal_actions ();
void signal_wait_trigger ();

#endif /* MECHANICS_H_ */
