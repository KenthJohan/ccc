/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "csc_debug.h"
#include "csc_basic.h"
#include "csc_math.h"
#include "csc_v3f32.h"



//https://keisan.casio.com/exec/system/1223596129
static void csc_linmat_plane_from_3points (float plane[4], v3f32 const * a, v3f32 const * b, v3f32 const * c)
{
	v3f32 ab;
	v3f32 ac;
	v3f32_sub (&ab, a, b);
	v3f32_sub (&ac, a, c);
	v3f32_cross ((v3f32*)plane, &ab, &ac);
	plane[3] = -v3f32_dot ((v3f32*)plane, a);
}


//https://keisan.casio.com/exec/system/1223614315
//https://mathinsight.org/distance_point_plane
static float csc_linmat_plane_point_distance (float const plane[4], v3f32 const * x)
{
	float px;
	float p2;
	float l;
	px = v3f32_dot ((v3f32*)plane, x) + plane[3];
	p2 = v3f32_dot ((v3f32*)plane, (v3f32*)plane);
	ASSERT (p2 > 0.0f);
	l = fabs (px) / sqrtf (p2);
	ASSERT (l >= 0.0f);
	return l;
}


static float csc_linmat_plane_point_distance2 (float const plane[4], v3f32 const * x)
{
	float px;
	float p2;
	float l;
	px = v3f32_dot ((v3f32*)plane, x) + plane[3];
	p2 = v3f32_dot ((v3f32*)plane, (v3f32*)plane);
	ASSERT (p2 > 0.0f);
	l = fabs (px) / p2;
	ASSERT (l >= 0.0f);
	return l;
}


//http://www.easy-math.net/transforming-between-plane-forms/
static void csc_linmat_plane_to_parametric (float x[3], float const plane[4])
{
	float a = plane[0];
	float b = plane[1];
	float c = plane[2];
	float d = plane[3];
	float r = x[1];
	float s = x[2];
	ASSERT (a != 0.0f);
	x[0] = (d/a) - (r * (b/a)) - (s * (c/a));
}



static unsigned csc_linmat_plane_point_count
(
float const plane[4],
float const x[],//stride 4
unsigned n,
float lthreshold
)
{
	unsigned j = 0;
	for (unsigned i = 0; i < n; ++i)
	{
		float l = csc_linmat_plane_point_distance2 (plane, (v3f32*)x);
		if (l < lthreshold)
		{
			j++;
		}
		x += 4;
	}
	return j;
}
