/*
 * time.c
 *
 *  Created on: 12 марта 2016 г.
 *      Author: developer
 */
#include "avr/io.h"
#include "avr/interrupt.h"
#include <stdint.h>
#include <util/delay.h>
#include <stdbool.h>

#include "structs.h"
#include "time.h"

#define MAX_SUBSECONDS 31250

volatile uint16_t _seconds = 0;


ISR (TIMER1_COMPA_vect) {
	 _seconds++;
}


void time_service_init (){
	TCCR1A= 0;
	//TCCR1B = (1<< WGM12)|(1<< WGM13);//| (1<< CS12);
	TCCR1B = (1<< WGM12)|(0<< WGM13);//| (1<< CS12);
	//ICR1 = 31250;
	OCR1A = MAX_SUBSECONDS;
	TIMSK = (1<<OCIE1A);
	TCCR1B |= (1<< CS12);

}

time_data_t time_service_get (){
	time_data_t retval;
	cli();

	retval.subseconds = TCNT1;
	retval.seconds = _seconds ;

	sei();

	return retval;
	}

time_data_t time_sum (time_data_t aa, time_data_t b) {
	time_data_t summa;
	summa.seconds = aa.seconds + b.seconds;
	summa.subseconds = aa.subseconds + b.subseconds;
	if (summa.subseconds >= MAX_SUBSECONDS){
		summa.seconds = summa.seconds + 1;
		summa.subseconds = summa.subseconds - MAX_SUBSECONDS;
	}
	return summa;
}

bool time_compare (time_data_t aa, time_data_t bb) {
	//if aa > bb return true, else - false
	if ((aa.seconds > bb.seconds) || ((aa.seconds == bb.subseconds) && (aa.subseconds > bb.subseconds)))
		return true;
		else return false;
}
