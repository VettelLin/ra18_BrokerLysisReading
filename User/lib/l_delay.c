/*
 * delay.c
 *
 *  Created on: Sep 8, 2021
 *      Author: abel
 */

#include "stm32f1xx_hal.h"

#include "l_delay.h"

#if 1

void delay_us(uint32_t nTime)
{
	__IO uint32_t beg, cur, dif = 0;

	nTime *= SystemCoreClock / (1000 * 1000);

	#ifdef USE_FREERTOS
	vTaskSuspendAll();
	#endif
	beg = DWT->CYCCNT;
	while(dif < nTime){
		cur = DWT->CYCCNT;
		if(cur <= beg){
			dif = beg - cur;
		}else{
			dif = SystemCoreClock - dif + beg;
		}
	}
	#ifdef USE_FREERTOS
	xTaskResumeAll();
	#endif
}



//void delay_us(int delay)
//{
//	uint64_t tickstart = GET_TIMESTAMP_US();
//
//	while ((GET_TIMESTAMP_US()- tickstart) < delay)
//	{
//	}
//}


#else
void rcc_delay(int ns)
{
	uint64_t tickstart = get_timestamp_ns();

	while (!is_time_elapsed(get_timestamp_ns(), tickstart, ns))
	{
	  __NOP();
	}

}


void delay_us(int delay)
{
	uint64_t tickstart = get_timestamp_us();

	while (!is_time_elapsed(get_timestamp_us(), tickstart, delay))
	{
	  __NOP();
	}
}
#endif
