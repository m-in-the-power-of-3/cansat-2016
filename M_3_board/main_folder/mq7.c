#include <rscs/error.h>
#include "mq7.h"
#include <avr/io.h>
#include <rscs/adc.h>
#include <stdbool.h>
#include <util/delay.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define MQ7_DDR DDRA
#define MQ7_PORT PORTA
#define MQ7_PIN 0

// канал ацп на котором сидит датчик
#define ADC_CHANNEL RSCS_ADC_SINGLE_0

// номинал резистора установленный на плату в КОм
#define nominal_resis 10

// коеффициент чистого воздуха
#define getRoInCleanAir 27

#define MQ_SAMPLE_TIMES 5
#define MQ_SAMPLE_INTERVAL 20

float calculateResistance(int32_t rawAdc);
int32_t analogRead();

// калибровка и инициализация
float calibrate(){
	// инициализация цифрового порта
	MQ7_DDR |= (1 << MQ7_PIN);
	// инициализация аналогово порта
	float ro = 0;
	for (int i = 0; i < MQ_SAMPLE_TIMES; i++) {
		ro += calculateResistance(analogRead());
		_delay_ms(MQ_SAMPLE_INTERVAL);
	}
	ro = ro/MQ_SAMPLE_TIMES;
	ro = ro/getRoInCleanAir;
	return ro;
}

// чтение с цифрового пина
bool mq7_digital_read(){
	if(MQ7_PORT & (1 << MQ7_PIN))
		return true;
	else
		return false;
}

// узнаем-ка значение СО

int32_t analogRead(){
	int32_t result = 0;
	if(rscs_adc_start_single_conversion(ADC_CHANNEL) == RSCS_E_BUSY)
		printf("ERROR_0\n");
	rscs_adc_wait_result();
	if( rscs_adc_get_result(&result) == RSCS_E_BUSY){
		printf("ERROR_1\n");
		_delay_ms(MQ_SAMPLE_INTERVAL);
	}
	return result;
}

float calculateResistance(int32_t rawAdc){
	float vrl = rawAdc*(5.0 / 1023);
	float rsAir = (5.0 - vrl)/vrl* nominal_resis;
	return rsAir;
}

float readRs(float RO){
	float rs = 0.0;
	for (int i = 0; i < MQ_SAMPLE_TIMES; i++) {
		rs += calculateResistance(analogRead());
		_delay_ms(MQ_SAMPLE_INTERVAL);
	}
	rs = rs/MQ_SAMPLE_TIMES;
	return rs / RO;
}

float readCO(float RO){
	float a = -0.77, b =  3.38;
	float ratio = readRs(RO);
	return exp((log(ratio)-b)/a);
}

