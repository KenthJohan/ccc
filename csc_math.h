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
typedef float v2f32[2];
typedef float v3f32[3];
typedef float v4f32[4];
typedef float qf32[4];




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









