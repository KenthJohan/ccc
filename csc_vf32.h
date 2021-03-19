/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"


// r := a < b
static void vvf32_lt (uint32_t n, float r [], float const a [], float const b [])
{
	for (uint32_t i = 0; i < n; ++i)
	{
		r [i] = a [i] < b [i];
	}
}


// r := a > b
static void vvf32_gt (uint32_t n, float r [], float const a [], float const b [])
{
	for (uint32_t i = 0; i < n; ++i)
	{
		r [i] = a [i] > b [i];
	}
}


// r := a + b
static void vvf32_add (uint32_t n, float r [], float const a [], float const b [])
{
	for (uint32_t i = 0; i < n; ++i)
	{
		r [i] = a [i] + b [i];
	}
}


// r := a - b
static void vvf32_sub (uint32_t n, float r [], float const a [], float const b [])
{
	for (uint32_t i = 0; i < n; ++i)
	{
		r [i] = a [i] - b [i];
	}
}


static void vf32_addv (uint32_t dim, float y[], uint32_t y_stride, float const a[], uint32_t a_stride, float const b[], uint32_t b_stride, uint32_t n)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		vvf32_add (dim, y, a, b);
		y += y_stride;
		a += a_stride;
		b += b_stride;
	}
}


static void vf32_subv (uint32_t dim, float y[], uint32_t y_stride, float const a[], uint32_t a_stride, float const b[], uint32_t b_stride, uint32_t n)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		vvf32_sub (dim, y, a, b);
		y += y_stride;
		a += a_stride;
		b += b_stride;
	}
}


static void vf32_set1_strided (float v[], float x, uint32_t n, unsigned inc)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		v[0] = x;
		v += inc;
	}
}


static void vf32_set2 (float v[], float v0, float v1)
{
	v[0] = v0;
	v[1] = v1;
}


static void vf32_set3 (float v[], float v0, float v1, float v2)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
}


static void vf32_set4 (float v[], float v0, float v1, float v2, float v3)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	v[3] = v3;
}



// r := a . b
static float vf32_dot (unsigned n, float const a [], float const b [])
{
	float r = 0.0f;
	while (n--)
	{
		r += a [n] * b [n];
	}
	return r;
}



static void vf32_cpy (unsigned n, float des [], float const src [])
{
	for (uint32_t i = 0; i < n; ++i)
	{
		des [i] = src [i];
	}
}







// r := r + a
static void vf32_acc (unsigned n, float r [], float const a [])
{
	vvf32_add (n, r, r, a);
}


// r := r - a
static void vf32_decc (unsigned n, float r [], float const a [])
{
	vvf32_sub (n, r, r, a);
}





// ret a > b
static int vf32_gt (unsigned n, float const a [], float const b [])
{
	while (n--)
	{
		if (a [n] <= b [n])
		{
			return 0;
		}
	}
	return 1;
}


// ret a < b
static int vf32_lt (unsigned n, float const a [], float const b [])
{
	while (n--)
	{
		if (a [n] <= b [n]) {return 0;}
	}
	return 1;
}


// Set all element (x) of r to b
// r := {x | x = ?}
static void vf32_random (uint32_t n, float r [])
{
	for (uint32_t i = 0; i < n; ++i)
	{
		r [i] = (float)rand() / (float)RAND_MAX;
		//r [i] = 1.0f;
	}
}


// Set all element (x) of r to b
// r := {x | x = b}
static void vf32_set1 (uint32_t n, float r [], float const b)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		r [i] = b;
	}
}


// r := a * b
static void vvf32_hadamard (unsigned n, float r [], float const a [], float const b [])
{
	for (uint32_t i = 0; i < n; ++i)
	{
		r [i] = a [i] * b [i];
	}
}


// r := r + a * b
static void vvf32_macc (uint32_t n, float r [], float const a [], float const b [])
{
	for (uint32_t i = 0; i < n; ++i)
	{
		r [i] += a [i] * b [i];
	}
}


static float vf32_sum (uint32_t n, float const v [])
{
	float sum = 0;
	for (uint32_t i = 0; i < n; ++i)
	{
		sum += v [i];
	}
	return sum;
}










static void vf32_linespace (uint32_t n, float x [], float x1, float x2)
{
	float const d = (x2 - x1) / n;
	float a = x1;
	while (n--)
	{
		x [n] = a;
		a += d;
	}
}


static void vf32_repeat (uint32_t n, float v [], float value, uint32_t offset, uint32_t stride)
{
	v += offset;
	while (n--)
	{
		v [0] = value;
		v += stride;
	}
}


static void vf32_weight_ab (uint32_t n, float y [], float a [], float b [], float k)
{
	float A = k;
	float B = 1.0f - k;
	while (n--)
	{
		y [n] = A * a [n] + B * b [n];
	}
}


static void vf32_setl (float r [], uint32_t n, ...)
{
	va_list ap;
	va_start (ap, n);
	for (uint32_t i = 0; i < n; ++i)
	{
		r [i] = va_arg (ap, double);
	}
	va_end (ap);
}


static float vf32_avg (uint32_t n, float v[])
{
	ASSERT (n > 0);
	float sum = 0.0f;
	for (uint32_t i = 0; i < n; ++i)
	{
		sum += v[i];
	}
	return sum / n;
}



static float vf32_max (uint32_t n, float v[])
{
	ASSERT (n >= 1);
	float max = v[0];
	for (uint32_t i = 0; i < n; ++i)
	{
		if (v[i] > max)
		{
			max = v[i];
		}
	}
	return max;
}


static float vf32_maxabs (uint32_t n, float v[])
{
	ASSERT (n >= 1);
	float max = v[0];
	for (uint32_t i = 0; i < n; ++i)
	{
		float x = fabs(v[i]);
		if (x > max)
		{
			max = x;
		}
	}
	return max;
}
























// r := a - b
static void vsf32_sub (unsigned n, float r [], float const a [], float b)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		r [i] = a [i] - b;
	}
}

static void vsf32_macc (uint32_t n, float vy[], float const vx[], float sb)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		vy [i] += vx [i] * sb;
	}
}

// r := a + b
static void vsf32_add (uint32_t n, float r [], float const a [], float const b)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		r [i] = a [i] + b;
	}
}

// r := a + b
static void vsf32_add_max (uint32_t n, float r [], float const a [], float const b, float max)
{
	for (uint32_t i = 0; i < n; ++i)
	{
		if (r [i] < max)
		{
			r [i] = a [i] + b;
		}
	}
}

// r := a * b
static void vsf32_mul (uint32_t n, float r [], float const a [], float const b)
{
	while (n--)
	{
		r [n] = a [n] * b;
	}
}





















// ret |a|^2
static float vf32_norm2 (uint32_t n, float const a [])
{
	return vf32_dot (n, a, a);
}


// ret |a|
static float vf32_norm (uint32_t n, float const a [])
{
	return sqrtf (vf32_norm2 (n, a));
}


// r := a / |a|
static void vf32_normalize (uint32_t n, float r [], float const a [])
{
	float const l = vf32_norm (n, a);
	float const s = l > 0.0f ? 1.0f / l : 0.0f;
	vsf32_mul (n, r, a, s);
}


