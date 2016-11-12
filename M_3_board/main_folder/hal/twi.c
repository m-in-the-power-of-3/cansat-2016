/*
 * twi.c
 *
 *  Created on: 14 мая 2016 г.
 *      Author: developer
 */
#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>

#include "structs.h"
#include "time.h"

#define ERROR_TIME {0,15625}
#define _ERROR_TWI_TIME_END 0x01

void twi_init (){
	TWBR = 100;
	TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
	TWCR = (1<<TWEN);
}

void twi_reset (){
	TWCR &= ~(1<<TWEN);
	TWCR |= (1<<TWEN);
}

void twi_stop (){
	TWCR = (1<<TWEN)|(1<<TWSTO)|(1<<TWINT);
}

void twi_start (uint8_t *error){
	TWCR = (1<<TWEN)|(1<<TWSTA)|(1<<TWINT);
	time_data_t error_time = ERROR_TIME;
	time_data_t error_time_end = time_sum(time_service_get(),error_time);
	while((TWCR & (1<<TWINT)) == 0){
		if(time_compare(time_service_get(),error_time_end)){
			*(error) = _ERROR_TWI_TIME_END;
			return;
		}
	}
	uint8_t state = TWSR & 0xF8;
	if (state == 0x08 || state == 0x10){
		return;
	}
	else{
		*(error) = state;
		twi_reset();
		return;
	}
}

void twi_write_byte(uint8_t byte){
	TWDR = byte;
	TWCR = (1<<TWEN)|(1<<TWINT);
	while((TWCR & (1<<TWINT)) == 0){}
	uint8_t state = TWSR & 0xF8;
	if (state == 0x28|| state == 0x18||state == 0x40){
		return;
	}
	else {
		twi_reset();
		return;
	}
}

uint8_t twi_read_byte (bool mode){
	// true - Ack, false -  Nack
	if(mode){
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	}
	else {
		TWCR = (1<<TWINT)|(1<<TWEN);
	}
	while((TWCR & (1<<TWINT)) == 0){}
	uint8_t state = TWSR & 0xF8;
	if (state == 0x50 || state == 0x58){
		return TWDR;
	}
	else{
		twi_reset();
		return 0;
	}
}

void twi_write_adress(uint8_t adress,uint8_t read){
	// 0 - Write,1 - Read
	uint8_t adress_rw = (adress<<1) | read;
	twi_write_byte(adress_rw);
}

void twi_read_2_bytes(uint8_t adress, void * value_16){
	uint8_t error = 0;
	uint16_t * value_ptr = (uint16_t*)value_16;
	twi_start(&error);
	twi_write_adress(0x77, false);
	twi_write_byte(adress);
	twi_start(&error);
	twi_write_adress(0x77, true);
	*value_ptr = twi_read_byte(true);
	*value_ptr = *value_ptr << 8;
	*value_ptr |= twi_read_byte(false);
}
