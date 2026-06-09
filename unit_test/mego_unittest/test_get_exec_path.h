#ifndef MEGO_TEST_GET_EXEC_PATH_H_INCLUDED
#define MEGO_TEST_GET_EXEC_PATH_H_INCLUDED

#include <mego/util/get_exec_path.h>
#include <minunit.h>

#include <string.h>

MU_TEST(ut_get_exec_path_basic)
{
    char buf[4096];
    int ret = mgu_get_exec_path(buf, sizeof(buf), NULL);
    mu_assert(ret > 0, "mgu_get_exec_path should return a positive length");
    mu_assert(strlen(buf) > 0, "exec path should not be empty");
}

MU_TEST(ut_get_exec_path_with_dirname)
{
    char buf[4096];
    int dir_pos = -1;
    int ret = mgu_get_exec_path(buf, sizeof(buf), &dir_pos);
    mu_assert(ret > 0, "mgu_get_exec_path should return a positive length");
    mu_assert(dir_pos >= 0, "dirname position should be set");
    mu_assert(dir_pos < ret, "dirname position should be within path length");
    mu_assert(buf[dir_pos] == '\\' || buf[dir_pos] == '/',
              "dirname position should point to a path separator");
}

MU_TEST(ut_get_exec_path_null_buffer)
{
    int ret = mgu_get_exec_path(NULL, 0, NULL);
    mu_assert(ret > 0, "mgu_get_exec_path(NULL,0) should return the required length");
}

MU_TEST(ut_get_exec_path_small_buffer)
{
    char buf[1];
    int ret = mgu_get_exec_path(buf, 1, NULL);
    mu_assert(ret > 0, "mgu_get_exec_path with tiny buffer should return required length");
}

MU_TEST_SUITE(test_get_exec_path)
{
    MU_RUN_TEST(ut_get_exec_path_basic);
    MU_RUN_TEST(ut_get_exec_path_with_dirname);
    MU_RUN_TEST(ut_get_exec_path_null_buffer);
    MU_RUN_TEST(ut_get_exec_path_small_buffer);
}

#endif /* MEGO_TEST_GET_EXEC_PATH_H_INCLUDED */
