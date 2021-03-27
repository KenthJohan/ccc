/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"


static float f32_lerp1 (float a, float b, float t)
{
	return a + t * (b - a);
}


static float f32_lerp2 (float a, float b, float t)
{
	return (1 - t) * a + t * b;
}


//Linearly map (x) value from (A0 .. A1) to (B0 .. B1)
static float f32_linmap (float x, float a0, float a1, float b0, float b1)
{
	//cropping
	if (x < a0) {return b0;}
	if (x > a1) {return b1;}
	//calculate delta
	float da = a1 - a0;
	float db = b1 - b0;
	//zero division protection
	if (da == 0) {return b1;};
	//move to zero
	x = x - a0;
	x = x * db / da;
	//new offset
	x = x + b0;
	return x;
}




static float f32_deg_to_rad (float angle360)
{
	return angle360 * (M_PI/180.0f);
}


static float f32_rad_to_deg (float rad)
{
	return rad * (180.0f/M_PI);
}
