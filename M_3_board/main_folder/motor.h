/*
 * Motor.h
 *
 *  Created on: 26 марта 2016 г.
 *      Author: developer
 */

#ifndef MOTOR_H_
#define MOTOR_H_

void motor_init();
void motor_on (uint8_t);
void motor_off (uint8_t);

void porsh_check (porsh_state_t * state);

#endif /* MOTOR_H_ */
