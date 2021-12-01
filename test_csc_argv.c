#include "csc_basic.h"
#include "csc_assert.h"
#include "csc_crossos.h"
#include "csc_argv.h"
#include "csc_type_str.h"
#include "csc_strf.h"


#define FLAG_DEFAULT 0x0001
#define FLAG_READ    0x0010
#define FLAG_WRITE   0x0020
#define FLAG_EXEC    0x0040
#define FLAG_EXEC2   0x0080
#define FLAG_A       0x0100
#define FLAG_B       0x0200
#define FLAG_C       0x0400
#define FLAG_D       0x0800


static void test_float1()
{
	char const * a[] =
	{
	"--duration=3",
	NULL,
	};
	float value = 1.0f;
	csc_argv_parse (a, 'd', "duration", CSC_TYPE_FLOAT, &value, 0);
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
	csc_argv_parse (a, 'd', "duration", CSC_TYPE_FLOAT, &value, 0);
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
	csc_argv_parse (a, 'd', "duration", CSC_TYPE_FLOAT, &value, 0);
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
	csc_argv_parse (a, 'a', "name", CSC_TYPE_STRING, &value, 0);
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
	csc_argv_parse (a, 'a', "name", CSC_TYPE_STRING, &value, 0);
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
	csc_argv_parse (a, 'a', "name", CSC_TYPE_STRING, &value, 0);
	ASSERT (strcmp("Bob", value) == 0);
}





static void test_expanded()
{
	char const * a[] ={"-xX","--read","-h","-a","192.168.0.1:4000",NULL};
	char * address = NULL;
	int threads = 0;
	uint32_t perm = 0;
	struct csc_argv_option o[] =
{
	{CSC_ARGV_DEFINE_GROUP("Options")},
	{'a', "address", CSC_TYPE_STRING, &address, 0, "The address"},
	{'j', "threads", CSC_TYPE_INT, &threads, 0, "Number of threads"},
	{CSC_ARGV_DEFINE_GROUP("Permissions")},
	{'r', "read", CSC_TYPE_U32, &perm, FLAG_READ, "Read"},
	{'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE, "Write"},
	{'x', "exec", CSC_TYPE_U32, &perm, FLAG_EXEC, "Exec"},
	{'X', "exec2", CSC_TYPE_U32, &perm, FLAG_EXEC2, "Exec2"},
	{CSC_ARGV_END}
};
	csc_argv_parseall (a, o);
	csc_argv_description0 (o, stdout);
	csc_argv_description1 (o, stdout);
}






static void test_flags1()
{
	char const * a[] =
	{
	"-r",
	NULL,
	};
	uint32_t perm = FLAG_DEFAULT;
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE);
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
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE);
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
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE);
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
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE);
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
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE);
	csc_argv_parse (a, 'x', "exec", CSC_TYPE_U32, &perm, FLAG_EXEC);
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
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE);
	csc_argv_parse (a, 'x', "exec", CSC_TYPE_U32, &perm, FLAG_EXEC);
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
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE);
	csc_argv_parse (a, 'x', "exec", CSC_TYPE_U32, &perm, FLAG_EXEC);
	ASSERT (perm == (FLAG_DEFAULT|FLAG_READ|FLAG_WRITE|FLAG_EXEC));
}

static void test_flags8()
{
	char const * a[] =
	{
	"-rx",
	"-xrb",
	"-cw",
	"-Dwwww",
	"-D4",
	NULL,
	};
	float value = 1.0f;
	uint32_t perm = FLAG_DEFAULT;
	csc_argv_parse (a, 'a', "a", CSC_TYPE_U32, &perm, FLAG_A);
	csc_argv_parse (a, 'b', "b", CSC_TYPE_U32, &perm, 0);
	csc_argv_parse (a, 'c', "c", CSC_TYPE_U32, &perm, FLAG_C);
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE);
	csc_argv_parse (a, 'x', "exec", CSC_TYPE_U32, &perm, FLAG_EXEC|FLAG_A);
	csc_argv_parse (a, 'D', "Duration", CSC_TYPE_FLOAT, &value, 0);
	csc_argv_parse (a, 'D', "Duration", CSC_TYPE_U32, &perm, FLAG_D);
	csc_argv_parse (a, '4', "4", CSC_TYPE_U32, &perm, FLAG_B);
	ASSERT (perm & FLAG_DEFAULT);
	ASSERT (perm & FLAG_EXEC);
	ASSERT (perm & FLAG_READ);
	ASSERT (perm & FLAG_WRITE);
	ASSERT (perm & FLAG_A);
	ASSERT (perm & FLAG_B);
	ASSERT (perm & FLAG_C);
	ASSERT (perm & FLAG_D);
	ASSERT (perm == (FLAG_DEFAULT|FLAG_EXEC|FLAG_READ|FLAG_WRITE|FLAG_A|FLAG_B|FLAG_C|FLAG_D));
	ASSERT (value == 4.0);
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
	"-xrw",
	NULL,
	};
	float value = 1.0f;
	uint32_t perm = 0;
	csc_argv_parse (a, 'd', "duration", CSC_TYPE_FLOAT, &value, 0);
	csc_argv_parse (a, 'r', "read", CSC_TYPE_U32, &perm, FLAG_READ);
	csc_argv_parse (a, 'w', "write", CSC_TYPE_U32, &perm, FLAG_WRITE);
	*/




	return EXIT_SUCCESS;
}
