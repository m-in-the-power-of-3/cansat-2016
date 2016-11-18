#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"

#define SPI_PORT PORTB
#define SPI_DDR DDRB
#define SPI_SS_PIN 0

void spi_init(){
	// разрешать работу spi
	// spi íàñòŵîåí êàê ìàñòåŵ
	// рабата sclk аналогична работе scl в i2c
	// данные передаются (старшим) битом вперед
	// частота равна fosk/128
	SPCR |= (1 << SPE)|(1 << MSTR)|(1 << CPHA)|(1 << CPOL)|(1 << DORD)|(1 << SPR1)|(1 << SPR0);
	SPI_DDR |= (1 << SPI_SS_PIN); // SS íàñòŵîåííà êàê âûõîä
	SPI_PORT &= ~(1 << SPI_SS_PIN); // SS óñòàíîâëåííà â ëîã. 0 ò.å. MISO àêòèâíà
}

void spi_read(int *data_prt, int data_size){
	for(int i = 0; i < data_size; i++)
		data_prt[i] = SPDR;
	return;
}

void spi_write(const int *data_prt, int data_size){
	for(int i = 0; i < data_size; i++)
		SPDR = data_prt[i];
	return;
}

