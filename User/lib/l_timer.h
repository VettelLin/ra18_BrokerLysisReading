/*
 * u_timer.h
 *
 *  Created on: Sep 3, 2021
 *      Author: abel
 */

#ifndef INC_L_TIMER_H_
#define INC_L_TIMER_H_

#include "tim.h"

#define DELAY_TIMER_IDX		5

#define __TIMER_REG_BASE(idx)		TIM##idx
#define __TIMER_HAL_OBJ(idx)		htim##idx

#define TIMER_REG_BASE(idx)		__TIMER_REG_BASE(idx)
#define TIMER_HAL_OBJ(idx)		__TIMER_HAL_OBJ(idx)

#define DELAY_TIMER		TIMER_REG_BASE(DELAY_TIMER_IDX)
#define DELAY_HTIM		TIMER_HAL_OBJ(DELAY_TIMER_IDX)



#define NS_IN_SECOND	1000000000ULL
#define US_IN_SECOND	1000000ULL
#define MS_IN_SECOND	1000ULL

//#define get_timestamp()	TIM2->CNT
extern uint32_t psc_coreClock;
extern uint64_t delay_timer_elapsed_count;



#define GET_DELAYTIMER_TICK()	(((delay_timer_elapsed_count * (DELAY_TIMER->ARR + 1ULL))  + DELAY_TIMER->CNT) * (DELAY_TIMER->PSC + 1))
//#define GET_DELAYTIMER_TIMESTAMP(unit)	(GET_DELAYTIMER_TICK() * (unit) / SystemCoreClock)
#define GET_DELAYTIMER_TIMESTAMP(unit)	(((delay_timer_elapsed_count * (DELAY_TIMER->ARR + 1ULL))  + DELAY_TIMER->CNT) * (unit) / psc_coreClock)


#define GET_DELAYTIMER_TIMESTAMP_US()		GET_DELAYTIMER_TIMESTAMP(US_IN_SECOND)
#define GET_DELAYTIMER_TIMESTAMP_MS()		GET_DELAYTIMER_TIMESTAMP(MS_IN_SECOND)

extern uint64_t get_delaytimer_timestamp_ns(void);
extern uint64_t get_delaytimer_timestamp_us(void);
extern uint32_t get_delaytimer_timestamp_ms(void);
int is_time_elapsed(uint64_t cur, uint64_t bgn , int take_ns);


//#define timer_set_count(htim, cnt)		(htim)->Instance->CNT = cnt
void timer_set_count(TIM_HandleTypeDef *htim, uint32_t cnt);
#define timer_get_count(htim)		(htim)->Instance->CNT

#define timer_start_base(htim)		HAL_TIM_Base_Start(htim)
#define timer_stop_base(htim)		HAL_TIM_Base_Stop(htim)

#define timer_start_base_it(htim)		HAL_TIM_Base_Start_IT(htim)
#define timer_stop_base_it(htim)		HAL_TIM_Base_Stop_IT(htim)

extern void start_delay_timer(void);
extern void stop_delay_timer(void);



extern void timer_set_arr(TIM_HandleTypeDef *htim, uint32_t arr);
extern void timer_set_period(TIM_HandleTypeDef *htim, uint32_t period);
extern uint32_t timer_get_period(TIM_HandleTypeDef *htim);


extern void timer_base_user_handler(TIM_HandleTypeDef *htim);


#define TIMER_IC_POLARITY_RISING	1
#define TIMER_IC_POLARITY_FALLING	2
#define TIMER_IC_POLARITY_BOTHEDGE	3

typedef struct timer_input_capture timer_ic_st;
struct timer_input_capture{
	TIM_HandleTypeDef *htim;
	uint8_t idx;	// 1 - 17
	uint8_t channel;	// 1 - 4
	uint8_t polarity;
	uint8_t edge;
//	uint32_t count;
	void *ptr_user;
	int (*cb_fun)(timer_ic_st *);
};



