/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include "csc_basic.h"
#include "csc_str.h"
#include "csc_strto.h"
#include "csc_strfrom.h"





//format specifier
//%[width][u,i][size]_[base]
void strf_fmtv (char * o, uint32_t n, char const * f, va_list va)
{
	char const * g = o;
	uint32_t size = 0;
	uint32_t width = 0;
	enum csc_type type = CSC_TYPE_NONE;
	int base;
	union
	{
		intmax_t i;
		intmax_t u;
		float f;
		double d;
	} value;
	while (1)
	{
		//Look for format specifier starting with '%'
		switch (*f)
		{
		case '\0':
			goto end;
		case '%':
			f ++;
			break;
		default:
			*o = *f;
			o ++;
			n --;
			f ++;
			continue;
		}

		if (isdigit (*f))
		{
			width = strto_u32 (&f, 10);
		}

		//Look for signed or unsigned
		//Look for size
		switch (*f)
		{
		case 'u':
			f ++;
			type = CSC_TYPE_U;
			break;
		case 'i':
			f ++;
			type = CSC_TYPE_I;
			break;
		}

		if (isdigit (*f))
		{
			size = strto_u32 (&f, 10);
			type += size;
		}

		assert (sizeof (uint8_t) <= sizeof (unsigned));
		assert (sizeof (uint16_t) <= sizeof (unsigned));
		assert (sizeof (int8_t) <= sizeof (int));
		assert (sizeof (int16_t) <= sizeof (int));


		switch (*f)
		{
		case '_':
			f ++;
			base = (int) strto_i32 (&f, 10);
			break;
		default:
			base = 10;
			break;
		}



		uint32_t m = width ? MIN (n, width) : n;
		switch (type)
		{
		case CSC_TYPE_U:
		case CSC_TYPE_U8:
		case CSC_TYPE_U16:
		case CSC_TYPE_U32:
		case CSC_TYPE_U64:
			m = strfrom_umax (o, va_arg (va, uint64_t), m, base, '0');
			break;
		case CSC_TYPE_I:
		case CSC_TYPE_I8:
		case CSC_TYPE_I16:
		case CSC_TYPE_I32:
		case CSC_TYPE_I64:
			m = strfrom_imax (o, va_arg (va, int64_t), m, base, '0');
			break;
		default:
			ASSERT (0);
			break;
		}

		o += m;
		n -= m;

		//Apply padding
		if (width > m)
		{
			width = MIN (n, width - m);
			str_rep (o, width, '.');
			o += width;
			n -= width;
		}
	}
end:
	return;
}


void strf_fmt (char * o, uint32_t n, char const * f, ...)
{
	va_list va;
	va_start (va, f);
	strf_fmtv (o, n, f, va);
	va_end (va);
}


void strf_printf (char const * f, ...)
{
	va_list va;
	va_start (va, f);
	char buf [1000] = {'\0'};
	strf_fmtv (buf, 100, f, va);
	puts (buf);
	va_end (va);
}



