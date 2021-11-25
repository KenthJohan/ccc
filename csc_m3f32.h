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




static void m3f32_add_mul (m3f32 * r, m3f32 const * a, m3f32 const * b, float alpha, float beta)
{
	r->m[0] = a->m[0] * alpha + b->m[0] * beta;
	r->m[1] = a->m[1] * alpha + b->m[1] * beta;
	r->m[2] = a->m[2] * alpha + b->m[2] * beta;
	r->m[3] = a->m[3] * alpha + b->m[3] * beta;
	r->m[4] = a->m[4] * alpha + b->m[4] * beta;
	r->m[5] = a->m[5] * alpha + b->m[5] * beta;
	r->m[6] = a->m[6] * alpha + b->m[6] * beta;
	r->m[7] = a->m[7] * alpha + b->m[7] * beta;
	r->m[8] = a->m[8] * alpha + b->m[8] * beta;
	r->m[9] = a->m[9] * alpha + b->m[9] * beta;
}





