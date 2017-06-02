/*
 * paket.h
 *
 *  Created on: 12 марта 2016 г.
 *      Author: developer
 */
#ifndef PAKET_H_
#define PAKET_H_

uint16_t count_sum ();
void send_packet_uart (rscs_uart_bus_t * bus,packet_t * packet,size_t size_of_packet);
void update_packet (packet_t * packet, size_t size_of_packet);
rscs_e send_packet_sd (rscs_sdcard_t * sd,packet_t * packet,size_t size_of_packet);
void update_packet_extra (packet_extra_t * packet, size_t size_of_packet);

#endif /* PAKET_H_ */
