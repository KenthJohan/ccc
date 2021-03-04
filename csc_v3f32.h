/* SPDX-License-Identifier: GPL-2.0 */
#pragma once

#include <stdint.h>
#include "csc_math.h"


static void v3f32_set_xyz (v3f32 r, float x, float y, float z)
{
	r[0] = x;
	r[1] = y;
	r[2] = z;
}


static void v3f32_cpy (v3f32 r, v3f32 const a)
{
	r[0] = a[0];
	r[1] = a[1];
	r[2] = a[2];
}


static void v3f32_mul (v3f32 r, v3f32 const a, float b)
{
	r[0] = a[0] * b;
	r[1] = a[1] * b;
	r[2] = a[2] * b;
}


static void v3f32_cross (v3f32 r, v3f32 const a, v3f32 const b)
{
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
}


float v3f32_dot (v3f32 const a, v3f32 const b)
{
	float sum = 0;
	sum += a[0] * b[0];
	sum += a[1] * b[1];
	sum += a[2] * b[2];
	return sum;
}


static void v3f32_crossacc (v3f32 r, v3f32 const a, v3f32 const b)
{
	r[0] += a[1] * b[2] - a[2] * b[1];
	r[1] += a[2] * b[0] - a[0] * b[2];
	r[2] += a[0] * b[1] - a[1] * b[0];
}


static void v3f32_crossacc_scalar (v3f32 r, float s, v3f32 const a, v3f32 const b)
{
	r[0] += s * (a[1] * b[2] - a[2] * b[1]);
	r[1] += s * (a[2] * b[0] - a[0] * b[2]);
	r[2] += s * (a[0] * b[1] - a[1] * b[0]);
}


static void v3f32_sum (v3f32 y, float x[], uint32_t x_stride, uint32_t x_count)
{
	for (uint32_t i = 0; i < x_count; ++i)
	{
		y[0] += x[0];
		y[1] += x[1];
		y[2] += x[2];
		x += x_stride;
	}
}





static int v3f32_ray_sphere_intersect (v3f32 p, v3f32 d, v3f32 sc, float sr, float *t, v3f32 q)
{
	//Vector m = p - s.c;
	float m [3];
	vvf32_sub (3, p, sc, m);
	//float b = Dot(m, d);
	float b = vvf32_dot (3, m, d);
	//float c = Dot(m, m) - s.r * s.r;
	float c = vvf32_dot (3, m, m) - (sr * sr);

	// Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
	if (c > 0.0f && b > 0.0f) {return 0;}
	float discr = (b * b) - c;

	// A negative discriminant corresponds to ray missing sphere
	if (discr < 0.0f) {return 0;}

	// Ray now found to intersect sphere, compute smallest t value of intersection
	*t = -b - sqrtf (discr);

	// If t is negative, ray started inside sphere so clamp t to zero
	if (*t < 0.0f) {*t = 0.0f;}
	*t = MAX (*t, 0.0f);

	//q = p + dt;
	vsf32_mul (3, q, d, *t);
	vvf32_add (3, q, p, q);

	return 1;
}







static void mv3f32_mul (v3f32 y, m3f32 const a, v3f32 const b)
{
	float r[3] = {0.0f};
	mvf32_macc (r, a, b, 3, 3);
	memcpy (y, r, sizeof (r));
}
