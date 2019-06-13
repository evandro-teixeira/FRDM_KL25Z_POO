/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "MKL25Z4.h"
#include "gpio.h"
#include "spi.h"
#include "mcp41010.h"

/**
 *
 */
#define OFF 			(gpio_value_t)high
#define ON  			(gpio_value_t)low
#define POT_P1_PIN_CS	GPIOE,4
#define POT_P2_PIN_CS	GPIOE,5

/**
 *
 */
void led_rgb_blink(void);
void led_rgb_init(void);
void potentiometer_init(void);
void potentiometer_application(void);

/**
 *
 */
gpio_t *led_red;
gpio_t *led_green;
gpio_t *led_blue;
mcp41010_t *pot_p1;
mcp41010_t *pot_p2;

int main(void)
{
	/* Variaveis */
	uint32_t time = 0;

	led_rgb_init();
	potentiometer_init();

	for (;;)
	{
		led_rgb_blink();
		potentiometer_application();
		for(time=0;time<1000000;time++);
    }
    /* Never leave main */
    return 0;
}

/**
 *
 */
void potentiometer_init(void)
{
	spi_config_t config;

	config.alt = Alt_0;
	config.div = DIVISOR_1;
	config.pre = PRESCALE_2;
	config.spi = SPI0;

	pot_p1 = mcp41010_new_object();
	pot_p2 = mcp41010_new_object();

	mcp41010_add_attributes(pot_p1,config,POT_P1_PIN_CS);
	mcp41010_add_attributes(pot_p2,config,POT_P2_PIN_CS);

	/** Inicializa SPI */
	mcp41010_init(pot_p1);
	//mcp41010_init(pot_p2);
}

/**
 *
 */
void potentiometer_application(void)
{
	static uint8_t value_p1 = 0, value_p2 = 0;
	static uint8_t state = 0;

	switch(state)
	{
		case 0:
			// Envia comando de Escrita para o MCP41010
			// e conteudo da variavel value_p1
			mcp41010_write(pot_p1,0b00010001,value_p1++);
			if(value_p1 >= 255)
			{
				value_p1 = 0;
			}
			state = 1;
		break;
		case 1:
			if(value_p2 <= 0)
			{
				value_p2 = 255;
			}
			// Envia comando de Escrita para o MCP41010
			// e conteudo da variavel value_p1
			mcp41010_write(pot_p2,0b00010001,value_p2--);
			state = 0;
		break;
		default:
			state = 0;
		break;
	}
}

/**
 *
 */
void led_rgb_init(void)
{
	/* Estancia Objeto LED's */
	led_red = gpio_new_object();
	led_green = gpio_new_object();
	led_blue = gpio_new_object();

	/* Add os parametros do atributos dos Objtos LED's */
	gpio_add_attributes(led_red,GPIOB,output,18);
	gpio_add_attributes(led_green,GPIOB,output,19);
	gpio_add_attributes(led_blue,GPIOD,output,1);

	/* Inicializa Objetos LED's */
	gpio_init(led_red);
	gpio_init(led_green);
	gpio_init(led_blue);

	/* Apaga LED's */
	gpio_write(led_red,OFF);
	gpio_write(led_green,OFF);
	gpio_write(led_blue,OFF);
}

/**
 *
 */
void led_rgb_blink(void)
{
	static uint8_t state = 0;

	switch(state)
	{
		case 0:
			gpio_toggle(led_red);
			state = 1;
		break;
		case 1:
			gpio_toggle(led_red);
			state = 2;
		break;
		case 2:
			gpio_toggle(led_green);
			state = 3;
		break;
		case 3:
			gpio_toggle(led_green);
			state = 4;
		break;
		case 4:
			gpio_toggle(led_blue);
			state = 5;
		break;
		case 5:
			gpio_toggle(led_blue);
			state = 0;
		break;
		default:
			state = 0;
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
