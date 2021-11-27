/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"
#include "csc_mf32.h"
#include "csc_m3f32.h"



static void v3f32_set1 (v3f32 * r, float b)
{
	r->x = b;
	r->y = b;
	r->z = b;
}


static void v3f32_set_xyz (v3f32 * r, float x, float y, float z)
{
	r->x = x;
	r->y = y;
	r->z = z;
}


static void v3f32_cpy (v3f32 * r, v3f32 const * a)
{
	r->x = a->x;
	r->y = a->y;
	r->z = a->z;
}


static void v3f32_mul (v3f32 * r, v3f32 const * a, float b)
{
	r->x = a->x * b;
	r->y = a->y * b;
	r->z = a->z * b;
}

static void v3f32_mul_hadamard (v3f32 * r, v3f32 const * a, v3f32 const * b)
{
	r->x = a->x * b->x;
	r->y = a->y * b->y;
	r->z = a->z * b->z;
}


static void v3f32_sub (v3f32 * r, v3f32 const * a, v3f32 const * b)
{
	r->x = a->x - b->x;
	r->y = a->y - b->y;
	r->z = a->z - b->z;
}

static void v3f32_subv (v3f32 r[], v3f32 const a[], v3f32 const b[], uint32_t incr, uint32_t inca, uint32_t incb, uint32_t n)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		v3f32_sub (r, a, b);
		r += incr;
		a += inca;
		b += incb;
	}
}

static void v3f32_add (v3f32 * r, v3f32 const * a, v3f32 const * b)
{
	r->x = a->x + b->x;
	r->y = a->y + b->y;
	r->z = a->z + b->z;
}

static void v3f32_addv (v3f32 r[], v3f32 const a[], v3f32 const b[], uint32_t incr, uint32_t inca, uint32_t incb, uint32_t n)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		v3f32_add (r, a, b);
		r += incr;
		a += inca;
		b += incb;
	}
}


static void v3f32_add_mul (v3f32 * r, v3f32 const * a, v3f32 const * b, float alpha, float beta)
{
	r->x = a->x * alpha + b->x * beta;
	r->y = a->y * alpha + b->y * beta;
	r->z = a->z * alpha + b->z * beta;
}


static void v3f32_cross (v3f32 * r, v3f32 const * a, v3f32 const * b)
{
	r->x = a->y * b->z - a->z * b->y;
	r->y = a->z * b->x - a->x * b->z;
	r->z = a->x * b->y - a->y * b->x;
}


static float v3f32_dot (v3f32 const * a, v3f32 const * b)
{
	float sum = 0;
	sum += a->x * b->x;
	sum += a->y * b->y;
	sum += a->z * b->z;
	return sum;
}


static float v3f32_dotv (float r[], v3f32 const a[], v3f32 const b[], int inca, int incb, int count)
{
	float sum = 0;
	for (int i = 0; i < count; ++i)
	{
		r[i] = (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
		a += inca;
		b += incb;
	}
	return sum;
}



static void v3f32_crossacc (v3f32 * r, v3f32 const * a, v3f32 const * b)
{
	r->x += a->y * b->z - a->z * b->y;
	r->y += a->z * b->x - a->x * b->z;
	r->z += a->x * b->y - a->y * b->x;
}


static void v3f32_crossacc_scalar (v3f32 * r, float s, v3f32 const * a, v3f32 const * b)
{
	r->x += s * (a->y * b->z - a->z * b->y);
	r->y += s * (a->z * b->x - a->x * b->z);
	r->z += s * (a->x * b->y - a->y * b->x);
}


static void v3f32_sum (v3f32 * y, v3f32 x[], uint32_t x_stride, uint32_t x_count)
{
	for (uint32_t i = 0; i < x_count; ++i)
	{
		y->x += x->x;
		y->y += x->y;
		y->z += x->z;
		x += x_stride;
	}
}


static float v3f32_norm2 (v3f32 const * a)
{
	return v3f32_dot (a, a);
}

static float v3f32_norm (v3f32 const * a)
{
	return sqrtf (v3f32_norm2 (a));
}

static void v3f32_normalize (v3f32 * a)
{
	float l = v3f32_norm (a);
	ASSERT (l != 0.0f);
	a->x /= l;
	a->y /= l;
	a->z /= l;
}




static int v3f32_ray_sphere_intersect (v3f32 * p, v3f32 * d, v3f32 * sc, float sr, float *t, v3f32 * q)
{
	//Vector m = p - s.c;
	v3f32 m;
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


static void v3f32_m4_mul (v3f32 * y, m4f32 const * a, v3f32 * const b)
{
	y->x += (a->m11 * b->x) + (a->m21 * b->y) + (a->m31 * b->z);
	y->y += (a->m12 * b->x) + (a->m22 * b->y) + (a->m32 * b->z);
	y->z += (a->m13 * b->x) + (a->m23 * b->y) + (a->m33 * b->z);
}


static void v3f32_m3_mul (v3f32 * y, m3f32 const * a, v3f32 * const b)
{
	y->x += (a->m11 * b->x) + (a->m21 * b->y) + (a->m31 * b->z);
	y->y += (a->m12 * b->x) + (a->m22 * b->y) + (a->m32 * b->z);
	y->z += (a->m13 * b->x) + (a->m23 * b->y) + (a->m33 * b->z);
}




//Copy all points inside the ball
static uint32_t v3f32_ball (v3f32 const x[], uint32_t n, v3f32 const * c, v3f32 y[], float r)
{
	uint32_t m = 0;
	for(uint32_t i = 0; i < n; ++i)
	{
		v3f32 d;
		v3f32_sub (&d, x + i, c);
		if (v3f32_norm2 (&d) < (r*r))
		{
			y[m] = x[i];
			m++;
		}
	}
	return m;
}







static void v3f32_meanacc (v3f32 * y, v3f32 const x[], uint32_t n)
{
	ASSERT (n > 0); //Divide by zero protection
	for (uint32_t i = 0; i < n; ++i)
	{
		y->x += x[i].x;
		y->y += x[i].y;
		y->z += x[i].z;
	}
	float s = 1.0f / n;
	y->x *= s;
	y->y *= s;
	y->z *= s;
}





static void v3f32_lerp2 (v3f32 * y, v3f32 const * a, v3f32 * const b, float t)
{
	y->x = (1 - t) * a->x + t * b->x;
	y->y = (1 - t) * a->y + t * b->y;
	y->z = (1 - t) * a->z + t * b->z;
}

