/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"
#include "csc_mf32.h"





static void m3f32_print (m3f32 * m, FILE * f)
{
	mf32_print ((float*)m, 3, 3, f);
}


static void m3f32_mul (m3f32 * y, m3f32 const * a, m3f32 const * b)
{
	m3f32 r = M3F32_ZERO;
	mmf32_macc ((float*)&r, (float const*)a, (float const*)b, 3, 3, 3);
	memcpy (y, &r, sizeof (r));
}


static void m3f32_sub (m3f32 * y, m3f32 const * a, m3f32 const * b)
{
	vvf32_sub (3*3, (float *)y, (float const*)a, (float const*)b);
}


static void m3f32_rotate_z (m3f32 * r, float angle)
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








