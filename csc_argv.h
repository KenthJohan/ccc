/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <inttypes.h>

#include "csc_basic.h"
#include "csc_debug.h"
#include "csc_type_str.h"
#include "csc_base64set.h"









static void csc_argv_convert_flag (enum csc_type type, union csc_union * dst, uint64_t flag)
{
	ASSERT_PARAM_NOTNULL (dst);
	switch (type)
	{
	case CSC_TYPE_U8:
		dst->val_u8 |= flag;
		break;
	case CSC_TYPE_U16:
		dst->val_u16 |= flag;
		break;
	case CSC_TYPE_U32:
		dst->val_u32 |= flag;
		break;
	case CSC_TYPE_U64:
		dst->val_u64 |= flag;
		break;
	default:
		break;
	}
}


static void csc_argv_convert_value (enum csc_type type, union csc_union * dst, char const * src)
{
	ASSERT_PARAM_NOTNULL (dst);
	ASSERT_PARAM_NOTNULL (src);
	char * endptr = NULL;
	switch (type)
	{
	case CSC_TYPE_STRING:
		dst->val_string = src;
		break;
	case CSC_TYPE_FLOAT:
		dst->val_float = strtod (src, &endptr);
		break;
	case CSC_TYPE_DOUBLE:
		dst->val_double = strtod (src, &endptr);
		break;
	case CSC_TYPE_U8:
		dst->val_u8 = strtoumax (src, &endptr, 0);
		break;
	case CSC_TYPE_U16:
		dst->val_u16 = strtoumax (src, &endptr, 0);
		break;
	case CSC_TYPE_U32:
		dst->val_u32 = strtoumax (src, &endptr, 0);
		break;
	case CSC_TYPE_U64:
		dst->val_u64 = strtoumax (src, &endptr, 0);
		break;
	case CSC_TYPE_INT:
		dst->val_int = strtoimax (src, &endptr, 0);
		break;
	case CSC_TYPE_LONG:
		dst->val_long = strtoimax (src, &endptr, 0);
		break;
	case CSC_TYPE_I8:
		dst->val_i8 = strtoimax (src, &endptr, 0);
		break;
	case CSC_TYPE_I16:
		dst->val_i16 = strtoimax (src, &endptr, 0);
		break;
	case CSC_TYPE_I32:
		dst->val_i32 = strtoimax (src, &endptr, 0);
		break;
	case CSC_TYPE_I64:
		dst->val_i64 = strtoimax (src, &endptr, 0);
		break;
	default:
		break;
	}
}


static void csc_argv_parse (char const * argv[], char name_char, char const * name_str, enum csc_type type, void * dst, uint64_t setflag, uint64_t flags)
{
	ASSERT_PARAM_NOTNULL (argv);
	ASSERT_PARAM_NOTNULL (name_str);
	ASSERT_PARAM_NOTNULL (dst);
	char const * s;
	argv--;
again:
	argv++;
	s = argv[0];
	if (s == NULL) {return;}
	if (s[0] != '-') {goto again;}

	//Handles long names: e.g: {"--read", "--write", "--year=2", "--name=Bob"}:
	if (s[1] == '-')
	{
		//Compare the long argument name with the long option name:
		int len = strlen (name_str);
		int diff = strncmp (s+2, name_str, len);
		if (diff){goto again;}
		//Point s to the value:
		s += len + 2 + 1;
		//Handles long name with values: e.g: {"--year=2", "--name=Bob"}:
		if (s[-1] == '=')
		{
			csc_argv_convert_value (type, (union csc_union*)dst, s);
			return;
		}
		//Handles long name for flags: e.g: {"--read", "--write"}:
		else if (s[-1] == '\0' && setflag)
		{
			csc_argv_convert_flag (type, (union csc_union*)dst, setflag);
			return;
		}
	}
	//Handles short names for flags: e.g: {"-x", "-rw", -rWarren}:
	else if (setflag)
	{
		//Make sure every character in the argument is part of a flag:
		uint64_t a = csc_base64set_fromstr (s+1);
		if ((a & flags) == 0) {goto again;}
		//Check if this flag matches a character in the argument:
		uint64_t b = csc_base64set_fromchar (name_char);
		if ((b & a) == 0) {goto again;}
		csc_argv_convert_flag (type, (union csc_union*)dst, setflag);
		return;
	}
	//Handles short names for values: e.g: {"-aHello", "-a", "Hello"}:
	else if (s[1] == name_char)
	{
		//Check if value exist in current argument:
		if (s[2]){s += 2;}
		//Check if the value exist in the next argument:
		else if (argv[1]) {s = argv[1];}
		else {goto again;}
		csc_argv_convert_value (type, (union csc_union*)dst, s);
		return;
	}
	goto again;
}







