#include "csc_crossos.h"
#include "csc_argparse.h"


#define FLAG_READ    0x1
#define FLAG_WRITE   0x2
#define FLAG_EXEC    0x4
#define FLAG_DEFAULT 0x8


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
	{.prefix = 'S', .longname = "String",   .type = CSC_ARGV_TYPE_STRING,    .value = &S},
	{.prefix = 'F', .longname = "Float",    .type = CSC_ARGV_TYPE_FLOAT,     .value = &F},
	{.prefix = 'D', .longname = "Double",   .type = CSC_ARGV_TYPE_DOUBLE,    .value = &D},
	{.prefix = 'f', .longname = "filename", .type = CSC_ARGV_TYPE_STRING,    .value = &filename},
	{.prefix = 'j', .longname = "threads",  .type = CSC_ARGV_TYPE_LONG,      .value = &j},
	{.prefix = 'r', .longname = "read",     .type = CSC_ARGV_TYPE_FLAG_INT,  .value = &permission, .flag_int = FLAG_READ},
	{.prefix = 'w', .longname = "write",    .type = CSC_ARGV_TYPE_FLAG_INT,  .value = &permission, .flag_int = FLAG_WRITE},
	{.prefix = 'x', .longname = "exec",     .type = CSC_ARGV_TYPE_FLAG_INT,  .value = &permission, .flag_int = FLAG_EXEC},
	{.prefix = 'x', .longname = "exec",     .type = CSC_ARGV_TYPE_INT,       .value = &x},
	{.type = CSC_ARGV_TYPE_END}
	};

	csc_argv_parse (option, "-j3");
	ASSERT (x == 123);
	ASSERT (j == 3);
	ASSERT (F == 0.0f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Default") == 0);
	ASSERT ((permission & FLAG_READ) == 0);
	ASSERT ((permission & FLAG_WRITE) == 0);
	ASSERT ((permission & FLAG_EXEC) == 0);
	ASSERT (permission & FLAG_DEFAULT);

	csc_argv_parse (option, "--threads=4");
	ASSERT (x == 123);
	ASSERT (j == 4);
	ASSERT (F == 0.0f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Default") == 0);
	ASSERT ((permission & FLAG_READ) == 0);
	ASSERT ((permission & FLAG_WRITE) == 0);
	ASSERT ((permission & FLAG_EXEC) == 0);
	ASSERT (permission & FLAG_DEFAULT);

	csc_argv_parse (option, "-F4.3");
	ASSERT (x == 123);
	ASSERT (j == 4);
	ASSERT (F == 4.3f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Default") == 0);
	ASSERT ((permission & FLAG_READ) == 0);
	ASSERT ((permission & FLAG_WRITE) == 0);
	ASSERT ((permission & FLAG_EXEC) == 0);
	ASSERT (permission & FLAG_DEFAULT);

	csc_argv_parse (option, "-wrgx");
	ASSERT (x == 123);
	ASSERT (j == 4);
	ASSERT (F == 4.3f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Default") == 0);
	ASSERT (permission & FLAG_READ);
	ASSERT (permission & FLAG_WRITE);
	ASSERT (permission & FLAG_EXEC);
	ASSERT (permission & FLAG_DEFAULT);

	csc_argv_parse (option, "--file=Banana");
	ASSERT (x == 123);
	ASSERT (j == 4);
	ASSERT (F == 4.3f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Default") == 0);
	ASSERT (permission & FLAG_READ);
	ASSERT (permission & FLAG_WRITE);
	ASSERT (permission & FLAG_EXEC);
	ASSERT (permission & FLAG_DEFAULT);

	csc_argv_parse (option, "--filename=Banana");
	ASSERT (x == 123);
	ASSERT (j == 4);
	ASSERT (F == 4.3f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Banana") == 0);
	ASSERT (permission & FLAG_READ);
	ASSERT (permission & FLAG_WRITE);
	ASSERT (permission & FLAG_EXEC);
	ASSERT (permission & FLAG_DEFAULT);

	permission = 0;
	csc_argv_parse (option, "-x999");
	ASSERT (x == 999);
	ASSERT (j == 4);
	ASSERT (F == 4.3f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Banana") == 0);
	ASSERT ((permission & FLAG_READ) == 0);
	ASSERT ((permission & FLAG_WRITE) == 0);
	ASSERT (permission & FLAG_EXEC);
	ASSERT ((permission & FLAG_DEFAULT) == 0);

	permission = 0;
	csc_argv_parse (option, "-xd");
	ASSERT (x == 0);
	ASSERT (j == 4);
	ASSERT (F == 4.3f);
	ASSERT (D == 0.0f);
	ASSERT (S == NULL);
	ASSERT (strcmp (filename, "Banana") == 0);
	ASSERT ((permission & FLAG_READ) == 0);
	ASSERT ((permission & FLAG_WRITE) == 0);
	ASSERT (permission & FLAG_EXEC);
	ASSERT ((permission & FLAG_DEFAULT) == 0);

	return EXIT_SUCCESS;
}
