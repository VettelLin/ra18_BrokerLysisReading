/*
 * l_math.c
 *
 *  Created on: Jul 22, 2022
 *      Author: abel
 */


#include "l_math.h"


void plane_coordinate_xy2polar(polar_coordinate_st *polar, double x_axis, double y_axis)
{
	polar->radian = atan2(y_axis, x_axis);
	polar->radius = sqrt((x_axis * x_axis) + (y_axis * y_axis));
}

void plane_coordinate_rad2rect(plane_coordinate_st *rect, double radius, double radian)
{
	rect->x_axis = radius * cos(radian);
	rect->y_axis = radius * sin(radian);
}



void plane_coordinate_rect2polar(polar_coordinate_st *polar, plane_coordinate_st *rect)
{
	plane_coordinate_xy2polar(polar, rect->x_axis, rect->y_axis);
}

void plane_coordinate_polar2rect(plane_coordinate_st *rect, polar_coordinate_st *polar)
{
	plane_coordinate_rad2rect(rect, polar->radius, polar->radian);
}

void space_coordinate_rect2spher(spherical_coordinate_st *spher, space_coordinate_st *rect)
{
	spher->radius = sqrt((rect->x_axis * rect->x_axis) + (rect->y_axis * rect->y_axis) + (rect->z_axis * rect->z_axis));
	spher->latitude = atan2(rect->y_axis, rect->x_axis);
	spher->longitude = atan2(sqrt((rect->x_axis * rect->x_axis) + (rect->y_axis * rect->y_axis)), rect->z_axis);
}

void space_coordinate_spher2rect(space_coordinate_st *rect, spherical_coordinate_st *spher)
{
	rect->x_axis = spher->radius * sin(spher->longitude) * cos(spher->latitude);
	rect->y_axis = spher->radius * sin(spher->longitude) * sin(spher->latitude);
	rect->z_axis = spher->radius * sin(spher->longitude);
}
