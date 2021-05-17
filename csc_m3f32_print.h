/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdio.h>
#include "csc_math.h"



#define CSC_M3F32_PRINT_RGB1 \
	TCOLF("1")"%+5.3E"\
	TCOLF("2")"%+5.3E"\
	TCOLF("4")"%+5.3E\n"\
	TCOLF("1")"%+5.3E"\
	TCOLF("2")"%+5.3E"\
	TCOLF("4")"%+5.3E\n"\
	TCOLF("1")"%+5.3E"\
	TCOLF("2")"%+5.3E"\
	TCOLF("4")"%+5.3E"

#define CSC_M3F32_PRINT_RGB \
	TCOLF("1")"%+f"\
	TCOLF("2")"%+f"\
	TCOLF("4")"%+f\n"\
	TCOLF("1")"%+f"\
	TCOLF("2")"%+f"\
	TCOLF("4")"%+f\n"\
	TCOLF("1")"%+f"\
	TCOLF("2")"%+f"\
	TCOLF("4")"%+f"

static void csc_m3f32_print_rgb (FILE * f, m3f32 * m)
{
	char const * fmt = CSC_M3F32_PRINT_RGB"\n"TCOL_RST;
	fprintf (f, fmt, m->m11, m->m12, m->m13, m->m21, m->m22, m->m23, m->m31, m->m32, m->m33);
}
