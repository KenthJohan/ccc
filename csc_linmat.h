#pragma once

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "csc_debug.h"
#include "csc_basic.h"
#include "csc_math.h"



//https://keisan.casio.com/exec/system/1223596129
static void csc_linmat_plane_from_3points (float plane[4], float const a[3], float const b[3], float const c[3])
{
	float ab[3];
	float ac[3];
	vvf32_sub (3, ab, a, b);
	vvf32_sub (3, ac, a, c);
	v3f32_cross (plane, ab, ac);
	plane[3] = -vvf32_dot (3, plane, a);
}


//https://keisan.casio.com/exec/system/1223614315
//https://mathinsight.org/distance_point_plane
static float csc_linmat_plane_point_distance (float const plane[4], float const x[3])
{
	float px;
	float p2;
	float l;
	px = vvf32_dot (3, plane, x) + plane[4];
	p2 = vvf32_dot (3, plane, plane);
	ASSERT (p2 > 0.0f);
	l = fabs (px) / sqrtf (p2);
	ASSERT (l >= 0.0f);
	return l;
}


static float csc_linmat_plane_point_distance2 (float const plane[4], float const x[3])
{
	float px;
	float p2;
	float l;
	px = vvf32_dot (3, plane, x) + plane[4];
	p2 = vvf32_dot (3, plane, plane);
	ASSERT (p2 > 0.0f);
	l = fabs (px) / p2;
	ASSERT (l >= 0.0f);
	return l;
}


//http://www.easy-math.net/transforming-between-plane-forms/
static void csc_linmat_plane_to_parametric (float x[3], float const plane[4])
{
	float a = plane[0];
	float b = plane[1];
	float c = plane[2];
	float d = plane[3];
	float r = x[1];
	float s = x[2];
	ASSERT (a != 0.0f);
	x[0] = (d/a) - (r * (b/a)) - (s * (c/a));
}



static unsigned csc_linmat_plane_point_count
(
float const plane[4],
float const x[],//stride 4
unsigned n,
float lthreshold
)
{
	unsigned j = 0;
	for (unsigned i = 0; i < n; ++i)
	{
		float l = csc_linmat_plane_point_distance2 (plane, x);
		if (l < lthreshold)
		{
			j++;
		}
		x += 4;
	}
	return j;
}
