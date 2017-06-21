/*
 * data.c
 *
 *  Created on: 29 окт. 2016 г.
 *      Author: developer
 */
#include <stdint.h>

#include <rscs/ds18b20.h>
#include <rscs/error.h>
#include <rscs/gps_nmea.h>
#include <rscs/bmp280.h>

#include "BMP180.h"
#include "HC_SR04.h"
#include "init.h"
#include "mq7.h"
#include "hal/config.h"
#include "hal/structs.h"

void take_data_from_HC_SR04 (){
	if (HC_SR04_read (&packet_extra.HC_SR04) != RSCS_E_NONE)
		packet_extra.HC_SR04 = 0;

	if ((packet_extra.HC_SR04 < HC_SR04_DATA_MIN) || (packet_extra.HC_SR04 > HC_SR04_DATA_MAX))
		packet_extra.HC_SR04 = 0;
}

void take_data_from_gps () {
	rscs_e error = rscs_gps_read(gps.descriptor,&main_packet.GPS_lon,&main_packet.GPS_lat,&main_packet.GPS_height,&gps.hasFix);
	if ((gps.hasFix == false) || (error != RSCS_E_NONE))
		STATUS_BECOME_ERROR(STATUS_GPS)
	else
		STATUS_BECOME_ALL_RIGHT(STATUS_GPS)
}

void take_data_from_co () {
	if (STATUS_IS_ALL_RIGHT(STATUS_CO_INIT)){
		if (mq7_read_co(&main_packet.CO,RO) != RSCS_E_NONE)
			STATUS_BECOME_ERROR(STATUS_CO_DATA)
		else
			STATUS_BECOME_ALL_RIGHT(STATUS_CO_DATA)
	}
}

void take_data_from_adxl345 () {
	if (STATUS_IS_ALL_RIGHT(STATUS_ADXL345_INIT)){
		if (rscs_adxl345_read(adxl345,&main_packet.ADXL345_x,&main_packet.ADXL345_y,&main_packet.ADXL345_z) != RSCS_E_NONE)
			STATUS_BECOME_ERROR(STATUS_ADXL345_DATA)
		else
			STATUS_BECOME_ALL_RIGHT(STATUS_ADXL345_DATA)
	}
}

void take_data_from_ds18b20 () {
	if (rscs_ds18b20_check_ready()) {
		if(STATUS_IS_ALL_RIGHT(STATUS_DS18B20)) {
			if (rscs_ds18b20_read_temperature(ds18b20,&main_packet.DS18B20_temperature) != RSCS_E_NONE) {
				main_packet.DS18B20_temperature = DS18B20_DEFECTIVE_VALUE;
			}
		}
		if (rscs_ds18b20_start_conversion(ds18b20) != RSCS_E_NONE)
			STATUS_BECOME_ERROR(STATUS_DS18B20)
	}
}

void take_data_from_bmp (){
  //BMP280
	if (STATUS_IS_ALL_RIGHT(STATUS_BMP280)){
		if ((rscs_bmp280_read(bmp280.descriptor,&bmp280.raw_press,&bmp280.raw_temp)) != RSCS_E_NONE)
			main_packet.BMP280_pressure = 0;
		else {
			if ((rscs_bmp280_calculate(bmp280.calibration_values,bmp280.raw_press,bmp280.raw_temp,&main_packet.BMP280_pressure,&main_packet.BMP280_temperature)) != RSCS_E_NONE)
				main_packet.BMP280_pressure = 0;
		}
	}

  //BMP180
	if (STATUS_IS_ALL_RIGHT(STATUS_BMP180)){
		if (bmp180_count_all (&main_packet.BMP180_pressure,&main_packet.BMP180_temperature) != RSCS_E_NONE)
			main_packet.BMP180_pressure = 0;
	}
}

void take_data_for_packet (){
	take_data_from_adxl345();
	take_data_from_co();
	take_data_from_ds18b20();
	take_data_from_gps();
	take_data_from_bmp();
}

void take_data_for_packet_extra (){
	take_data_from_HC_SR04();
}

uint32_t count_average_pressure(){
	uint8_t n = 0;
	if((CHECK_MAX_PRESSURE < main_packet.BMP180_pressure) || (CHECK_MIN_PRESSURE > main_packet.BMP180_pressure))
		main_packet.BMP180_pressure = 0;
	else
		n += 1;

	if((CHECK_MAX_PRESSURE < main_packet.BMP280_pressure) || (CHECK_MIN_PRESSURE > main_packet.BMP280_pressure))
		main_packet.BMP280_pressure = 0;
	else
		n += 1;

	if (n != 0)
		return (main_packet.BMP180_pressure + (uint32_t)main_packet.BMP280_pressure)/n;

	return 0;
}

rscs_e count_height (float * height,const uint32_t pressure_at_start){
	uint32_t average_pressure = count_average_pressure();
	if ((average_pressure != 0) && (pressure_at_start != 0)){
		float X = (float)average_pressure/pressure_at_start;
		*height = 44330 * (1.0 - pow(X,0.1903));
		return RSCS_E_NONE;
	}
	return RSCS_E_NULL;
}

void count_height_points (){
	packet_mission.height_1 = (3 * packet_mission.height_separation) / 4;
	packet_mission.height_2 = packet_mission.height_separation / 2;
	packet_mission.height_3 = packet_mission.height_separation / 4;
}

