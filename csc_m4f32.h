/* SPDX-License-Identifier: GPL-2.0 */
#pragma once

#include <stdint.h>
#include "csc_math.h"

static void m4f32_mul (float y[4*4], float const a[4*4], float const b[4*4])
{
	float r[4*4] = {0.0f};
	mmf32_macc (r, a, b, 4, 4, 4);
	memcpy (y, r, sizeof (r));
}







void m4f32_print (float M [4*4], FILE * f)
{
	mf32_print (M, 4, 4, f);
}


void m4f32_set (float M [16], float c)
{
	mf32_set (M, c, 16);
}


// 4x4 matrix identity
// M := I
void m4f32_identity (float M [16])
{
	m4f32_set (M, 0);
	M [M4_S0] = 1.0f;
	M [M4_S1] = 1.0f;
	M [M4_S2] = 1.0f;
	M [M4_S3] = 1.0f;
}


void m4f32_translation (float M [16], float const t [4])
{
	M [M4_T0 + 0] = t [0];
	M [M4_T0 + 1] = t [1];
	M [M4_T0 + 2] = t [2];
}


void m4f32_translation_xyz (float M [16], float x, float y, float z)
{
	M [M4_T0 + 0] = x;
	M [M4_T0 + 1] = y;
	M [M4_T0 + 2] = z;
}


void m4f32_scale_xyz (float M [16], float x, float y, float z)
{
	M [M4_S0] = x;
	M [M4_S1] = y;
	M [M4_S2] = z;
}


void m4f32_scale (float M [16], float s)
{
	M [M4_S0] = s;
	M [M4_S1] = s;
	M [M4_S2] = s;
}


void m4f32_frustum
(float M [16], float l, float r, float b, float t, float n, float f)
{
	m4f32_set (M, 0);
	M [0] = (2 * n) / (r - l);
	M [5] = (2 * n) / (t - b);
	M [8] = (r + l) / (r - l);
	M [9] = (t + b) / (t - b);
	M [10] = (-f - n) / (f - n);
	M [11] = -1;
	M [14] = (-2 * f * n) / (f - n);
}


static void m4f32_perspective1 (float m[4*4], float fov, float aspect, float n, float f)
{
	float const a = 1.0f / tan (fov / 2.0f);
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


void m4f32_ortho
()
{

}


// 4x4 matrix inversion (glu version)
// R := M^(-1)
float m4f32_glu_inv (float r[16], float const m[16])
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
