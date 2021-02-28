#include "csc_debug.h"
#include "csc_crossos.h"
#include "csc_argv.h"
#include "csc_basic.h"
#include "csc_type_str.h"
#include "csc_base64set.h"


#define FLAG_READ    0x01
#define FLAG_WRITE   0x02
#define FLAG_EXEC    0x04
#define FLAG_EXEC2   0x08
#define FLAG_DEFAULT 0x10


static void test_float1()
{
	char const * a[] =
	{
	"--duration=3",
	NULL,
	};
	float value = 1.0f;
	csc_argv_parse (a, 'd', "duration", CSC_TYPE_FLOAT, &value, 0, 0);
	ASSERT (value == 3.0);
}

static void test_float2()
{
	char const * a[] =
	{
	"-d3",
	NULL,
	};
	float value = 1.0f;
	csc_argv_parse (a, 'd', "duration", CSC_TYPE_FLOAT, &value, 0, 0);
	ASSERT (value == 3.0);
}

static void test_float3()
{
	char const * a[] =
	{
	"-d",
	"4",
	NULL,
	};
	float value = 1.0f;
	csc_argv_parse (a, 'd', "duration", CSC_TYPE_FLOAT, &value, 0, 0);
	ASSERT (value == 4.0);
}

static void test_string1()
{
	char const * a[] =
	{
	"-aBob",
	NULL,
	};
	char * value;
	csc_argv_parse (a, 'a', "name", CSC_TYPE_STRING, &value, 0, 0);
	ASSERT (strcmp("Bob", value) == 0);
}

static void test_string2()
{
	char const * a[] =
	{
	"-a",
	"Bob",
	NULL,
	};
	char * value;
	csc_argv_parse (a, 'a', "name", CSC_TYPE_STRING, &value, 0, 0);
	ASSERT (strcmp("Bob", value) == 0);
}

static void test_string3()
{
	char const * a[] =
	{
	"--name=Bob",
	NULL,
	};
	char * value;
	csc_argv_parse (a, 'a', "name", CSC_TYPE_STRING, &value, 0, 0);
	ASSERT (strcmp("Bob", value) == 0);
}

static void test_flags1()
{
	char const * a[] =
	{
	"-r",
	NULL,
	};
	uint32_t perm = FLAG_DEFAULT;
	uint64_t flagmask = csc_base64set_fromstr ("rw");
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ, flagmask);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE, flagmask);
	ASSERT (perm == (FLAG_DEFAULT|FLAG_READ));
}

static void test_flags2()
{
	char const * a[] =
	{
	"-rw",
	NULL,
	};
	uint32_t perm = FLAG_DEFAULT;
	uint64_t flagmask = csc_base64set_fromstr("rw");
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ, flagmask);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE, flagmask);
	ASSERT (perm == (FLAG_DEFAULT|FLAG_READ|FLAG_WRITE));
}

static void test_flags3()
{
	char const * a[] =
	{
	"-wr",
	NULL,
	};
	uint32_t perm = FLAG_DEFAULT;
	uint64_t flagmask = csc_base64set_fromstr("rw");
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ, flagmask);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE, flagmask);
	ASSERT (perm == (FLAG_DEFAULT|FLAG_READ|FLAG_WRITE));
}

static void test_flags4()
{
	char const * a[] =
	{
	"-r",
	"-w",
	NULL,
	};
	uint32_t perm = FLAG_DEFAULT;
	uint64_t flagmask = csc_base64set_fromstr("rw");
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ, flagmask);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE, flagmask);
	ASSERT (perm == (FLAG_DEFAULT|FLAG_READ|FLAG_WRITE));
}

