/*
 * OneWire.h
 *
 *  Created on: 27 февр. 2016 г.
 *      Author: developer
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_

void one_wire_init();
void one_wire_write_byte(uint8_t byte);
uint8_t one_wire_read_byte();
int one_wire_reset();


#endif /* ONEWIRE_H_ */
