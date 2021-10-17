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
void strf_fmtv (char * buffer, uint32_t count, char const * f, va_list va)
{
	char * o = buffer;
	uint32_t n = count;
	uint32_t size = 0;
	uint32_t width = 0;
	char sign = 0;
	enum csc_type type = CSC_TYPE_NONE;
	int base = 0;
	char pad = 0;
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
parser:
		switch (f[0])
		{
		case 'w':
			pad = f[1];
			f += 2;
			width = strto_u32 (&f, 10);
			goto parser;

			break;
		case ' ':
		case '+':
			sign = f[0];
			f ++;
			goto parser;
			break;
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

		memset (o, pad, width);
		n -= width;

		uint32_t m = 0;
		uint32_t k = width ? width : n;
		switch (type)
		{
		case CSC_TYPE_U:
		case CSC_TYPE_U8:
		case CSC_TYPE_U16:
		case CSC_TYPE_U32:
			m = strfrom_umax (o, k, va_arg (va, uint32_t), base);
			break;
		case CSC_TYPE_U64:
			m = strfrom_umax (o, k, va_arg (va, uint64_t), base);
			break;
		case CSC_TYPE_I:
		case CSC_TYPE_I8:
		case CSC_TYPE_I16:
		case CSC_TYPE_I32:
			m = strfrom_imax (o, k, va_arg (va, int32_t), base, sign);
			break;
		case CSC_TYPE_I64:
			m = strfrom_imax (o, k, va_arg (va, int64_t), base, sign);
			break;
		default:
			ASSERT (0);
			break;
		}

		// .....000000000000
		//      o        ABC
		// .....ABC000000ABC
		//         o
		if (width == 0)
		{
			memmove (o, o + m, k - m);
			o += k - m;
		}
		else
		{
			o += width;
		}
		o[0] = '\0';



		//memset (o, '#', m);
		//o += m;
		//n -= m;

		/*
		//Apply padding
		if (width > m)
		{
			width = MIN (n, width - m);
			str_rep (o, width, '.');
			o += width;
			n -= width;
		}
		*/
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



static void test_csc_strf1 (char const * f, int32_t value, char const * expect)
{
	char buf[100+1] = {'#'};
	memset (buf, '#', sizeof(buf));
	strf_fmt (buf, 10, f, value);
	ASSERTF (strcmp (buf, expect) == 0, "%s == %s", buf, expect);
}


static void test_csc_strf()
{


	test_csc_strf1 ("% i32_10", 10, " 10");
	test_csc_strf1 ("% i32_-10", 10, " 190");
	test_csc_strf1 ("% i32_2", 10, " 1010");
	test_csc_strf1 ("%i32_10", 10, "10");
	test_csc_strf1 ("%i32_-10", 10, "190");
	test_csc_strf1 ("%i32_2", 10, "1010");
	test_csc_strf1 ("%+i32_10", 10, "+10");
	test_csc_strf1 ("%+i32_-10", 10, "+190");
	test_csc_strf1 ("%+i32_2", 10, "+1010");


	{
		char buf[100+1] = {'\0'};
		strf_fmt (buf, 100, "num: %w#12+i32_10", 345);
		puts (buf);
		strf_fmt (buf, 100, "num: %+i32_10", 345);
		puts (buf);
	}

}


