#ifndef TEST_MEME_STRING_INDEX_H_INCLUDED
#define TEST_MEME_STRING_INDEX_H_INCLUDED

#include <minunit.h>
#include "test_shared.h"

static int is_digit_cond(MemeByte_t ch, void* _user_data)
{
    (void)_user_data;
    return (ch >= '0' && ch <= '9') ? 1 : 0;
}

MU_TEST(test_index_of_with_utf8bytes_found)
{
    mmstrstk_t s;
    init_cstr(&s, "hello world");
    mmint_t pos = MemeString_indexOfWithUtf8bytes(AS_MMSTR_C(&s), 0,
        (const mmbyte_t*)"world", 5, MemeFlag_AllSensitive);
    mu_assert(pos == 6, "'world' should be at index 6");
    mmstrstk_uninit(&s);
}

MU_TEST(test_index_of_with_utf8bytes_not_found)
{
    mmstrstk_t s;
    init_cstr(&s, "hello");
    mmint_t pos = MemeString_indexOfWithUtf8bytes(AS_MMSTR_C(&s), 0,
        (const mmbyte_t*)"xyz", 3, MemeFlag_AllSensitive);
    mu_assert(pos < 0, "'xyz' should not be found");
    mmstrstk_uninit(&s);
}

MU_TEST(test_index_of_with_utf8bytes_insensitive)
{
    mmstrstk_t s;
    init_cstr(&s, "Hello World");
    mmint_t pos = MemeString_indexOfWithUtf8bytes(AS_MMSTR_C(&s), 0,
        (const mmbyte_t*)"world", 5, MemeFlag_AllInsensitive);
    mu_assert(pos < 0, "'world' (case insensitive) not supported for multi-byte");
    mmstrstk_uninit(&s);
}

MU_TEST(test_index_of_with_utf8bytes_offset)
{
    mmstrstk_t s;
    init_cstr(&s, "hello hello");
    mmint_t pos = MemeString_indexOfWithUtf8bytes(AS_MMSTR_C(&s), 6,
        (const mmbyte_t*)"hello", 5, MemeFlag_AllSensitive);
    mu_assert(pos == 6, "second 'hello' should be at index 6");
    mmstrstk_uninit(&s);
}

MU_TEST(test_index_of_with_byte)
{
    mmstrstk_t s;
    init_cstr(&s, "hello");
    mmint_t pos = MemeString_indexOfWithByte(AS_MMSTR_C(&s), 0, 'l', MemeFlag_CaseSensitive);
    mu_assert(pos == 2, "'l' should be at index 2");
    mmstrstk_uninit(&s);
}

MU_TEST(test_index_of_with_other)
{
    mmstrstk_t s, needle;
    init_cstr(&s, "hello world");
    init_cstr(&needle, "world");
    mmint_t pos = MemeString_indexOfWithOther(AS_MMSTR_C(&s), 0, AS_MMSTR_C(&needle), MemeFlag_AllSensitive);
    mu_assert(pos == 6, "'world' should be at index 6");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&needle);
}

MU_TEST(test_last_index_of_with_utf8bytes)
{
    mmstrstk_t s;
    init_cstr(&s, "hello hello");
    mmint_t pos = MemeString_lastIndexOfWithUtf8bytes(AS_MMSTR_C(&s), -1,
        (const mmbyte_t*)"hello", 5, MemeFlag_AllSensitive);
    mu_assert(pos == 6, "last 'hello' should be at index 6");
    mmstrstk_uninit(&s);
}

MU_TEST(test_match_count_with_utf8bytes)
{
    mmstrstk_t s;
    init_cstr(&s, "a,b,c,d");
    mmint_t cnt = MemeString_matchCountWithUtf8bytes(AS_MMSTR_C(&s), 0,
        (const mmbyte_t*)",", 1, MemeFlag_CaseSensitive);
    mu_assert(cnt == 3, "there should be 3 commas");
    mmstrstk_uninit(&s);
}

