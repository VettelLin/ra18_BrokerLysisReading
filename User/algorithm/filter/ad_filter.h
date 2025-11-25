/*
 * ad_filter.h
 *
 *  Created on: Jun 5, 2022
 *      Author: abel
 */

#ifndef ALGORITHM_FILTER_AD_FILTER_H_
#define ALGORITHM_FILTER_AD_FILTER_H_

#include <stdint.h>

typedef struct __ad_filter_st ad_filter_st;

struct __ad_filter_st{
	uint16_t (*handle)(ad_filter_st *filter, uint16_t* pData, int Length, void *priv);
	uint16_t value;		// final value;
	uint16_t shake_amp;
	uint16_t shake_times;

};

uint16_t default_ad_filter_handler(ad_filter_st *filter, uint16_t *vals, int qty, void *priv);

#endif /* ALGORITHM_FILTER_AD_FILTER_H_ */
