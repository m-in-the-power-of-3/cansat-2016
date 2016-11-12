/*
 * temperature.c
 *
 *  Created on: 05 марта 2016 г.
 *      Author: developer
 */
#include <stdint.h>
#include <util/delay.h>

#include "hal/one_wire.h"

uint16_t DS18B20_read (){
	//OneWireReset();
	while(!one_wire_reset()){}
	one_wire_write_byte(0xCC);
	one_wire_write_byte(0x44);
	_delay_ms(1000);
	one_wire_reset();
	one_wire_write_byte(0xCC);
	one_wire_write_byte(0xBE);
	int str [9];
	for(int i = 0; i<9; i++) {
		str[i] = one_wire_read_byte();
	}
	uint16_t value = str[0] | (str[1]<<8);
	//float t = value / 16.0f;
	return value;
}

float DS18D20_count_temperature (uint16_t temperature_raw){
	float temperature = temperature_raw / 16.3;
	return temperature;
}
