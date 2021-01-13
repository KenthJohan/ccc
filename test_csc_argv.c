#include "csc_debug.h"
#include "csc_crossos.h"
#include "csc_argv.h"


#define FLAG_READ    0x01
#define FLAG_WRITE   0x02
#define FLAG_EXEC    0x04
#define FLAG_EXEC2   0x08
#define FLAG_DEFAULT 0x10


void test_strings()
{
	char * string1;
	char * string2;

	struct csc_argv_option option[] =
	{
	{.character = 'a', .name = "string1", .type = CSC_ARGV_TYPE_STRING, .value = &string1, .description = "The string value"},
	{.character = 'b', .name = "string2", .type = CSC_ARGV_TYPE_STRING, .value = &string2, .description = "The string value"},
	{.type = CSC_ARGV_TYPE_END}};

	string1 = NULL;
	csc_argv_parse (option, "-apotato", csc_argv_build_flags (option));
	ASSERT_NOTNULL (string1);
	ASSERT (strcmp (string1, "potato") == 0);

	string1 = NULL;
	csc_argv_parse (option, "-a tomato", csc_argv_build_flags (option));
	ASSERT_NOTNULL (string1);
	ASSERT (strcmp (string1, "tomato") == 0);

	string1 = NULL;
	csc_argv_parse (option, "--string1=carrot", csc_argv_build_flags (option));
	ASSERT_NOTNULL (string1);
	ASSERT (strcmp (string1, "carrot") == 0);

	string1 = NULL;
	csc_argv_parse (option, "--string1=lime", csc_argv_build_flags (option));
	ASSERT_NOTNULL (string1);
	ASSERT (strcmp (string1, "lime") == 0);

	string2 = NULL;
	csc_argv_parse (option, "-bonion", csc_argv_build_flags (option));
	ASSERT_NOTNULL (string2);
	ASSERT (strcmp (string2, "onion") == 0);

	string2 = NULL;
	csc_argv_parse (option, "-b cabbage", csc_argv_build_flags (option));
	ASSERT_NOTNULL (string2);
	ASSERT (strcmp (string2, "cabbage") == 0);

	string2 = NULL;
	csc_argv_parse (option, "--string2=garlic", csc_argv_build_flags (option));
	ASSERT_NOTNULL (string2);
	ASSERT (strcmp (string2, "garlic") == 0);
}

void test_numbers()
{
	float f;
	double d;
	long l;
	struct csc_argv_option option[] =
	{
	{.character = 'f', .name = "float",  .type = CSC_ARGV_TYPE_FLOAT,  .value = &f, .description = "The float value"},
	{.character = 'd', .name = "double", .type = CSC_ARGV_TYPE_DOUBLE, .value = &d, .description = "The double value"},
	{.character = 'l', .name = "long",   .type = CSC_ARGV_TYPE_LONG,   .value = &l, .description = "The number of threads value"},
	{.type = CSC_ARGV_TYPE_END}};

	f = 0.0f;
	csc_argv_parse (option, "-f3.3", csc_argv_build_flags (option));
	ASSERT (f == 3.3f);
	f = 0.0f;
	csc_argv_parse (option, "-f 3.3", csc_argv_build_flags (option));
	ASSERT (f == 3.3f);
	f = 0.0f;
	csc_argv_parse (option, "--float=3.3", csc_argv_build_flags (option));
	ASSERT (f == 3.3f);

	d = 0.0;
	csc_argv_parse (option, "-d3.4", csc_argv_build_flags (option));
	ASSERT (d == 3.4);
	d = 0.0;
	csc_argv_parse (option, "-d 3.4", csc_argv_build_flags (option));
	ASSERT (d == 3.4);
	d = 0.0;
	csc_argv_parse (option, "--double=3.4", csc_argv_build_flags (option));
	ASSERT (d == 3.4);


	l = 0;
	csc_argv_parse (option, "-l345", csc_argv_build_flags (option));
	ASSERT (l == 345);
	l = 0;
	csc_argv_parse (option, "-l 345", csc_argv_build_flags (option));
	ASSERT (l == 345);
	l = 0;
	csc_argv_parse (option, "--long=345", csc_argv_build_flags (option));
	ASSERT (l == 345);
}

