#include "csc_strfrom.h"
#include "csc_crossos.h"

#define STR_LENGTH 20


void hr()
{
	printf ("==============================\n");
}




int main (int argc, char * argv [])
{
	csc_crossos_enable_ansi_color();
	ASSERT (argc);
	ASSERT (argv);

	{
		test_strfrom_imax_cases();
	}

	/*

	hr();

	{
		char str[STR_LENGTH] = {0};
		strfrom_imax2 (str, STR_LENGTH, 123456789, 10);
		fwrite (str, 1, sizeof(str), stdout);
		printf ("\n");
	}

	hr();

	{
		char str[STR_LENGTH] = {0};
		strfrom_imax2 (str, STR_LENGTH, 123456789, -10);
		fwrite (str, 1, sizeof(str), stdout);
		printf ("\n");
	}

	hr();

	{
		char str[STR_LENGTH] = {0};
		strfrom_imax2 (str, 0, 123456789, -10);
		fwrite (str, 1, sizeof(str), stdout);
		printf ("\n");
	}
	*/
	return EXIT_SUCCESS;
}
