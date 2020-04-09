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

#include "csc_debug.h"
#include "csc_dlist.h"
#include "csc_basic.h"


unsigned hash8_buf (void const * buf, unsigned size8, unsigned max)
{
	ASSERT_ISPOW2 (max);
	unsigned key = 0;
	uint8_t const * b = buf;
	for (size_t i = 0; i < size8; i++)
	{
		key += b [i];
	}
	return key & (max - 1);
}


unsigned hash8_str (char const * buf, char const * buf_end, unsigned max)
{
	ASSERT_ISPOW2 (max);
	unsigned key = 0;
	while (*buf)
	{
		if (buf[0] == '\0') {break;}
		if (buf_end && (buf >= buf_end)) {break;}
		key += (unsigned)buf [0];
		buf ++;
	}
	return key & (max - 1);
}


unsigned hash8_str_ab (char const * a, char const * b, unsigned max)
{
	ASSERT_ISPOW2 (max);
	unsigned key = 0;
	while (1)
	{
		if (a[0] == '\0') {break;}
		if (a >= b) {break;}
		key += (unsigned)a[0];
		a ++;
	}
	return key & (max - 1);
}








