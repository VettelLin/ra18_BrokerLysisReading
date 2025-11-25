
/*
 * ad_filter.c
 *
 *  Created on: Jun 4, 2022
 *      Author: abel
 */

#include <algorithm/filter/u_filter.h>


uint32_t AmplitudeLimiterFilter(uint32_t last_val, uint32_t new_val, uint16_t amp)
{
   if(((new_val - last_val) > amp) || ((last_val - new_val) > amp))
	   return last_val;
   else
	   return new_val;
}

unsigned char MiddlevalueFilter(uint32_t *vals, int qty)

{
  unsigned char i,j,k,temp;

  for (j=0;j<qty-1;j++)
  {
   for (k=0;k<qty-j;k++)
   {
    if(vals[k]>vals[k+1])
     {
       temp = vals[k];
       vals[k] = vals[k+1];
       vals[k+1] = temp;
     }
   }
  }
  return vals[(qty-1)/2];
}
