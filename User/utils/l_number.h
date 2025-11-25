/*
 * mumber.h
 *
 *  Created on: Jun 4, 2022
 *      Author: abel
 */

#ifndef UTILS_L_NUMBER_H_
#define UTILS_L_NUMBER_H_

#define F_ABS(num)	(((num) > 0) ? (num) : -(num))

static inline float f_abs(float num)
{
	return F_ABS(num);
}

static inline double lf_abs(double num)
{
	return F_ABS(num);
}

int pfloatcmp(float *pdata1, float *pdata2);
int pdoublecmp(double *pdata1, double *pdata2);

int floatcmp(float data1, float data2);
int doublecmp(double data1, double data2);

#endif /* UTILS_L_NUMBER_H_ */
