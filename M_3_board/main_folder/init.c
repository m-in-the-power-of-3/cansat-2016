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
#include "mechanics.h"
#include "mq7.h"
#include "send_packets.h"
#include "hal/time.h"
#include "hal/config.h"
#include "hal/structs.h"

bmp280_t bmp280;
gps_t gps;

float RO;

rscs_ds18b20_t * ds18b20;
rscs_adxl345_t * adxl345;

packet_t main_packet = {0xFF,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
packet_extra_t packet_extra = {0xFE,0,0,0,0,0};

rscs_uart_bus_t * uart_1;

rscs_sdcard_t * sd;

state_porsh_t porsh_1 = {{0,0},false,1};
state_porsh_t porsh_2 = {{0,0},false,2};
state_porsh_t porsh_3 = {{0,0},false,3};

const time_data_t time_for_porsh = TIME_FOR_PORSH;

uint8_t blink = 0;

void init_status (){
	STATUS_BECOME_ERROR(STATUS_INTAKE_1)
	STATUS_BECOME_ERROR(STATUS_INTAKE_2)
	STATUS_BECOME_ERROR(STATUS_INTAKE_3)
	STATUS_BECOME_ERROR(STATUS_INTAKECO_1)
	STATUS_BECOME_ERROR(STATUS_INTAKECO_2)
}

void init_low_hardware (){
  //ONE WIRE
	rscs_ow_init_bus();

  //UART 1
	uart_1 = rscs_uart_init(RSCS_UART_ID_UART1, RSCS_UART_FLAG_ENABLE_TX);
	rscs_uart_set_baudrate(uart_1, 9100);
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
	LED_MK_INIT
}

void init_hardware (){
  //MOTOR
	motor_init();

  //SEPARATION_SENSORS
	separation_sensors_init();

  //TRIGER
	trigger_init();

  //SD
	buffer_for_sd_init ();
	sd = rscs_sd_init(&SD_DDR,&SD_PORT,SD_PIN);
	rscs_sd_set_timeout(sd,4000);
	rscs_sd_spi_setup_slow();

	for (uint8_t i = 1;i <= INIT_TRY_SD;i++) {
		if (rscs_sd_startup(sd) == RSCS_E_NONE) {
			STATUS_BECOME_ALL_RIGHT(STATUS_SD)
			break;
		}
		else STATUS_BECOME_ERROR(STATUS_SD)
	}

	rscs_sd_spi_setup();

  //LED
	led_init();
}

void init_extra_sensors (){
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
		if (rscs_bmp280_setup(bmp280.descriptor,&bmp280.parameters) == RSCS_E_NONE){
			if (rscs_bmp280_changemode (bmp280.descriptor,RSCS_BMP280_MODE_NORMAL) == RSCS_E_NONE){
				STATUS_BECOME_ALL_RIGHT(STATUS_BMP280)
				break;
			}
			else STATUS_BECOME_ERROR(STATUS_BMP280)
		}
		else STATUS_BECOME_ERROR(STATUS_BMP280)
	}

	bmp280.calibration_values = rscs_bmp280_get_calibration_values(bmp280.descriptor);

  //ADXL345
	adxl345 = rscs_adxl345_initi2c (RSCS_ADXL345_ADDR_ALT);
	for (uint8_t i = 1;i <= INIT_TRY_ADXL345;i++){
		if (rscs_adxl345_startup(adxl345) == RSCS_E_NONE){
			if ((rscs_adxl345_set_range(adxl345,RSCS_ADXL345_RANGE_16G) == RSCS_E_NONE) &&
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
	init_extra_sensors();
	init_standart_sensors();
}
