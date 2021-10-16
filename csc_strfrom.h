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
#include "csc_assert.h"


#define STRFROM_LOOKUP_0Z "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define STRFROM_LOOKUP_Z0 "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210"

/**
 * @brief Converts integer to string right align
 * @param o
 * @param n
 * @param value
 * @param base
 * @param pad
 */
static inline uint32_t strfrom_imax
(char * o, uint32_t n, intmax_t value, int base, char plus)
{
	ASSERT_PARAM_NOTNULL (o);
	ASSERT (base != 0);
	ASSERT (base < (int8_t)sizeof (STRFROM_LOOKUP_0Z));
	int rem;
	if (n == 0) {return n;}
	o += n;
	if (value < 0)
	{
		value = -value;
		plus = -plus;
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
		*o = STRFROM_LOOKUP_0Z [rem];
		if (value == 0) {break;}
	}
	if ((n > 0) && plus)
	{
		o --;
		n --;
		*o = (plus > 0) ? plus : '-';
	}
	/*
	while (1)
	{
		if (n == 0) {break;}
		o --;
		n --;
		*o = pad;
	}
	*/
	return n;
}



/**
 * @brief Converts integer to string right align
 * @param o
 * @param n
 * @param value
 * @param base
 * @param pad
 */
static inline void strfrom_umax
(char * o, uint32_t n, uintmax_t value, int base, char pad)
{
	ASSERT_PARAM_NOTNULL (o);
	ASSERT (base != 0);
	ASSERT (base < (int8_t)sizeof (STRFROM_LOOKUP_0Z));
	int rem;
	if (n == 0) {return;}
	o += n;
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
		*o = STRFROM_LOOKUP_0Z [rem];
		if (value == 0) {break;}
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



/*
static inline uint32_t strfrom_imax2
(char * o, uint32_t n, intmax_t value, int base, pad)
{
	char buf[64];
	strfrom_imax (buf, 64, value, base, pad);

}
*/










static void test_strfrom_imax(intmax_t value, int base, char plus, char pad, char const * expect_str, uint32_t expect_return)
{
	char s[100] = {'\0'};
	uint32_t n = strlen (expect_str);
	uint32_t m = strfrom_imax (s, n, value, base, plus);
	memset (s, pad, m);
	ASSERTF (m == expect_return, "%i == %i, %s, %s", n, expect_return, s, expect_str);
	ASSERTF (strncmp(s, expect_str, n) == 0, "%s == %s", s, expect_str);
}


static void test_strfrom_imax_cases()
{
	// Base 2
	test_strfrom_imax (/*value*/ 3, /*base*/  2, /*plus*/  0, /*pad*/'#', /*expect_str*/   "11", /*expect_return*/0);
	test_strfrom_imax (/*value*/ 3, /*base*/  2, /*plus*/  0, /*pad*/'#', /*expect_str*/  "#11", /*expect_return*/1);
	test_strfrom_imax (/*value*/ 3, /*base*/  2, /*plus*/  0, /*pad*/'#', /*expect_str*/ "##11", /*expect_return*/2);
	test_strfrom_imax (/*value*/ 3, /*base*/  2, /*plus*/'+', /*pad*/'#', /*expect_str*/  "+11", /*expect_return*/0);
	test_strfrom_imax (/*value*/ 3, /*base*/  2, /*plus*/'+', /*pad*/'#', /*expect_str*/ "#+11", /*expect_return*/1);
	test_strfrom_imax (/*value*/ 3, /*base*/  2, /*plus*/'+', /*pad*/'#', /*expect_str*/"##+11", /*expect_return*/2);

	// Base -10
	test_strfrom_imax (/*value*/10, /*base*/-10, /*plus*/  0, /*pad*/'#', /*expect_str*/   "190", /*expect_return*/0);
	test_strfrom_imax (/*value*/10, /*base*/-10, /*plus*/  0, /*pad*/'#', /*expect_str*/  "#190", /*expect_return*/1);
	test_strfrom_imax (/*value*/10, /*base*/-10, /*plus*/  0, /*pad*/'#', /*expect_str*/ "##190", /*expect_return*/2);
	test_strfrom_imax (/*value*/10, /*base*/-10, /*plus*/'+', /*pad*/'#', /*expect_str*/  "+190", /*expect_return*/0);
	test_strfrom_imax (/*value*/10, /*base*/-10, /*plus*/'+', /*pad*/'#', /*expect_str*/ "#+190", /*expect_return*/1);
	test_strfrom_imax (/*value*/10, /*base*/-10, /*plus*/'+', /*pad*/'#', /*expect_str*/"##+190", /*expect_return*/2);

	// Base 16
	test_strfrom_imax (/*value*/10, /*base*/16, /*plus*/  0, /*pad*/'#', /*expect_str*/   "A", /*expect_return*/0);
	test_strfrom_imax (/*value*/10, /*base*/16, /*plus*/  0, /*pad*/'#', /*expect_str*/  "#A", /*expect_return*/1);
	test_strfrom_imax (/*value*/10, /*base*/16, /*plus*/  0, /*pad*/'#', /*expect_str*/ "##A", /*expect_return*/2);
	test_strfrom_imax (/*value*/10, /*base*/16, /*plus*/'+', /*pad*/'#', /*expect_str*/  "+A", /*expect_return*/0);
	test_strfrom_imax (/*value*/10, /*base*/16, /*plus*/'+', /*pad*/'#', /*expect_str*/ "#+A", /*expect_return*/1);
	test_strfrom_imax (/*value*/10, /*base*/16, /*plus*/'+', /*pad*/'#', /*expect_str*/"##+A", /*expect_return*/2);
}
