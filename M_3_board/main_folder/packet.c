/*
 * paket.c
 *
 *  Created on: 22 мая 2016 г.
 *      Author: developer
 */
#include <stddef.h>
#include <stdint.h>

uint16_t count_sum (const void * value,size_t size){
	uint16_t control_summ = 0;
	for (uint8_t i = 0; i < size; i++) {
		control_summ = control_summ + *((const uint8_t*)value + i);
	}
	return control_summ;
}

uint16_t count_state (int state_now){
	//----------------------------------------------------------add function
	return 0;
}

/*void make_packet (packet_t * pkt , uint16_t pkt_number,int state_now){
	time_data time = time_service_get();
	pkt->control = 0xff;
	pkt->number = pkt_number;
	pkt->pressure_1 = adc_read();
	pkt->pressure_2 = 123;
	pkt->temperature_1 = temperature();
	pkt->temperature_2 = 123;
	pkt->state = count_state (state_now);
	pkt->time_h = time.seconds;
	pkt->time_l = time.subseconds;
	pkt->sum =	count_sum (&pkt, sizeof(pkt)-sizeof(pkt->control));
}*/

