/*
 * mq7.c
 *
 *  Created on: 15 марта 2017 г.
 *      Author: developer
 */

#include "mq7.h"
#include <avr/io.h>
#include "rscs/adc.h"
#include "stdbool.h"

#define MQ7_DDR DDRA
#define MQ7_PIN 5
#define MQ7_PORT PORTA

void mq7_init(){
	MQ7_DDR |= (1 << MQ7_PIN);
	return;
}

bool mq7_digital_read(){
	if(MQ7_PORT & (1 << MQ7_PIN)) return true;
	else return false;
}

float mq7_analod_read(){
	uint32_t result = 0;
	rscs_adc_start_conversion(RSCS_ADC_DIFF_0_0_10X);
	rscs_adc_get_result(&result , RSCS_ADC_DIFF_0_0_10X);
	(float)result;
	result *= 0.08/1024;
	return result;
}
