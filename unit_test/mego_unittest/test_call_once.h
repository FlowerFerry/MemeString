#ifndef MEGO_TEST_CALL_ONCE_H_INCLUDED
#define MEGO_TEST_CALL_ONCE_H_INCLUDED

#include <mego/thrd/call_once.h>
#include <minunit.h>

static int g_call_once_counter = 0;

static void test_once_func(void)
{
    ++g_call_once_counter;
}

MU_TEST(ut_call_once_basic)
{
    g_call_once_counter = 0;

    mgthrd_once_flag flag = MGTHRD_ONCE_FLAG_INIT;
    mgthrd_call_once(&flag, test_once_func);
    mu_assert(g_call_once_counter == 1, "call_once should invoke the function once");
}

MU_TEST(ut_call_once_idempotent)
{
    g_call_once_counter = 0;

    mgthrd_once_flag flag = MGTHRD_ONCE_FLAG_INIT;
    mgthrd_call_once(&flag, test_once_func);
    mu_assert(g_call_once_counter == 1, "first call: should invoke the function");

    mgthrd_call_once(&flag, test_once_func);
    mu_assert(g_call_once_counter == 1, "second call: should NOT invoke the function again");

    mgthrd_call_once(&flag, test_once_func);
    mu_assert(g_call_once_counter == 1, "third call: should NOT invoke the function again");
}

MU_TEST_SUITE(test_call_once)
{
    MU_RUN_TEST(ut_call_once_basic);
    MU_RUN_TEST(ut_call_once_idempotent);
}

#endif /* MEGO_TEST_CALL_ONCE_H_INCLUDED */
