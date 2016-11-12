/*
 * adc.h
 *
 *  Created on: 05 марта 2016 г.
 *      Author: developer
 */

#ifndef ADC_H_
#define ADC_H_

#include "structs.h"

void adc_init ();
uint8_t adc_read (uint16_t * adc_value);

#endif /* ADC_H_ */
