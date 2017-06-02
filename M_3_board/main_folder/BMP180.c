/*
 * BMP180.c
 *
 *  Created on: 22 мая 2016 г.
 *      Author: developer
 */
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include <rscs/error.h>
#include <rscs/i2c.h>

#include "hal/structs.h"
#include "hal/config.h"

bmp180_calibration_values_t bmp180_calibration_values;

rscs_e bmp180_read_calibration_value (uint8_t adress, void * calibration_value){
	rscs_e error;
	uint8_t * value_ptr = (uint8_t*)calibration_value;
	GO_TO_END_IF_ERROR(rscs_i2c_start())
	GO_TO_END_IF_ERROR(rscs_i2c_send_slaw(BMP180_ADRESS,rscs_i2c_slaw_write))
	GO_TO_END_IF_ERROR(rscs_i2c_write_byte(adress))
	GO_TO_END_IF_ERROR(rscs_i2c_start())
	GO_TO_END_IF_ERROR(rscs_i2c_send_slaw(BMP180_ADRESS,rscs_i2c_slaw_read))
	GO_TO_END_IF_ERROR(rscs_i2c_read(&value_ptr[1],1,false))
	GO_TO_END_IF_ERROR(rscs_i2c_read(&value_ptr[0],1,true))
	end:
	rscs_i2c_stop();
	return error;
}

rscs_e bmp180_init(){
	rscs_e error; //uint8_t check;
	GO_TO_END_IF_ERROR(bmp180_read_calibration_value(BMP180_ADRESS_AC1,&bmp180_calibration_values.ac1))
	GO_TO_END_IF_ERROR(bmp180_read_calibration_value(BMP180_ADRESS_AC2,&bmp180_calibration_values.ac2))
	GO_TO_END_IF_ERROR(bmp180_read_calibration_value(BMP180_ADRESS_AC3,&bmp180_calibration_values.ac3))
	GO_TO_END_IF_ERROR(bmp180_read_calibration_value(BMP180_ADRESS_AC4,&bmp180_calibration_values.ac4))
	GO_TO_END_IF_ERROR(bmp180_read_calibration_value(BMP180_ADRESS_AC5,&bmp180_calibration_values.ac5))
	GO_TO_END_IF_ERROR(bmp180_read_calibration_value(BMP180_ADRESS_AC6,&bmp180_calibration_values.ac6))
	GO_TO_END_IF_ERROR(bmp180_read_calibration_value(BMP180_ADRESS_B1,&bmp180_calibration_values.b1))
	GO_TO_END_IF_ERROR(bmp180_read_calibration_value(BMP180_ADRESS_B2,&bmp180_calibration_values.b2))
	GO_TO_END_IF_ERROR(bmp180_read_calibration_value(BMP180_ADRESS_MB,&bmp180_calibration_values.mb))
	GO_TO_END_IF_ERROR(bmp180_read_calibration_value(BMP180_ADRESS_MC,&bmp180_calibration_values.mc))
	GO_TO_END_IF_ERROR(bmp180_read_calibration_value(BMP180_ADRESS_MD,&bmp180_calibration_values.md))
	end:
	return error;
}

rscs_e bmp180_read_pressure (uint32_t * raw_pressure){
	rscs_e error;
	uint8_t buffer[3];
	GO_TO_END_IF_ERROR(rscs_i2c_start())
	GO_TO_END_IF_ERROR(rscs_i2c_send_slaw(BMP180_ADRESS,rscs_i2c_slaw_write))
	GO_TO_END_IF_ERROR(rscs_i2c_write_byte(BMP180_MODE_ADRESS))
	GO_TO_END_IF_ERROR(rscs_i2c_write_byte(BMP180_PRESSURE_COD))
	rscs_i2c_stop();
	_delay_ms (BMP180_TIME_PRESSURE);
	GO_TO_END_IF_ERROR(rscs_i2c_start())
	GO_TO_END_IF_ERROR(rscs_i2c_send_slaw(BMP180_ADRESS,rscs_i2c_slaw_write))
	GO_TO_END_IF_ERROR(rscs_i2c_write_byte(BMP180_ADRESS_TO_READ))
	GO_TO_END_IF_ERROR(rscs_i2c_start())
	GO_TO_END_IF_ERROR(rscs_i2c_send_slaw(BMP180_ADRESS,rscs_i2c_slaw_read))
	GO_TO_END_IF_ERROR(rscs_i2c_read(&buffer,sizeof buffer,true))
	*raw_pressure = (uint32_t)buffer[0] << 16;
	*raw_pressure |= (uint16_t)buffer[1] << 8;
	*raw_pressure |= buffer[2];
	*raw_pressure = *raw_pressure >> (8-BMP180_OSS);
	end:
	rscs_i2c_stop();
	return error;
}

