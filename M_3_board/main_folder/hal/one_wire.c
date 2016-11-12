/*
 * one_wire.c
 *
 *  Created on: 27 февр. 2016 г.
 *      Author: developer
 */

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define one_wire_port PORTC
#define one_wire_ppin PINC
#define one_wire_ddr DDRC
#define one_wire_pin 0

void one_wire_init() {
	one_wire_ddr &= ~(1 << one_wire_pin);
	one_wire_port &= ~(1 << one_wire_pin);
}

void one_wire_set_bus_zero() {
	one_wire_port |= (1 << one_wire_pin);
}

void one_wire_set_bus_one() {
	one_wire_port &= ~(1 << one_wire_pin);
}

int one_wire_read_bus() {
	return one_wire_ppin & (1 << one_wire_pin);
}

int one_wire_reset() {
	one_wire_set_bus_zero();
	_delay_us(500);
	one_wire_set_bus_one();
	_delay_us(10);

	int is_someone_here = 0;
	for (int i = 0; i < 480; i++) 	{
		if (one_wire_read_bus() == 0)
			is_someone_here = 1;

		_delay_us(1);
	}
return is_someone_here;
}
void one_wire_write_bit(int value) {
	one_wire_set_bus_zero();
	_delay_us(2);
	if (value != 0) {
		one_wire_set_bus_one();
	}

	_delay_us(60);
	one_wire_set_bus_one();
}
void one_wire_write_byte(uint8_t byte) {
	for (uint8_t i = 0; i < 8; i++) {
		uint8_t bit = (1<<i) &byte;
		one_wire_write_bit(bit);
	}
}
int one_wire_read_bit () {
	one_wire_set_bus_zero();
	_delay_us (2);
	one_wire_set_bus_one();
	_delay_us (20);
	int retval = one_wire_read_bus();
	_delay_us(30);
	return retval;
}

uint8_t one_wire_read_byte() {
	uint8_t retval = 0;
	for(uint8_t i=0; i < 8; i++) {
		uint8_t bit = one_wire_read_bit();
		if (bit !=0) {bit = 1;}
		retval = retval |(bit<<i);
	}
	return retval;
}
