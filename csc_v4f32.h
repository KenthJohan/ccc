/* SPDX-License-Identifier: GPL-2.0 */
#pragma once

#include <stdint.h>
#include "csc_math.h"


static void v4f32_cpy (float r [4], float const a [4])
{
	vf32_cpy (4, r, a);
}


static void v4f32_add (float r [4], float const a [4], float const b [4])
{
	vvf32_add (4, r, a, b);
}


static void v4f32_acc (float r [4], float const a [4])
{
	vf32_acc (4, r, a);
}


static void v4f32_sub (float r [4], float const a [4], float const b [4])
{
	vvf32_sub (4, r, a, b);
}


static void v4f32_set1 (float r [4], float const b)
{
	vf32_set1 (4, r, b);
}


static void v4f32_mul (float r [4], float const a [4], float const b)
{
	vsf32_mul (4, r, a, b);
}


static void v4f32_normalize (float r [4], float const a [4])
{
	vf32_normalize (4, r, a);
}


static float v4f32_norm2 (float const a [4])
{
	return vvf32_dot (4, a, a);
}


//xyzw xyzw xyzw ... n times
static void v4f32_repeat4 (uint32_t n, float r [], float x, float y, float z, float w)
{
	while (n--)
	{
		//TRACE_F ("%i", n);
		r [0] = x;
		r [1] = y;
		r [2] = z;
		r [3] = w;
		r += 4;
	}
}


static void v4f32_set_xyzw (float v[4], float x, float y, float z, float w)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
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


static void v4f_normalize (float r [4], float v [4])
{
	vf32_normalize (4, r, v);
}


static void mv4f32_macc (float y[4], float const a[4*4], float const b[4])
{
	mvf32_macc (y, a, b, 4, 4);
}


static void mv4f32_mul (float y[4], float const a[4*4], float const b[4])
{
	float r[4] = {0.0f};
	mvf32_macc (r, a, b, 4, 4);
	memcpy (y, r, sizeof (r));
}


static void mv4f32_macc_transposed (float y[4], float const a[4*4], float const b[4])
{
	mvf32_macc_transposed (y, a, b, 4, 4);
}
