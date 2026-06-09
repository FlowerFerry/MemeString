#ifndef MEGO_TEST_HELPER_MACROS_H_INCLUDED
#define MEGO_TEST_HELPER_MACROS_H_INCLUDED

#include <mego/predef/helper_macros.h>
#include <minunit.h>

#include <string.h>

/* ---- MEGO__STRINGIZE ---- */

MU_TEST(ut_stringize_int)
{
    mu_assert_string_eq("42", MEGO__STRINGIZE(42));
}

MU_TEST(ut_stringize_macro)
{
#define TEST_VAL 123
    mu_assert_string_eq("123", MEGO__STRINGIZE(TEST_VAL));
#undef TEST_VAL
}

MU_TEST(ut_stringize_empty)
{
    mu_assert_string_eq("", MEGO__STRINGIZE());
}

MU_TEST(ut_stringize_text)
{
    mu_assert_string_eq("hello", MEGO__STRINGIZE(hello));
}

/* ---- MEGO__JOIN ---- */

MU_TEST(ut_join_two_tokens)
{
    int MEGO__JOIN(my_, var) = 42;
    mu_assert(my_var == 42, "MEGO__JOIN should concatenate my_ and var");
}

MU_TEST(ut_join_macro_args)
{
#define PREFIX test_
#define SUFFIX _val
    int MEGO__JOIN(PREFIX, SUFFIX) = 99;
    mu_assert(test__val == 99, "MEGO__JOIN should expand macros then concatenate");
#undef PREFIX
#undef SUFFIX
}

MU_TEST(ut_join_empty_part)
{
    int MEGO__JOIN(x, ) = 5;
    mu_assert(x == 5, "MEGO__JOIN with empty second part");
}

MU_TEST(ut_join_three_parts_chained)
{
#define CAT3(A,B,C) MEGO__JOIN(MEGO__JOIN(A,B),C)
    int CAT3(a_, b_, c) = 7;
    mu_assert(a_b_c == 7, "chained MEGO__JOIN for three parts");
#undef CAT3
}

/* ---- MEGO__MAKE_VERSION_NUMBER ---- */

MU_TEST(ut_make_version_number_basic)
{
    int v = MEGO__MAKE_VERSION_NUMBER(1, 0, 0);
    mu_assert(v == 10000000, "version 1.0.0 should be 10000000");
}

MU_TEST(ut_make_version_number_major_minor_patch)
{
    int v = MEGO__MAKE_VERSION_NUMBER(3, 2, 1);
    mu_assert(v == 30200001, "version 3.2.1 should be 30200001");
}

MU_TEST(ut_make_version_number_overflow)
{
    /* Components are clamped to %100/%100000 */
    int v = MEGO__MAKE_VERSION_NUMBER(101, 0, 0);
    mu_assert(v == 10000000, "version 101.0.0 clamps to 1.0.0 because 101%%100 == 1");
}

MU_TEST(ut_get_version_major)
{
    int v = MEGO__MAKE_VERSION_NUMBER(2, 5, 10);
    mu_assert(MEGO__GET_VERSION_MAJOR(v) == 2, "GET_VERSION_MAJOR should be 2");
}

MU_TEST(ut_get_version_minor)
{
    int v = MEGO__MAKE_VERSION_NUMBER(2, 5, 10);
    mu_assert(MEGO__GET_VERSION_MINOR(v) == 5, "GET_VERSION_MINOR should be 5");
}

MU_TEST(ut_get_version_patch)
{
    int v = MEGO__MAKE_VERSION_NUMBER(2, 5, 10);
    mu_assert(MEGO__GET_VERSION_PATCH(v) == 10, "GET_VERSION_PATCH should be 10");
}

/* ---- suite ---- */

MU_TEST_SUITE(test_helper_macros)
{
    MU_RUN_TEST(ut_stringize_int);
    MU_RUN_TEST(ut_stringize_macro);
    MU_RUN_TEST(ut_stringize_empty);
    MU_RUN_TEST(ut_stringize_text);

    MU_RUN_TEST(ut_join_two_tokens);
    MU_RUN_TEST(ut_join_macro_args);
    MU_RUN_TEST(ut_join_empty_part);
    MU_RUN_TEST(ut_join_three_parts_chained);

    MU_RUN_TEST(ut_make_version_number_basic);
    MU_RUN_TEST(ut_make_version_number_major_minor_patch);
    MU_RUN_TEST(ut_make_version_number_overflow);
    MU_RUN_TEST(ut_get_version_major);
    MU_RUN_TEST(ut_get_version_minor);
    MU_RUN_TEST(ut_get_version_patch);
}

#endif /* MEGO_TEST_HELPER_MACROS_H_INCLUDED */
