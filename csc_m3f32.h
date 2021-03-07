/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"

void m3f32_print (float M [3*3], FILE * f)
{
	mf32_print (M, 3, 3, f);
}






