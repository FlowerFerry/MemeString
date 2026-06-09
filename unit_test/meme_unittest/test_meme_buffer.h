#ifndef TEST_MEME_BUFFER_H_INCLUDED
#define TEST_MEME_BUFFER_H_INCLUDED

#include <minunit.h>
#include "test_shared.h"

/* Rules: mmbufstk_* functions take mmbufstk_t* (no cast);
          mmbuf_* functions take mmbuf_cptr_t/mm buf_ptr_t (need AS_MMBUF_C/AS_MMBUF). */

MU_TEST(test_buffer_init_empty)
{
    mmbufstk_t b;
    int rc = mmbufstk_init(&b, MMBUF__OBJ_SIZE);
    mu_assert(rc == 0, "mmbufstk_init should succeed");
    mu_check(mmbuf_is_empty(AS_MMBUF_C(&b)) != 0);
    mu_check(mmbuf_size(AS_MMBUF_C(&b)) == 0);
    mmbufstk_uninit(&b, MMBUF__OBJ_SIZE);
}

MU_TEST(test_buffer_init_by_bytes)
{
    mmbufstk_t b;
    const mmbyte_t data[] = { 0x01, 0x02, 0x03, 0x04 };
    int rc = mmbufstk_init_by_bytes(&b, MMBUF__OBJ_SIZE, data, 4);
    mu_assert(rc == 0, "mmbufstk_init_by_bytes should succeed");
    mu_assert(mmbuf_size(AS_MMBUF_C(&b)) == 4, "buffer size should be 4");
    mu_assert(memcmp(mmbuf_data(AS_MMBUF_C(&b)), data, 4) == 0, "buffer data should match");
    mmbufstk_uninit(&b, MMBUF__OBJ_SIZE);
}

MU_TEST(test_buffer_init_by_other)
{
    mmbufstk_t b1, b2;
    const mmbyte_t data[] = { 'a', 'b', 'c' };
    mmbufstk_init_by_bytes(&b1, MMBUF__OBJ_SIZE, data, 3);

    int rc = mmbufstk_init_by_other(&b2, MMBUF__OBJ_SIZE, &b1);
    mu_assert(rc == 0, "mmbufstk_init_by_other should succeed");
    mu_assert(mmbuf_size(AS_MMBUF_C(&b2)) == 3, "copied buffer size should be 3");
    mu_assert(memcmp(mmbuf_data(AS_MMBUF_C(&b2)), data, 3) == 0, "copied data should match");
    mmbufstk_uninit(&b1, MMBUF__OBJ_SIZE);
    mmbufstk_uninit(&b2, MMBUF__OBJ_SIZE);
}

MU_TEST(test_buffer_init_cond_copy)
{
    mmbufstk_t b1, b2;
    mmbufstk_init_by_bytes(&b1, MMBUF__OBJ_SIZE, (const mmbyte_t*)"test", 4);
    int rc = mmbufstk_init_cond_copy(&b2, MMBUF__OBJ_SIZE, AS_MMBUF_C(&b1));
    mu_assert(rc == 0, "mmbufstk_init_cond_copy should succeed");
    mu_assert(mmbuf_size(AS_MMBUF_C(&b2)) == 4, "cond copy size should be 4");
    mmbufstk_uninit(&b1, MMBUF__OBJ_SIZE);
    mmbufstk_uninit(&b2, MMBUF__OBJ_SIZE);
}

MU_TEST(test_buffer_assign)
{
    mmbufstk_t b1, b2;
    mmbufstk_init(&b1, MMBUF__OBJ_SIZE);
    mmbufstk_init_by_bytes(&b2, MMBUF__OBJ_SIZE, (const mmbyte_t*)"data", 4);

    int rc = mmbufstk_assign(&b1, MMBUF__OBJ_SIZE, &b2);
    mu_assert(rc == 0, "mmbufstk_assign should succeed");
    mu_assert(mmbuf_size(AS_MMBUF_C(&b1)) == 4, "assigned buffer size should be 4");
    mmbufstk_uninit(&b1, MMBUF__OBJ_SIZE);
    mmbufstk_uninit(&b2, MMBUF__OBJ_SIZE);
}

