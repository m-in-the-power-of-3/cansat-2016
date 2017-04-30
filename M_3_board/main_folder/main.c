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

int main (){
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

  //UART 0
	rscs_uart_bus_t * uart_0 = rscs_uart_init(RSCS_UART_ID_UART0, RSCS_UART_FLAG_ENABLE_TX);
	rscs_uart_set_baudrate(uart_0, 9600);
	rscs_uart_set_character_size(uart_0, 8);
	rscs_uart_set_parity(uart_0, RSCS_UART_PARITY_NONE);
	rscs_uart_set_stop_bits(uart_0, RSCS_UART_STOP_BITS_ONE);

  //PRINTF
	FILE * f = rscs_make_uart_stream(uart_1);
	stdout = f;

  //TWI
	rscs_i2c_init();
	rscs_i2c_set_scl_rate(200);

  //SPI
	rscs_spi_init();
	rscs_spi_set_clk(100);

  //TIME
	time_service_init();

  //OTHER
	LED_INIT
	motor_init();
	sensor_init();

  //DS18B20
	rscs_ds18b20_t * ds18b20_1 = rscs_ds18b20_init(0);

  //BMP180
	bmp180_init();

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
	rscs_adxl345_set_range(adxl345,RSCS_ADXL345_RANGE_2G);
	rscs_adxl345_set_rate(adxl345,RSCS_ADXL345_RATE_200HZ);
  //MQ7
	float R0 = calibrate();
	//============================================================================
	//CONST
	//============================================================================
	bmp280.calibration_values = rscs_bmp280_get_calibration_values (bmp280.descriptor);
	//============================================================================
	//VARIABLE
	//============================================================================
	float x_g = 0;
	float y_g = 0;
	float z_g = 0;
	float CO;
	bmp280.raw_press = 0;
	bmp280.raw_temp = 0;
	uint16_t state_now = 0;
	packet_t main_packet = {0,0,0,0,0,0,0,0,0,0};

	rscs_ds18b20_start_conversion(ds18b20_1);

	//============================================================================
	//TEST
	//============================================================================

	const uint32_t pressure_at_start = count_average_pressure(&main_packet,&bmp280);
	printf("pressure at start = %ld\n",pressure_at_start);

	float height = 0;
	while(1){
		main_packet.state = state_now;
		LED_BLINK(400);
		//DS18b20
		if (rscs_ds18b20_check_ready()){
			rscs_ds18b20_read_temperature(ds18b20_1,&main_packet.DS18B20_temperature);
			rscs_ds18b20_start_conversion(ds18b20_1);
		}
		//bmp180
		bmp180_count_all(&main_packet.BMP180_pressure,&main_packet.BMP180_temperature);
		//bmp280
		rscs_bmp280_read(bmp280.descriptor,&bmp280.raw_press,&bmp280.raw_temp);
		rscs_bmp280_calculate(bmp280.calibration_values,bmp280.raw_press,bmp280.raw_temp,&main_packet.BMP280_pressure,&main_packet.BMP280_temperature);
		cli();
		//heigt
		count_height(&height,&main_packet,&bmp280,pressure_at_start);
		//adxl345
		rscs_adxl345_read(adxl345,&main_packet.adxl345_x,&main_packet.adxl345_y,&main_packet.adxl345_z);
		rscs_adxl345_cast_to_G(adxl345,main_packet.adxl345_x,main_packet.adxl345_y,main_packet.adxl345_z,&x_g,&y_g,&z_g);
		//MQ7
		CO = readCO(R0);
		printf("========================================== \n");
		printf("bmp180 - t = %f C\n",main_packet.BMP180_temperature/10.0);
		printf("ds18b20 - t = %f C\n",main_packet.DS18B20_temperature/16.0);
		printf("bmp280 - t = %f\n",main_packet.BMP280_temperature/100.0);
		printf("------------------------------------------ \n");
		printf("bmp180 - p = %lu P\n",main_packet.BMP180_pressure);
		printf("bmp280 - p = %li\n",main_packet.BMP280_pressure);
		printf("------------------------------------------ \n");
		printf("adxl345 =  %f\n",x_g);
		printf("adxl345 =  %f\n",y_g);
		printf("adxl345 =  %f\n",z_g);
		printf("------------------------------------------ \n");
		printf("CO = %f\n",CO);
		printf("------------------------------------------ \n");
		printf("height = %f\n",height);
		printf("height_hc = %u\n",HC_SR04_read());
		printf("========================================== \n");
		send_packet (uart_1,&main_packet,sizeof(main_packet));
	}
}