rscs_e bmp180_read_temperature (uint16_t * raw_temperature){
	rscs_e error;
	uint8_t *raw_ptr = (uint8_t*)raw_temperature;

	GO_TO_END_IF_ERROR(rscs_i2c_start())
	GO_TO_END_IF_ERROR(rscs_i2c_send_slaw(BMP180_ADRESS,rscs_i2c_slaw_write))
	GO_TO_END_IF_ERROR(rscs_i2c_write_byte(BMP180_MODE_ADRESS))
	GO_TO_END_IF_ERROR(rscs_i2c_write_byte(BMP180_TEMPERATURE_COD))
	rscs_i2c_stop();
	_delay_ms (BMP180_TIME_TEMPERATURE);
	GO_TO_END_IF_ERROR(rscs_i2c_start())
	GO_TO_END_IF_ERROR(rscs_i2c_send_slaw(BMP180_ADRESS,rscs_i2c_slaw_write))
	GO_TO_END_IF_ERROR(rscs_i2c_write_byte(BMP180_ADRESS_TO_READ))
	GO_TO_END_IF_ERROR(rscs_i2c_start())
	GO_TO_END_IF_ERROR(rscs_i2c_send_slaw(BMP180_ADRESS,rscs_i2c_slaw_read))
	GO_TO_END_IF_ERROR(rscs_i2c_read(&raw_ptr[1],1, false));
	GO_TO_END_IF_ERROR(rscs_i2c_read(&raw_ptr[0],1,true));
	end:
	rscs_i2c_stop();
	return error;
}

int32_t bmp180_count_B5 (int32_t UT){
  int32_t X1 = (UT - (int32_t)bmp180_calibration_values.ac6) * ((int32_t)bmp180_calibration_values.ac5) >> 15;
  int32_t X2 = ((int32_t)bmp180_calibration_values.mc << 11) / (X1+(int32_t)bmp180_calibration_values.md);
  return X1 + X2;
}

int32_t bmp180_count_p (int32_t B5,uint32_t UP){
	int32_t B6 = B5 - 4000;
	int32_t X1 = ((int32_t)bmp180_calibration_values.b2 * ( (B6 * B6)>>12 )) >> 11;
	int32_t X2 = ((int32_t)bmp180_calibration_values.ac2 * B6) >> 11;
	int32_t X3 = X1 + X2;
	int32_t B3 = ((((int32_t)bmp180_calibration_values.ac1*4 + X3) << BMP180_OSS) + 2) / 4;
	X1 = ((int32_t)bmp180_calibration_values.ac3 * B6) >> 13;
	X2 = ((int32_t)bmp180_calibration_values.b1 * ((B6 * B6) >> 12)) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;
	uint32_t B4 = ((uint32_t)bmp180_calibration_values.ac4 * (uint32_t)(X3 + 32768))>>15;
	uint32_t B7 = ((uint32_t)UP - B3) * (uint32_t)(50000UL >> BMP180_OSS);
	int32_t p;
	if (B7 < 0x80000000) {
	p = (B7 * 2) / B4;
	} else {
	p = (B7 * 2) / B3;
	}
	X1 = (p >> 8) * (p >> 8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * p) >> 16;
	p = p + ((X1 + X2 + (int32_t)3791)>>4);
	return p;
}

rscs_e bmp180_count_temperature(int16_t * temperature){
	rscs_e error;
	uint16_t raw_temperature;
	GO_TO_END_IF_ERROR(bmp180_read_temperature(&raw_temperature));
	int32_t B5 = bmp180_count_B5(raw_temperature);
	*temperature = (B5 + 8)/16;
	end:
	return error;
}

rscs_e bmp180_count_pressure(uint32_t * pressure){
	rscs_e error;
	uint16_t UT;
	GO_TO_END_IF_ERROR(bmp180_read_temperature(&UT))
	uint32_t UP;
	GO_TO_END_IF_ERROR(bmp180_read_pressure(&UP))
	int32_t B5 = bmp180_count_B5(UT);
	*pressure = bmp180_count_p(B5,UP);
	end:
	return error;
}

rscs_e bmp180_count_all (uint32_t * pressure,int16_t * temperature){
	rscs_e error;
	uint16_t UT;
	GO_TO_END_IF_ERROR(bmp180_read_temperature(&UT))
	uint32_t UP;
	GO_TO_END_IF_ERROR(bmp180_read_pressure(&UP))
	int32_t B5 = bmp180_count_B5(UT);
	*temperature = (B5 + 8)/16;
	*pressure = bmp180_count_p(B5,UP);
	end:
	return error;
}

/* Only for test
void bmp180_test_calibration_values(){
	bmp180_calibration_values.ac1 = BMP180_TEST_AC1;
	bmp180_calibration_values.ac2 = BMP180_TEST_AC2;
	bmp180_calibration_values.ac3 = BMP180_TEST_AC3;
	bmp180_calibration_values.ac4 = BMP180_TEST_AC4;
	bmp180_calibration_values.ac5 = BMP180_TEST_AC5;
	bmp180_calibration_values.ac6 = BMP180_TEST_AC6;
	bmp180_calibration_values.b1 = BMP180_TEST_B1;
	bmp180_calibration_values.b2 = BMP180_TEST_B2;
	bmp180_calibration_values.mb = BMP180_TEST_MB;
	bmp180_calibration_values.mc = BMP180_TEST_MC;
	bmp180_calibration_values.md = BMP180_TEST_MD;
}*/