MU_TEST(test_starts_match_with_utf8bytes)
{
    mmstrstk_t s;
    init_cstr(&s, "hello world");
    mu_assert(MemeString_startsMatchWithUtf8bytes(AS_MMSTR_C(&s),
        (const mmbyte_t*)"hello", 5, MemeFlag_CaseSensitive) != 0,
        "should start with 'hello'");
    mu_assert(MemeString_startsMatchWithUtf8bytes(AS_MMSTR_C(&s),
        (const mmbyte_t*)"world", 5, MemeFlag_CaseSensitive) == 0,
        "should not start with 'world'");
    mmstrstk_uninit(&s);
}

MU_TEST(test_ends_match_with_utf8bytes)
{
    mmstrstk_t s;
    init_cstr(&s, "hello world");
    mu_assert(MemeString_endsMatchWithUtf8bytes(AS_MMSTR_C(&s),
        (const mmbyte_t*)"world", 5, MemeFlag_CaseSensitive) != 0,
        "should end with 'world'");
    mu_assert(MemeString_endsMatchWithUtf8bytes(AS_MMSTR_C(&s),
        (const mmbyte_t*)"hello", 5, MemeFlag_CaseSensitive) == 0,
        "should not end with 'hello'");
    mmstrstk_uninit(&s);
}

MU_TEST(test_starts_match_with_other)
{
    mmstrstk_t s, prefix;
    init_cstr(&s, "hello world");
    init_cstr(&prefix, "hello");
    mu_assert(MemeString_startsMatchWithOther(AS_MMSTR_C(&s), AS_MMSTR_C(&prefix), MemeFlag_CaseSensitive) != 0,
        "should start with 'hello'");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&prefix);
}

MU_TEST(test_ends_match_with_other)
{
    mmstrstk_t s, suffix;
    init_cstr(&s, "hello world");
    init_cstr(&suffix, "world");
    mu_assert(MemeString_endsMatchWithOther(AS_MMSTR_C(&s), AS_MMSTR_C(&suffix), MemeFlag_CaseSensitive) != 0,
        "should end with 'world'");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&suffix);
}

MU_TEST(test_index_by_cond_byte_func)
{
    mmstrstk_t s;
    init_cstr(&s, "abc123");

    mmint_t pos = MemeString_indexByCondByteFunc(AS_MMSTR_C(&s), 0,
        is_digit_cond, NULL);
    mu_assert(pos == 3, "first digit should be at index 3");
    mmstrstk_uninit(&s);
}

MU_TEST(test_index_of_utf8bytes_size_limit)
{
    mmstrstk_t s;
    init_cstr(&s, "hello world");
    mmint_t pos = MemeString_indexOfWithUtf8bytesAndSizeLimit(AS_MMSTR_C(&s), 0, 5,
        (const mmbyte_t*)"o", 1, MemeFlag_CaseSensitive);
    mu_assert(pos == 4, "'o' in first 5 bytes should be at index 4");
    mmstrstk_uninit(&s);
}

MU_TEST_SUITE(test_meme_string_index)
{
    MU_RUN_TEST(test_index_of_with_utf8bytes_found);
    MU_RUN_TEST(test_index_of_with_utf8bytes_not_found);
    MU_RUN_TEST(test_index_of_with_utf8bytes_insensitive);
    MU_RUN_TEST(test_index_of_with_utf8bytes_offset);
    MU_RUN_TEST(test_index_of_with_byte);
    MU_RUN_TEST(test_index_of_with_other);
    MU_RUN_TEST(test_last_index_of_with_utf8bytes);
    MU_RUN_TEST(test_match_count_with_utf8bytes);
    MU_RUN_TEST(test_starts_match_with_utf8bytes);
    MU_RUN_TEST(test_ends_match_with_utf8bytes);
    MU_RUN_TEST(test_starts_match_with_other);
    MU_RUN_TEST(test_ends_match_with_other);
    MU_RUN_TEST(test_index_by_cond_byte_func);
    MU_RUN_TEST(test_index_of_utf8bytes_size_limit);
}

#endif // TEST_MEME_STRING_INDEX_H_INCLUDED
