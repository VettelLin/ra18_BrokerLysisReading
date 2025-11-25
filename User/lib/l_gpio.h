/*
 * u_gpio.h
 *
 *  Created on: Sep 4, 2021
 *      Author: abel
 */

#ifndef INC_L_GPIO_H_
#define INC_L_GPIO_H_

#include "gpio.h"

#define GPIO_INPUT	0
#define GPIO_OUTPUT	1

typedef struct{
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
	char mode;
	char status;
}gpio_st;

#define GPIO_PIN(pin)	(1 << pin)

void gpio_set_mode(gpio_st *p_gpio, int mode);
void gpio_set_status(gpio_st *p_gpio, int status);
int gpio_get_status(gpio_st *p_gpio);

#define gpio_read(GPIOx, GPIO_Pin)	HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)
#define gpio_write(GPIOx, GPIO_Pin, val)	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, val)
#define gpio_toggle(GPIOx, GPIO_Pin)	 HAL_GPIO_TogglePin(GPIOx, GPIO_Pin)

#define gpio_get(GPIOx, pin_idx)	gpio_read(GPIOx, GPIO_PIN(pin_idx))
#define gpio_set(GPIOx, pin_idx)	gpio_write(GPIOx, GPIO_PIN(pin_idx), GPIO_PIN_SET)
#define gpio_clr(GPIOx, pin_idx)	gpio_write(GPIOx, GPIO_PIN(pin_idx), GPIO_PIN_RESET)
#define gpio_clean(GPIOx, pin_idx)	gpio_write(GPIOx, GPIO_PIN(pin_idx), GPIO_PIN_RESET)

extern void gpio_reverse(gpio_st *p_gpio);


#endif /* INC_L_GPIO_H_ */
