/*
 * number.c
 *
 *  Created on: Jun 4, 2022
 *      Author: abel
 */



#include <utils/l_number.h>


#define	EP	1e-6

#define DECIMALS_COMPARE(pdata1, pdata2)	\
	if(((pdata1) > (pdata2) + EP)){	\
		return 1;	\
	}else if(((pdata1) < (pdata2) -EP)){	\
		return -1;	\
	}else{ 	\
		return 0;	\
	}


int pfloatcmp(float *pdata1, float *pdata2)
{
	DECIMALS_COMPARE(*pdata1, *pdata2);
}

int pdoublecmp(double *pdata1, double *pdata2)
{
	DECIMALS_COMPARE(*pdata1, *pdata2);
}

int floatcmp(float data1, float data2)
{
	DECIMALS_COMPARE(data1, data2);
}

int doublecmp(double data1, double data2)
{
	DECIMALS_COMPARE(data1, data2);
}

