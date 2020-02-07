/*
MIT License

Copyright (c) 2019 CSC Project

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "csc_basic.h"
#include "csc_debug.h"

#define CSC_STRNCMP_LITERAL(str1,str2) strncmp ((str1), (str2), sizeof ((str2))-1)

#define STR_SET_0Z "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define STR_SET_Z0 "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210"
#define STR_INB(x, a, b, base) INRANGE ((x), (a), MIN ((base) + (a), (b)))
#define STR_SIGNED (1 << 0)
#define STR_UNSIGNED (1 << 1)
#define STR_ALIGN_LEFT (1 << 2)
#define STR_ALIGN_RIGHT (1 << 3)
#define STR_ALIGN_MIDDLE (1 << 4)


__attribute__ ((unused))
static inline
intmax_t str_to_imax (char ** f, int base)
{
	intmax_t a = 0;
	int c;
	int neg = 0;
	c = (**f);
	if (c == '-') {neg = 1; (*f) ++;}
	else if (c == '+') {neg = 0; (*f) ++;}
	while (1)
	{
		c = (**f);
		if (c == '\0') {break;}
		else if (STR_INB (c, '0', '9', abs(base))) {c -= '0';}
		else if (abs(base) > 10 && STR_INB (c, 'a', 'z', abs(base))) {c -= ('a' - 10);}
		else if (abs(base) > 10 && STR_INB (c, 'A', 'Z', abs(base))) {c -= ('A' - 10);}
		else {break;}
		a *= (intmax_t) base;
		a += (intmax_t) c;
		(*f) ++;
	}
	if (neg) {return -a;}
	else {return a;}
}


__attribute__ ((unused))
static inline
uintmax_t str_to_umax (char ** f, int base)
{
	uintmax_t a = 0;
	int c;
	while (1)
	{
		c = (**f);
		if (c == '\0') {break;}
		else if (STR_INB (c, '0', '9', abs (base))) {c -= '0';}
		else if (abs (base) > 10 && STR_INB (c, 'a', 'z', abs (base))) {c -= ('a' - 10);}
		else if (abs (base) > 10 && STR_INB (c, 'A', 'Z', abs (base))) {c -= ('A' - 10);}
		else {break;}
		a *= (uintmax_t) base;
		a += (uintmax_t) c;
		(*f) ++;
	}
	return a;
}


__attribute__ ((unused))
static inline
uint32_t str_to_u32 (char ** f, int base)
{
	uintmax_t v = str_to_umax (f, base);
	return (uint32_t) v;
}


__attribute__ ((unused))
static inline
int32_t str_to_i32 (char ** f, int base)
{
	intmax_t v = str_to_imax (f, base);
	return (int8_t) v;
}


__attribute__ ((unused))
static inline
int8_t str_to_i8 (char ** f, int base)
{
	intmax_t v = str_to_imax (f, base);
	return (int8_t) v;
}


__attribute__ ((unused))
static inline
uint8_t str_to_u8 (char ** f, int base)
{
	uintmax_t v = str_to_umax (f, base);
	return (uint8_t) v;
}


__attribute__ ((unused))
static inline
void str_rev (char * o, uint32_t n)
{
	char * e = o + n;
	n /= 2;
	while (n--)
	{
		e --;
		SWAPX (*o, *e);
		o ++;
	}
}


__attribute__ ((unused))
static inline
void str_rep (char * o, uint32_t n, char pad)
{
	while (n--)
	{
		*o = pad;
		o ++;
	}
}


__attribute__ ((unused))
static inline
void str_from_imax (char * o, uint32_t n, intmax_t value, int base, char pad)
{
	ASSERT_PARAM_NOTNULL (o);
	ASSERT (base != 0);
	ASSERT (base < (int8_t)sizeof (STR_SET_0Z));
	int rem;
	int negative = 0;
	if (n == 0) {return;}
	o += n;
	if (value < 0)
	{
		value = -value;
		negative = 1;
	}
	while (1)
	{
		if (n == 0) {break;}
		rem = value % base;
		value /= base;
		if (rem < 0)
		{
			rem += abs (base);
			value += 1;
		}
		o --;
		n --;
		*o = STR_SET_0Z [rem];
		if (value == 0) {break;}
	}
	if (n > 0 && negative)
	{
		o --;
		n --;
		*o = '-';
	}
	while (1)
	{
		if (n == 0) {break;}
		o --;
		n --;
		*o = pad;
	}
	return;
}


__attribute__ ((unused))
static inline
uint32_t str_from_imax2 (char * o, uint32_t n, intmax_t value, int base)
{
	ASSERT_PARAM_NOTNULL (o);
	ASSERT (base != 0);
	ASSERT (base < (int8_t)sizeof (STR_SET_0Z));
	int rem;
	uint32_t m = 0;
	if (m >= n) {return m;}
	if (value < 0)
	{
		value = -value;
		*o = '-';
		o ++;
		m ++;
	}
	else
	{
		*o = '+';
		o ++;
		m ++;
	}
	while (1)
	{
		if (m >= n) {break;}
		rem = value % base;
		value /= base;
		if (rem < 0)
		{
			rem += abs (base);
			value += 1;
		}
		*o = STR_SET_0Z [rem];
		o ++;
		m ++;
		if (value == 0) {break;}
	}
	str_rev (o-m+1, m-1);
	return m;
}


/*
returns positive when (str1) contains substring of (str2) seperated by (delimiters).
returns 0 otherwise.
eg:
	char * ext = strrchr (filename, '.');
	size_t n = csc_str_contains1 (ext, ".txt, .png ,.json", ", ");
*/
__attribute__ ((unused))
static inline
size_t csc_str_contains1 (char const * str1, char const * str2, char const * delimiters)
{
	if (str1 == NULL) {return 0;}
	if (str2 == NULL) {return 0;}
	size_t n = 0;
	while (1)
	{
		str2 += strspn (str2, delimiters);
		int end = ((*str1) == '\0') || ((*str2) == '\0');
		if (end) {break;}
		else if ((*str1) == (*str2))
		{
			str1 ++;
			str2 ++;
			n ++;
		}
		else
		{
			str1 -= n;
			n = 0;
			str2 += strcspn (str2, delimiters);
		}
	}
	return n;
}

