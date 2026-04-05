#ifndef MEGO_TEST_STD_STRING_H_INCLUDED
#define MEGO_TEST_STD_STRING_H_INCLUDED

#include <mego/util/std/string.h>
#include <minunit.h>

#include <string.h>
#include <errno.h>

/* ---- normal copies (all platforms) ---- */

MU_TEST(ut_strncpy_s_basic)
{
    char dest[16];
    errno_t ec = mgu_strncpy_s(dest, sizeof(dest), "hello", 5);
    mu_assert(ec == 0, "strncpy_s basic copy should succeed");
    mu_assert(strcmp(dest, "hello") == 0, "strncpy_s basic: content mismatch");
}

MU_TEST(ut_strncpy_s_zero_count)
{
    char dest[8] = {'X', 'X', 'X', 'X', '\0'};
    errno_t ec = mgu_strncpy_s(dest, sizeof(dest), "hello", 0);
    mu_assert(ec == 0, "strncpy_s zero count should succeed");
    mu_assert(dest[0] == '\0', "strncpy_s zero count: dest[0] should be '\\0'");
}

MU_TEST(ut_strncpy_s_partial)
{
    char dest[8];
    errno_t ec = mgu_strncpy_s(dest, sizeof(dest), "hello world", 5);
    mu_assert(ec == 0, "strncpy_s partial copy should succeed");
    mu_assert(strncmp(dest, "hello", 5) == 0, "strncpy_s partial: content mismatch");
    mu_assert(dest[5] == '\0', "strncpy_s partial: null terminator missing");
}

MU_TEST(ut_strncpy_s_exact_fit)
{
    /* count == destsz - 1: fits exactly */
    char dest[6];
    errno_t ec = mgu_strncpy_s(dest, 6, "hello", 5);
    mu_assert(ec == 0, "strncpy_s exact fit should succeed");
    mu_assert(strcmp(dest, "hello") == 0, "strncpy_s exact fit: content mismatch");
}

MU_TEST(ut_strncpy_s_single_char)
{
    char dest[4];
    errno_t ec = mgu_strncpy_s(dest, sizeof(dest), "A", 1);
    mu_assert(ec == 0, "strncpy_s single char should succeed");
    mu_assert(dest[0] == 'A', "strncpy_s single char: content mismatch");
    mu_assert(dest[1] == '\0', "strncpy_s single char: null terminator missing");
}

/* ---- error cases guarded on Windows (system strncpy_s uses constraint handlers) ---- */

#if !MG_OS__WIN_AVAIL && !defined(__STDC_LIB_EXT1__)

MU_TEST(ut_strncpy_s_null_dest)
{
    errno_t ec = mgu_strncpy_s(NULL, 8, "hello", 5);
    mu_assert(ec != 0, "strncpy_s NULL dest should return error");
}

MU_TEST(ut_strncpy_s_zero_destsz)
{
    char dest[8];
    errno_t ec = mgu_strncpy_s(dest, 0, "hello", 5);
    mu_assert(ec != 0, "strncpy_s destsz==0 should return error");
}

MU_TEST(ut_strncpy_s_null_src)
{
    char dest[8];
    errno_t ec = mgu_strncpy_s(dest, sizeof(dest), NULL, 5);
    mu_assert(ec != 0, "strncpy_s NULL src should return error");
    mu_assert(dest[0] == '\0', "strncpy_s NULL src: dest[0] should be '\\0'");
}

MU_TEST(ut_strncpy_s_count_gt_destsz)
{
    /* count > destsz → ERANGE */
    char dest[4];
    errno_t ec = mgu_strncpy_s(dest, 4, "hello", 5);
    mu_assert(ec != 0, "strncpy_s count > destsz should return error");
    mu_assert(dest[0] == '\0', "strncpy_s overflow: dest[0] should be '\\0'");
}

MU_TEST(ut_strncpy_s_count_eq_destsz_overflow)
{
    /* count == destsz but src is longer → ERANGE */
    char dest[5];
    /* src has 5 chars + '\0', count == destsz == 5, src[4] != '\0' (index 4 = 'o') */
    errno_t ec = mgu_strncpy_s(dest, 5, "hello", 5);
    mu_assert(ec != 0, "strncpy_s count==destsz with full src should return error");
    mu_assert(dest[0] == '\0', "strncpy_s overflow: dest[0] should be '\\0'");
}

#endif /* !MG_OS__WIN_AVAIL && !defined(__STDC_LIB_EXT1__) */

/* ---- suite ---- */

MU_TEST_SUITE(test_std_string)
{
    MU_RUN_TEST(ut_strncpy_s_basic);
    MU_RUN_TEST(ut_strncpy_s_zero_count);
    MU_RUN_TEST(ut_strncpy_s_partial);
    MU_RUN_TEST(ut_strncpy_s_exact_fit);
    MU_RUN_TEST(ut_strncpy_s_single_char);

#if !MG_OS__WIN_AVAIL && !defined(__STDC_LIB_EXT1__)
    MU_RUN_TEST(ut_strncpy_s_null_dest);
    MU_RUN_TEST(ut_strncpy_s_zero_destsz);
    MU_RUN_TEST(ut_strncpy_s_null_src);
    MU_RUN_TEST(ut_strncpy_s_count_gt_destsz);
    MU_RUN_TEST(ut_strncpy_s_count_eq_destsz_overflow);
#endif
}

#endif /* MEGO_TEST_STD_STRING_H_INCLUDED */
