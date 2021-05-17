/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include "csc_debug.h"
#include "csc_basic.h"




//typedef float m4f32[16];
//typedef float m3f32[9];
//typedef float v2f32[2];
//typedef float v3f32[3];
//typedef float v4f32[4];
//typedef float qf32[4];


#define V2F32_ZERO {0.0f, 0.0f}
#define V2F32_REPEAT(x) {(x), (x)}
struct v2f32
{
	float x;
	float y;
};


#define QF32_IDENTITY  {0.0f, 0.0f, 0.0f, 1.0f}
typedef union qf32
{
	struct
	{
		float x;
		float y;
		float z;
		float w;
	};
} qf32;


#define V4F32_ZERO {0.0f, 0.0f, 0.0f, 0.0f}
#define V4F32_REPEAT(x) {(x), (x), (x), (x)}
#define V4F32_V3(v) (v4f32){{(v).x, (v).y, (v).z, 0.0f}}
typedef union v4f32
{
	float e[4];
	struct
	{
		float x;
		float y;
		float z;
		float w;
	};
} v4f32;




#define V3F32_ZERO {0.0f, 0.0f, 0.0f}
#define V3F32_REPEAT(x) {(x), (x), (x)}
#define V3F32_V4F32(x) {(x).x, (x).y, (x).z}
typedef union v3f32
{
	float e[3];
	struct
	{
		float x;
		float y;
		float z;
	};
} v3f32;


#define M3_11 0
#define M3_21 1
#define M3_31 2
#define M3_12 3
#define M3_22 4
#define M3_32 5
#define M3_13 6
#define M3_23 7
#define M3_33 8
#define M3_ARGS(m) m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8]
#define M3_ARGST(m) m[0],m[3],m[6],m[1],m[4],m[7],m[2],m[5],m[8]
//                         |----Column1-----|-----Column2-----|-----Column3-----|
#define M3F32_ZERO         {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}
#define M3F32_REPEAT(x)    {{ (x),  (x),  (x),  (x),  (x),  (x),  (x),  (x),  (x)}}
#define M3F32_IDENTITY     {{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f}}
#define M3F32_DIAGONAL(x)  {{ (x), 0.0f, 0.0f, 0.0f,  (x), 0.0f, 0.0f, 0.0f,  (x)}}
#define M3F32_SCALE(x,y,z) {{ (x), 0.0f, 0.0f, 0.0f,  (y), 0.0f, 0.0f, 0.0f,  (z)}}
typedef union m3f32
{
	float m[9];
	struct
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
	v3f32 c[3];
} m3f32;








#define M4_11 0
#define M4_21 1
#define M4_31 2
#define M4_41 3
#define M4_12 4
#define M4_22 5
#define M4_32 6
#define M4_42 7
#define M4_13 8
#define M4_23 9
#define M4_33 10
#define M4_43 11
#define M4_14 12
#define M4_24 13
#define M4_34 14
#define M4_44 15
//                                 |-------Column1--------|--------Column2--------|--------Column2--------|--------Column3--------|
#define M4F32_ZERO                 {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}
#define M4F32_IDENTITY             {{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}}
#define M4F32_DIAGONAL(x)          {{ (x), 0.0f, 0.0f, 0.0f, 0.0f,  (x), 0.0f, 0.0f, 0.0f, 0.0f,  (x), 0.0f, 0.0f, 0.0f, 0.0f,  (x)}}
#define M4F32_SCALE(x,y,z,w)       {{ (x), 0.0f, 0.0f, 0.0f, 0.0f,  (y), 0.0f, 0.0f, 0.0f, 0.0f,  (z), 0.0f, 0.0f, 0.0f, 0.0f,  (w)}}
#define M4F32_TRANSLATION(x,y,z)   {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  (x),  (y),  (z), 1.0f}}
typedef union m4f32
{
	float m[16];
	struct
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
} m4f32;





struct m2f32
{
	float m11;
	float m21;
	float m12;
	float m22;
};


struct m1f32
{
	float m11;
};


struct qf32_xyzw
{
	float x;
	float y;
	float z;
	float w;
};

struct v4f32_xyzw
{
	float x;
	float y;
	float z;
	float w;
};



struct v2f32_xy
{
	float x;
	float y;
	float z;
};

struct v2f32_wh
{
	float w;
	float h;
};


struct v4f32_line
{
	float a[4];
	float b[4];
};

struct u32_line
{
	uint32_t a;
	uint32_t b;
};









