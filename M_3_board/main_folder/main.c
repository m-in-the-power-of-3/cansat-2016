/*
 * main.c
 *
 *  Created on: 27 февр. 2016 г.
 *      Author: developer
 */
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include <avr/interrupt.h>

#include <rscs/adc.h>
#include <rscs/adxl345.h>
#include <rscs/bmp280.h>
#include <rscs/ds18b20.h>
#include <rscs/error.h>
#include <rscs/gps_nmea.h>
#include <rscs/i2c.h>
#include <rscs/onewire.h>
#include <rscs/sdcard.h>
#include <rscs/spi.h>

#include <rscs/stdext/stdio.h>

#include <rscs/uart.h>
#include <rscs/stdext/stdio.h>

#include "BMP180.h"
#include "data.h"
#include "HC_SR04.h"
#include "motor.h"
#include "mq7.h"
#include "packet.h"
#include "sensor.h"

#include "hal/structs.h"
#include "hal/config.h"
#include "hal/time.h"

#define STATUS_BMP180 1
#define STATUS_BMP280 2
#define STATUS_ADXL345 3
#define STATUS_DS18B20 4
#define STATUS_CO 5
#define STATUS_SD 6
#define STATUS_INTAKE_1 7
#define STATUS_INTAKE_2 8
#define STATUS_INTAKE_3 9
#define STATUS_INTAKECO_1 10
#define STATUS_INTAKECO_2 11

#define INIT_TRY_BMP180 10
#define INIT_TRY_ADXL345 10
#define INIT_TRY_DS18B20 10
#define TRY_SD 10
#define ERROR_CHECK(ERROR,STATUS)\
	error = ERROR; \
	if (error == RSCS_E_NONE) \
		status_now &= ~(1 << STATUS); \
	else \
		status_now |= (1 << STATUS);\


