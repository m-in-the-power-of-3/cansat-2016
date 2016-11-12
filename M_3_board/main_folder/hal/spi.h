/*
  spi.h

   Created on: 01 ieo. 2016 a.
       Author: developer

 	 Îïèñàíèå øèíû : http://www.gaw.ru/html.cgi/txt/interface/spi/index.htm
  	 Îïèñàíèå ŵåãèñòŵîâ äëÿ AVR:  http://www.gaw.ru/html.cgi/txt/doc/micros/avr/arh128/15_1.htm

 */

#ifndef SPI_H_
#define SPI_H_

// инициализация spi
void spi_init();

//читает из устройства data_size байт и пишет их в память по указателю *data_prt
void spi_read(int *data_prt, int data_size);

// пишет в устройство data_size байт
// из памяти по указателю *data_prt
void spi_write(const int *data_prt, int data_size);

#endif /* SPI_H_ */
