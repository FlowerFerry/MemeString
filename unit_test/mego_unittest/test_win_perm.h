#ifndef MEGO_TEST_WIN_PERM_H_INCLUDED
#define MEGO_TEST_WIN_PERM_H_INCLUDED

#include <mego/os/win/perm/is_run_as_admin.h>
#include <minunit.h>

#include <stdbool.h>

MU_TEST(ut_win_perm_is_run_as_admin_no_crash)
{
    /* The result depends on the runtime context (may be true or false).
     * The test only verifies the function completes without crashing and
     * returns a value that is boolean (true or false). */
    bool result = mg_win_perm_is_run_as_admin();
    mu_assert(result == true || result == false,
              "mg_win_perm_is_run_as_admin should return a valid bool");
}

MU_TEST_SUITE(test_win_perm)
{
    MU_RUN_TEST(ut_win_perm_is_run_as_admin_no_crash);
}

#endif /* MEGO_TEST_WIN_PERM_H_INCLUDED */