MU_TEST(test_buffer_reset)
{
    mmbufstk_t b;
    mmbufstk_init_by_bytes(&b, MMBUF__OBJ_SIZE, (const mmbyte_t*)"data", 4);
    int rc = mmbufstk_reset(&b, MMBUF__OBJ_SIZE);
    mu_assert(rc == 0, "mmbufstk_reset should succeed");
    mu_check(mmbuf_is_empty(AS_MMBUF_C(&b)) != 0);
    mmbufstk_uninit(&b, MMBUF__OBJ_SIZE);
}

MU_TEST(test_buffer_storage_type)
{
    mmbufstk_t b;
    mmbufstk_init(&b, MMBUF__OBJ_SIZE);
    mmbuf_strg_t st = mmbuf_strg_type(AS_MMBUF_C(&b));
    mu_assert(st >= 0, "storage type should be valid");
    mmbufstk_uninit(&b, MMBUF__OBJ_SIZE);
}

MU_TEST(test_buffer_swap)
{
    mmbufstk_t a, b;
    mmbufstk_init_by_bytes(&a, MMBUF__OBJ_SIZE, (const mmbyte_t*)"ab", 2);
    mmbufstk_init_by_bytes(&b, MMBUF__OBJ_SIZE, (const mmbyte_t*)"cd", 2);

    int rc = mmbuf_swap(AS_MMBUF(&a), AS_MMBUF(&b));
    mu_assert(rc == 0, "mmbuf_swap should succeed");

    mu_check(mmbuf_data(AS_MMBUF_C(&a))[0] == 'c');
    mmbufstk_uninit(&a, MMBUF__OBJ_SIZE);
    mmbufstk_uninit(&b, MMBUF__OBJ_SIZE);
}

MU_TEST(test_buffer_is_equal)
{
    mmbufstk_t b;
    int result;
    mmbufstk_init_by_bytes(&b, MMBUF__OBJ_SIZE, (const mmbyte_t*)"test", 4);

    int rc = mmbuf_is_equal(AS_MMBUF_C(&b), (const mmbyte_t*)"test", 4, &result);
    mu_assert(rc == 0, "mmbuf_is_equal should succeed");
    mu_check(result != 0);

    rc = mmbuf_is_equal(AS_MMBUF_C(&b), (const mmbyte_t*)"other", 5, &result);
    mu_assert(rc == 0, "mmbuf_is_equal should succeed");
    mu_check(result == 0);
    mmbufstk_uninit(&b, MMBUF__OBJ_SIZE);
}

MU_TEST(test_buffer_is_equal_other)
{
    mmbufstk_t a, b;
    int result;
    mmbufstk_init_by_bytes(&a, MMBUF__OBJ_SIZE, (const mmbyte_t*)"data", 4);
    mmbufstk_init_by_bytes(&b, MMBUF__OBJ_SIZE, (const mmbyte_t*)"data", 4);

    int rc = mmbuf_is_equal_other(AS_MMBUF_C(&a), AS_MMBUF_C(&b), &result);
    mu_assert(rc == 0, "mmbuf_is_equal_other should succeed");
    mu_check(result != 0);
    mmbufstk_uninit(&a, MMBUF__OBJ_SIZE);
    mmbufstk_uninit(&b, MMBUF__OBJ_SIZE);
}

MU_TEST(test_buffer_at)
{
    mmbufstk_t b;
    mmbufstk_init_by_bytes(&b, MMBUF__OBJ_SIZE, (const mmbyte_t*)"hello", 5);
    mu_check(mmbuf_at(AS_MMBUF_C(&b), 1)[0] == 'e');
    mmbufstk_uninit(&b, MMBUF__OBJ_SIZE);
}

MU_TEST(test_buffer_is_shared_strg_type)
{
    mmbufstk_t b;
    mmbufstk_init(&b, MMBUF__OBJ_SIZE);
    mu_check(mmbuf_is_shared_strg_type(AS_MMBUF_C(&b)) == 0);
    mmbufstk_uninit(&b, MMBUF__OBJ_SIZE);
}

