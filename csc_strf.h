/* SPDX-License-Identifier: GPL-2.0 */
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




//format specifier
//%[width][u,i][size]_[base]
void strf_fmtv (char * o, uint32_t n, char const * f, va_list va)
{
	uint32_t flag = 0;
	uint32_t size = 0;
	uint32_t width = 0;
	int base;
	intmax_t value = 0;
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

		//Look if width is specified
		if (INRANGE (*f, '0', '9'))
		{
			flag |= STR_ALIGN_RIGHT;
			width = str_to_u32 (&f, 10);
		}

		//Look for signed or unsigned
		//Look for size
		switch (*f)
		{
		case 'u':
			f ++;
			flag |= STR_UNSIGNED;
			break;
		case 'i':
			f ++;
			flag |= STR_SIGNED;
			break;
		}

		if (*f == 'l') {flag |= STR_ALIGN_LEFT;}

		if (INRANGE (*f, '0', '9'))
		{
			size = str_to_u32 (&f, 10);
		}

		assert (sizeof (uint8_t) <= sizeof (unsigned));
		assert (sizeof (uint16_t) <= sizeof (unsigned));
		assert (sizeof (int8_t) <= sizeof (int));
		assert (sizeof (int16_t) <= sizeof (int));

		if (flag & STR_UNSIGNED)
		{
			switch (size)
			{
			case 8:  value = (intmax_t) va_arg (va, unsigned); break;
			case 16: value = (intmax_t) va_arg (va, unsigned); break;
			case 32: value = (intmax_t) va_arg (va, uint32_t); break;
			case 64: value = (intmax_t) va_arg (va, uint64_t); break;
			}
		}
		else if (flag & STR_SIGNED)
		{
			switch (size)
			{
			case 8:  value = (intmax_t) va_arg (va, int); break;
			case 16: value = (intmax_t) va_arg (va, int); break;
			case 32: value = (intmax_t) va_arg (va, int32_t); break;
			case 64: value = (intmax_t) va_arg (va, int64_t); break;
			}
		}

		switch (*f)
		{
		case '_':
			f ++;
			base = (int) str_to_i32 (&f, 10);
			break;
		default:
			base = 10;
			break;
		}


		uint32_t m = str_from_imax2 (o, MIN (n, width), value, base);
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
	char buf [100] = {'\0'};
	strf_fmtv (buf, 100, f, va);
	puts (buf);
	va_end (va);
}



