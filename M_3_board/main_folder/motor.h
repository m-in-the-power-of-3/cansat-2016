/*
 * Motor.h
 *
 *  Created on: 26 марта 2016 г.
 *      Author: developer
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "hal/structs.h"

void motor_init();

void motor_on (uint8_t motor_number);
void motor_off (uint8_t motor_number);

//TODO:?
void porsh_check (state_porsh_t * state);

#endif /* MOTOR_H_ */
