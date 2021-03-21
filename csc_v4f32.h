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

static void v4f32_cpy (struct v4f32 * r, struct v4f32 const * a)
{
	r->x = a->x;
	r->y = a->y;
	r->z = a->z;
	r->w = a->w;
}


static void v4f32_add (struct v4f32 * r, struct v4f32 const * a, struct v4f32 const * b)
{
	r->x = a->x + b->x;
	r->y = a->y + b->y;
	r->z = a->z + b->z;
	r->w = a->w + b->w;
}


static void v4f32_acc (struct v4f32 * r, struct v4f32 const * a)
{
	r->x += a->x;
	r->y += a->y;
	r->z += a->z;
	r->w += a->w;
}


static void v4f32_sub (struct v4f32 * r, struct v4f32 const * a, struct v4f32 const * b)
{
	r->x = a->x - b->x;
	r->y = a->y - b->y;
	r->z = a->z - b->z;
	r->w = a->w - b->w;
}


static void v4f32_set1 (struct v4f32 * r, float b)
{
	r->x = b;
	r->y = b;
	r->z = b;
	r->w = b;
}


static void v4f32_mul (struct v4f32 * r, struct v4f32 const * a, float b)
{
	r->x = a->x * b;
	r->y = a->y * b;
	r->z = a->z * b;
	r->w = a->w * b;
}


static void v4f32_div (struct v4f32 * r, struct v4f32 const * a, float b)
{
	r->x = a->x / b;
	r->y = a->y / b;
	r->z = a->z / b;
	r->w = a->w / b;
}


float v4f32_dot (struct v4f32 const * a, struct v4f32 const * b)
{
	float sum = 0;
	sum += a->x * b->x;
	sum += a->y * b->y;
	sum += a->z * b->z;
	sum += a->w * b->w;
	return sum;
}


static float v4f32_norm2 (struct v4f32 const * a)
{
	return v4f32_dot (a, a);
}


static float v4f32_norm (struct v4f32 const * a)
{
	return sqrtf (v4f32_norm2 (a));
}


static void v4f32_normalize (struct v4f32 * r, struct v4f32 const * a)
{
	float l = v4f32_norm (a);
	v4f32_div (r, a, l);
}


static void v4f32_set_xyzw (struct v4f32 * v, float x, float y, float z, float w)
{
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
}




static void v4f32_dotv (struct v4f32 r[], uint32_t inc_r, struct v4f32 a[], uint32_t inc_a, struct v4f32 b[], uint32_t inc_b, uint32_t n)
{
	for (uint32_t i = 0; i < n; ++i)
	{

		r += inc_r;
		a += inc_a;
		b += inc_b;
	}
}








static void mv4f32_macc (struct v4f32 * y, struct m4f32 const * a, struct v4f32 * const b)
{
	y->x += (a->m11 * b->x) + (a->m12 * b->y) + (a->m13 * b->z) + (a->m14 * b->w);
	y->y += (a->m21 * b->x) + (a->m22 * b->y) + (a->m23 * b->z) + (a->m24 * b->w);
	y->z += (a->m31 * b->x) + (a->m32 * b->y) + (a->m33 * b->z) + (a->m34 * b->w);
	y->w += (a->m41 * b->x) + (a->m42 * b->y) + (a->m43 * b->z) + (a->m44 * b->w);
}


static void mv4f32_mul (struct v4f32 * y, struct m4f32 const * a, struct v4f32 * const b)
{
	ASSERT_PTR_NEQ (y, &a->m11);
	ASSERT_PTR_NEQ (y, &a->m12);
	ASSERT_PTR_NEQ (y, &a->m13);
	ASSERT_PTR_NEQ (y, &a->m14);
	ASSERT_PTR_NEQ (y, b);
	y->x += (a->m11 * b->x) + (a->m12 * b->y) + (a->m13 * b->z) + (a->m14 * b->w);
	y->y += (a->m21 * b->x) + (a->m22 * b->y) + (a->m23 * b->z) + (a->m24 * b->w);
	y->z += (a->m31 * b->x) + (a->m32 * b->y) + (a->m33 * b->z) + (a->m34 * b->w);
	y->w += (a->m41 * b->x) + (a->m42 * b->y) + (a->m43 * b->z) + (a->m44 * b->w);
}


static void mv4f32_macc_transposed (struct v4f32 * y, struct m4f32 const * a, struct v4f32 * const b)
{
	y->x += (a->m11 * b->x) + (a->m21 * b->y) + (a->m31 * b->z) + (a->m41 * b->w);
	y->y += (a->m12 * b->x) + (a->m22 * b->y) + (a->m32 * b->z) + (a->m42 * b->w);
	y->z += (a->m13 * b->x) + (a->m23 * b->y) + (a->m33 * b->z) + (a->m43 * b->w);
	y->w += (a->m14 * b->x) + (a->m24 * b->y) + (a->m34 * b->z) + (a->m44 * b->w);
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









