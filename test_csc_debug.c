#include <stdint.h>
#include "csc_crossos.h"
#include "csc_debug.h"


void foo (char * str)
{
	ASSERT_PARAM_NOTNULL (str);
	puts (str);
}


int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);
	csc_crossos_enable_ansi_color();

#if 0
	foo (NULL);
#endif

#if 0
	void * x = (void *)(intptr_t)0x44;
	ASSERT_NOTNULL(x);
#endif

#if 0
	int a = 4;
	int b = 3;
	ASSERT_LTI(a, b);
#endif

#if 0
	errno = 4;
	struct s{int alpha;};
	struct s * a = (struct s *)(intptr_t)0xABC54;
	struct s * b = (struct s *)(intptr_t)0xABC44;
	ASSERT_LTP(a, b);
#endif

#if 0
	errno = 4;
	void * a = (void *)(intptr_t)0xABC54;
	void * b = (void *)(intptr_t)0xABC44;
	ASSERT_EQP(a, b);
#endif

#if 0
	errno = 4;
	int a = 1;
	ASSERT_FALSE(a);
#endif

#if 0
	errno = 4;
	unsigned a = 3;
	ASSERT_POW2(a);
#endif

#if 1
	errno = 4;
	void * a = (void *)(intptr_t)0xABC5431;
	unsigned x = 2;
	ASSERT_ALIGN(a, x);
#endif

	return EXIT_SUCCESS;
}
