#ifndef MEGO_TEST_MATH_H_INCLUDED
#define MEGO_TEST_MATH_H_INCLUDED

#include <mego/util/math.h>
#include <minunit.h>

/* ---- MGU_MATH__MAX ---- */

MU_TEST(ut_math_max_basic)
{
    mu_assert(MGU_MATH__MAX(1, 2) == 2, "max(1,2) should be 2");
    mu_assert(MGU_MATH__MAX(5, 3) == 5, "max(5,3) should be 5");
    mu_assert(MGU_MATH__MAX(-1, 1) == 1, "max(-1,1) should be 1");
}

MU_TEST(ut_math_max_equal)
{
    mu_assert(MGU_MATH__MAX(7, 7) == 7, "max(7,7) should be 7");
}

MU_TEST(ut_math_max_negative)
{
    mu_assert(MGU_MATH__MAX(-5, -3) == -3, "max(-5,-3) should be -3");
}

MU_TEST(ut_math_max_unsigned)
{
    unsigned int a = 10, b = 20;
    mu_assert(MGU_MATH__MAX(a, b) == 20u, "max(unsigned,unsigned) should work");
}

/* ---- MGU_MATH__MIN ---- */

MU_TEST(ut_math_min_basic)
{
    mu_assert(MGU_MATH__MIN(1, 2) == 1, "min(1,2) should be 1");
    mu_assert(MGU_MATH__MIN(5, 3) == 3, "min(5,3) should be 3");
    mu_assert(MGU_MATH__MIN(-1, 1) == -1, "min(-1,1) should be -1");
}

MU_TEST(ut_math_min_equal)
{
    mu_assert(MGU_MATH__MIN(7, 7) == 7, "min(7,7) should be 7");
}

MU_TEST(ut_math_min_negative)
{
    mu_assert(MGU_MATH__MIN(-5, -3) == -5, "min(-5,-3) should be -5");
}

MU_TEST(ut_math_min_unsigned)
{
    unsigned int a = 10, b = 20;
    mu_assert(MGU_MATH__MIN(a, b) == 10u, "min(unsigned,unsigned) should work");
}

/* ---- suite ---- */

MU_TEST_SUITE(test_math)
{
    MU_RUN_TEST(ut_math_max_basic);
    MU_RUN_TEST(ut_math_max_equal);
    MU_RUN_TEST(ut_math_max_negative);
    MU_RUN_TEST(ut_math_max_unsigned);

    MU_RUN_TEST(ut_math_min_basic);
    MU_RUN_TEST(ut_math_min_equal);
    MU_RUN_TEST(ut_math_min_negative);
    MU_RUN_TEST(ut_math_min_unsigned);
}

#endif /* MEGO_TEST_MATH_H_INCLUDED */
