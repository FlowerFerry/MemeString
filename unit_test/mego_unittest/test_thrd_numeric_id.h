#ifndef MEGO_TEST_THRD_NUMERIC_ID_H_INCLUDED
#define MEGO_TEST_THRD_NUMERIC_ID_H_INCLUDED

#include <mego/thrd/numeric_id.h>
#include <minunit.h>

MU_TEST(ut_thrd_numeric_id_nonzero)
{
    size_t id = mgthrd_numeric_id();
    mu_assert(id != 0, "mgthrd_numeric_id should return a non-zero thread ID");
}

MU_TEST(ut_thrd_numeric_id_stable)
{
    size_t id1 = mgthrd_numeric_id();
    size_t id2 = mgthrd_numeric_id();
    size_t id3 = mgthrd_numeric_id();
    mu_assert(id1 == id2, "mgthrd_numeric_id should be stable within same thread");
    mu_assert(id2 == id3, "mgthrd_numeric_id should be stable (second call)");
}

MU_TEST(ut_thrd_numeric_id_default_value)
{
    /* __mgthrd_numeric_id returns the raw thread ID from the OS */
    size_t raw_id = __mgthrd_numeric_id();
    mu_assert(raw_id > 0, "__mgthrd_numeric_id should return a positive thread ID");
}

MU_TEST_SUITE(test_thrd_numeric_id)
{
    MU_RUN_TEST(ut_thrd_numeric_id_nonzero);
    MU_RUN_TEST(ut_thrd_numeric_id_stable);
    MU_RUN_TEST(ut_thrd_numeric_id_default_value);
}

#endif /* MEGO_TEST_THRD_NUMERIC_ID_H_INCLUDED */
