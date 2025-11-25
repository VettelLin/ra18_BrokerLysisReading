/*
 * u_gpio.c
 *
 *  Created on: Sep 4, 2021
 *      Author: abel
 */


#include "l_gpio.h"

#include "l_uart.h"


void gpio_set_mode(gpio_st *p_gpio, int mode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(mode == GPIO_OUTPUT){
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	}else{
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
	}
	GPIO_InitStruct.Pin = p_gpio->GPIO_Pin;
	HAL_GPIO_Init(p_gpio->GPIOx, &GPIO_InitStruct);
	p_gpio->mode = mode;
}

int gpio_get_status(gpio_st *p_gpio)
{
	int status;

	status = HAL_GPIO_ReadPin(p_gpio->GPIOx, p_gpio->GPIO_Pin);
	p_gpio->status = status;

	return status;
}

void gpio_set_status(gpio_st *p_gpio, int status)
{
	p_gpio->status = status;
	HAL_GPIO_WritePin(p_gpio->GPIOx, p_gpio->GPIO_Pin, status ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	console_printx(fln, "set %p gpio port %#.4x pin : %#x", p_gpio->GPIOx, p_gpio->GPIO_Pin, status);
}

void gpio_reverse(gpio_st *p_gpio)
{
	gpio_set_status(p_gpio, !p_gpio->status);
}

