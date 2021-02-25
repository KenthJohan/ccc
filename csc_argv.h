/* SPDX-License-Identifier: GPL-2.0 */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <inttypes.h>

#ifndef ASSERT
#define ASSERT(x)
#endif

static uint64_t csc_argv_alphanumbits (char a)
{
	uint64_t o = 0;
	if ('a' <= a && a <= 'z')
	{
		o = a - 'a' + 0;
		o = UINT64_C(1) << o;
	}
	if ('A' <= a && a <= 'A')
	{
		o = a - 'A' + 26;
		o = UINT64_C(1) << o;
	}
	if ('0' <= a && a <= '9')
	{
		o = a - 'A' + 26 + 26;
		o = UINT64_C(1) << o;
	}
	return o;
}


static uint64_t csc_argv_alphanumbits_fromstr (char const * str)
{
	uint64_t set = 0;
	while (*str)
	{
		uint64_t a = csc_argv_alphanumbits (*str);
		set |= a;
		str++;
	}
	return set;
}


static void csc_argv_flag_get_str (uint64_t flag, char buf[64])
{
	unsigned i = 0;
	for (char a = 'a'; a <= 'z'; ++a)
	{
		if (flag & csc_argv_alphanumbits(a))
		{
			buf[i] = a;
			i++;
		}
	}
	for (char a = 'A'; a <= 'Z'; ++a)
	{
		if (flag & csc_argv_alphanumbits(a))
		{
			buf[i] = a;
			i++;
		}
	}
	for (char a = '0'; a <= '0'; ++a)
	{
		if (flag & csc_argv_alphanumbits(a))
		{
			buf[i] = a;
			i++;
		}
	}
}


enum csc_argv_type
{
	CSC_ARGV_TYPE_END,
	CSC_ARGV_TYPE_GROUP,
	CSC_ARGV_TYPE_STRING,
	CSC_ARGV_TYPE_FLOAT,
	CSC_ARGV_TYPE_DOUBLE,
	CSC_ARGV_TYPE_INT,
	CSC_ARGV_TYPE_LONG,
	CSC_ARGV_TYPE_U8,
	CSC_ARGV_TYPE_U16,
	CSC_ARGV_TYPE_U32,
	CSC_ARGV_TYPE_U64,
	CSC_ARGV_TYPE_I8,
	CSC_ARGV_TYPE_I16,
	CSC_ARGV_TYPE_I32,
	CSC_ARGV_TYPE_I64,
	/*
	CSC_ARGV_TYPE_FLAG_INT,
	CSC_ARGV_TYPE_FLAG_LONG,
	CSC_ARGV_TYPE_FLAG_U8,
	CSC_ARGV_TYPE_FLAG_U16,
	CSC_ARGV_TYPE_FLAG_U32,
	CSC_ARGV_TYPE_FLAG_U64,
	CSC_ARGV_TYPE_FLAG_I8,
	CSC_ARGV_TYPE_FLAG_I16,
	CSC_ARGV_TYPE_FLAG_I32,
	CSC_ARGV_TYPE_FLAG_I64,
	*/
};


union csc_argv_value
{
	char const * val_string;
	float val_float;
	double val_double;
	int val_int;
	long val_long;
	uint8_t val_u8;
	uint16_t val_u16;
	uint32_t val_u32;
	uint64_t val_u64;
	uintmax_t val_umax;
	int8_t val_i8;
	int16_t val_i16;
	int32_t val_i32;
	int64_t val_i64;
	intmax_t val_imax;
};


struct csc_argv_option
{
	char character;
	char const * name;
	enum csc_argv_type type;
	void * value;
	union csc_argv_value flag;
	char const * description;
};


static uint64_t csc_argv_build_flags (struct csc_argv_option const * option)
{
	uint64_t flags = 0;
	for (struct csc_argv_option const * o = option; o->type != CSC_ARGV_TYPE_END; ++o)
	{
		if (o->flag.val_umax == 0){continue;}
		switch (o->type)
		{
		case CSC_ARGV_TYPE_INT:
		case CSC_ARGV_TYPE_LONG:
		case CSC_ARGV_TYPE_U8:
		case CSC_ARGV_TYPE_U16:
		case CSC_ARGV_TYPE_U32:
		case CSC_ARGV_TYPE_U64:
		case CSC_ARGV_TYPE_I8:
		case CSC_ARGV_TYPE_I16:
		case CSC_ARGV_TYPE_I32:
		case CSC_ARGV_TYPE_I64:
			flags |= csc_argv_alphanumbits (o->character);
			break;
		default:
			break;
		}
	}
	return flags;
}


