/*
 * adc.c
 *
 *  Created on: 05 марта 2016 г.
 *      Author: developer
 */
#include <avr/io.h>

#include "structs.h"

void adc_init (){
    ADMUX = (1<<REFS0);
    ADCSRA = (1<<ADEN)| (1<<ADPS2) | (1<<ADPS1);
    ADCSRA |= (1<<ADSC);
}

uint8_t adc_read (uint16_t * adc_value){
	if((ADCSRA & (1<<ADIF)) == 0){
		return 1;
	}
	else{
		*(adc_value) = ADC;
		ADCSRA |= (1<<ADIF);
		ADCSRA |= (1<<ADSC);
		return 0;
	}
	/*ADCSRA |= (1<<ADSC);
	while ((ADCSRA & (1<<ADIF)) == 0);
	uint16_t retval = ADC;
	ADCSRA |= (1<<ADIF);
*/
}




