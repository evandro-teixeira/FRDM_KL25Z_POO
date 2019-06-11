/*
 * gpio.h
 *
 *  Created on: 09/06/2019
 *      Author: evandro
 */

#ifndef SOURCES_GPIO_H_
#define SOURCES_GPIO_H_

#include <stdlib.h>
#include "MKL25Z4.h"

typedef enum
{
	output = 0,
	input
}gpio_dir_t;

typedef enum
{
	low = 0,
	high,
}gpio_value_t;

typedef struct
{
	uint32_t pin;
	gpio_dir_t dir;
	GPIO_MemMapPtr  port;
}gpio_t;

gpio_t *gpio_new_object(void);

void gpio_destroys_object(gpio_t *objt);

void gpio_add_attributes(gpio_t *objt,GPIO_MemMapPtr port,gpio_dir_t dir,uint32_t pin);

void gpio_init(gpio_t *objt);

void gpio_toggle(gpio_t *objt);

void gpio_write(gpio_t *objt,gpio_value_t value);

#endif /* SOURCES_GPIO_H_ */
