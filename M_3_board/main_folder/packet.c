/*
 * paket.c
 *
 *  Created on: 22 мая 2016 г.
 *      Author: developer
 */
#include <stddef.h>
#include <stdint.h>

#include <rscs/uart.h>
#include <rscs/sdcard.h>

#include "hal/structs.h"
#include "hal/time.h"

uint16_t count_sum (const void * value,size_t size){
	uint16_t control_summ = 0;
	const uint8_t * ptr = (const uint8_t *)value;
	for (uint8_t i = 0; i < size; i++) {
		control_summ = control_summ + *(ptr + i);
	}
	return control_summ;
}

void update_packet (packet_t * packet, size_t size_of_packet){
	packet->number++;
	time_data_t time_now = time_service_get();
	packet->time_h = time_now.seconds;
	packet->time_l = time_now.subseconds;
	packet->sum = count_sum(packet,size_of_packet - 2);
}

void update_packet_extra (packet_extra_t * packet, size_t size_of_packet){
	packet->number++;
	time_data_t time_now = time_service_get();
	packet->time_h = time_now.seconds;
	packet->time_l = time_now.subseconds;
	packet->sum = count_sum(packet,size_of_packet - 2);
}

void send_packet_uart (rscs_uart_bus_t * bus,uint8_t * packet,size_t size_of_packet){
	rscs_uart_write(bus,packet,size_of_packet);
}

rscs_e send_packet_sd (rscs_sdcard_t * sd, buffer_for_sd_t * buffer_for_sd,uint8_t * packet,size_t size_of_packet){
	if ((512 - buffer_for_sd->busy_bytes) >= size_of_packet){
		for (int i = 0; i < size_of_packet;i++)
			buffer_for_sd->buffer[buffer_for_sd->busy_bytes + i] = *(packet + i);

		buffer_for_sd->busy_bytes += size_of_packet;
		return RSCS_E_NONE;
	}
	else {
		rscs_e error;
		error = rscs_sd_block_write(sd,buffer_for_sd->number,&buffer_for_sd->buffer,1);
		if (error != RSCS_E_NONE)
			return error;

		buffer_for_sd->number += 1;
		buffer_for_sd->busy_bytes = 0;

		for (int i = 0; i < size_of_packet;i++)
			buffer_for_sd->buffer[buffer_for_sd->busy_bytes + i] = *(packet + i);

		buffer_for_sd->busy_bytes += size_of_packet;

		return RSCS_E_NONE;
	}
}