#define TIMER_IC(idx, chan, pol)	timer_ic_st timer##idx_ic##chan = {&htim##idx, idx, chan, pol, 0}


#define timer_start_ic(htim, channel)		HAL_TIM_IC_Start_IT(htim, ((channel) - 1) << 2)
#define timer_stop_ic(htim, channel)		HAL_TIM_IC_Stop_IT(htim, ((channel) - 1) << 2)

#define timer_read_ic(htim, channel)		HAL_TIM_ReadCapturedValue(htim, ((channel) - 1) << 2)


#define start_timeric(timer_ic)		timer_start_ic((timer_ic)->htim, (timer_ic)->channel)
#define stop_timeric(timer_ic)		timer_stop_ic((timer_ic)->htim, (timer_ic)->channel)

//#define timer_set_polarity(htim, channel, polarity)			__HAL_TIM_SET_CAPTUREPOLARITY(htim, ((channel) - 1) << 2, polarity);
//#define timer_reset_polarity(htim, channel)			TIM_RESET_CAPTUREPOLARITY(htim, ((channel) - 1) << 2);

extern void timer_set_ic_polarity(TIM_HandleTypeDef *htim, int channel, int polarity);
#define timer_ic_set_polarity(timer_ic, val)		do{(timer_ic)->polarity = val;timer_set_ic_polarity((timer_ic)->htim, (timer_ic)->channel, (timer_ic)->polarity);}while(0)

extern void timer_ic_user_handler(TIM_HandleTypeDef *htim);


#define PWM_DUTY_POSITIVE	0
#define PWM_DUTY_NEGATIVE	1


typedef struct{
	TIM_HandleTypeDef *htim;
	uint8_t channel;	// 1 - 4
	uint8_t polar;	// 0 positive, 1 negative
	uint16_t cap_cmp;	// Capture/Compare Register , range : 0 - AutoReload Register
}timer_oc_st;

typedef timer_oc_st pwm_st;


extern void timer_set_ccr(TIM_HandleTypeDef *htim, int idx, uint32_t cc);


#define timer_start_oc(htim, channel)		HAL_TIM_OC_Start(htim, ((channel) - 1) << 2)
#define timer_stop_oc(htim, channel)		HAL_TIM_OC_Stop(htim, ((channel) - 1) << 2)

#define timer_start_pwm(htim, channel)		HAL_TIM_PWM_Start(htim, ((channel) - 1) << 2)
#define timer_stop_pwm(htim, channel)		HAL_TIM_PWM_Stop(htim, ((channel) - 1) << 2)

#define timer_start_pwmn(htim, channel)		HAL_TIMEx_PWMN_Start(htim, ((channel) - 1) << 2)
#define timer_stop_pwmn(htim, channel)		HAL_TIMEx_PWMN_Stop(htim, ((channel) - 1) << 2)

#define pwm_start(pwm)		timer_start_pwm((pwm)->htim, (pwm)->channel)
#define pwm_stop(pwm)		timer_stop_pwm((pwm)->htim, (pwm)->channel)

#define pwmn_start(pwm)		timer_start_pwmn((pwm)->htim, (pwm)->channel)
#define pwmn_stop(pwm)		timer_stop_pwmn((pwm)->htim, (pwm)->channel)


#define pwm_get_period(pwm)		timer_get_period((pwm)->htim)

//#define pwm_set_ccr(pwm, cc)		timer_set_ccr((pwm)->htim, (pwm)->channel, cc)
extern void pwm_set_ccr(pwm_st *pwm, uint32_t cc);
#define pwm_update_ccr(pwm)		timer_set_ccr((pwm)->htim, (pwm)->channel, (pwm)->cap_cmp)
extern void pwm_set_percent(pwm_st *pwm, uint32_t cent);
extern void pwm_set_permill(pwm_st *pwm, uint32_t mill);




#endif /* INC_L_TIMER_H_ */
