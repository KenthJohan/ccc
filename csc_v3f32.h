/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"
#include "csc_mf32.h"
#include "csc_m3f32.h"



static void v3f32_set_xyz (struct v3f32 * r, float x, float y, float z)
{
	r->x = x;
	r->y = y;
	r->z = z;
}


static void v3f32_cpy (struct v3f32 * r, struct v3f32 const * a)
{
	r->x = a->x;
	r->y = a->y;
	r->z = a->z;
}


static void v3f32_mul (struct v3f32 * r, struct v3f32 const * a, float b)
{
	r->x = a->x * b;
	r->y = a->y * b;
	r->z = a->z * b;
}


static void v3f32_sub (struct v3f32 * r, struct v3f32 const * a, struct v3f32 const * b)
{
	r->x = a->x - b->x;
	r->y = a->y - b->y;
	r->z = a->z - b->z;
}


static void v3f32_add (struct v3f32 * r, struct v3f32 const * a, struct v3f32 const * b)
{
	r->x = a->x + b->x;
	r->y = a->y + b->y;
	r->z = a->z + b->z;
}


static void v3f32_cross (struct v3f32 * r, struct v3f32 const * a, struct v3f32 const * b)
{
	r->x = a->y * b->z - a->z * b->y;
	r->y = a->z * b->x - a->x * b->z;
	r->z = a->x * b->y - a->y * b->x;
}


float v3f32_dot (struct v3f32 const * a, struct v3f32 const * b)
{
	float sum = 0;
	sum += a->x * b->x;
	sum += a->y * b->y;
	sum += a->z * b->z;
	return sum;
}


static void v3f32_crossacc (struct v3f32 * r, struct v3f32 const * a, struct v3f32 const * b)
{
	r->x += a->y * b->z - a->z * b->y;
	r->y += a->z * b->x - a->x * b->z;
	r->z += a->x * b->y - a->y * b->x;
}


static void v3f32_crossacc_scalar (struct v3f32 * r, float s, struct v3f32 const * a, struct v3f32 const * b)
{
	r->x += s * (a->y * b->z - a->z * b->y);
	r->y += s * (a->z * b->x - a->x * b->z);
	r->z += s * (a->x * b->y - a->y * b->x);
}


static void v3f32_sum (struct v3f32 * y, struct v3f32 x[], uint32_t x_stride, uint32_t x_count)
{
	for (uint32_t i = 0; i < x_count; ++i)
	{
		y->x += x->x;
		y->y += x->y;
		y->z += x->z;
		x += x_stride;
	}
}


static float v3f32_norm2 (struct v3f32 const * a)
{
	return v3f32_dot (a, a);
}

static float v3f32_norm (struct v3f32 const * a)
{
	return sqrtf (v3f32_norm2 (a));
}




static int v3f32_ray_sphere_intersect (struct v3f32 * p, struct v3f32 * d, struct v3f32 * sc, float sr, float *t, struct v3f32 * q)
{
	//Vector m = p - s.c;
	struct v3f32 m;
	v3f32_sub (&m, p, sc);
	//float b = Dot(m, d);
	float b = v3f32_dot (&m, d);
	//float c = Dot(m, m) - s.r * s.r;
	float c = v3f32_dot (&m, &m) - (sr * sr);

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
	v3f32_mul (q, d, *t);
	v3f32_add (q, q, p);

	return 1;
}


static void v3f32_m4_mul (struct v3f32 * y, struct m4f32 const * a, struct v3f32 * const b)
{
	y->x += (a->m11 * b->x) + (a->m21 * b->y) + (a->m31 * b->z);
	y->y += (a->m12 * b->x) + (a->m22 * b->y) + (a->m32 * b->z);
	y->z += (a->m13 * b->x) + (a->m23 * b->y) + (a->m33 * b->z);
}


static void v3f32_m3_mul (struct v3f32 * y, struct m3f32 const * a, struct v3f32 * const b)
{
	y->x += (a->m11 * b->x) + (a->m21 * b->y) + (a->m31 * b->z);
	y->y += (a->m12 * b->x) + (a->m22 * b->y) + (a->m32 * b->z);
	y->z += (a->m13 * b->x) + (a->m23 * b->y) + (a->m33 * b->z);
}

