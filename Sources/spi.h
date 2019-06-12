/*
 * spi.h
 *
 *  Created on: 11/06/2019
 *      Author: evandro
 */

#ifndef SOURCES_SPI_H_
#define SOURCES_SPI_H_

#include "gpio.h"

#define PRESCALE_0	1
#define PRESCALE_1	2
#define PRESCALE_2	3
#define PRESCALE_3	4
#define PRESCALE_4	5
#define PRESCALE_5	6
#define PRESCALE_6	7
#define PRESCALE_7	8
#define DIVISOR_0	2
#define DIVISOR_1	4
#define DIVISOR_2	8
#define DIVISOR_3	16
#define DIVISOR_4	32
#define DIVISOR_5	64
#define DIVISOR_6	128
#define DIVISOR_7	256
#define DIVISOR_8	512

typedef enum
{
	Alt_0 = 0,
	Alt_1
}spi_alt_pint_t;

typedef struct
{
	SPI_MemMapPtr spi;
	spi_alt_pint_t alt;
	uint8_t pre;
	uint16_t div;
}spi_config_t;

typedef struct
{
	spi_config_t config;
	//SPI_MemMapPtr spi;
	//GPIO_MemMapPtr  port;
	//uint32_t pin;
	gpio_t io_cs;
}spi_t;

spi_t *spi_new_object(void);

void spi_destroys_object(spi_t *objt);

void spi_add_attributes(spi_t *objt,/*SPI_MemMapPtr spi,*/ spi_config_t config_spi,GPIO_MemMapPtr port,uint32_t pin );

void spi_init(spi_t *objt);

//void spi_write(spi_t *objt,uint8_t *buffer);
void spi_write(spi_t *objt,uint8_t *buffer, uint8_t number_byte);

uint8_t spi_read(spi_t *objt);


#endif /* SOURCES_SPI_H_ */