void test_bitfield()
{
	int permission;
	struct csc_argv_option option[] =
	{
	{.character = 'r', .name = "read",     .type = CSC_ARGV_TYPE_INT,  .value = &permission, .flag.val_int = FLAG_READ,  .description = "The permission read flag"},
	{.character = 'w', .name = "write",    .type = CSC_ARGV_TYPE_INT,  .value = &permission, .flag.val_int = FLAG_WRITE, .description = "The permission write flag"},
	{.character = 'x', .name = "exec",     .type = CSC_ARGV_TYPE_INT,  .value = &permission, .flag.val_int = FLAG_EXEC,  .description = "The permission exec flag"},
	{.character = 'x', .name = "exec",     .type = CSC_ARGV_TYPE_INT,  .value = &permission, .flag.val_int = FLAG_EXEC2, .description = "The permission exec2 flag"},
	{.type = CSC_ARGV_TYPE_END}};

	permission = FLAG_DEFAULT;
	csc_argv_parse (option, "-r", csc_argv_build_flags (option));
	ASSERT (permission == (FLAG_DEFAULT|FLAG_READ));

	permission = FLAG_DEFAULT;
	csc_argv_parse (option, "-w", csc_argv_build_flags (option));
	ASSERT (permission == (FLAG_DEFAULT|FLAG_WRITE));

	permission = FLAG_DEFAULT;
	csc_argv_parse (option, "-r", csc_argv_build_flags (option));
	csc_argv_parse (option, "-w", csc_argv_build_flags (option));
	ASSERT (permission == (FLAG_DEFAULT|FLAG_READ|FLAG_WRITE));

	permission = FLAG_DEFAULT;
	csc_argv_parse (option, "-rw", csc_argv_build_flags (option));
	ASSERT (permission == (FLAG_DEFAULT|FLAG_READ|FLAG_WRITE));

	permission = FLAG_DEFAULT;
	csc_argv_parse (option, "-wr", csc_argv_build_flags (option));
	ASSERT (permission == (FLAG_DEFAULT|FLAG_READ|FLAG_WRITE));

	permission = FLAG_DEFAULT;
	csc_argv_parse (option, "--read", csc_argv_build_flags (option));
	csc_argv_parse (option, "--write", csc_argv_build_flags (option));
	ASSERT (permission == (FLAG_DEFAULT|FLAG_READ|FLAG_WRITE));

	permission = FLAG_DEFAULT;
	csc_argv_parse (option, "-x", csc_argv_build_flags (option));
	ASSERT (permission == (FLAG_DEFAULT|FLAG_EXEC|FLAG_EXEC2));

	permission = FLAG_DEFAULT;
	csc_argv_parse (option, "--exec", csc_argv_build_flags (option));
	ASSERT (permission == (FLAG_DEFAULT|FLAG_EXEC|FLAG_EXEC2));

	permission = FLAG_DEFAULT;
	csc_argv_parse (option, "--read", csc_argv_build_flags (option));
	csc_argv_parse (option, "--write", csc_argv_build_flags (option));
	csc_argv_parse (option, "--exec", csc_argv_build_flags (option));
	ASSERT (permission == (FLAG_DEFAULT|FLAG_READ|FLAG_WRITE|FLAG_EXEC|FLAG_EXEC2));

	permission = FLAG_DEFAULT;
	csc_argv_parse (option, "-wxr", csc_argv_build_flags (option));
	ASSERT (permission == (FLAG_DEFAULT|FLAG_READ|FLAG_WRITE|FLAG_EXEC|FLAG_EXEC2));
}


