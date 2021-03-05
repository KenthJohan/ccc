/* SPDX-License-Identifier: GPL-2.0 */
#pragma once
#include <stddef.h> //offsetof
#include <stdint.h> //offsetof

#include "csc_basic.h"
#include "csc_debug.h"


static uint64_t csc_base64set_fromchar (char a)
{
	uint64_t o = 0;
	if ('a' <= a && a <= 'z')
	{
		o -= 'a';
		o += 0;
		o = UINT64_C(1) << o;
	}
	if ('A' <= a && a <= 'Z')
	{
		o -= 'A';
		o += 26;
		o = UINT64_C(1) << o;
	}
	if ('0' <= a && a <= '9')
	{
		o -= '0';
		o += 26+26;
		o = UINT64_C(1) << o;
	}
	return o;
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
