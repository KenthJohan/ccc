/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"
#include "csc_vf32.h"
#include "csc_mf32.h"

#define V4F32_FORMAT "(%f %f %f %f)\n"
#define V4F32_ARGS(x) (x)[0], (x)[1], (x)[2], (x)[3]

static void v4f32_cpy (v4f32 * r, v4f32 const * a)
{
	r->x = a->x;
	r->y = a->y;
	r->z = a->z;
	r->w = a->w;
}


static void v4f32_add (v4f32 * r, v4f32 const * a, v4f32 const * b)
{
	r->x = a->x + b->x;
	r->y = a->y + b->y;
	r->z = a->z + b->z;
	r->w = a->w + b->w;
}


static void v4f32_acc (v4f32 * r, v4f32 const * a)
{
	r->x += a->x;
	r->y += a->y;
	r->z += a->z;
	r->w += a->w;
}


static void v4f32_sub (v4f32 * r, v4f32 const * a, v4f32 const * b)
{
	r->x = a->x - b->x;
	r->y = a->y - b->y;
	r->z = a->z - b->z;
	r->w = a->w - b->w;
}

static void v4f32_subv (v4f32 r[], v4f32 const a[], v4f32 const b[], uint32_t incr, uint32_t inca, uint32_t incb, uint32_t n)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		v4f32_sub (r, a, b);
		r += incr;
		a += inca;
		b += incb;
	}
}


static void v4f32_set1 (v4f32 * r, float b)
{
	r->x = b;
	r->y = b;
	r->z = b;
	r->w = b;
}


static void v4f32_mul (v4f32 * r, v4f32 const * a, float b)
{
	r->x = a->x * b;
	r->y = a->y * b;
	r->z = a->z * b;
	r->w = a->w * b;
}


static void v4f32_div (v4f32 * r, v4f32 const * a, float b)
{
	r->x = a->x / b;
	r->y = a->y / b;
	r->z = a->z / b;
	r->w = a->w / b;
}


static float v4f32_dot (v4f32 const * a, v4f32 const * b)
{
	float sum = 0;
	sum += a->x * b->x;
	sum += a->y * b->y;
	sum += a->z * b->z;
	sum += a->w * b->w;
	return sum;
}


static float v4f32_norm2 (v4f32 const * a)
{
	return v4f32_dot (a, a);
}


static float v4f32_norm (v4f32 const * a)
{
	return sqrtf (v4f32_norm2 (a));
}


static void v4f32_normalize (v4f32 * r, v4f32 const * a)
{
	float l = v4f32_norm (a);
	v4f32_div (r, a, l);
}


static void v4f32_set_xyzw (v4f32 * v, float x, float y, float z, float w)
{
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
}




static void v4f32_dotv (v4f32 r[], uint32_t inc_r, v4f32 a[], uint32_t inc_a, v4f32 b[], uint32_t inc_b, uint32_t n)
{
	for (uint32_t i = 0; i < n; ++i)
	{

		r += inc_r;
		a += inc_a;
		b += inc_b;
	}
}






static void v4f32_m4_macc_unsafe (v4f32 * y, m4f32 const * a, v4f32 * const b)
{
	ASSERT_NEQP (y, b);
	y->x += (a->m11 * b->x) + (a->m12 * b->y) + (a->m13 * b->z) + (a->m14 * b->w);
	y->y += (a->m21 * b->x) + (a->m22 * b->y) + (a->m23 * b->z) + (a->m24 * b->w);
	y->z += (a->m31 * b->x) + (a->m32 * b->y) + (a->m33 * b->z) + (a->m34 * b->w);
	y->w += (a->m41 * b->x) + (a->m42 * b->y) + (a->m43 * b->z) + (a->m44 * b->w);
}

