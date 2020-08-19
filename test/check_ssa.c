#include <stdlib.h>
#include <check.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include "ssa.h"


START_TEST(check_nCr)
{
        ck_assert(ssa_nCr(0, 0) == 1);
        ck_assert(ssa_nCr(0, 1) == 0);
        ck_assert(ssa_nCr(0, 2) == 0);
        ck_assert(ssa_nCr(0, 1000) == 0);
        ck_assert(ssa_nCr(1, 1) == 1);
        ck_assert(ssa_nCr(10, 10) == 1);
        ck_assert(ssa_nCr(6, 2) == 15);
        ck_assert(ssa_nCr(100, 1) == 100);
        ck_assert(ssa_nCr(100, 2) == 4950);
        ck_assert(ssa_nCr(25, 13) == 5200300);
}
END_TEST


Suite *ssa_suite()
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("ssa");
	tc_core = tcase_create("core");

	tcase_add_test(tc_core, check_nCr);

	suite_add_tcase(s, tc_core);

	return s;
}


int main(void)
{
	int number_failed;  
	Suite *s;  
	SRunner *sr;  

	s = ssa_suite();  
	sr = srunner_create(s);  

	srunner_run_all(sr, CK_NORMAL);  
	number_failed = srunner_ntests_failed(sr);  
	srunner_free(sr);  
	return (number_failed == 0) ? 0 : 1;  
}
