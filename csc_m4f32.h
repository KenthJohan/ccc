/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"
#include "csc_mf32.h"


//Column 0
#define M4_11 0
#define M4_21 1
#define M4_31 2
#define M4_41 3
//Column 1
#define M4_12 4
#define M4_22 5
#define M4_32 6
#define M4_42 7
//Column 2
#define M4_13 8
#define M4_23 9
#define M4_33 10
#define M4_43 11
//Column 3
#define M4_14 12
#define M4_24 13
#define M4_34 14
#define M4_44 15

//                                 |--------Column1--------|--------Column2--------|--------Column2--------|--------Column3--------|
#define M3F32_ZERO                 {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}
#define M3F32_IDENTITY             {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}
#define M3F32_DIAGONAL(x)          { (x), 0.0f, 0.0f, 0.0f, 0.0f,  (x), 0.0f, 0.0f, 0.0f, 0.0f,  (x), 0.0f, 0.0f, 0.0f, 0.0f,  (x)}
#define M3F32_SCALE(x,y,z,w)       { (x), 0.0f, 0.0f, 0.0f, 0.0f,  (y), 0.0f, 0.0f, 0.0f, 0.0f,  (z), 0.0f, 0.0f, 0.0f, 0.0f,  (w)}
#define M3F32_TRANSLATION(x,y,z)   {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  (x),  (y),  (z), 1.0f}


struct m4f32
{
	float m11;
	float m21;
	float m31;
	float m41;
	float m12;
	float m22;
	float m32;
	float m42;
	float m13;
	float m23;
	float m33;
	float m43;
	float m14;
	float m24;
	float m34;
	float m44;
};




static void m4f32_mul (struct m4f32 * y, struct m4f32 const * a, struct m4f32 const * b)
{
	struct m4f32 r = M3F32_ZERO;
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


static void m4f32_translation (struct m4f32 * m, v3f32 const t)
{
	//Translation vector in 4th column
	m->m14 = t [0];
	m->m24 = t [1];
	m->m34 = t [2];
}


static void m4f32_set_columns (struct m4f32 * m, v4f32 const x, v4f32 const y, v4f32 const z, v4f32 const w)
{
	m->m11 = x [0];
	m->m21 = x [1];
	m->m31 = x [2];
	m->m41 = x [3];
	m->m12 = y [0];
	m->m22 = y [1];
	m->m32 = y [2];
	m->m42 = y [3];
	m->m13 = z [0];
	m->m23 = z [1];
	m->m33 = z [2];
	m->m43 = z [3];
	m->m14 = w [0];
	m->m24 = w [1];
	m->m34 = w [2];
	m->m44 = w [3];
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


static void m4f32_scale (struct m4f32 * m, v3f32 s)
{
	m->m11 = s[0];
	m->m22 = s[1];
	m->m33 = s[2];
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