void test_mixing()
{
	int permission;
	int x;
	struct csc_argv_option option[] =
	{
	{.character = 'r', .name = "read",     .type = CSC_ARGV_TYPE_INT,  .value = &permission, .flag.val_int = FLAG_READ,  .description = "The permission read flag"},
	{.character = 'w', .name = "write",    .type = CSC_ARGV_TYPE_INT,  .value = &permission, .flag.val_int = FLAG_WRITE, .description = "The permission write flag"},
	{.character = 'x', .name = "exec",     .type = CSC_ARGV_TYPE_INT,  .value = &permission, .flag.val_int = FLAG_EXEC,  .description = "The permission exec flag"},
	{.character = 'x', .name = "exec",     .type = CSC_ARGV_TYPE_INT,  .value = &permission, .flag.val_int = FLAG_EXEC2, .description = "The permission exec2 flag"},
	{.character = 'x', .name = "x",        .type = CSC_ARGV_TYPE_INT,  .value = &x, .description = "The x value"},
	{.type = CSC_ARGV_TYPE_END}};

	permission = FLAG_DEFAULT;
	x = 0;
	csc_argv_parse (option, "-x1", csc_argv_build_flags (option));
	ASSERT (permission == (FLAG_DEFAULT|FLAG_EXEC|FLAG_EXEC2));
	ASSERT (x == 1);

	permission = FLAG_DEFAULT;
	x = 0;
	csc_argv_parse (option, "-x999", csc_argv_build_flags (option));
	ASSERT (permission == (FLAG_DEFAULT|FLAG_EXEC|FLAG_EXEC2));
	ASSERT (x == 999);

	permission = FLAG_DEFAULT;
	x = 0;
	csc_argv_parse (option, "-x 999", csc_argv_build_flags (option));
	csc_argv_parse (option, "-wr", csc_argv_build_flags (option));
	ASSERT (permission == (FLAG_DEFAULT|FLAG_READ|FLAG_WRITE|FLAG_EXEC|FLAG_EXEC2));
	ASSERT (x == 999);
}


void test_printing()
{
	float F = 0.0f;
	double D = 0.0;
	char * S = NULL;
	char * filename = "Default";
	long j = 0;
	int permission = FLAG_DEFAULT|FLAG_EXEC2|FLAG_READ;
	int x = 123;
	struct csc_argv_option option[] =
	{
	{.type = CSC_ARGV_TYPE_GROUP, .description = "Basic group"},
	{.character = 'S', .name = "String",   .type = CSC_ARGV_TYPE_STRING,    .value = &S, .description = "The string value"},
	{.character = 'F', .name = "Float",    .type = CSC_ARGV_TYPE_FLOAT,     .value = &F, .description = "The float value"},
	{.character = 'D', .name = "Double",   .type = CSC_ARGV_TYPE_DOUBLE,    .value = &D, .description = "The double value"},
	{.character = 'f', .name = "filename", .type = CSC_ARGV_TYPE_STRING,    .value = &filename, .description = "The filename value"},
	{.character = 'j', .name = "threads",  .type = CSC_ARGV_TYPE_LONG,      .value = &j, .description = "The number of threads value"},
	{.type = CSC_ARGV_TYPE_GROUP, .description = "Permission bits"},
	{.character = 'r', .name = "read",     .type = CSC_ARGV_TYPE_INT,  .value = &permission, .flag.val_int = FLAG_READ, .description = "The permission value"},
	{.character = 'w', .name = "write",    .type = CSC_ARGV_TYPE_INT,  .value = &permission, .flag.val_int = FLAG_WRITE, .description = "The permission value"},
	{.character = 'x', .name = "exec",     .type = CSC_ARGV_TYPE_INT,  .value = &permission, .flag.val_int = FLAG_EXEC, .description = "The permission value"},
	{.character = 'x', .name = "exec",     .type = CSC_ARGV_TYPE_INT,  .value = &permission, .flag.val_int = FLAG_EXEC2, .description = "The permission value"},
	{.character = 'x', .name = "exec",     .type = CSC_ARGV_TYPE_INT,       .value = &x, .description = "The permission value"},
	{.type = CSC_ARGV_TYPE_END}
};
	csc_argv_print_description (option);
	csc_argv_print_value (option);
}


int main (int argc, char const * argv [])
{
	csc_crossos_enable_ansi_color();
	ASSERT (argc);
	ASSERT (argv);

	test_strings();
	test_numbers();
	test_bitfield();
	test_mixing();
	test_printing();

	return EXIT_SUCCESS;
}
