/*
 * mcp41010.c
 *
 *  Created on: 12/06/2019
 *      Author: evandro
 */
#include "mcp41010.h"

/**
 *
 */
mcp41010_t *mcp41010_new_object(void)
{
	mcp41010_t* mcp41010;
	mcp41010 = malloc(sizeof(mcp41010_t));
	return mcp41010;
}

/**
 *
 */
void mcp41010_del(mcp41010_t *objt)
{
	free(objt);
}

/**
 *
 */
void mcp41010_add_attributes(mcp41010_t *objt,spi_config_t config_spi,GPIO_MemMapPtr port,uint32_t pin)
{
	//objt->mcp41010.config.spi = config_spi.spi;
	objt->mcp41010.config.alt = config_spi.alt;
	objt->mcp41010.config.div = config_spi.div;
	objt->mcp41010.config.pre = config_spi.pre;
	objt->mcp41010.config.spi = config_spi.spi;
	objt->mcp41010.io_cs.dir = output;
	objt->mcp41010.io_cs.pin = pin;
	objt->mcp41010.io_cs.port = port;

	gpio_init((gpio_t*)&objt->mcp41010.io_cs);
	gpio_write((gpio_t*)&objt->mcp41010.io_cs,high);
}

/**
 *
 */
void mcp41010_init(mcp41010_t *objt)
{
	spi_init((spi_t*)&objt->mcp41010.config);
	//gpio_init((gpio_t*)&objt->mcp41010.io_cs);
	//gpio_write((gpio_t*)&objt->mcp41010.io_cs,high);
}

/**
 *
 */
void mcp41010_write(mcp41010_t *objt, uint8_t command, uint8_t value)
{
	uint8_t buffer[2] = {0};
	buffer[0] = command;
	buffer[1] = value;
	spi_write((spi_t*)&objt->mcp41010,buffer,2);
}
