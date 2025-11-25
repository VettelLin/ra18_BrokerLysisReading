/*
 * ad_filter.c
 *
 *  Created on: Jun 5, 2022
 *      Author: abel
 */


#include <algorithm/filter/ad_filter.h>



uint16_t default_ad_filter_handler(ad_filter_st *filter, uint16_t *vals, int qty, void *priv)
{
	int i;
	uint8_t v_count;	// normal value count;
	uint8_t sk_count;	// shacked value count;
	uint32_t v_sum;		// normal values sum
	uint32_t sk_sum;	// shacked values sum


	v_count = 0;
	sk_count = 0;
	v_sum = 0, sk_sum = 0;
	sk_sum = 0, sk_sum = 0;


	for (i = 0; i < qty; i++) {

		if(filter->value){
		   if(abs(vals[i] - filter->value) < filter->shake_amp){
			   v_count++;
			   v_sum += vals[i];
			   if(sk_count){
				   sk_count = 0;
			   }
		   }else{
			   sk_count++;
			   sk_sum += vals[i];
		   }
	   }else{
		   v_count++;
		   v_sum += vals[i];
		   filter->value = vals[i];
	   }

	}
	if(v_count > qty * 2 / 4){
	   filter->value = v_sum / v_count;
	}
	else
	if(sk_count >= filter->shake_times){
	   filter->value = sk_sum / sk_count;
	}


	return filter->value;
}
