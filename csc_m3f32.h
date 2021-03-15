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


static void m3f32_mul (m3f32 y, m3f32 const a, m3f32 const b)
{
	m3f32 r = {0.0f};
	mmf32_macc (r, a, b, 3, 3, 3);
	memcpy (y, r, sizeof (r));
}


static void m3f32_sub (m3f32 y, m3f32 const a, m3f32 const b)
{
	vvf32_sub (3*3, y, a, b);
}


static void m3f32_rotate_z (m3f32 r, float angle)
{
	float s = sinf (angle);
	float c = cosf (angle);

	r[M3_00] = c;
	r[M3_01] = -s;
	r[M3_02] = 0.0f;

	r[M3_10] = s;
	r[M3_11] = c;
	r[M3_12] = 0.0f;

	r[M3_20] = 0.0f;
	r[M3_21] = 0.0f;
	r[M3_22] = 1.0f;
}