static void v4f32_m4_macc (v4f32 * y, m4f32 const * a, v4f32 * const b)
{
	v4f32 t;
	v4f32_cpy (&t, y);
	t.x += (a->m11 * b->x) + (a->m12 * b->y) + (a->m13 * b->z) + (a->m14 * b->w);
	t.y += (a->m21 * b->x) + (a->m22 * b->y) + (a->m23 * b->z) + (a->m24 * b->w);
	t.z += (a->m31 * b->x) + (a->m32 * b->y) + (a->m33 * b->z) + (a->m34 * b->w);
	t.w += (a->m41 * b->x) + (a->m42 * b->y) + (a->m43 * b->z) + (a->m44 * b->w);
	v4f32_cpy (y, &t);
}



static void v4f32_m4_macct_unsafe (v4f32 * y, m4f32 const * a, v4f32 * const b)
{
	ASSERT_NEQP (y, b);
	y->x += (a->m11 * b->x) + (a->m21 * b->y) + (a->m31 * b->z) + (a->m41 * b->w);
	y->y += (a->m12 * b->x) + (a->m22 * b->y) + (a->m32 * b->z) + (a->m42 * b->w);
	y->z += (a->m13 * b->x) + (a->m23 * b->y) + (a->m33 * b->z) + (a->m43 * b->w);
	y->w += (a->m14 * b->x) + (a->m24 * b->y) + (a->m34 * b->z) + (a->m44 * b->w);
}

static void v4f32_m4_macct (v4f32 * y, m4f32 const * a, v4f32 * const b)
{
	v4f32 t;
	v4f32_cpy (&t, y);
	t.x += (a->m11 * b->x) + (a->m21 * b->y) + (a->m31 * b->z) + (a->m41 * b->w);
	t.y += (a->m12 * b->x) + (a->m22 * b->y) + (a->m32 * b->z) + (a->m42 * b->w);
	t.z += (a->m13 * b->x) + (a->m23 * b->y) + (a->m33 * b->z) + (a->m43 * b->w);
	t.w += (a->m14 * b->x) + (a->m24 * b->y) + (a->m34 * b->z) + (a->m44 * b->w);
	v4f32_cpy (y, &t);
}

static void v4f32_m4_mul_unsafe (v4f32 * y, m4f32 const * a, v4f32 * const b)
{
	ASSERT_NEQP (y, b);
	y->x = (a->m11 * b->x) + (a->m12 * b->y) + (a->m13 * b->z) + (a->m14 * b->w);
	y->y = (a->m21 * b->x) + (a->m22 * b->y) + (a->m23 * b->z) + (a->m24 * b->w);
	y->z = (a->m31 * b->x) + (a->m32 * b->y) + (a->m33 * b->z) + (a->m34 * b->w);
	y->w = (a->m41 * b->x) + (a->m42 * b->y) + (a->m43 * b->z) + (a->m44 * b->w);
}

static void v4f32_m4_mul (v4f32 * y, m4f32 const * a, v4f32 * const b)
{
	v4f32 t;
	t.x = (a->m11 * b->x) + (a->m12 * b->y) + (a->m13 * b->z) + (a->m14 * b->w);
	t.y = (a->m21 * b->x) + (a->m22 * b->y) + (a->m23 * b->z) + (a->m24 * b->w);
	t.z = (a->m31 * b->x) + (a->m32 * b->y) + (a->m33 * b->z) + (a->m34 * b->w);
	t.w = (a->m41 * b->x) + (a->m42 * b->y) + (a->m43 * b->z) + (a->m44 * b->w);
	v4f32_cpy (y, &t);
}


























//Set r = xyzw xyzw xyzw ... n times
static void v4f32_set_xyzw_repeat (uint32_t n, float r [], float x, float y, float z, float w)
{
	while (n--)
	{
		r [0] = x;
		r [1] = y;
		r [2] = z;
		r [3] = w;
		r += 4;
	}
}


static void v4f32_set_w_repeat (uint32_t n, float r [], float w)
{
	while (n--)
	{
		r [3] = w;
		r += 4;
	}
}


static void v4f32_repeat_random (unsigned n, float r [])
{
	uint32_t const dim = 4;
	while (n--)
	{
		vf32_random (dim, r);
		r += dim;
	}
}


static void v4f32_repeat_channel (unsigned n, float r [], unsigned channel, float a)
{
	while (n--)
	{
		r [channel] = a;
		r += 4;
	}
}