static char const * csc_argv_type_tostr (enum csc_argv_type t)
{
	switch (t)
	{
	case CSC_ARGV_TYPE_STRING:return "string";
	case CSC_ARGV_TYPE_INT:return "int";
	case CSC_ARGV_TYPE_LONG:return "long";
	case CSC_ARGV_TYPE_U8:return "u8";
	case CSC_ARGV_TYPE_U16:return "u16";
	case CSC_ARGV_TYPE_U32:return "u32";
	case CSC_ARGV_TYPE_U64:return "u64";
	case CSC_ARGV_TYPE_I8:return "i8";
	case CSC_ARGV_TYPE_I16:return "i16";
	case CSC_ARGV_TYPE_I32:return "i32";
	case CSC_ARGV_TYPE_I64:return "i64";
	case CSC_ARGV_TYPE_FLOAT:return "float";
	case CSC_ARGV_TYPE_DOUBLE:return "double";
		/*
	case CSC_ARGV_TYPE_FLAG_INT:return "flag_int";
	case CSC_ARGV_TYPE_FLAG_LONG:return "flag_long";
	case CSC_ARGV_TYPE_FLAG_U8:return "flag_u8";
	case CSC_ARGV_TYPE_FLAG_U16:return "flag_u16";
	case CSC_ARGV_TYPE_FLAG_U32:return "flag_u32";
	case CSC_ARGV_TYPE_FLAG_U64:return "flag_u64";
	case CSC_ARGV_TYPE_FLAG_I8:return "flag_i8";
	case CSC_ARGV_TYPE_FLAG_I16:return "flag_i16";
	case CSC_ARGV_TYPE_FLAG_I32:return "flag_i32";
	case CSC_ARGV_TYPE_FLAG_I64:return "flag_i64";
		*/
	default:return "unknown";
	}
}


static void csc_argv_convert_value (enum csc_argv_type type, void * des, union csc_argv_value value)
{
	switch (type)
	{
	case CSC_ARGV_TYPE_STRING:
		*(char const **)des = value.val_string;
		break;
	case CSC_ARGV_TYPE_U8:
		*(uint8_t *)des = value.val_umax;
		break;
	case CSC_ARGV_TYPE_U16:
		*(uint16_t *)des = value.val_umax;
		break;
	case CSC_ARGV_TYPE_U32:
		*(uint32_t *)des = value.val_umax;
		break;
	case CSC_ARGV_TYPE_U64:
		*(uint64_t *)des = value.val_umax;
		break;
	case CSC_ARGV_TYPE_INT:
		*(int *)des = value.val_imax;
		break;
	case CSC_ARGV_TYPE_LONG:
		*(long *)des = value.val_imax;
		break;
	case CSC_ARGV_TYPE_I8:
		*(int8_t *)des = value.val_imax;
		break;
	case CSC_ARGV_TYPE_I16:
		*(int16_t *)des = value.val_imax;
		break;
	case CSC_ARGV_TYPE_I32:
		*(int32_t *)des = value.val_imax;
		break;
	case CSC_ARGV_TYPE_I64:
		*(int64_t *)des = value.val_imax;
		break;
	case CSC_ARGV_TYPE_FLOAT:
		*(float *)des = value.val_double;
		break;
	case CSC_ARGV_TYPE_DOUBLE:
		*(double *)des = value.val_double;
		break;
	default:
		break;
	}
}


static void csc_argv_convert_flag (enum csc_argv_type type, void * des, union csc_argv_value flag)
{
	switch (type)
	{
	case CSC_ARGV_TYPE_INT:
		*(int*)des |= flag.val_int;
		break;
	case CSC_ARGV_TYPE_LONG:
		*(long*)des |= flag.val_int;
		break;
	case CSC_ARGV_TYPE_U8:
		*(uint8_t*)des |= flag.val_u8;
		break;
	case CSC_ARGV_TYPE_U16:
		*(uint16_t*)des |= flag.val_u16;
		break;
	case CSC_ARGV_TYPE_U32:
		*(uint32_t*)des |= flag.val_u32;
		break;
	case CSC_ARGV_TYPE_U64:
		*(uint64_t*)des |= flag.val_u64;
		break;
	case CSC_ARGV_TYPE_I8:
		*(int8_t*)des |= flag.val_i8;
		break;
	case CSC_ARGV_TYPE_I16:
		*(int16_t*)des |= flag.val_i16;
		break;
	case CSC_ARGV_TYPE_I32:
		*(int32_t*)des |= flag.val_i32;
		break;
	case CSC_ARGV_TYPE_I64:
		*(int64_t*)des |= flag.val_i64;
		break;
	default:
		break;
	}
}


