/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"

#define V4F32_FORMAT "(%f %f %f %f)\n"
#define V4F32_ARGS(x) (x)[0], (x)[1], (x)[2], (x)[3]

static void v4f32_cpy (v4f32 r, v4f32 const a)
{
	vf32_cpy (4, r, a);
}


static void v4f32_add (v4f32 r, v4f32 const a, v4f32 const b)
{
	vvf32_add (4, r, a, b);
}


static void v4f32_acc (v4f32 r, v4f32 const a)
{
	vf32_acc (4, r, a);
}


static void v4f32_sub (v4f32 r, v4f32 const a, v4f32 const b)
{
	vvf32_sub (4, r, a, b);
}


static void v4f32_set1 (v4f32 r, float const b)
{
	vf32_set1 (4, r, b);
}


static void v4f32_mul (v4f32 r, v4f32 const a, float const b)
{
	vsf32_mul (4, r, a, b);
}


static void v4f32_normalize (v4f32 r, v4f32 const a)
{
	vf32_normalize (4, r, a);
}


static float v4f32_norm2 (v4f32 const a)
{
	return vvf32_dot (4, a, a);
}


static void v4f32_set_xyzw (v4f32 v, float x, float y, float z, float w)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}













static void mv4f32_macc (v4f32 y, struct m4f32 const * a, v4f32 const b)
{
	mvf32_macc (y, a, b, 4, 4);
}


static void mv4f32_mul (v4f32 y, struct m4f32 const * a, v4f32 const b)
{
	float r[4] = {0.0f};
	mvf32_macc (r, a, b, 4, 4);
	memcpy (y, r, sizeof (r));
}


static void mv4f32_macc_transposed (v4f32 y, struct m4f32 const * a, v4f32 const b)
{
	mvf32_macc_transposed (y, a, b, 4, 4);
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


static void vu32_repeat_random_mask (unsigned n, uint32_t r [], uint32_t mask)
{
	for (unsigned i = 0; i < n; ++i)
	{
		uint32_t g = rand ();
		g *= (UINT32_MAX / RAND_MAX);
		r[i] = g & mask;
	}
}