//Expanded functionality:
enum csc_argv_type
{
	CSC_ARGV_GROUP = CSC_TYPE_RESERVED0
};
#define CSC_ARGV_END 0, NULL, CSC_TYPE_NONE, NULL, 0, NULL
#define CSC_ARGV_DEFINE_GROUP(x) 0, NULL, (enum csc_type)CSC_ARGV_GROUP, NULL, 0, (x)


struct csc_argv_option
{
	char c;
	char const * s;
	enum csc_type t;
	void * v;
	uint64_t f;
	char const * d;
};


/**
 * @brief Parse all options
 * @param[in]      argv     The argv from the main function: main(int argc, char const * argv[])
 * @param[in,out]  options  A array of options
 */
static void csc_argv_parseall (char const * argv[], struct csc_argv_option * options)
{
	ASSERT_PARAM_NOTNULL (argv);
	ASSERT_PARAM_NOTNULL (options);

	//Find all characters that is used for flags:
	uint64_t flagmask = 0;
	for (struct csc_argv_option * o = options; o->t != CSC_TYPE_NONE; ++o)
	{
		if (o->f)
		{
			flagmask |= csc_base64set_fromchar (o->c);
		}
	}

	//Go through all options:
	for (struct csc_argv_option * o = options; o->t != CSC_TYPE_NONE; ++o)
	{
		if (o->t != CSC_ARGV_GROUP)
		{
			csc_argv_parse (argv, o->c, o->s, o->t, o->v, o->f, flagmask);
		}
	}
}


/**
 * @brief Print all options
 * @param[in] options  A array of options
 * @param[in] f        The file to print to
 */
static void csc_argv_description0 (struct csc_argv_option const * option, FILE * f)
{
	ASSERT_PARAM_NOTNULL (option);
	ASSERT_PARAM_NOTNULL (f);
	fputc ('\n', f);
	for (struct csc_argv_option const * o = option; o->t != CSC_TYPE_NONE; ++o)
	{
		switch (o->t)
		{
		case CSC_ARGV_GROUP:
			fprintf (stdout, "%s\n", o->d);
			break;
		default:
			if (o->f)
			{
				fprintf (f, " -%c --%-20.20s: [%s] %s\n", o->c, o->s, csc_type_tostr (o->t), o->d);
			}
			else
			{
				fprintf (f, " -%c --%-20.20s: (%s) %s\n", o->c, o->s, csc_type_tostr (o->t), o->d);
			}
			break;
		}
	}
	fputc ('\n', f);
}


/**
 * @brief Print all options with current value
 * @param[in] options  A array of options
 * @param[in] f        The file to print to
 */
static void csc_argv_description1 (struct csc_argv_option const * option, FILE * f)
{
	ASSERT_PARAM_NOTNULL (option);
	ASSERT_PARAM_NOTNULL (f);
	fputc ('\n', stdout);
	for (struct csc_argv_option const * o = option; o->t != CSC_TYPE_NONE; ++o)
	{
		switch (o->t)
		{
		case CSC_ARGV_GROUP:
			fprintf (stdout, "%s\n", o->d);
			break;
		default:
			fprintf (stdout, " -%c --%-20.20s", o->c, o->s);
			break;
		}
		if (o->f == 0)
		{
			csc_type_print (o->t, o->v, f);
		}
		else
		{
			csc_type_printflag (o->t, o->v, o->f);
		}

	}
	fputc ('\n', stdout);
}
