#include "csc_str.h"



int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);

	{
		char * a = "2Hello";
		char * p = a;
		intmax_t i = str_to_imax (&p, 10);
		ASSERT (i == 2);
		ASSERT ((p - a) == 1);
	}

	{
		char * a = "ABC123XYX";
		char * p = a;
		intmax_t i = str_to_imax (&p, 10);
		ASSERT (i == 0);
		ASSERT ((p - a) == 0);
	}

	{
		char * a = "ABC123XYX";
		char * p = a;
		intmax_t i = str_to_imax (&p, 16);
		ASSERT (i == 0xABC123);
		ASSERT ((p - a) == 6);
	}

	{
		char * a = "-123-";
		char * p = a;
		intmax_t i = str_to_imax (&p, 10);
		ASSERT (i == -123);
		ASSERT ((p - a) == 4);
	}

	{
		char * a = "+123-";
		char * p = a;
		intmax_t i = str_to_imax (&p, 10);
		ASSERT (i == 123);
		ASSERT ((p - a) == 4);
	}

	return EXIT_SUCCESS;
}
