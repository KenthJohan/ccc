/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "csc_str.h"
#include "csc_basic.h"
#include "csc_debug.h"



static inline intmax_t strto_imax (char const ** f, int base)
{
	ASSERT_PARAM_NOTNULL (f);
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



static inline uintmax_t strto_umax (char ** f, int base)
{
	ASSERT_PARAM_NOTNULL (f);
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


//TODO: base is not correct
static inline uintmax_t strto_umax_ab (char const * a, char const * b, int base)
{
	ASSERT_PARAM_NOTNULL (a);
	ASSERT_PARAM_NOTNULL (b);
	uintmax_t v = 0;
	int c;
	while (1)
	{
		c = a [0];
		if (a == b) {break;}
		if (c == '\0') {break;}
		else if (STR_INB (c, '0', '9', abs (base))) {c -= '0';}
		else if (abs (base) > 10 && STR_INB (c, 'a', 'z', abs (base))) {c -= ('a' - 10);}
		else if (abs (base) > 10 && STR_INB (c, 'A', 'Z', abs (base))) {c -= ('A' - 10);}
		else {break;}
		v *= (uintmax_t) base;
		v += (uintmax_t) c;
		a ++;
	}
	return v;
}



static inline uintmax_t str_to_umax_ab_adabase (char const * a, char const * b)
{
	ASSERT_PARAM_NOTNULL (a);
	ASSERT_PARAM_NOTNULL (b);
	char const * aa = a;
	int base = (int)strto_imax (&aa, 10);
	if (aa[0] == '#')
	{
		aa++;
		return strto_umax_ab (aa, b, base);
	}
	return strto_umax_ab (a, b, 10);
}



static inline uint32_t strto_u32 (char ** f, int base)
{
	ASSERT_PARAM_NOTNULL (f);
	uintmax_t v = strto_umax (f, base);
	return (uint32_t) v;
}



static inline int32_t strto_i32 (char const ** f, int base)
{
	ASSERT_PARAM_NOTNULL (f);
	intmax_t v = strto_imax (f, base);
	return (int8_t) v;
}



static inline int8_t strto_i8 (char const ** f, int base)
{
	ASSERT_PARAM_NOTNULL (f);
	intmax_t v = strto_imax (f, base);
	return (int8_t) v;
}



static inline uint8_t strto_u8 (char ** f, int base)
{
	ASSERT_PARAM_NOTNULL (f);
	uintmax_t v = strto_umax (f, base);
	return (uint8_t) v;
}
