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

double calculateResistance(int rawAdc);

// калибровка и инициализация
double calibrate(){
	// инициализация цифрового порта
	MQ7_DDR |= (1 << MQ7_PIN);
	// инициализация аналогово порта
	double ro = 0;
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

double calculateResistance(int rawAdc){
	double vrl = rawAdc*(5.0 / 1023);
	double rsAir = (5.0 - vrl)/vrl* nominal_resis;
	return rsAir;
}

double readRs(double RO){
	double rs = 0.0;
	for (int i = 0; i < MQ_SAMPLE_TIMES; i++) {
		rs += calculateResistance(analogRead());
		_delay_ms(MQ_SAMPLE_INTERVAL);
	}
	rs = rs/MQ_SAMPLE_TIMES;
	return rs / RO;
}

double readCO(double RO){
	float a = -0.77, b =  3.38;
	float ratio = readRs(RO);
	return exp((log(ratio)-b)/a);
}

