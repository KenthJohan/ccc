/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"
#include "csc_mf32.h"


//Column 0
#define M3_11 0
#define M3_21 1
#define M3_31 2
//Column 1
#define M3_12 3
#define M3_22 4
#define M3_32 5
//Column 2
#define M3_13 6
#define M3_23 7
#define M3_33 8

#define M3_ARGS(m) m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8]
#define M3_ARGST(m) m[0],m[3],m[6],m[1],m[4],m[7],m[2],m[5],m[8]

#define M3F32_ZERO {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}
#define M3F32_IDENTITY {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f}
#define M3F32_DIAGONAL(x) {(x), 0.0f, 0.0f, 0.0f, (x), 0.0f, 0.0f, 0.0f, (x)}
#define M3F32_SCALE(x,y,z) {(x), 0.0f, 0.0f, 0.0f, (y), 0.0f, 0.0f, 0.0f, (z)}


struct m3f32
{
	float m11;
	float m21;
	float m31;
	float m12;
	float m22;
	float m32;
	float m13;
	float m23;
	float m33;
};


static void m3f32_print (struct m3f32 * m, FILE * f)
{
	mf32_print ((float*)m, 3, 3, f);
}


static void m3f32_mul (struct m3f32 * y, struct m3f32 const * a, struct m3f32 const * b)
{
	struct m3f32 r = M3F32_ZERO;
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