MU_TEST(test_buffer_index_of_bytes)
{
    mmbufstk_t b;
    mmbufstk_init_by_bytes(&b, MMBUF__OBJ_SIZE, (const mmbyte_t*)"hello world", 11);
    mu_assert(mmbuf_index_of_bytes(AS_MMBUF_C(&b), 0, (const mmbyte_t*)"world", 5) == 6,
        "'world' should be at index 6");
    mmbufstk_uninit(&b, MMBUF__OBJ_SIZE);
}

MU_TEST(test_buffer_index_of_other)
{
    mmbufstk_t b, needle;
    mmbufstk_init_by_bytes(&b, MMBUF__OBJ_SIZE, (const mmbyte_t*)"hello world", 11);
    mmbufstk_init_by_bytes(&needle, MMBUF__OBJ_SIZE, (const mmbyte_t*)"world", 5);
    mu_assert(mmbuf_index_of_other(AS_MMBUF_C(&b), 0, AS_MMBUF_C(&needle)) == 6,
        "'world' should be at index 6");
    mmbufstk_uninit(&b, MMBUF__OBJ_SIZE);
    mmbufstk_uninit(&needle, MMBUF__OBJ_SIZE);
}

MU_TEST(test_buffer_starts_end_match)
{
    mmbufstk_t b;
    mmbufstk_init_by_bytes(&b, MMBUF__OBJ_SIZE, (const mmbyte_t*)"hello world", 11);
    mu_check(mmbuf_starts_match_with_bytes(AS_MMBUF_C(&b), (const mmbyte_t*)"hello", 5) != 0);
    mu_check(mmbuf_ends_match_with_bytes(AS_MMBUF_C(&b), (const mmbyte_t*)"world", 5) != 0);
    mu_check(mmbuf_starts_match_with_bytes(AS_MMBUF_C(&b), (const mmbyte_t*)"world", 5) == 0);
    mmbufstk_uninit(&b, MMBUF__OBJ_SIZE);
}

MU_TEST(test_buffer_split)
{
    mmbufstk_t b;
    mmbufstk_init_by_bytes(&b, MMBUF__OBJ_SIZE, (const mmbyte_t*)"a,b,c", 5);
    mmbufstk_t out[5];
    mmint_t out_count = 5;
    mmint_t search_index = 0;

    mmint_t rc = mmbuf_split(AS_MMBUF_C(&b), (const mmbyte_t*)",", 1,
        MemeFlag_KeepEmptyParts, out, &out_count, &search_index);
    mu_assert(rc == 0, "split should succeed");
    mu_assert(out_count == 3, "should get 3 parts");

    mmbufstk_uninit(&b, MMBUF__OBJ_SIZE);
    for (mmint_t i = 0; i < out_count; i++)
        mmbufstk_uninit(&out[i], MMBUF__OBJ_SIZE);
}

MU_TEST_SUITE(test_meme_buffer)
{
    MU_RUN_TEST(test_buffer_init_empty);
    MU_RUN_TEST(test_buffer_init_by_bytes);
    MU_RUN_TEST(test_buffer_init_by_other);
    MU_RUN_TEST(test_buffer_init_cond_copy);
    MU_RUN_TEST(test_buffer_assign);
    MU_RUN_TEST(test_buffer_reset);
    MU_RUN_TEST(test_buffer_storage_type);
    MU_RUN_TEST(test_buffer_swap);
    MU_RUN_TEST(test_buffer_is_equal);
    MU_RUN_TEST(test_buffer_is_equal_other);
    MU_RUN_TEST(test_buffer_at);
    MU_RUN_TEST(test_buffer_is_shared_strg_type);
    MU_RUN_TEST(test_buffer_index_of_bytes);
    MU_RUN_TEST(test_buffer_index_of_other);
    MU_RUN_TEST(test_buffer_starts_end_match);
    MU_RUN_TEST(test_buffer_split);
}

#endif // TEST_MEME_BUFFER_H_INCLUDED
