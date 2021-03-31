/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdio.h>
#include "csc_math.h"

#define CSC_V3F32_PRINT_RGB TCOLF("1")"%+5.3E"TCOLF("2")"%+5.3E"TCOLF("4")"%+5.3E"

static void csc_v3f32_print_rgb (FILE * f, struct v3f32 * v)
{
	char const * fmt = CSC_V3F32_PRINT_RGB"\n"TCOL_RST;
	fprintf (f, fmt, v->x, v->y, v->z);
}
