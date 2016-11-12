/*
 * twi.h
 *
 *  Created on: 22 мая 2016 г.
 *      Author: developer
 */

#ifndef TWI_H_
#define TWI_H_

#include <stdbool.h>

void twi_init ();
void twi_reset ();
void twi_start ();
void twi_write_byte(uint8_t byte);
uint8_t twi_read_byte (bool mode);
void twi_write_adress(uint8_t adress,uint8_t read);
void twi_read_2_bytes(uint8_t adress, void * value_16);
void twi_stop ();

#endif /* TWI_H_ */
