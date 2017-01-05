/*
 * main.c
 *
 *  Created on: 27 февр. 2016 г.
 *      Author: developer
 */
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <math.h>

#include "hal/adc.h"
#include "BMP180.h"
#include "motor.h"
#include "hal/one_wire.h"
#include "packet.h"
#include "sensor.h"
#include "hal/spi.h"
#include "DS18B20.h"
#include "hal/time.h"
#include "hal/twi.h"
#include "hal/uart.h"
#include "HC_SR04.h"
#include "hal/structs.h"

#define CHECK_PRESSURE_MIN 0 //<<========================== add value
#define CHECK_PRESSURE_MAX 0 //<<========================== add value
#define CHECK_TEMPERATURE_MIN 0 //<<========================== add value
#define CHECK_TEMPERATURE_MAX 0 //<<========================== add value
#define ERROR_BMP180_PRESSURE 0x01
#define ERROR_BMP180_TEMPERATURE 0x02
#define ALL_RIGHT  0x00
#define WAIT_PL _delay_ms (50);

int main() {
	motor_init();
	time_service_init();
	sensor_init();

	DDRG |= (1<<3);
	PORTG|= (1<<3);
	_delay_ms(300);
	PORTG &= ~(1<<3);
	_delay_ms(300);
	PORTG|= (1<<3);
	_delay_ms(300);
	PORTG &= ~(1<<3);
	_delay_ms(300);
	typedef enum {
		FIRST_PORSH,
		AFTER_FIRST_PORSH,
		SECOND_PORSH,
		AFTER_SECOND_PORSH,
		THIRD_PORSH,
		AFTER_THIRD_PORSH
	}state;
	state state_now = FIRST_PORSH;
	time_data_t time_porsh = {2,0};
	porsh_state_t m_1 = {{0,0},false,1};
	porsh_state_t m_2 = {{0,0},false,2};
	porsh_state_t m_3 = {{0,0},false,3};
	/*while(1){
		if (selector_switch()){
			PORTG |= (1<<3);
		}else{
			PORTG &= ~(1<<3);
		}
	}*/
	while (1){
		switch (state_now){
		case FIRST_PORSH:
			if (selector_switch()){
				PORTG|= (1<<3);
				motor_on(1);
				WAIT_PL
				m_1.time_krit = time_sum(time_service_get(),time_porsh);
				m_1.end = true;
				state_now = AFTER_FIRST_PORSH;
			}
			break;
		case AFTER_FIRST_PORSH:
			if (!(selector_switch())){
				PORTG &= ~(1<<3);
				WAIT_PL
				state_now = SECOND_PORSH;
			}
			break;
		case SECOND_PORSH:
			if (selector_switch()){
				PORTG|= (1<<3);
				motor_on(2);
				WAIT_PL
				m_2.time_krit = time_sum(time_service_get(),time_porsh);
				m_2.end = true;
				state_now = AFTER_SECOND_PORSH;
			}
			break;
		case AFTER_SECOND_PORSH:
			if (!selector_switch()){
				PORTG &= ~(1<<3);
				WAIT_PL
				state_now = THIRD_PORSH;
			}
			break;
		case THIRD_PORSH:
			if (selector_switch()){
				PORTG|= (1<<3);
				motor_on(3);
				WAIT_PL
				m_3.time_krit = time_sum(time_service_get(),time_porsh);
				m_3.end = true;
				state_now = AFTER_THIRD_PORSH;
			}
			break;
		case AFTER_THIRD_PORSH:
			break;
		}
		porsh_check(&m_1);
		porsh_check(&m_2);
		porsh_check(&m_3);
	}
}
