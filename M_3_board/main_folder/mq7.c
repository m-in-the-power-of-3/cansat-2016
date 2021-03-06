#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>

#include "rscs/adc.h"
#include "rscs/error.h"

#include "hal/config.h"

float calculateResistance(int32_t rawAdc);
rscs_e analogRead(int32_t * result);

// калибровка и инициализация
rscs_e mq7_calibrate(float * ro){
	rscs_e error = RSCS_E_NONE;
	// инициализация цифрового порта
	//MQ7_DDR |= (1 << MQ7_PIN);
	// инициализация аналогово порта

	*ro = 0;
	int32_t raw_adc;
	for (int i = 0; i < MQ_SAMPLE_TIMES; i++) {
		raw_adc = 0;
		GO_TO_END_IF_ERROR(analogRead(&raw_adc))
		*ro += calculateResistance(raw_adc);
		_delay_ms(MQ_SAMPLE_INTERVAL);
	}
	*ro = *ro/MQ_SAMPLE_TIMES;
	*ro = *ro/GET_RO_IN_CLEAN_AIR;
	end:
	return error;
}

// чтение с цифрового пина
bool mq7_digital_read(){
	if(MQ7_PORT & (1 << MQ7_PIN))
		return true;
	else
		return false;
}

// узнаем-ка значение СО
rscs_e analogRead(int32_t * result){
	rscs_e error = RSCS_E_NONE;
	GO_TO_END_IF_ERROR(rscs_adc_start_single_conversion(ADC_CHANNEL))

	rscs_adc_wait_result();
	GO_TO_END_IF_ERROR(rscs_adc_get_result(result))
	end:
	rscs_adc_wait_result();
	return error;
}

float calculateResistance(int32_t rawAdc){
	float vrl = rawAdc*(5.0 / 1023);
	float rsAir = (5.0 - vrl)/vrl* NOMINAL_RESIS;
	return rsAir;
}

rscs_e readRs(float * rs, float RO){
	rscs_e error = RSCS_E_NONE;
	*rs = 0.0;
	int32_t raw_adc;
	for (int i = 0; i < MQ_SAMPLE_TIMES; i++) {
		raw_adc = 0;
		GO_TO_END_IF_ERROR(analogRead(&raw_adc))
		*rs += calculateResistance(raw_adc);
		_delay_ms(MQ_SAMPLE_INTERVAL);
	}
	*rs = *rs / MQ_SAMPLE_TIMES;
	*rs = *rs / RO;
	end:
	return error;
}

rscs_e mq7_read_co(float * CO, float RO){
	rscs_e error = RSCS_E_NONE;
	float a = -0.77, b =  3.38;
	float ratio;
	GO_TO_END_IF_ERROR(readRs(&ratio, RO))
	*CO = exp((log(ratio)-b)/a);
	end:
	return error;
}
