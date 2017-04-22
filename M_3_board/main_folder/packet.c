/*
 * paket.c
 *
 *  Created on: 22 мая 2016 г.
 *      Author: developer
 */
#include <stddef.h>
#include <stdint.h>

#include <rscs/uart.h>
#include <rscs/timeservice.h>

#include "hal/structs.h"

uint16_t count_sum (const void * value,size_t size){
	uint16_t control_summ = 0;
	const uint8_t * ptr = (const uint8_t *)value;
	for (uint8_t i = 0; i < size; i++) {
		control_summ = control_summ + *(ptr + i);
	}
	return control_summ;
}

uint16_t count_state (int state_now){
	//----------------------------------------------------------add function
	return 0;
}

void send_packet (rscs_uart_bus_t * bus,packet_t * packet,size_t size_of_packet){
	packet->number++;
	packet->sum = count_sum(packet,size_of_packet - 2);
	//packet->time = rscs_time_get();
	rscs_uart_write(bus,packet,size_of_packet);
}
