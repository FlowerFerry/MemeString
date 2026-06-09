#ifndef TEST_MEME_MEMORY_H_INCLUDED
#define TEST_MEME_MEMORY_H_INCLUDED

#include <minunit.h>
#include <meme/string_memory.h>
#include <stdlib.h>

MU_TEST(test_mmmem_malloc)
{
    void* p = mmmem_malloc(64);
    mu_assert(p != NULL, "mmmem_malloc(64) should succeed");
    mmmem_free(p);
}

MU_TEST(test_mmmem_calloc)
{
    void* p = mmmem_calloc(4, 16);
    mu_assert(p != NULL, "mmmem_calloc(4, 16) should succeed");
    /* Verify zero-initialized */
    unsigned char* cp = (unsigned char*)p;
    for (int i = 0; i < 64; i++)
        mu_assert(cp[i] == 0, "calloc'd memory should be zeroed");
    mmmem_free(p);
}

MU_TEST(test_mmmem_realloc)
{
    void* p = mmmem_malloc(32);
    mu_assert(p != NULL, "initial malloc should succeed");

    void* q = mmmem_realloc(p, 64);
    mu_assert(q != NULL, "realloc should succeed");
    mmmem_free(q);
}

MU_TEST(test_mmmem_realloc_from_null)
{
    /* realloc(NULL, size) should behave like malloc */
    void* p = mmmem_realloc(NULL, 32);
    mu_assert(p != NULL, "realloc(NULL, 32) should succeed");
    mmmem_free(p);
}

MU_TEST(test_mmmem_free_null)
{
    /* free(NULL) should be a no-op */
    mmmem_free(NULL);
    mu_assert(1, "free(NULL) should not crash");
}

MU_TEST_SUITE(test_meme_memory)
{
    MU_RUN_TEST(test_mmmem_malloc);
    MU_RUN_TEST(test_mmmem_calloc);
    MU_RUN_TEST(test_mmmem_realloc);
    MU_RUN_TEST(test_mmmem_realloc_from_null);
    MU_RUN_TEST(test_mmmem_free_null);
}

#endif // TEST_MEME_MEMORY_H_INCLUDED
