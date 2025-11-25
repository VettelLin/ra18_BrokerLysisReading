/*
 * ad_filter.h
 *
 *  Created on: Jun 4, 2022
 *      Author: abel
 */

#ifndef ALGORITHM_FILTER_U_FILTER_H_
#define ALGORITHM_FILTER_U_FILTER_H_

#include <stdint.h>

typedef struct __digital_filter_st digital_filter_st;


struct __digital_filter_st{
	uint32_t (*handle)(digital_filter_st *filter, uint16_t* pData, int Length, void *priv);
	uint32_t value;

};

#endif /* ALGORITHM_FILTER_U_FILTER_H_ */
