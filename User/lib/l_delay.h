/*
 * delay.h
 *
 *  Created on: Sep 8, 2021
 *      Author: abel
 */

#ifndef INC_L_DELAY_H_
#define INC_L_DELAY_H_


#include "l_timer.h"


//void rcc_delay(int ns);
//#define delay_ns(ns)		rcc_delay(ns)


extern void delay_us(uint32_t delay);
#define delay_ms(ms)		HAL_Delay(ms)

#endif /* INC_L_DELAY_H_ */
