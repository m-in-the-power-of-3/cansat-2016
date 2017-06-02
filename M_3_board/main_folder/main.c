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
#define STATUS_GPS 7
#define STATUS_INTAKE_1 8
#define STATUS_INTAKE_2 9
#define STATUS_INTAKE_3 10
#define STATUS_INTAKECO_1 11
#define STATUS_INTAKECO_2 12

#define DS18B20_TIMEOUT 15000
#define DS18B20_DEFECTIVE_VALUE -2200

#define INIT_TRY_BMP180 10
#define INIT_TRY_BMP280 10
#define INIT_TRY_ADXL345 10
#define INIT_TRY_DS18B20 10
#define TRY_SD 10

#define ERROR_CHECK(ERROR,STATUS)        \
	error = ERROR;                       \
	if (error == RSCS_E_NONE)            \
		STATUS_BECOME_ERROR_NONE(STATUS) \
	else                                 \
		STATUS_BECOME_ERROR(STATUS)

#define STATUS_BECOME_ERROR(STATUS) status_now |= (1 << STATUS);
#define STATUS_BECOME_ERROR_NONE(STATUS) status_now &= ~(1 << STATUS);

int main (){
//============================================================================
//BEFORE INIT
//============================================================================
  //CONST
	const time_data_t time_for_porsh = TIME_FOR_PORSH;

  //VARIABLE
	state_mission_t state_mission_now = STATE_IN_FIRST_MEASURE;

	uint16_t status_now = 0;

	state_porsh_t porsh_1 = {{0,0},false,1};
	state_porsh_t porsh_2 = {{0,0},false,2};
	state_porsh_t porsh_3 = {{0,0},false,3};

	packet_t main_packet = {0xFF,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	packet_extra_t packet_extra = {0xFE,0,0,0,0,0};

	bmp280_t bmp280;
	adxl345_t adxl345;

	gps_t gps;

	float RO;

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

  //OTHER
	LED_INIT
	motor_init();
	separation_sensors_init();
	trigger_init();

  //DS18B20
	rscs_ds18b20_t * ds18b20 = rscs_ds18b20_init(0);
	{
		rscs_e error;
		for (uint8_t i = 1;i <= INIT_TRY_DS18B20;i++){
			error = rscs_ds18b20_start_conversion(ds18b20);
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
		for (uint8_t i = 1;i <= INIT_TRY_BMP180;i++){
			error = bmp180_init();
			if (error == RSCS_E_NONE){
				status_now &= ~(1 << STATUS_BMP180);
				break;
			}
			else
				status_now |= (1 << STATUS_BMP180);
		}
	}

  //BMP280
	bmp280.descriptor = rscs_bmp280_initi2c(RSCS_BMP280_I2C_ADDR_LOW);
	bmp280.parameters.filter = RSCS_BMP280_FILTER_X16;
	bmp280.parameters.pressure_oversampling = RSCS_BMP280_OVERSAMPLING_X16;
	bmp280.parameters.standbytyme = RSCS_BMP280_STANDBYTIME_500MS;
	bmp280.parameters.temperature_oversampling = RSCS_BMP280_OVERSAMPLING_X2;

	{
		rscs_e error_setup;
		rscs_e error_changemode;
		for (uint8_t i = 1;i <= INIT_TRY_BMP280;i++){
			error_setup = rscs_bmp280_setup(bmp280.descriptor,&bmp280.parameters);
			error_changemode = rscs_bmp280_changemode (bmp280.descriptor,RSCS_BMP280_MODE_NORMAL);
			if ((error_setup == RSCS_E_NONE)&&(error_changemode == RSCS_E_NONE)){
				status_now &= ~(1 << STATUS_BMP280);
				break;
			}
			else
				status_now |= (1 << STATUS_BMP280);
		}
	}

  //ADXL345
	adxl345.descriptor = rscs_adxl345_initi2c (RSCS_ADXL345_ADDR_ALT);
	{
		rscs_e error_range;
		rscs_e error_rate;
		for (uint8_t i = 1;i <= INIT_TRY_ADXL345;i++){
			error_range = rscs_adxl345_set_range(adxl345.descriptor,RSCS_ADXL345_RANGE_2G);
			error_rate = rscs_adxl345_set_rate(adxl345.descriptor,RSCS_ADXL345_RATE_200HZ);
			if ((error_range == RSCS_E_NONE)&&(error_rate == RSCS_E_NONE)){
				status_now &= ~(1 << STATUS_ADXL345);
				break;
			}
			else
				status_now |= (1 << STATUS_ADXL345);
		}
	}

  //HC_SR04
	HC_SR04_init();

  //GPS
	gps.descriptor = rscs_gps_init(RSCS_UART_ID_UART0);

  //MQ7
	if (mq7_calibrate(&RO) == RSCS_E_NONE)
		status_now &= ~(1 << STATUS_CO);
	else status_now |= (1 << STATUS_CO);

//============================================================================
//FIRST DATA
//============================================================================
  //BMP280
	if ((status_now & (1 << STATUS_BMP280)) == 0){
		if ((rscs_bmp280_read(bmp280.descriptor,&bmp280.raw_press,&bmp280.raw_temp)) != RSCS_E_NONE)
			main_packet.BMP280_pressure = 0;
		else {
			if ((rscs_bmp280_calculate(bmp280.calibration_values,bmp280.raw_press,bmp280.raw_temp,&main_packet.BMP280_pressure,&main_packet.BMP280_temperature)) != RSCS_E_NONE)
				main_packet.BMP280_pressure = 0;
		}
	}

  //BMP180
	if ((status_now & (1 << STATUS_BMP180)) == 0){
		if (bmp180_count_all (&main_packet.BMP180_pressure,&main_packet.BMP180_temperature) != RSCS_E_NONE)
			main_packet.BMP180_pressure = 0;
	}
  //DS18B20
	{
		if ((status_now & (1 << STATUS_DS18B20)) == 0) {
			uint16_t timeout;
			for (int i = 0;i < DS18B20_TIMEOUT;i++){
				if (rscs_ds18b20_check_ready){
					if (rscs_ds18b20_read_temperature(ds18b20,main_packet.DS18B20_temperature) != RSCS_E_NONE)
						main_packet.DS18B20_temperature = DS18B20_DEFECTIVE_VALUE;
					if (rscs_ds18b20_start_conversion(ds18b20) != RSCS_E_NONE)
						STATUS_BECOME_ERROR(STATUS_DS18B20)
				}
				_delay_us (100);
			}
			if (timeout <= DS18B20_TIMEOUT)
				STATUS_BECOME_ERROR(STATUS_DS18B20)
		}
	}
  //ADXL345
	/*
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
	}*/
}
