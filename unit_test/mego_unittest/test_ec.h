#ifndef MEGO_TEST_EC_H_INCLUDED
#define MEGO_TEST_EC_H_INCLUDED

#include <mego/err/ec.h>
#include <mego/err/ec_impl.h>
#include <minunit.h>

MU_TEST(ut_ec_ok_is_zero)
{
    mu_assert(MGEC__OK == 0, "MGEC__OK should be 0");
}

MU_TEST(ut_ec_err_is_negative)
{
    mu_assert(MGEC__ERR < 0, "MGEC__ERR should be negative");
}

MU_TEST(ut_ec_posix_offset_range)
{
    mu_assert(MGEC__POSIX_OFFSET_BEGIN == -100000, "POSIX offset should be -100000");
    mu_assert(MGEC__INVAL == MGEC__POSIX_OFFSET_BEGIN - 22, "MGEC__INVAL should be POSIX offset - 22");
}

MU_TEST(ut_ec_common_codes)
{
    mu_assert(MGEC__NOMEM != 0, "MGEC__NOMEM should be non-zero");
    mu_assert(MGEC__INVAL != 0, "MGEC__INVAL should be non-zero");
    mu_assert(MGEC__EXIST != 0, "MGEC__EXIST should be non-zero");
}

MU_TEST(ut_ec_offset1_range)
{
    mu_assert(MGEC__OFFSET1_BEGIN == -200000, "Offset1 should be -200000");
    mu_assert(MGEC__INVALID_FUNC == MGEC__OFFSET1_BEGIN - 1, "MGEC__INVALID_FUNC should be in offset1 range");
}

MU_TEST(ut_ec_offset2_range)
{
    mu_assert(MGEC__OFFSET2_BEGIN == -300000, "Offset2 should be -300000");
    mu_assert(MGEC__SQLITE_INTERNAL == MGEC__OFFSET2_BEGIN - 2, "MGEC__SQLITE_INTERNAL should be in offset2 range");
}

MU_TEST(ut_ec_unique_values)
{
    mu_assert(MGEC__OK    != MGEC__ERR,   "OK and ERR should differ");
    mu_assert(MGEC__NOMEM != MGEC__INVAL, "NOMEM and INVAL should differ");
}

MU_TEST(ut_ec_from_posix_err)
{
    mgec_t ec = mgec__from_posix_err(EINVAL);
    mu_assert(ec != MGEC__OK, "from_posix_err(EINVAL) should not be OK");

    ec = mgec__from_posix_err(0);
    mu_assert(ec == MGEC__OK, "from_posix_err(0) should be MGEC__OK");
}

MU_TEST(ut_ec_from_sys_err)
{
    mgec_t ec = mgec__from_sys_err(0);
    mu_assert(ec == MGEC__OK, "from_sys_err(0) should be MGEC__OK");
}

MU_TEST_SUITE(test_ec)
{
    MU_RUN_TEST(ut_ec_ok_is_zero);
    MU_RUN_TEST(ut_ec_err_is_negative);
    MU_RUN_TEST(ut_ec_posix_offset_range);
    MU_RUN_TEST(ut_ec_common_codes);
    MU_RUN_TEST(ut_ec_offset1_range);
    MU_RUN_TEST(ut_ec_offset2_range);
    MU_RUN_TEST(ut_ec_unique_values);
    MU_RUN_TEST(ut_ec_from_posix_err);
    MU_RUN_TEST(ut_ec_from_sys_err);
}

#endif /* MEGO_TEST_EC_H_INCLUDED */
