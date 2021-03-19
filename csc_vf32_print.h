/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include <stdio.h>
#include "csc_math.h"


static void vf32_print (FILE * f, float const x [], size_t n, char const * format)
{
	fprintf (f, "(");
	for (size_t i = 0; i < n; ++ i)
	{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
		fprintf (f, format, (double) x [i]);
#pragma GCC diagnostic pop
	}
	fprintf (f, "\b)\n");
	fflush (f);
}


static void vf32_print2 (FILE * f, float const x [], size_t n1, size_t n2, char const * format)
{
	for (size_t i = 0; i < n1; ++ i)
	{
		vf32_print (f, x + (i*n2), n2, format);
	}
}
