/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"
#include "csc_mf32.h"
#include "csc_v3f32.h"








static void m4f32_mul (struct m4f32 * y, struct m4f32 const * a, struct m4f32 const * b)
{
	struct m4f32 r = M4F32_ZERO;
	mmf32_macc ((float *)&r, (float const*)a, (float const*)b, 4, 4, 4);
	memcpy (y, &r, sizeof (r));
}


static void m4f32_print (struct m4f32 const * m, FILE * f)
{
	mf32_print ((float const *)m, 4, 4, f);
}


static void m4f32_set (struct m4f32 * m, float c)
{
	mf32_set ((float*)m, c, 16);
}


// 4x4 matrix identity
// M := I
static void m4f32_identity (struct m4f32 * m)
{
	m4f32_set (m, 0);
	m->m11 = 1.0f;
	m->m22 = 1.0f;
	m->m33 = 1.0f;
	m->m44 = 1.0f;
}


static void m4f32_translation (struct m4f32 * m, struct v3f32 const * t)
{
	//Translation vector in 4th column
	m->m14 = t->x;
	m->m24 = t->y;
	m->m34 = t->z;
}


static void m4f32_set_columns (struct m4f32 * m, struct v4f32 const * x, struct v4f32 const * y, struct v4f32 const * z, struct v4f32 const * w)
{
	m->m11 = x->x;
	m->m21 = x->y;
	m->m31 = x->x;
	m->m41 = x->w;
	m->m12 = y->x;
	m->m22 = y->y;
	m->m32 = y->x;
	m->m42 = y->w;
	m->m13 = z->x;
	m->m23 = z->y;
	m->m33 = z->x;
	m->m43 = z->w;
	m->m14 = w->x;
	m->m24 = w->y;
	m->m34 = w->x;
	m->m44 = w->w;
}



















static void m4f32_translation_xyz (struct m4f32 * m, float x, float y, float z)
{
	//Translation vector in 4th column
	m->m14 = x;
	m->m24 = y;
	m->m34 = z;
}


static void m4f32_scale_xyz (struct m4f32 * m, float x, float y, float z)
{
	m->m11 = x;
	m->m22 = y;
	m->m33 = z;
}


static void m4f32_scale (struct m4f32 * m, struct v4f32 * s)
{
	m->m11 = s->x;
	m->m22 = s->y;
	m->m33 = s->z;
	m->m44 = s->w;
}


static void m4f32_frustum (struct m4f32 * m, float l, float r, float b, float t, float n, float f)
{
	//Column vector 1:
	m->m11 = (2.0f * n) / (r - l);
	m->m21 = 0.0f;
	m->m31 = 0.0f;
	m->m41 = 0.0f;
	//Column vector 2:
	m->m12 = 0.0f;
	m->m22 = (2 * n) / (t - b);
	m->m32 = 0.0f;
	m->m42 = 0.0f;
	//Column vector 3:
	m->m13 = (r + l) / (r - l);
	m->m23 = (t + b) / (t - b);
	m->m33 = (-f - n) / (f - n);
	m->m43 = -1.0f;
	//Column vector 4:
	m->m14 = 0.0f;
	m->m24 = 0.0f;
	m->m34 = (-2 * f * n) / (f - n);
	m->m44 = 0.0f;
}


static void m4f32_perspective1 (struct m4f32 * m, float fov, float aspect, float n, float f)
{
	float a = 1.0f / tan (fov / 2.0f);
	//Column vector 1:
	m->m11 = a / aspect;
	m->m21 = 0.0f;
	m->m31 = 0.0f;
	m->m41 = 0.0f;
	//Column vector 2:
	m->m12 = 0.0f;
	m->m22 = a;
	m->m32 = 0.0f;
	m->m42 = 0.0f;
	//Column vector 3:
	m->m13 = 0.0f;
	m->m23 = 0.0f;
	m->m33 = -((f + n) / (f - n));
	m->m43 = -1.0f;
	//Column vector 4:
	m->m14 = 0.0f;
	m->m24 = 0.0f;
	m->m34 = -((2.0f * f * n) / (f - n));
	m->m44 = 0.0f;
}


static void m4f32_ortho ()
{

}