static void test_flags5()
{
	char const * a[] =
	{
	"-rw",
	"-x",
	NULL,
	};
	uint32_t perm = FLAG_DEFAULT;
	uint64_t flagmask = csc_base64set_fromstr("rwx");
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ, flagmask);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE, flagmask);
	csc_argv_parse (a, 'x', "exec", CSC_TYPE_U32, &perm, FLAG_EXEC, flagmask);
	ASSERT (perm == (FLAG_DEFAULT|FLAG_READ|FLAG_WRITE|FLAG_EXEC));
}

static void test_flags6()
{
	char const * a[] =
	{
	"-xrw",
	NULL,
	};
	uint32_t perm = FLAG_DEFAULT;
	uint64_t flagmask = csc_base64set_fromstr("rwx");
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ, flagmask);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE, flagmask);
	csc_argv_parse (a, 'x', "exec", CSC_TYPE_U32, &perm, FLAG_EXEC, flagmask);
	ASSERT (perm == (FLAG_DEFAULT|FLAG_READ|FLAG_WRITE|FLAG_EXEC));
}

static void test_flags7()
{
	char const * a[] =
	{
	"-x",
	"-r",
	"-w",
	NULL,
	};
	uint32_t perm = FLAG_DEFAULT;
	uint64_t flagmask = csc_base64set_fromstr("rwx");
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ, flagmask);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE, flagmask);
	csc_argv_parse (a, 'x', "exec", CSC_TYPE_U32, &perm, FLAG_EXEC, flagmask);
	ASSERT (perm == (FLAG_DEFAULT|FLAG_READ|FLAG_WRITE|FLAG_EXEC));
}

static void test_flags8()
{
	char const * a[] =
	{
	"-x",
	"-r",
	"-w",
	NULL,
	};
	uint32_t perm = FLAG_DEFAULT;
	uint64_t flagmask = csc_base64set_fromstr("r");
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ, flagmask);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE, flagmask);
	csc_argv_parse (a, 'x', "exec", CSC_TYPE_U32, &perm, FLAG_EXEC, flagmask);
	ASSERT (perm == (FLAG_DEFAULT|FLAG_READ));
}



static void test_expanded()
{
	char const * a[] ={"-xX","--read","-h","-a","192.168.0.1:4000",NULL};
	char * address = NULL;
	int threads = 0;
	uint32_t perm = 0;
	struct csc_argv_option o[] =
	{
	CSC_ARGV_DEFINE_GROUP("Options"),
	{'a', "address", CSC_TYPE_STRING, &address, 0, "The address"},
	{'j', "threads", CSC_TYPE_INT, &threads, 0, "Number of threads"},
	CSC_ARGV_DEFINE_GROUP("Permissions"),
	{'r', "read", CSC_TYPE_U32, &perm, FLAG_READ, "Read"},
	{'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE, "Write"},
	{'x', "exec", CSC_TYPE_U32, &perm, FLAG_EXEC, "Exec"},
	{'X', "exec2", CSC_TYPE_U32, &perm, FLAG_EXEC2, "Exec2"},
	CSC_ARGV_END
};
//
csc_argv_parseall (a, o);
csc_argv_description0 (o, stdout);
csc_argv_description1 (o, stdout);
}


int main (int argc, char const * argv [])
{
	csc_crossos_enable_ansi_color();
	ASSERT (argc);
	ASSERT (argv);


	test_float1();
	test_float2();
	test_float3();
	test_string1();
	test_string2();
	test_string3();
	test_flags1();
	test_flags2();
	test_flags3();
	test_flags4();
	test_flags5();
	test_flags6();
	test_flags7();
	test_flags8();
	test_expanded();

	/*
	char const * a [4] =
	{
	"--duration=3",
	NULL,
	};
	float value = 1.0f;
	uint32_t perm = 0;
	uint64_t flagmask = csc_argv_alphanumbits_fromstr("rw");
	csc_argv_parse (a, 'd', "duration", CSC_TYPE_FLOAT, &value, 0, 0);
	ASSERT (value == 3.0);
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ, flagmask);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE, flagmask);
	*/



	return EXIT_SUCCESS;
}
