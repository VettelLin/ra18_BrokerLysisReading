/*
 * l_math.h
 *
 *  Created on: Jul 22, 2022
 *      Author: abel
 */

#ifndef UTILS_L_MATH_H_
#define UTILS_L_MATH_H_


#include <math.h>
#include <stdlib.h>
#include "utils/l_number.h"

typedef struct {
	double x_axis;
	double y_axis;
}plane_coordinate_st;

typedef struct {
	double x_axis;
	double y_axis;
	double z_axis;
}space_coordinate_st;

typedef struct {
	double radius;
	double radian;
}polar_coordinate_st;

typedef struct {
	double radius;
	double longitude;
	double latitude;
}spherical_coordinate_st;


typedef plane_coordinate_st plane_vector_st;
typedef space_coordinate_st space_vector_st;




#define angle2radian(angle)		((angle) * M_PI / 180.0)
#define radian2angle(radian)		((radian) * 180.0 / M_PI)
#define axes2radian(x_axis, y_axis)		atan2(y_axis, x_axis)
#define axes2angle(x_axis, y_axis)		radian2angle(axes2radian(x_axis, y_axis))


extern void plane_coordinate_rect2polar(polar_coordinate_st *polar, plane_coordinate_st *rect);
extern void plane_coordinate_polar2rect(plane_coordinate_st *rect, polar_coordinate_st *polar);

extern void plane_coordinate_xy2polar(polar_coordinate_st *polar, double x, double y);
extern void plane_coordinate_rad2rect(plane_coordinate_st *rect, double radius, double radian);

extern void space_coordinate_rect2spher(spherical_coordinate_st *spher, space_coordinate_st *rect);
extern void space_coordinate_spher2rect(space_coordinate_st *rect, spherical_coordinate_st *spher);


#endif /* UTILS_L_MATH_H_ */
