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
static float sf32_linmap (float X, float A0, float A1, float B0, float B1)
{
	//cropping
	if (X < A0) {return B0;}
	if (X > A1) {return B1;}
	//calculate delta
	float DA;
	float DB;
	DA = A1 - A0;
	DB = B1 - B0;
	//move to zero
	X = X - A0;
	//zero division protection
	if (DA == 0) {return B1;};
	X = X / DA;
	//new scale
	X = X * DB;
	//new offset
	X = X + B0;
	return X;
}
