/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"




static void m3f32_print (float M [3*3], FILE * f)
{
	mf32_print (M, 3, 3, f);
}


static void m3f32_mul (struct m3f32 * y, struct m3f32 const * a, struct m3f32 const * b)
{
	struct m3f32 r = {0.0f};
	mmf32_macc ((float*)&r, (float const*)a, (float const*)b, 3, 3, 3);
	memcpy (y, &r, sizeof (r));
}


static void m3f32_sub (struct m3f32 * y, struct m3f32 const * a, struct m3f32 const * b)
{
	vvf32_sub (3*3, (float *)y, (float const*)a, (float const*)b);
}


static void m3f32_rotate_z (struct m3f32 * r, float angle)
{
	float s = sinf (angle);
	float c = cosf (angle);

	r->m11 = c;
	r->m12 = -s;
	r->m13 = 0.0f;

	r->m21 = s;
	r->m22 = c;
	r->m23 = 0.0f;

	r->m31 = 0.0f;
	r->m32 = 0.0f;
	r->m33 = 1.0f;
}



