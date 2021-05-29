#include "csc_strto.h"


int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);

	{
		char const * a = "2Hello";
		char const * p = a;
		intmax_t i = strto_imax (&p, 10);
		ASSERT (i == 2);
		ASSERT ((p - a) == 1);
	}

	{
		char const * a = "ABC123XYX";
		char const * p = a;
		intmax_t i = strto_imax (&p, 10);
		ASSERT (i == 0);
		ASSERT ((p - a) == 0);
	}

	{
		char const * a = "ABC123XYX";
		char const * p = a;
		intmax_t i = strto_imax (&p, 16);
		ASSERT (i == 0xABC123);
		ASSERT ((p - a) == 6);
	}

	{
		char const * a = "-123-";
		char const * p = a;
		intmax_t i = strto_imax (&p, 10);
		ASSERT (i == -123);
		ASSERT ((p - a) == 4);
	}

	{
		char const * a = "+123-";
		char const * p = a;
		intmax_t i = strto_imax (&p, 10);
		ASSERT (i == 123);
		ASSERT ((p - a) == 4);
	}

	return EXIT_SUCCESS;
}
