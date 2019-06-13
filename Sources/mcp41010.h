/*
 * mcp41010.h
 *
 *  Created on: 12/06/2019
 *      Author: evandro
 */

#ifndef SOURCES_MCP41010_H_
#define SOURCES_MCP41010_H_

#include "spi.h"

typedef struct
{
	spi_t mcp41010;
}mcp41010_t;

mcp41010_t *mcp41010_new_object(void);

void mcp41010_del(mcp41010_t *objt);

void mcp41010_add_attributes(mcp41010_t *objt,spi_config_t config_spi,GPIO_MemMapPtr port,uint32_t pin);

void mcp41010_init(mcp41010_t *objt);

void mcp41010_write(mcp41010_t *objt, uint8_t command, uint8_t value);

#endif /* SOURCES_MCP41010_H_ */
