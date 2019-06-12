/*
 * spi.c
 *
 *  Created on: 11/06/2019
 *      Author: evandro
 */

#include "spi.h"

/**
 *
 */
spi_t *spi_new_object(void)
{
	spi_t* spi;
	spi = malloc(sizeof(spi_t));
	return spi;
}

/**
 *
 */
void spi_destroys_object(spi_t *objt)
{
	free(objt);
}

/**
 *
 */
void spi_add_attributes(spi_t *objt,/*SPI_MemMapPtr spi,*/ spi_config_t config_spi,GPIO_MemMapPtr port,uint32_t pin )
{
	//objt->spi = spi;
	objt->config.spi = config_spi.spi;
	objt->config.div = config_spi.div;
	objt->config.pre = config_spi.pre;
	objt->config.alt = config_spi.alt;
	//objt->port = port;
	//objt->pin = pin;
	objt->io_cs.port = port;
	objt->io_cs.pin = pin;
	objt->io_cs.dir = output;
}

/**
 *
 */
void spi_init(spi_t *objt)
{
	//if(objt->spi == SPI0)
	if(objt->config.spi == SPI0 )
	{
		switch(objt->config.alt)
		{
			case Alt_0:
				SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;      //Turn on clock to C module
				PORTC_PCR5 = PORT_PCR_MUX(0x2);    		//Set PTC5 to mux 2 [SPI0_SCK]
				PORTC_PCR6 = PORT_PCR_MUX(0x2);    		//Set PTC6 to mux 2 [SPI0_MOSI]
				PORTC_PCR7 = PORT_PCR_MUX(0x2);    		//Set PTC7 to mux 2 [SPIO_MISO]
			break;
			case Alt_1:
				SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;      //Turn on clock to A module
				PORTA_PCR15 = PORT_PCR_MUX(0x2);    	//Set PTA15 to mux 2 [SPI0_SCK]
				PORTA_PCR16 = PORT_PCR_MUX(0x2);    	//Set PTA16 to mux 2 [SPI0_MOSI]
				PORTA_PCR17 = PORT_PCR_MUX(0x2);    	//Set PTA17 to mux 2 [SPIO_MISO]
			break;
			default:
				return;
			break;
		}
		//Enable SPI0 clock
		SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;
	}
	//else if(objt->spi == SPI1)
	else if(objt->config.spi == SPI1 )
	{
		switch(objt->config.alt)
		{
			case Alt_0:
				SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;      //Turn on clock to E module
				PORTE_PCR2 = PORT_PCR_MUX(0x2);    		//Set PTE2 to mux 2 [SPI1_SCK]
				PORTE_PCR1 = PORT_PCR_MUX(0x2);    		//Set PTE1 to mux 2 [SPI1_MOSI]
				PORTE_PCR3 = PORT_PCR_MUX(0x2);    		//Set PTE3 to mux 2 [SPI1_MISO]
			break;
			case Alt_1:
				SIM_SCGC5  |= SIM_SCGC5_PORTB_MASK;     //Turn on clock to B module
				PORTB_PCR11 = PORT_PCR_MUX(0x2);    	//Set PTB11 to mux 2 [SPI1_SCK]
				PORTB_PCR16 = PORT_PCR_MUX(0x2);    	//Set PTB16 to mux 2 [SPI1_MOSI]
				PORTB_PCR17 = PORT_PCR_MUX(0x2);    	//Set PTB17 to mux 2 [SPI1_MISO]
			break;
			default:
				return;
			break;
		}
		//Enable SPI1 clock
		SIM_SCGC4 |= SIM_SCGC4_SPI1_MASK;
	}
	else
	{
		return;
	}

	// Set SPI to Master
	objt->config.spi->C1 = SPI_C1_MSTR_MASK;
	//objt->spi->C1 = SPI_C1_MSTR_MASK;

	// Configure SPI Register C2
	objt->config.spi->C2 = SPI_C2_MODFEN_MASK;   //Master SS pin acts as slave select output

	// Set baud rate prescale divisor to 6 & set baud rate divisor to 4 for baud rate of 1 Mhz
	objt->config.spi->BR = (SPI_BR_SPPR(objt->config.pre) | SPI_BR_SPR(objt->config.div));    //  Mhz

	// Enable SPI
	objt->config.spi->C1 |= SPI_C1_SPE_MASK;

	// Inicializa GPIO CS
	gpio_init(&objt->io_cs);
	gpio_write(&objt->io_cs,high);
}

/**
 *
 */
void spi_write(spi_t *objt,uint8_t *buffer, uint8_t number_byte)
{
	gpio_write(&objt->io_cs,low);

	while(number_byte)
	{
		while(!(SPI_S_SPTEF_MASK & objt->config.spi->S))
		{
			__asm("nop");
		}
		objt->config.spi->D = *buffer;
		buffer++;
		number_byte--;
	}

	gpio_write(&objt->io_cs,high);
}

/**
 *
 */
uint8_t spi_read(spi_t *objt)
{
	gpio_write(&objt->io_cs,low);

	while(!(objt->config.spi->S & SPI_S_SPRF_MASK))
	{
		__asm("nop");
	}
	return objt->config.spi->D;

	gpio_write(&objt->io_cs,low);
}
