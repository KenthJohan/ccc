/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once
#include <stddef.h> //offsetof
#include <stdint.h> //offsetof

#include "csc_basic.h"
#include "csc_assert.h"


static uint64_t csc_base64set_fromchar (char a)
{
	if ('a' <= a && a <= 'z')
	{
		a -= 'a';
		a += 0;
		return UINT64_C(1) << a;
	}
	if ('A' <= a && a <= 'Z')
	{
		a -= 'A';
		a += 26;
		return UINT64_C(1) << a;
	}
	if ('0' <= a && a <= '9')
	{
		a -= '0';
		a += 26+26;
		return UINT64_C(1) << a;
	}
	return 0;
}



static uint64_t csc_base64set_fromstr (char const * str)
{
	ASSERT_PARAM_NOTNULL (str);
	uint64_t set = 0;
	while (*str)
	{
		uint64_t a = csc_base64set_fromchar (*str);
		set |= a;
		str++;
	}
	return set;
}



static void csc_base64set_tostr (uint64_t flag, char buf[64])
{
	ASSERT_PARAM_NOTNULL (buf);
	unsigned i = 0;
	for (char a = 'a'; a <= 'z'; ++a)
	{
		if (flag & csc_base64set_fromchar(a))
		{
			buf[i] = a;
			i++;
		}
	}
	for (char a = 'A'; a <= 'Z'; ++a)
	{
		if (flag & csc_base64set_fromchar(a))
		{
			buf[i] = a;
			i++;
		}
	}
	for (char a = '0'; a <= '9'; ++a)
	{
		if (flag & csc_base64set_fromchar(a))
		{
			buf[i] = a;
			i++;
		}
	}
}
