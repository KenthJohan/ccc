/* SPDX-License-Identifier: GPL-2.0 */
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
			csc_argv_convert_value (type, dst, s);
			return;
		}
		//Handles long name for flags: e.g: {"--read", "--write"}:
		else if (s[-1] == '\0' && setflag)
		{
			csc_argv_convert_flag (type, dst, setflag);
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
		if ((csc_base64set_fromchar (name_char) & a) == 0) {goto again;}
		csc_argv_convert_flag (type, dst, setflag);
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
		csc_argv_convert_value (type, dst, s);
		return;
	}
	goto again;
}







//Expanded functionality:
enum csc_argv_type
{
	CSC_ARGV_GROUP = CSC_TYPE_RESERVED0
};
#define CSC_ARGV_END {0, NULL, CSC_TYPE_NONE, NULL, 0, NULL}
#define CSC_ARGV_DEFINE_GROUP(x) {0, NULL, (enum csc_type)CSC_ARGV_GROUP, NULL, 0, (x)}


struct csc_argv_option
{
	char c;
	char const * s;
	enum csc_type t;
	void * v;
	uint64_t f;
	char * d;
};


static void csc_argv_parseall (char const * argv[], struct csc_argv_option * options)
{
	ASSERT_PARAM_NOTNULL (argv);
	ASSERT_PARAM_NOTNULL (options);
	uint64_t flagmask = 0;
	for (struct csc_argv_option * o = options; o->t != CSC_TYPE_NONE; ++o)
	{
		flagmask |= csc_base64set_fromchar (o->c);
	}
	for (struct csc_argv_option * o = options; o->t != CSC_TYPE_NONE; ++o)
	{
		if (o->t != CSC_ARGV_GROUP)
		{
			csc_argv_parse (argv, o->c, o->s, o->t, o->v, o->f, flagmask);
		}
	}
}


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









/*


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



static int csc_argv_parse (struct csc_argv_option const * option, char const * arg0, char const * arg1, uint64_t flags)
{
	int errorcode = 0;
	char * endptr = NULL;
	char const * strvalue;

	if (arg0[0] != '-'){return 1;}
	if (arg0[1] == '\0'){return 1;}
	if (arg0[1] == '-')
	{

	}

	strvalue = arg0 + 2;

	if (strvalue[0] == ' ')
	{
		if (arg1 == NULL){return errorcode;}
		if (isalnum (arg1[0]) == 0){return errorcode;}
		strvalue = arg1;
	}
	for (struct csc_argv_option const * o = option; o->type != CSC_ARGV_TYPE_END; ++o)
	{
		if (arg0[1] == '-')
		{
			if (o->name == NULL){continue;}
			if (arg0[2] == '\0'){continue;}
			int len = strlen (o->name);
			if (strncmp (o->name, arg0+2, len)){continue;}
			//--name=strvalue, +2 for double dash (--), +len for longname length, +1 for equal sign (=)
			strvalue = arg0 + 2 + len + 1;
			if ((o->flag.val_umax == 0) && (strvalue[-1] != '=')){continue;}
			if ((o->flag.val_umax != 0) && (strvalue[-1] != '='))
			{
				csc_argv_convert_flag (o->type, o->value, o->flag);
				continue;
			}
		}
		else if (o->character == 0){continue;}
		else if (o->flag.val_umax == 0 && arg0[1] != o->character){continue;};

		//When this option is a flag:
		if (o->flag.val_umax != 0)
		{
			//Check e.g. "r" against "wr".
			uint64_t a = csc_argv_alphanumbits (arg0[1]);
			if ((a & flags) == 0) {continue;} //Make sure the first arg char is a flag.
			uint64_t b = csc_argv_alphanumbits_fromstr (arg0+1);
			uint64_t c = csc_argv_alphanumbits (o->character);
			if ((b & c & flags) == 0) {continue;} //Check if current option matches any char in arg
			csc_argv_convert_flag (o->type, o->value, o->flag);
		}
		//When this option is not a flag:
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
		if (csc_argv_parse (option, a[0], a[1], flags))
		{
			errorcode = 1;
		};
	}
	return errorcode;
}

*/


