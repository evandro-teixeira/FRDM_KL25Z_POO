/*
 * gpio.c
 *
 *  Created on: 09/06/2019
 *      Author: evandro
 */
#include "gpio.h"

/**
 *
 */
gpio_t *gpio_new_object(void)
{
	gpio_t* io;
	io = malloc(sizeof(gpio_t));
	return io;
}

/**
 *
 */
void gpio_destroys_object(gpio_t *objt)
{
	free(objt);
}

/**
 *
 */
void gpio_add_attributes(gpio_t *objt,GPIO_MemMapPtr port,gpio_dir_t dir,uint32_t pin)
{
	objt->port = port;
	objt->dir = dir;
	objt->pin = pin;
}

/**
 *
 */
void gpio_init(gpio_t *objt)
{
	if(objt->port == GPIOA)
	{
		SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
		PORT_PCR_REG(PORTA_BASE_PTR,objt->pin) = PORT_PCR_MUX(1);
	}
	else if(objt->port == GPIOB)
	{
		SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
		PORT_PCR_REG(PORTB_BASE_PTR,objt->pin) = PORT_PCR_MUX(1);
	}
	else if(objt->port == GPIOC)
	{
		SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
		PORT_PCR_REG(PORTC_BASE_PTR,objt->pin) = PORT_PCR_MUX(1);
	}
	else if(objt->port == GPIOD)
	{
		SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
		PORT_PCR_REG(PORTD_BASE_PTR,objt->pin) = PORT_PCR_MUX(1);
	}
	else if(objt->port == GPIOE)
	{
		SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
		PORT_PCR_REG(PORTE_BASE_PTR,objt->pin) = PORT_PCR_MUX(1);
	}
	else
	{
		return;
	}

	if(objt->dir == input)
	{
		objt->port->PDDR &=~ (1 << objt->pin);
	}
	else if(objt->dir == output)
	{
		objt->port->PDDR |= (1 << objt->pin);
	}
	else
	{
		return;
	}
}

/**
 *
 */
void gpio_toggle(gpio_t *objt)
{
	objt->port->PTOR = (1 << objt->pin);
}

/**
 *
 */
void gpio_write(gpio_t *objt,gpio_value_t value)
{
	if(value == high)
	{
		objt->port->PSOR |= (1 << objt->pin);
	}
	else
	{
		objt->port->PDOR &=~ (1 << objt->pin);
	}
}