// 4x4 matrix inversion (glu version)
// R := M^(-1)
static float m4f32_glu_inv (float r[16], float m[16])
{
	float t[16];

	t[0] =
	m[5]  * m[10] * m[15] -
	m[5]  * m[11] * m[14] -
	m[9]  * m[6]  * m[15] +
	m[9]  * m[7]  * m[14] +
	m[13] * m[6]  * m[11] -
	m[13] * m[7]  * m[10];

	t[4] =
	-m[4]  * m[10] * m[15] +
	m[4]  * m[11] * m[14] +
	m[8]  * m[6]  * m[15] -
	m[8]  * m[7]  * m[14] -
	m[12] * m[6]  * m[11] +
	m[12] * m[7]  * m[10];

	t[8] =
	m[4]  * m[9] * m[15] -
	m[4]  * m[11] * m[13] -
	m[8]  * m[5] * m[15] +
	m[8]  * m[7] * m[13] +
	m[12] * m[5] * m[11] -
	m[12] * m[7] * m[9];

	t[12] =
	-m[4]  * m[9] * m[14] +
	m[4]  * m[10] * m[13] +
	m[8]  * m[5] * m[14] -
	m[8]  * m[6] * m[13] -
	m[12] * m[5] * m[10] +
	m[12] * m[6] * m[9];

	t[1] =
	-m[1]  * m[10] * m[15] +
	m[1]  * m[11] * m[14] +
	m[9]  * m[2] * m[15] -
	m[9]  * m[3] * m[14] -
	m[13] * m[2] * m[11] +
	m[13] * m[3] * m[10];

	t[5] =
	m[0]  * m[10] * m[15] -
	m[0]  * m[11] * m[14] -
	m[8]  * m[2] * m[15] +
	m[8]  * m[3] * m[14] +
	m[12] * m[2] * m[11] -
	m[12] * m[3] * m[10];

	t[9] =
	-m[0]  * m[9] * m[15] +
	m[0]  * m[11] * m[13] +
	m[8]  * m[1] * m[15] -
	m[8]  * m[3] * m[13] -
	m[12] * m[1] * m[11] +
	m[12] * m[3] * m[9];

	t[13] =
	m[0]  * m[9] * m[14] -
	m[0]  * m[10] * m[13] -
	m[8]  * m[1] * m[14] +
	m[8]  * m[2] * m[13] +
	m[12] * m[1] * m[10] -
	m[12] * m[2] * m[9];

	t[2] =
	m[1]  * m[6] * m[15] -
	m[1]  * m[7] * m[14] -
	m[5]  * m[2] * m[15] +
	m[5]  * m[3] * m[14] +
	m[13] * m[2] * m[7] -
	m[13] * m[3] * m[6];

	t[6] =
	-m[0]  * m[6] * m[15] +
	m[0]  * m[7] * m[14] +
	m[4]  * m[2] * m[15] -
	m[4]  * m[3] * m[14] -
	m[12] * m[2] * m[7] +
	m[12] * m[3] * m[6];

	t[10] =
	m[0]  * m[5] * m[15] -
	m[0]  * m[7] * m[13] -
	m[4]  * m[1] * m[15] +
	m[4]  * m[3] * m[13] +
	m[12] * m[1] * m[7] -
	m[12] * m[3] * m[5];

	t[14] =
	-m[0]  * m[5] * m[14] +
	m[0]  * m[6] * m[13] +
	m[4]  * m[1] * m[14] -
	m[4]  * m[2] * m[13] -
	m[12] * m[1] * m[6] +
	m[12] * m[2] * m[5];

	t[3] =
	-m[1] * m[6] * m[11] +
	m[1] * m[7] * m[10] +
	m[5] * m[2] * m[11] -
	m[5] * m[3] * m[10] -
	m[9] * m[2] * m[7] +
	m[9] * m[3] * m[6];

	t[7] =
	m[0] * m[6] * m[11] -
	m[0] * m[7] * m[10] -
	m[4] * m[2] * m[11] +
	m[4] * m[3] * m[10] +
	m[8] * m[2] * m[7] -
	m[8] * m[3] * m[6];

	t[11] =
	-m[0] * m[5] * m[11] +
	m[0] * m[7] * m[9] +
	m[4] * m[1] * m[11] -
	m[4] * m[3] * m[9] -
	m[8] * m[1] * m[7] +
	m[8] * m[3] * m[5];

	t[15] =
	m[0] * m[5] * m[10] -
	m[0] * m[6] * m[9] -
	m[4] * m[1] * m[10] +
	m[4] * m[2] * m[9] +
	m[8] * m[1] * m[6] -
	m[8] * m[2] * m[5];

	float det =
	m[0] * t[0] +
	m[1] * t[4] +
	m[2] * t[8] +
	m[3] * t[12];

	if (det == 0) {return det;}

	det = 1.0f / det;

	for (unsigned i = 0; i < 16; ++ i)
	{
		r [i] = t [i] * det;
	}

	return det;
}


