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


/**
 * @brief Converts integer to string right align
 * @param o
 * @param n
 * @param value
 * @param base
 * @param pad
 */
static inline void strfrom_imax
(char * o, uint32_t n, intmax_t value, int base, char pad)
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


/**
 * @brief Converts integer to string left align
 * @param o
 * @param n
 * @param value
 * @param base
 * @return
 */
static inline uint32_t strfrom_imax2
(char * o, uint32_t n, intmax_t value, int base)
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
