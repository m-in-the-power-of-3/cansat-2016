/*
 * init.c
 *
 *  Created on: 03 июня 2017 г.
 *      Author: developer
 */

#include <rscs/adc.h>
#include <rscs/adxl345.h>
#include <rscs/bmp280.h>
#include <rscs/ds18b20.h>
#include <rscs/error.h>
#include <rscs/gps_nmea.h>
#include <rscs/onewire.h>
#include <rscs/spi.h>
#include <rscs/uart.h>
#include <rscs/stdext/stdio.h>

#include "BMP180.h"
#include "HC_SR04.h"
#include "init.h"
#include "motor.h"
#include "mq7.h"
#include "sensor.h"
#include "hal/time.h"
#include "hal/config.h"
#include "hal/structs.h"

void init_low_hardware (){
  //ONE WIRE
	rscs_ow_init_bus();

  //UART 1
	rscs_uart_bus_t * uart_1 = rscs_uart_init(RSCS_UART_ID_UART1, RSCS_UART_FLAG_ENABLE_TX);
	rscs_uart_set_baudrate(uart_1, 9600);
	rscs_uart_set_character_size(uart_1, 8);
	rscs_uart_set_parity(uart_1, RSCS_UART_PARITY_NONE);
	rscs_uart_set_stop_bits(uart_1, RSCS_UART_STOP_BITS_ONE);

  //PRINTF
	FILE * f = rscs_make_uart_stream(uart_1);
	stdout = f;

  //I2C
	rscs_i2c_init();
	rscs_i2c_set_scl_rate(200);

  //SPI
	rscs_spi_init();
	rscs_spi_set_clk(100);

  //ADC
	rscs_adc_init();
	rscs_adc_set_refrence(RSCS_ADC_REF_EXTERNAL_VCC);
	rscs_adc_set_prescaler(RSCS_ADC_PRESCALER_64);

  //TIME
	time_service_init();

  //LED
	LED_INIT
}

void init_hardware (){
  //MOTOR
	motor_init();

  //SEPARATION_SENSORS
	separation_sensors_init();

  //TRIGER
	trigger_init();
}

void init_exstra_sensors (){
  //BMP180
	for (uint8_t i = 1;i <= INIT_TRY_BMP180;i++){
		if (bmp180_init() == RSCS_E_NONE){
			STATUS_BECOME_ALL_RIGHT(STATUS_BMP180)
			break;
		}
		else
			STATUS_BECOME_ERROR(STATUS_BMP180)
	}

  //HC_SR04
	HC_SR04_init();

  //GPS
	gps.descriptor = rscs_gps_init(RSCS_UART_ID_UART0);

  //MQ7
	for (uint8_t i = 1;i <= INIT_TRY_CO;i++) {
		if (mq7_calibrate(&RO) == RSCS_E_NONE) {
			STATUS_BECOME_ALL_RIGHT(STATUS_CO_INIT)
			break;
		}
		else
			STATUS_BECOME_ERROR(STATUS_CO_INIT)
	}
}

void init_standart_sensors (){
   //DS18B20
	ds18b20 = rscs_ds18b20_init(0);
	for (uint8_t i = 1;i <= INIT_TRY_DS18B20;i++){
		if (rscs_ds18b20_start_conversion(ds18b20) == RSCS_E_NONE){
			STATUS_BECOME_ALL_RIGHT(STATUS_DS18B20)
			break;
		}
		else
			STATUS_BECOME_ERROR(STATUS_DS18B20)
	}

  //BMP280
	bmp280.descriptor = rscs_bmp280_initi2c(RSCS_BMP280_I2C_ADDR_LOW);
	bmp280.parameters.filter = RSCS_BMP280_FILTER_X16;
	bmp280.parameters.pressure_oversampling = RSCS_BMP280_OVERSAMPLING_X16;
	bmp280.parameters.standbytyme = RSCS_BMP280_STANDBYTIME_500MS;
	bmp280.parameters.temperature_oversampling = RSCS_BMP280_OVERSAMPLING_X2;

	for (uint8_t i = 1;i <= INIT_TRY_BMP280;i++){
		if ((rscs_bmp280_setup(bmp280.descriptor,&bmp280.parameters) == RSCS_E_NONE) &&
			(rscs_bmp280_changemode (bmp280.descriptor,RSCS_BMP280_MODE_NORMAL) == RSCS_E_NONE)){

			STATUS_BECOME_ALL_RIGHT(STATUS_BMP280)
			break;
		}
		else
			STATUS_BECOME_ERROR(STATUS_BMP280)
	}

  //ADXL345
	adxl345 = rscs_adxl345_initi2c (RSCS_ADXL345_ADDR_ALT);
	for (uint8_t i = 1;i <= INIT_TRY_ADXL345;i++){
		if (rscs_adxl345_startup(adxl345) == RSCS_E_NONE){
			if ((rscs_adxl345_set_range(adxl345,RSCS_ADXL345_RANGE_2G) == RSCS_E_NONE) &&
				(rscs_adxl345_set_rate(adxl345,RSCS_ADXL345_RATE_200HZ) == RSCS_E_NONE)){

				STATUS_BECOME_ALL_RIGHT(STATUS_ADXL345_INIT)
				break;
			}
			else STATUS_BECOME_ERROR(STATUS_ADXL345_INIT)
		}
		else STATUS_BECOME_ERROR(STATUS_ADXL345_INIT)
	}
}

void init_sensors() {
	init_exstra_sensors();
	init_standart_sensors();
}