static void csc_argv_print_value1 (enum csc_argv_type type, void * value)
{
	switch (type)
	{
	case CSC_ARGV_TYPE_END:
	case CSC_ARGV_TYPE_GROUP:
		break;

	case CSC_ARGV_TYPE_DOUBLE:
		fprintf (stdout, "%f\n", *(double*)value);
		break;
	case CSC_ARGV_TYPE_FLOAT:
		fprintf (stdout, "%f\n", *(float*)value);
		break;
	case CSC_ARGV_TYPE_INT:
		fprintf (stdout, "%i\n", *(int*)value);
		break;
	case CSC_ARGV_TYPE_LONG:
		fprintf (stdout, "%li\n", *(long*)value);
		break;
	case CSC_ARGV_TYPE_I8:
		fprintf (stdout, "%i\n", *(int8_t*)value);
		break;
	case CSC_ARGV_TYPE_I16:
		fprintf (stdout, "%i\n", *(int16_t*)value);
		break;
	case CSC_ARGV_TYPE_I32:
		fprintf (stdout, "%i\n", *(int32_t*)value);
		break;
	case CSC_ARGV_TYPE_I64:
		fprintf (stdout, "%ji\n", *(int64_t*)value);
		break;
	case CSC_ARGV_TYPE_U8:
		fprintf (stdout, "%u\n", *(uint8_t*)value);
		break;
	case CSC_ARGV_TYPE_U16:
		fprintf (stdout, "%u\n", *(uint16_t*)value);
		break;
	case CSC_ARGV_TYPE_U32:
		fprintf (stdout, "%u\n", *(uint32_t*)value);
		break;
	case CSC_ARGV_TYPE_U64:
		fprintf (stdout, "%ju\n", *(uint64_t*)value);
		break;
	case CSC_ARGV_TYPE_STRING:
		fprintf (stdout, "%s\n", *(char**)value);
		break;
	default:
		break;
	}
}


static void csc_argv_print_value2 (enum csc_argv_type type, void * value, union csc_argv_value flag)
{
	switch (type)
	{
	case CSC_ARGV_TYPE_INT:
		fprintf (stdout, "%s\n", (*(int*)value & flag.val_int) ? "True" : "False");
		break;
	case CSC_ARGV_TYPE_LONG:
		fprintf (stdout, "%s\n", (*(int*)value & flag.val_int) ? "True" : "False");
		break;
	case CSC_ARGV_TYPE_I8:
		fprintf (stdout, "%s\n", (*(int8_t*)value & flag.val_int) ? "True" : "False");
		break;
	case CSC_ARGV_TYPE_I16:
		fprintf (stdout, "%s\n", (*(int16_t*)value & flag.val_int) ? "True" : "False");
		break;
	case CSC_ARGV_TYPE_I32:
		fprintf (stdout, "%s\n", (*(int32_t*)value & flag.val_int) ? "True" : "False");
		break;
	case CSC_ARGV_TYPE_I64:
		fprintf (stdout, "%s\n", (*(int64_t*)value & flag.val_int) ? "True" : "False");
		break;
	case CSC_ARGV_TYPE_U8:
		fprintf (stdout, "%s\n", (*(uint8_t*)value & flag.val_int) ? "True" : "False");
		break;
	case CSC_ARGV_TYPE_U16:
		fprintf (stdout, "%s\n", (*(uint16_t*)value & flag.val_int) ? "True" : "False");
		break;
	case CSC_ARGV_TYPE_U32:
		fprintf (stdout, "%s\n", (*(uint32_t*)value & flag.val_int) ? "True" : "False");
		break;
	case CSC_ARGV_TYPE_U64:
		fprintf (stdout, "%s\n", (*(uint64_t*)value & flag.val_int) ? "True" : "False");
		break;
	default:
		break;
	}
}