/*
eg:
	char * ext1 = strrchr ("C:/docs/hello.txt", '.');
	char * ext0 = strrchr ("C:/docs/hello.txt", '/');
*/
__attribute__ ((unused))
static inline
int csc_str_next_cmp (char const ** p, int * col, char const * str)
{
	size_t l = strlen (str);
	int diff = strncmp (*p, str, l);
	if (diff == 0)
	{
		(*p) += l;
		(*col) += l;
		return (int) l;
	}
	return 0;
}


__attribute__ ((unused))
static inline
void csc_str_skip (char const ** p, int (*f)(int))
{
	while (f (**p)) {(*p)++;}
}


void csc_isindentifer (int a)
{
	return isalnum (a) || (a == '_');
}


__attribute__ ((unused))
static inline
int csc_next_indentifer (char const ** p, int * col)
{
	char const * q = (*p);
	csc_str_skip (p, csc_isindentifer);
	ptrdiff_t n = (*p) - q;
	(*col) += n;
	return (int)n;
}


__attribute__ ((unused))
static inline
int csc_next_literal (char const ** p, int * col)
{
	char const * q = (*p);
	csc_str_skip (p, isdigit);
	ptrdiff_t n = (*p) - q;
	(*col) += n;
	return (int)n;
}


__attribute__ ((unused))
static inline
void csc_str_print_hex_array (char * s, size_t sn, uint8_t * v, size_t vn, char const * format, size_t step)
{
	while (vn--)
	{
		if (sn < step) {break;}
		s += snprintf (s, step, format, *v);
		v ++;
		sn -= step;
	}
}


__attribute__ ((unused))
static inline
void str_skip_space (char ** p)
{
	while (isspace (**p))
	{
		(*p)++;
	}
}


__attribute__ ((unused))
static inline
void str_skip_alnum (char ** p)
{
	while (isalnum (**p))
	{
		(*p)++;
	}
}


__attribute__ ((unused))
static inline
void str_skip_until (char ** p, char * needles)
{
	while ((**p) && strchr (needles, **p) == NULL)
	{
		(*p)++;
	}
}


__attribute__ ((unused))
static inline
void str_skip_after (char ** p, char * needles)
{
	while ((**p) && strchr (needles, **p) != NULL)
	{
		(*p)++;
	}
}
