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

#include <rscs/onewire.h>
#include <rscs/ds18b20.h>
#include <rscs/uart.h>
#include <rscs/i2c.h>
#include <rscs/stdext/stdio.h>
#include <rscs/spi.h>
#include <rscs/bmp280.h>
#include <rscs/adxl345.h>
#include <rscs/adc.h>
#include <rscs/sdcard.h>

#include "BMP180.h"
#include "motor.h"
#include "packet.h"
#include "sensor.h"
#include "HC_SR04.h"
#include "hal/structs.h"
#include "hal/config.h"
#include "data.h"
#include "hal/time.h"
#include "mq7.h"

#include "hal/config.h"
#include <rscs/uart.h>
#include <rscs/stdext/stdio.h>


#define STATUS_BMP180 1
#define STATUS_BMP280 2
#define STATUS_ADXL345 3
#define STATUS_DS18B20 4
#define STATUS_CO 5
#define STATUS_INTAKE_1 6
#define STATUS_INTAKE_2 7
#define STATUS_INTAKE_3 8
#define STATUS_INTAKECO_1 9
#define STATUS_INTAKECO_2 10

#define BMP180_INIT_TRY 10
#define ADXL345_INIT_TRY 10
#define DS18B20_INIT_TRY 10
#define ERROR_CHECK(ERROR,STATUS)\
	error = ERROR; \
	if (error == RSCS_E_NONE) \
		status_now &= ~(1 << STATUS); \
	else \
		status_now |= (1 << STATUS);\

#define SD_DDR DDRA
#define SD_PORT PORTA
#define SD_PIN (1 << 6)

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
	bmp280_t bmp280;

	bmp280.descriptor = rscs_bmp280_initi2c(RSCS_BMP280_I2C_ADDR_LOW);
	rscs_bmp280_parameters_t bmp280_parametrs = {RSCS_BMP280_OVERSAMPLING_X16,RSCS_BMP280_OVERSAMPLING_X2,RSCS_BMP280_STANDBYTIME_500MS,RSCS_BMP280_FILTER_X16};
	rscs_bmp280_setup(bmp280.descriptor,&bmp280_parametrs);
	rscs_bmp280_changemode (bmp280.descriptor,RSCS_BMP280_MODE_NORMAL);

  //ADXL345
	rscs_adxl345_t * adxl345 = rscs_adxl345_initi2c (RSCS_ADXL345_ADDR_ALT);
	{
		// TODO: Теперь есть новая функция rscs_adxl344_setup();
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
		printf("CO_calibrate_error"); // FIXME: Кто будет читать этот printf на стартовом столе?
	}
//============================================================================
//CONST
//============================================================================
	bmp280.calibration_values = rscs_bmp280_get_calibration_values (bmp280.descriptor);
	const time_data_t time_for_porsh = TIME_FOR_PORSH;
//============================================================================
//VARIABLE
//============================================================================

	buffer_for_sd_t buffer_for_sd;
	buffer_for_sd.number = 0;
	buffer_for_sd.busy_bytes = 0;
	rscs_e error;
	state_t state_now = STATE_IN_FIRST_MEASURE;

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

	//SD
	rscs_sdcard_t * sd = rscs_sd_init(&SD_DDR,&SD_PORT,SD_PIN);
	rscs_sd_set_timeout(sd,4000);
	rscs_sd_spi_setup_slow();
	rscs_e error1;
	while (1){
		error1 = rscs_sd_startup(sd);
		if (error1 == RSCS_E_NONE){
			break;
		}
	}
	rscs_sd_spi_setup();
//============================================================================
//TEST
//============================================================================

	//const uint32_t pressure_at_start = count_average_pressure(&main_packet,&bmp280);
	//printf("pressure at start = %ld\n",pressure_at_start);

	//float height = 0;
	while(1){
		main_packet.state = status_now;
		LED_BLINK(600);

		//DS18b20
		if ((rscs_ds18b20_check_ready())){
			if ((status_now &= (1 << STATUS_DS18B20)) == 0){
				rscs_ds18b20_read_temperature(ds18b20_1,&main_packet.DS18B20_temperature);
			}
			ERROR_CHECK(rscs_ds18b20_start_conversion(ds18b20_1),STATUS_DS18B20);
		}

		//bmp180
		bmp180_count_all(&main_packet.BMP180_pressure,&main_packet.BMP180_temperature);
		//bmp280
		rscs_bmp280_read(bmp280.descriptor,&bmp280.raw_press,&bmp280.raw_temp);
		rscs_bmp280_calculate(bmp280.calibration_values,bmp280.raw_press,bmp280.raw_temp,&main_packet.BMP280_pressure,&main_packet.BMP280_temperature);
		cli();
		//heigt
	//	count_height(&height,&main_packet,&bmp280,pressure_at_start);
		//adxl345
		rscs_adxl345_read(adxl345,&main_packet.ADXL345_x,&main_packet.ADXL345_y,&main_packet.ADXL345_z);
		rscs_adxl345_cast_to_G(adxl345,main_packet.ADXL345_x,main_packet.ADXL345_y,main_packet.ADXL345_z,&x_g,&y_g,&z_g);
		//MQ7
		if (mq7_read_co(&main_packet.CO, RO) == RSCS_E_NONE){
			status_now &= ~(1 << STATUS_CO);
		}else{
			status_now |= (1 << STATUS_CO);
		}
		//HC_SR04
		 HC_SR04_read(&packet_extra.HC_SR04);

		/*//printf("========================================== \n");
		printf("========================================== \n");
		//printf("bmp180 - t = %f C\n",main_packet.BMP180_temperature/10.0);
		printf("ds18b20 - t = %f C\n",main_packet.DS18B20_temperature/16.0);
		//printf("bmp280 - t = %f\n",(main_packet.BMP280_temperature/100.0) * 0.85);
		//printf("------------------------------------------ \n");
		//printf("bmp180 - p = %lu P\n",main_packet.BMP180_pressure);
		//printf("bmp280 - p = %li\n",main_packet.BMP280_pressure);
		printf("------------------------------------------ \n");
		//printf("adxl345 =  %f\n",x_g + 0.052);
		//printf("adxl345 =  %f\n",y_g + 0.04);
		//printf("adxl345 =  %f\n",z_g - 0.23);
		//printf("------------------------------------------ \n");
		printf("CO = %f\n",main_packet.CO);
		printf("------------------------------------------ \n");
		//printf("height = %f\n",height);
		printf("height_hc = %f\n",packet_extra.HC_SR04 / 100.0);*/

		 update_packet_extra(&packet_extra,sizeof(packet_extra));
		 update_packet(&main_packet,sizeof(main_packet));
		 send_packet_uart (uart_1,&main_packet,sizeof(main_packet));
		 send_packet_sd (sd,&buffer_for_sd,&main_packet,sizeof(main_packet));
	}
}