void csc_argv_print_value (struct csc_argv_option const * option)
{
	ASSERT (option);
	fputc ('\n', stdout);
	for (struct csc_argv_option const * o = option; o->type != CSC_ARGV_TYPE_END; ++o)
	{
		switch (o->type)
		{
		case CSC_ARGV_TYPE_GROUP:
			fprintf (stdout, "%s\n", o->description);
			break;
		default:
			fprintf (stdout, " -%c --%-20.20s", o->character, o->name);
			break;
		}
		if (o->flag.val_umax == 0)
		{
			csc_argv_print_value1 (o->type, o->value);
		}
		else
		{
			csc_argv_print_value2 (o->type, o->value, o->flag);
		}

	}
	fputc ('\n', stdout);
}


static int csc_argv_parse (struct csc_argv_option const * option, char const * arg, uint64_t flags)
{
	int errorcode = 0;
	if (arg[0] != '-'){return 1;}
	if (arg[1] == '\0'){return 1;}
	char * endptr = NULL;
	char const * strvalue = arg + 2;
	while (strvalue[0] == ' '){strvalue++;}
	for (struct csc_argv_option const * o = option; o->type != CSC_ARGV_TYPE_END; ++o)
	{
		if (arg[1] == '-')
		{
			if (o->name == NULL){continue;}
			if (arg[2] == '\0'){continue;}
			int len = strlen (o->name);
			if (strncmp (o->name, arg+2, len)){continue;}
			//--name=strvalue, +2 for double dash (--), +len for longname length, +1 for equal sign (=)
			strvalue = arg + 2 + len + 1;
			if ((o->flag.val_umax == 0) && (strvalue[-1] != '=')){continue;}
		}
		else if (o->character == 0){continue;}
		else if (o->flag.val_umax == 0 && arg[1] != o->character){continue;};


		if (o->flag.val_umax != 0)
		{
			//Check e.g. "r" against "wr".
			uint64_t a = csc_argv_alphanumbits (arg[1]);
			uint64_t b = csc_argv_alphanumbits_fromstr (arg+1);
			uint64_t c = csc_argv_alphanumbits (o->character);
			if ((a & b & c & flags) == 0)
			{
				continue;
			}
			csc_argv_convert_flag (o->type, o->value, o->flag);
		}
		else
		{
			union csc_argv_value value;
			switch (o->type)
			{
			case CSC_ARGV_TYPE_STRING:
				value.val_string = strvalue;
				break;
			case CSC_ARGV_TYPE_FLOAT:
			case CSC_ARGV_TYPE_DOUBLE:
				value.val_double = strtod (strvalue, &endptr);
				break;
			case CSC_ARGV_TYPE_U8:
			case CSC_ARGV_TYPE_U16:
			case CSC_ARGV_TYPE_U32:
			case CSC_ARGV_TYPE_U64:
				value.val_umax = strtoumax (strvalue, &endptr, 0);
				break;
			case CSC_ARGV_TYPE_INT:
			case CSC_ARGV_TYPE_LONG:
			case CSC_ARGV_TYPE_I8:
			case CSC_ARGV_TYPE_I16:
			case CSC_ARGV_TYPE_I32:
			case CSC_ARGV_TYPE_I64:
				value.val_imax = strtoimax (strvalue, &endptr, 0);
				break;
			default:
				continue;
			}
			csc_argv_convert_value (o->type, o->value, value);
		}


	}
	return errorcode;
}


void csc_argv_print_description (struct csc_argv_option const * option)
{
	ASSERT (option);
	fputc ('\n', stdout);
	for (struct csc_argv_option const * o = option; o->type != CSC_ARGV_TYPE_END; ++o)
	{
		switch (o->type)
		{
		case CSC_ARGV_TYPE_GROUP:
			fprintf (stdout, "%s\n", o->description);
			break;
		default:
			fprintf (stdout, " -%c --%-20.20s: %s (%s)\n", o->character, o->name, o->description, csc_argv_type_tostr (o->type));
			break;
		}
	}
	fputc ('\n', stdout);
}


int csc_argv_parsev (struct csc_argv_option const option[], char const * argv[])
{
	int errorcode = 0;
	uint64_t flags = csc_argv_build_flags (option);
	for (char const ** a = argv; *a != NULL; ++a)
	{
		if (csc_argv_parse (option, *a, flags))
		{
			errorcode = 1;
		};
	}
	return errorcode;
}




