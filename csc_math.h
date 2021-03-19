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


//Column 0
#define M3_00 0
#define M3_10 1
#define M3_20 2
//Column 1
#define M3_01 3
#define M3_11 4
#define M3_21 5
//Column 2
#define M3_02 6
#define M3_12 7
#define M3_22 8
#define M3_ARGS(m) m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8]
#define M3_ARGST(m) m[0],m[3],m[6],m[1],m[4],m[7],m[2],m[5],m[8]

//typedef float m4f32[16];
//typedef float m3f32[9];
typedef float v2f32[2];
typedef float v3f32[3];
typedef float v4f32[4];
typedef float qf32[4];


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

struct v3f32_xyz
{
	float x;
	float y;
	float z;
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









