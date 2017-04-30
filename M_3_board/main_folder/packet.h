/*
 * paket.h
 *
 *  Created on: 12 марта 2016 г.
 *      Author: developer
 */
#ifndef PAKET_H_
#define PAKET_H_

uint16_t count_sum ();
void send_packet (rscs_uart_bus_t * bus,packet_t * packet,size_t size_of_packet);

#endif /* PAKET_H_ */
