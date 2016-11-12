/*
 * sensor.c
 *
 *  Created on: 26 марта 2016 г.
 *      Author: developer
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>

#define ps_port PORTA
#define ps_ppin PINA
#define ps_ddr DDRA
#define ps_pin_1 3
#define ps_pin_2 4
#define ps_pin_3 5

void sensor_init (){
	ps_ddr &= ~(1<< ps_pin_1);
	ps_ddr &= ~(1<< ps_pin_2);
	ps_ddr &= ~(1<< ps_pin_3);
}

bool porsh_sensor_1 (){
	if ((ps_ppin & (1<< ps_pin_1)) != 0)
		return true;
	else return false;
}
bool porsh_sensor_2 (){
	if ((ps_ppin & (1<< ps_pin_2)) != 0)
		return true;
	else return false;
}
bool porsh_sensor_3 (){
	if ((ps_ppin & (1<< ps_pin_3)) != 0)
		return true;
	else return false;
}
