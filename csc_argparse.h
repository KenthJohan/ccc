#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include "csc_debug.h"
#include "csc_basic.h"


static uint64_t csc_argv_alphanumbits (char a)
{
	if ('a' <= a && a <= 'z')
	{
		int o = a - 'a' + 0;
		return 1 << o;
	}
	if ('A' <= a && a <= 'A')
	{
		int o = a - 'A' + 26;
		return 1 << o;
	}
	if ('0' <= a && a <= '9')
	{
		int o = a - 'A' + 26 + 26;
		return 1 << o;
	}
	return 0;
}

static uint64_t csc_argv_alphanumbits_fromstr (char const * str)
{
	uint64_t set = 0;
	while (*str)
	{
		set |= csc_argv_alphanumbits (*str);
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

	CSC_ARGV_TYPE_STRING,

	CSC_ARGV_TYPE_FLOAT,
	CSC_ARGV_TYPE_DOUBLE,
	CSC_ARGV_TYPE_INT,
	CSC_ARGV_TYPE_LONG,


	CSC_ARGV_TYPE_FLAG_INT,
	CSC_ARGV_TYPE_FLAG_LONG,
};


struct csc_argv_option
{
	char prefix;
	char const * longname;
	enum csc_argv_type type;
	void * value;
	union
	{
		int flag_int;
	};
};

static uint64_t csc_argv_build_flags (struct csc_argv_option const * option)
{
	uint64_t flags = 0;
	for (struct csc_argv_option const * o = option; o->type != CSC_ARGV_TYPE_END; ++o)
	{
		switch (o->type)
		{
		case CSC_ARGV_TYPE_FLAG_INT:
			flags |= csc_argv_alphanumbits (o->prefix);
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
	case CSC_ARGV_TYPE_FLOAT:return "float";
	case CSC_ARGV_TYPE_DOUBLE:return "double";
	case CSC_ARGV_TYPE_FLAG_INT:return "flag_int";
	case CSC_ARGV_TYPE_FLAG_LONG:return "flag_long";
	default:return "unknown";
	}
}


static void csc_argv_parse (struct csc_argv_option const * option, char const * arg)
{
	if (arg[0] != '-'){return;}
	if (arg[1] == '\0'){return;}
	char * endptr = NULL;
	char const * value = arg + 2;
	uint64_t flags = csc_argv_build_flags (option);
	for (struct csc_argv_option const * o = option; o->type != CSC_ARGV_TYPE_END; ++o)
	{
		if (arg[1] == '-')
		{
			if (o->longname == NULL){continue;}
			if (arg[2] == '\0'){continue;}
			int len = strlen (o->longname);
			if (strncmp (o->longname, arg+2, len)){continue;}
			value = arg + 3 + len;
		}
		else if (o->prefix == 0){continue;}
		else if (o->prefix != arg[1])
		{
			if ((csc_argv_alphanumbits_fromstr (arg+1) & flags & csc_argv_alphanumbits (o->prefix)) == 0){continue;}
			switch (o->type)
			{
			case CSC_ARGV_TYPE_FLAG_INT:
			case CSC_ARGV_TYPE_FLAG_LONG:
				break;
			default:
				continue;
			}
		}
		switch (o->type)
		{
		case CSC_ARGV_TYPE_STRING:
			*(char const **)o->value = value;
			break;
		case CSC_ARGV_TYPE_INT:
			*(int *)o->value = strtol (value, &endptr, 0);
			break;
		case CSC_ARGV_TYPE_LONG:
			*(long *)o->value = strtol (value, &endptr, 0);
			break;
		case CSC_ARGV_TYPE_FLOAT:
			*(float *)o->value = strtod (value, &endptr);
			break;
		case CSC_ARGV_TYPE_DOUBLE:
			*(double *)o->value = strtod (value, &endptr);
			break;
		case CSC_ARGV_TYPE_FLAG_INT:
			*(int*)o->value |= o->flag_int;
			break;
		case CSC_ARGV_TYPE_FLAG_LONG:
			*(long*)o->value |= o->flag_int;
			break;
		default:
			break;
		}

		if (endptr == NULL){}
		else if (endptr[0] == '\0'){}
		else if (endptr == value)
		{
			fprintf (stderr, "Error1 parse option (%c,%s,%s) argument (%s) value (%s)\n", o->prefix, o->longname, csc_argv_type_tostr (o->type), arg, value);
		}
		else if (1)
		{

			printf ("isalnum %i\n", isalnum (endptr[0]));
			fprintf (stderr, "Error2 parse option (%c,%s)\n", o->prefix, o->longname);
		}

	}
}



