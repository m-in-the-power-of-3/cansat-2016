/*
 * paket.h
 *
 *  Created on: 12 марта 2016 г.
 *      Author: developer
 */
#ifndef SEND_PACKETS_H_
#define SEND_PACKETS_H_

void buffer_for_sd_init ();

uint16_t count_sum (const void * value,size_t size);

void update_packet ();
void update_packet_extra ();

void send_packet_uart (rscs_uart_bus_t * bus,uint8_t * packet,size_t size_of_packet);
rscs_e send_packet_sd (rscs_sdcard_t * sd,uint8_t * packet,size_t size_of_packet);
rscs_e send_packet (rscs_uart_bus_t * bus,rscs_sdcard_t * sd,uint8_t * packet_ptr,size_t size_of_packet);

#endif /* PACKETS_H_ */
