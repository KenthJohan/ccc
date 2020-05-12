#include "csc_debug.h"
#include "csc_crossos.h"
#include "csc_argv.h"


#define FLAG_READ    0x01
#define FLAG_WRITE   0x02
#define FLAG_EXEC    0x04
#define FLAG_EXEC2   0x08
#define FLAG_DEFAULT 0x10


int main (int argc, char const * argv [])
{
	csc_crossos_enable_ansi_color();
	ASSERT (argc);
	ASSERT (argv);

	float F = 0.0f;
	double D = 0.0;
	char * S = NULL;
	char * filename = "Default";
	long j = 0;
	int permission = FLAG_DEFAULT;
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
	{.character = 'r', .name = "read",     .type = CSC_ARGV_TYPE_FLAG_INT,  .value = &permission, .flag.val_int = FLAG_READ, .description = "The permission value"},
	{.character = 'w', .name = "write",    .type = CSC_ARGV_TYPE_FLAG_INT,  .value = &permission, .flag.val_int = FLAG_WRITE, .description = "The permission value"},
	{.character = 'x', .name = "exec",     .type = CSC_ARGV_TYPE_FLAG_INT,  .value = &permission, .flag.val_int = FLAG_EXEC, .description = "The permission value"},
	{.character = 'x', .name = "exec",     .type = CSC_ARGV_TYPE_FLAG_INT,  .value = &permission, .flag.val_int = FLAG_EXEC2, .description = "The permission value"},
	{.character = 'x', .name = "exec",     .type = CSC_ARGV_TYPE_INT,       .value = &x, .description = "The permission value"},
	{.type = CSC_ARGV_TYPE_END}
	};

	csc_argv_parse (option, "-j3", csc_argv_build_flags (option));
	ASSERT (x == 123);
	ASSERT (j == 3);
	ASSERT (F == 0.0f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Default") == 0);
	ASSERT ((permission & FLAG_READ) == 0);
	ASSERT ((permission & FLAG_WRITE) == 0);
	ASSERT ((permission & FLAG_EXEC) == 0);
	ASSERT ((permission & FLAG_EXEC2) == 0);
	ASSERT (permission & FLAG_DEFAULT);

	csc_argv_parse (option, "--threads=4", csc_argv_build_flags (option));
	ASSERT (x == 123);
	ASSERT (j == 4);
	ASSERT (F == 0.0f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Default") == 0);
	ASSERT ((permission & FLAG_READ) == 0);
	ASSERT ((permission & FLAG_WRITE) == 0);
	ASSERT ((permission & FLAG_EXEC) == 0);
	ASSERT ((permission & FLAG_EXEC2) == 0);
	ASSERT (permission & FLAG_DEFAULT);

	csc_argv_parse (option, "-F4.3", csc_argv_build_flags (option));
	ASSERT (x == 123);
	ASSERT (j == 4);
	ASSERT (F == 4.3f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Default") == 0);
	ASSERT ((permission & FLAG_READ) == 0);
	ASSERT ((permission & FLAG_WRITE) == 0);
	ASSERT ((permission & FLAG_EXEC) == 0);
	ASSERT ((permission & FLAG_EXEC2) == 0);
	ASSERT (permission & FLAG_DEFAULT);

	csc_argv_parse (option, "-wrgx", csc_argv_build_flags (option));
	ASSERT (x == 123);
	ASSERT (j == 4);
	ASSERT (F == 4.3f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Default") == 0);
	ASSERT (permission & FLAG_READ);
	ASSERT (permission & FLAG_WRITE);
	ASSERT (permission & FLAG_EXEC);
	ASSERT (permission & FLAG_EXEC2);
	ASSERT (permission & FLAG_DEFAULT);

	csc_argv_parse (option, "--file=Banana", csc_argv_build_flags (option));
	ASSERT (x == 123);
	ASSERT (j == 4);
	ASSERT (F == 4.3f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Default") == 0);
	ASSERT (permission & FLAG_READ);
	ASSERT (permission & FLAG_WRITE);
	ASSERT (permission & FLAG_EXEC);
	ASSERT (permission & FLAG_EXEC2);
	ASSERT (permission & FLAG_DEFAULT);

	csc_argv_parse (option, "--filename=Banana", csc_argv_build_flags (option));
	ASSERT (x == 123);
	ASSERT (j == 4);
	ASSERT (F == 4.3f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Banana") == 0);
	ASSERT (permission & FLAG_READ);
	ASSERT (permission & FLAG_WRITE);
	ASSERT (permission & FLAG_EXEC);
	ASSERT (permission & FLAG_EXEC2);
	ASSERT (permission & FLAG_DEFAULT);

	permission = 0;
	csc_argv_parse (option, "-x999", csc_argv_build_flags (option));
	ASSERT (x == 999);
	ASSERT (j == 4);
	ASSERT (F == 4.3f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Banana") == 0);
	ASSERT ((permission & FLAG_READ) == 0);
	ASSERT ((permission & FLAG_WRITE) == 0);
	ASSERT (permission & FLAG_EXEC);
	ASSERT (permission & FLAG_EXEC2);
	ASSERT ((permission & FLAG_DEFAULT) == 0);

	permission = 0;
	csc_argv_parse (option, "-xd", csc_argv_build_flags (option));
	ASSERT (x == 999);
	ASSERT (j == 4);
	ASSERT (F == 4.3f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Banana") == 0);
	ASSERT ((permission & FLAG_READ) == 0);
	ASSERT ((permission & FLAG_WRITE) == 0);
	ASSERT (permission & FLAG_EXEC);
	ASSERT (permission & FLAG_EXEC2);
	ASSERT ((permission & FLAG_DEFAULT) == 0);


	csc_argv_print_description (option);
	csc_argv_print_value (option);

	return EXIT_SUCCESS;
}