int main (){
//============================================================================
//BEFORE INIT
//============================================================================
	uint16_t status_now = 0;
//============================================================================
//INIT
//============================================================================
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

  //TWI
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

  //OTHER
	LED_INIT
	motor_init();
	sensor_init();

  //DS18B20
	rscs_ds18b20_t * ds18b20_1 = rscs_ds18b20_init(0);
	{
		rscs_e error;
		for (uint8_t i = 1;i <= DS18B20_INIT_TRY;i++){
			error = rscs_ds18b20_start_conversion(ds18b20_1);
			if (error == RSCS_E_NONE){
				status_now &= ~(1 << STATUS_DS18B20);
				break;
			}
			else
				status_now |= (1 << STATUS_DS18B20);
		}
	}

  //BMP180
	{
		rscs_e error;
		for (uint8_t i = 1;i <= BMP180_INIT_TRY;i++){
			error = bmp180_init();
			if (error == RSCS_E_NONE){
				status_now &= ~(1 << STATUS_BMP180);
				break;
			}
			else
				status_now |= (1 << STATUS_BMP180);
		}
	}
  //HC_SR04
	HC_SR04_init();

  //BMP280
	bmp280_t bmp280 = {0,0,0,0};

	bmp280.descriptor = rscs_bmp280_initi2c(RSCS_BMP280_I2C_ADDR_LOW);
	rscs_bmp280_parameters_t bmp280_parametrs = {RSCS_BMP280_OVERSAMPLING_X16,RSCS_BMP280_OVERSAMPLING_X2,RSCS_BMP280_STANDBYTIME_500MS,RSCS_BMP280_FILTER_X16};
	rscs_bmp280_setup(bmp280.descriptor,&bmp280_parametrs);
	rscs_bmp280_changemode (bmp280.descriptor,RSCS_BMP280_MODE_NORMAL);

  //ADXL345
	rscs_adxl345_t * adxl345 = rscs_adxl345_initi2c (RSCS_ADXL345_ADDR_ALT);
	{
		rscs_e error;
		for (uint8_t i = 1;i <= ADXL345_INIT_TRY;i++){
			error = rscs_adxl345_set_range(adxl345,RSCS_ADXL345_RANGE_2G);
			if (error == RSCS_E_NONE){
				status_now &= ~(1 << STATUS_ADXL345);
				break;
			}
			else
				status_now |= (1 << STATUS_ADXL345);
		}

		for (uint8_t i = 1;i <= ADXL345_INIT_TRY;i++){
			error = rscs_adxl345_set_rate(adxl345,RSCS_ADXL345_RATE_200HZ);
			if (error == RSCS_E_NONE){
				status_now &= ~(1 << STATUS_ADXL345);
				break;
			}
			else
				status_now |= (1 << STATUS_ADXL345);
		}
	}
  //MQ7
	float RO;

	if (mq7_calibrate(&RO) == RSCS_E_NONE){
		status_now &= ~(1 << STATUS_CO);
	}else{
		status_now |= (1 << STATUS_CO);
		printf("CO_calibrate_error");
	}
//============================================================================
//CONST
//============================================================================
	bmp280.calibration_values = rscs_bmp280_get_calibration_values (bmp280.descriptor);
	const time_data_t time_for_porsh = TIME_FOR_PORSH;
//============================================================================
//VARIABLE
//============================================================================
	typedef enum {
		STATE_IN_FIRST_MEASURE,
		STATE_IN_SECOND_MEASURE,
		STATE_IN_THIRD_MEASURE,
		STATE_AFTER_THIRD_MEASURE
	} state;
	state state_now = STATE_IN_FIRST_MEASURE;

	porsh_state_t porsh_1 = {{0,0},false,1};
	porsh_state_t porsh_2 = {{0,0},false,2};
	porsh_state_t porsh_3 = {{0,0},false,3};

	float x_g = 0;
	float y_g = 0;
	float z_g = 0;
	bmp280.raw_press = 0;
	bmp280.raw_temp = 0;

	packet_t main_packet = {0xFF,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	packet_extra_t packet_extra = {0xFE,0,0,0,0,0};

	float hight = 0;

	uint32_t pressure_at_start = 0;//----------------------------- add function

//============================================================================
//CHECK
//============================================================================
	//-----------------------------------------------------------finish writing
//============================================================================
//BEFORE START
//============================================================================
	//pressure_at_start = //----------------------------- add function
//============================================================================
//BEFORE SEPARATION
//============================================================================
	//-----------------------------------------------------------finish writing
	while(1){}
//============================================================================
//AFTER SEPARATION
//============================================================================
	const float hight_at_separation = 2;//----------------------------add function
	const float hight_1 = (3 * hight_at_separation) / 4;
	const float hight_2 = hight_at_separation / 2;
	const float hight_3 = hight_at_separation / 4;

	while (1){
	//============================================================================
	//MAKE DATA
	//============================================================================
	//============================================================================
	//OTHER ACTIONS
	//============================================================================
	//============================================================================
	//PISTONS
	//============================================================================
		switch (state_now) {
	  //FIRST PISTON
		case STATE_IN_FIRST_MEASURE:
			if (hight >= hight_1){
				motor_on (1);
				porsh_1.time_krit = time_sum(time_service_get(),time_for_porsh);
				porsh_1.end = true;
				state_now = STATE_IN_SECOND_MEASURE;
				}
			break;
	  //SECOND PISTON
		case STATE_IN_SECOND_MEASURE:
			if (hight >= hight_2){
				motor_on (2);
				porsh_2.time_krit = time_sum(time_service_get(),time_for_porsh);
				porsh_2.end = true;
				state_now = STATE_IN_THIRD_MEASURE;
			}
	  //THIRD PISTON
		break;
			break;
		case STATE_IN_THIRD_MEASURE:
			if (hight >= hight_3) {
				motor_on (3);
				porsh_3.time_krit = time_sum(time_service_get(),time_for_porsh);
				porsh_3.end = true;
				state_now = STATE_AFTER_THIRD_MEASURE;
			}
			break;
		case STATE_AFTER_THIRD_MEASURE:
			break;
		};
	  //DEACTIVATION
		porsh_check(&porsh_1);
		porsh_check(&porsh_2);
		porsh_check(&porsh_3);
	//============================================================================
	//SEND DATA
	//============================================================================
		//send_packet (uart_1,&main_packet,sizeof(main_packet));
	}
}
