/*
 * u_timer.c
 *
 *  Created on: Sep 3, 2021
 *      Author: abel
 */


#include "l_timer.h"

#include "l_uart.h"

#include "stm32f1xx_hal.h"

void timer_set_ic_polarity(TIM_HandleTypeDef *htim, int channel, int polarity)
{
	if(channel < 1 || channel > 4){
		return;
	}

	channel = (channel - 1) << 2;

	switch(polarity){
		case 0 :
			TIM_RESET_CAPTUREPOLARITY(htim, channel);
			break;
		case TIMER_IC_POLARITY_RISING :
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, channel, TIM_ICPOLARITY_RISING);
			break;
		case TIMER_IC_POLARITY_FALLING :
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, channel, TIM_ICPOLARITY_FALLING);
			break;
		case TIMER_IC_POLARITY_BOTHEDGE :
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, channel, TIM_ICPOLARITY_BOTHEDGE);
			break;
	}
}


__weak void timer_ic_user_handler(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  /* NOTE : This function should not be modified, when the handler is needed,
            the timer_ic_user_handler could be implemented in the user file
   */
}
__weak void timer_base_user_handler(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  /* NOTE : This function should not be modified, when the handler is needed,
            the timer_ic_user_handler could be implemented in the user file
   */
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	timer_ic_user_handler(htim);

}


uint64_t delay_timer_elapsed_count = 0;
uint32_t psc_coreClock = 0;

void start_delay_timer(void)
{
#if 1
	timer_start_base_it(&DELAY_HTIM);
#else
	timer_start_base(&DELAY_HTIM);
#endif
	delay_timer_elapsed_count = 0;
//	psc_coreClock = SystemCoreClock / (DELAY_TIMER->PSC + 1);
	psc_coreClock = HAL_RCC_GetPCLK1Freq() * 2/ (DELAY_TIMER->PSC + 1);
//	psc_coreClock = HAL_RCC_GetPCLK2Freq() * 2/ (DELAY_TIMER->PSC + 1);
}

void stop_delay_timer(void)
{
#if 1
	timer_stop_base_it(&DELAY_HTIM);
#else
	timer_stop_base(&DELAY_HTIM);
#endif
}


uint64_t get_delaytimer_timestamp(void)
{
	return (delay_timer_elapsed_count * (DELAY_TIMER->ARR + 1))  + DELAY_TIMER->CNT;
}

uint64_t get_delaytimer_timestamp_ns(void)
{
	return GET_DELAYTIMER_TIMESTAMP(NS_IN_SECOND);

}

uint32_t get_delaytimer_timestamp_ms(void)
{
	return GET_DELAYTIMER_TIMESTAMP(MS_IN_SECOND);
}


uint64_t get_delaytimer_timestamp_us(void)
{
	return GET_DELAYTIMER_TIMESTAMP(US_IN_SECOND);
}


int is_time_elapsed(uint64_t cur, uint64_t bgn , int take)
{

	if(cur < bgn){
		return (-1ULL  - bgn + 1  +  cur) >= take;
	}else{
		return (cur - bgn) >= take;
	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//

	if(htim->Instance==DELAY_TIMER){
		delay_timer_elapsed_count++;
//		console_printx(fln, "........... delay timer PeriodElapsedCallback : elapsed_count %u : tick %u.........", delay_timer_elapsed_count, HAL_GetTick());
	}
	else{
		timer_base_user_handler(htim);
	}


}

void timer_set_count(TIM_HandleTypeDef *htim, uint32_t cnt)
{
//	timer_stop_base(htim);
	htim->Instance->CNT = cnt;
//	timer_start_base(htim);
}

void timer_set_arr(TIM_HandleTypeDef *htim, uint32_t arr)
{
	htim->Init.Period = arr;
	htim->Instance->ARR = arr;
}

void timer_set_period(TIM_HandleTypeDef *htim, uint32_t period)
{
	timer_set_arr(htim, period - 1);
}

uint32_t timer_get_period(TIM_HandleTypeDef *htim)
{
	return htim->Init.Period + 1;
}


void timer_set_ccr(TIM_HandleTypeDef *htim, int channel, uint32_t cc)
{
	TIM_TypeDef *timer_regs = htim->Instance;
	uint32_t period = timer_get_period(htim);
	__IO uint32_t *p_CCR;

	if(channel < 1 || channel > 4){
		return;
	}

	if(cc > period){
		cc = period;
	}



//	console_printx(fln, "set  timer %d channel pwm :  cc %d +++", channel, cc);
#if 0
	p_CCR = &timer_regs->CCR1;
	p_CCR[channel] = cc;
#else
	switch(channel){
		case 1:
			timer_regs->CCR1 = cc;
			break;
		case 2:
			timer_regs->CCR2 = cc;
			break;
		case 3:
			timer_regs->CCR3 = cc;
			break;
		case 4:
			timer_regs->CCR4 = cc;
			break;
	}
#endif
}

void pwm_set_ccr(pwm_st *pwm, uint32_t cc)
{
	pwm->cap_cmp = cc;
	pwm_update_ccr(pwm);
}


void pwm_set_permill(pwm_st *pwm, uint32_t mill)
{
	uint32_t period = timer_get_period(pwm->htim);

	uint32_t cc = period * mill / 1000;

	timer_set_ccr(pwm->htim, pwm->channel, cc);
}

void pwm_set_percent(pwm_st *pwm, uint32_t cent)
{
	uint32_t period = timer_get_period(pwm->htim);

	uint32_t cc = period * cent / 100;

	timer_set_ccr(pwm->htim, pwm->channel, cc);
}
