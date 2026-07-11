#ifndef TEST_MEME_VERSION_H_INCLUDED
#define TEST_MEME_VERSION_H_INCLUDED

#include <minunit.h>
#include <meme/version.h>

MU_TEST(test_meme_version_num)
{
    mmver_t v = mmver_num();
    mu_assert(v > 0, "mmver_num() should return a positive version number");
    mu_assert(v >= MMVER_NUM, "mmver_num() should be >= MMVER_NUM");
}

MU_TEST(test_meme_version_str)
{
    const char* s = mmver_str();
    mu_assert(s != NULL, "mmver_str() should return a non-NULL string");
    mu_assert(s[0] != '\0', "mmver_str() should return a non-empty string");
}

MU_TEST(test_meme_version_macros)
{
    mu_assert(MMVER_MAJOR >= 0, "MMVER_MAJOR should be non-negative");
    mu_assert(MMVER_MINOR >= 0, "MMVER_MINOR should be non-negative");
    mu_assert(MMVER_PATCH >= 0, "MMVER_PATCH should be non-negative");
}

MU_TEST_SUITE(test_meme_version)
{
    MU_RUN_TEST(test_meme_version_num);
    MU_RUN_TEST(test_meme_version_str);
    MU_RUN_TEST(test_meme_version_macros);
}

#endif // TEST_MEME_VERSION_H_INCLUDED
