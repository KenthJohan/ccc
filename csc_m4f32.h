/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"



//Translation vector
//Independent from row/col major
#define M4_TX 12
#define M4_TY 13
#define M4_TZ 14
#define M4_TW 15

#define M4_T0 12
#define M4_T1 13
#define M4_T2 14
#define M4_T3 15


//Column vectors
//Independent from row/col major
#define M4_V0 0
#define M4_V1 4
#define M4_V2 8
#define M4_V3 12

#define M4_VX 0
#define M4_VY 4
#define M4_VZ 8
#define M4_VT 12


//Scale scalars
//Independent from row/col major
#define M4_S0 0
#define M4_S1 5
#define M4_S2 10
#define M4_S3 15


//Column 0
#define M4_00 0
#define M4_10 1
#define M4_20 2
#define M4_30 3
//Column 1
#define M4_01 4
#define M4_11 5
#define M4_21 6
#define M4_31 7
//Column 2
#define M4_02 8
#define M4_12 9
#define M4_22 10
#define M4_32 11
//Column 3
#define M4_03 12
#define M4_13 13
#define M4_23 14
#define M4_33 15

static void m4f32_mul (m4f32 y, m4f32 const a, m4f32 const b)
{
	m4f32 r = {0.0f};
	mmf32_macc (r, a, b, 4, 4, 4);
	memcpy (y, r, sizeof (r));
}


static void m4f32_print (m4f32 m, FILE * f)
{
	mf32_print (m, 4, 4, f);
}


static void m4f32_set (m4f32 m, float c)
{
	mf32_set (m, c, 16);
}


// 4x4 matrix identity
// M := I
static void m4f32_identity (m4f32 m)
{
	m4f32_set (m, 0);
	m [M4_00] = 1.0f;
	m [M4_11] = 1.0f;
	m [M4_22] = 1.0f;
	m [M4_33] = 1.0f;
}


static void m4f32_translation (m4f32 m, v3f32 const t)
{
	//Translation vector in 4th column
	m [M4_03] = t [0];
	m [M4_13] = t [1];
	m [M4_23] = t [2];
}


static void m4f32_set_columns (m4f32 m, v4f32 const x, v4f32 const y, v4f32 const z, v4f32 const w)
{
	m [M4_00] = x [0];
	m [M4_10] = x [1];
	m [M4_20] = x [2];
	m [M4_20] = x [3];

	m [M4_01] = y [0];
	m [M4_11] = y [1];
	m [M4_21] = y [2];
	m [M4_21] = y [3];

	m [M4_02] = z [0];
	m [M4_12] = z [1];
	m [M4_22] = z [2];
	m [M4_22] = z [3];

	m [M4_03] = w [0];
	m [M4_13] = w [1];
	m [M4_23] = w [2];
	m [M4_23] = w [3];
}


static void m4f32_translation_xyz (m4f32 m, float x, float y, float z)
{
	//Translation vector in 4th column
	m [M4_03] = x;
	m [M4_13] = y;
	m [M4_23] = z;
}


static void m4f32_scale_xyz (m4f32 m, float x, float y, float z)
{
	m [M4_00] = x;
	m [M4_11] = y;
	m [M4_22] = z;
}


static void m4f32_scale (m4f32 m, v3f32 s)
{
	m [M4_00] = s[0];
	m [M4_11] = s[1];
	m [M4_22] = s[2];
}


static void m4f32_frustum (m4f32 m, float l, float r, float b, float t, float n, float f)
{
	//Column vector 1:
	m [0] = (2.0f * n) / (r - l);
	m [1] = 0.0f;
	m [2] = 0.0f;
	m [3] = 0.0f;
	//Column vector 2:
	m [4] = 0.0f;
	m [5] = (2 * n) / (t - b);
	m [6] = 0.0f;
	m [7] = 0.0f;
	//Column vector 3:
	m [8] = (r + l) / (r - l);
	m [9] = (t + b) / (t - b);
	m [10] = (-f - n) / (f - n);
	m [11] = -1.0f;
	//Column vector 4:
	m [12] = 0.0f;
	m [13] = 0.0f;
	m [14] = (-2 * f * n) / (f - n);
	m [15] = 0.0f;
}


static void m4f32_perspective1 (m4f32 m, float fov, float aspect, float n, float f)
{
	float a = 1.0f / tan (fov / 2.0f);
	//Column vector 1:
	m[0] = a / aspect;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	//Column vector 2:
	m[4] = 0.0f;
	m[5] = a;
	m[6] = 0.0f;
	m[7] = 0.0f;
	//Column vector 3:
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = -((f + n) / (f - n));
	m[11] = -1.0f;
	//Column vector 4:
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = -((2.0f * f * n) / (f - n));
	m[15] = 0.0f;
}


static void m4f32_ortho ()
{

}


// 4x4 matrix inversion (glu version)
// R := M^(-1)
static float m4f32_glu_inv (m4f32 r, m4f32 const m)
{
	m4f32 t;

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
