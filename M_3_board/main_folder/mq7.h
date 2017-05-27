/*
 * mq7.h
 *
 *  Created on: 15 марта 2017 г.
 *      Author: developer
 */

#ifndef MQ7_H_
#define MQ7_H_

#include "stdbool.h"
#include <stdint.h>

// инициализация датчика
// возвращает коээфициент, с которым в дальнейшем надо вызывать функцию readCO
rscs_e mq7_calibrate(float * ro);

// возвращает true если в атмосфере есть газ и false если нет
bool mq7_digital_read();

// возвращает значение CO
rscs_e mq7_read_co(float * CO, float RO);

#endif /* MQ7_H_ */
