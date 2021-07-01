/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include "csc_math.h"







static void v2f32_vertices6_set_rectangle (float v[], uint32_t stride, float x, float y, float w, float h)
{
	v[0] = x + 0; v[1] = y + h; v += stride;
	v[0] = x + w; v[1] = y + 0; v += stride;
	v[0] = x + 0; v[1] = y + 0; v += stride;
	v[0] = x + w; v[1] = y + 0; v += stride;
	v[0] = x + 0; v[1] = y + h; v += stride;
	v[0] = x + w; v[1] = y + h; v += stride;
}
