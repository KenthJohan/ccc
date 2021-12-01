#include "csc_basic.h"
#include "csc_assert.h"
#include "csc_crossos.h"
#include "csc_strf.h"
#include "csc_u32.h"



static void test__u32_roundup_pow2()
{
	ASSERT_EQ_U (u32_roundup_pow2(0), 0);
	ASSERT_EQ_U (u32_roundup_pow2(1), 1);
	ASSERT_EQ_U (u32_roundup_pow2(2), 2);
	ASSERT_EQ_U (u32_roundup_pow2(3), 4);
	ASSERT_EQ_U (u32_roundup_pow2(4), 4);
	ASSERT_EQ_U (u32_roundup_pow2(5), 8);
	ASSERT_EQ_U (u32_roundup_pow2(6), 8);
	ASSERT_EQ_U (u32_roundup_pow2(7), 8);
	ASSERT_EQ_U (u32_roundup_pow2(8), 8);
	ASSERT_EQ_U (u32_roundup_pow2(9), 16);
	ASSERT_EQ_U (u32_roundup_pow2(10), 16);
	ASSERT_EQ_U (u32_roundup_pow2(11), 16);
	ASSERT_EQ_U (u32_roundup_pow2(12), 16);
	ASSERT_EQ_U (u32_roundup_pow2(13), 16);
	ASSERT_EQ_U (u32_roundup_pow2(14), 16);
	ASSERT_EQ_U (u32_roundup_pow2(15), 16);
	ASSERT_EQ_U (u32_roundup_pow2(16), 16);
	ASSERT_EQ_U (u32_roundup_pow2(1023), 1024);
}

int main (int argc, char const * argv [])
{
	csc_crossos_enable_ansi_color();
	ASSERT (argc);
	ASSERT (argv);

	test__u32_roundup_pow2();


	return EXIT_SUCCESS;
}
