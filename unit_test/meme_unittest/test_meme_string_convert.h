#ifndef TEST_MEME_STRING_CONVERT_H_INCLUDED
#define TEST_MEME_STRING_CONVERT_H_INCLUDED

#include <minunit.h>
#include "test_shared.h"
#include <meme/utf/u8rune.h>

/* ==================== mappingConvert_v2 ==================== */

static int to_upper_mapping(MemeRune_t* rune, void* user_data)
{
    (void)user_data;
    if (rune->byte[0] >= 'a' && rune->byte[0] <= 'z')
        rune->byte[0] -= 32;
    return 0;
}

MU_TEST(test_mapping_convert_upper)
{
    mmstrstk_t s, out;
    init_cstr(&s, "hello");
    mgec_t rc = MemeStringStack_mappingConvert_v2(&s, to_upper_mapping, NULL, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "mappingConvert should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "converted should be 5 bytes");
    const mmbyte_t* d = MemeString_byteData(AS_MMSTR_C(&out));
    mu_assert(memcmp(d, "HELLO", 5) == 0, "converted should be 'HELLO'");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

static int skip_digit_mapping(MemeRune_t* rune, void* user_data)
{
    (void)user_data;
    if (rune->byte[0] >= '0' && rune->byte[0] <= '9')
        return -1; /* skip this rune */
    return 0;
}

MU_TEST(test_mapping_convert_skip)
{
    mmstrstk_t s, out;
    init_cstr(&s, "a1b2c3");
    mgec_t rc = MemeStringStack_mappingConvert_v2(&s, skip_digit_mapping, NULL, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "mappingConvert skip should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 3, "digits removed -> 3 bytes");
    const mmbyte_t* d = MemeString_byteData(AS_MMSTR_C(&out));
    mu_assert(memcmp(d, "abc", 3) == 0, "content should be 'abc'");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_from_uint16)
{
    mmstrstk_t out;
    mgec_t rc = MemeStringStack_fromUInt16(255, 10, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc != 0, "fromUInt16(255) should fail (buffer size=0)");
    mmstrstk_uninit(&out);
}

MU_TEST(test_from_uint16_hex)
{
    mmstrstk_t out;
    mgec_t rc = MemeStringStack_fromUInt16(255, 16, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc != 0, "fromUInt16(255, 16) should fail (buffer size=0)");
    mmstrstk_uninit(&out);
}

MU_TEST(test_from_uint32)
{
    mmstrstk_t out;
    mgec_t rc = MemeStringStack_fromUInt32(1000000, 10, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc != 0, "fromUInt32(1000000) should fail (buffer size=0)");
    mmstrstk_uninit(&out);
}

MU_TEST(test_from_uint64)
{
    mmstrstk_t out;
    mgec_t rc = MemeStringStack_fromUInt64(10000000000ULL, 10, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc != 0, "fromUInt64(10000000000) should fail (buffer size=0)");
    mmstrstk_uninit(&out);
}

MU_TEST(test_from_uint64_zero)
{
    mmstrstk_t out;
    mgec_t rc = MemeStringStack_fromUInt64(0, 10, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc != 0, "fromUInt64(0) should fail (buffer size=0)");
    mmstrstk_uninit(&out);
}

/* ==================== format ==================== */

MU_TEST(test_format_in_cstyle)
{
    mmstrstk_t out;
    mgec_t rc = MemeStringStack_formatWithLimitInCstyle(&out, MMSTR__OBJ_SIZE, -1, -1, "hello %s %d", "world", 42);
    mu_assert(rc == 0, "formatInCstyle should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 14, "'hello world 42' = 14 bytes");
    const mmbyte_t* d = MemeString_byteData(AS_MMSTR_C(&out));
    mu_assert(memcmp(d, "hello world 42", 14) == 0, "formatted content should match");
    mmstrstk_uninit(&out);
}

MU_TEST(test_format_with_limit)
{
    mmstrstk_t out;
    mgec_t rc = MemeStringStack_formatWithLimitInCstyle(&out, MMSTR__OBJ_SIZE, 5, -1, "hello world");
    mu_assert(rc == 0, "format with limit should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) <= 5, "limited format should be <= 5 bytes");
    mmstrstk_uninit(&out);
}

/* ==================== initByBuffer (string from buffer) ==================== */

MU_TEST(test_string_init_by_buffer)
{
    mmstrstk_t s;
    mmbufstk_t buf;
    int rc = mmbufstk_init_by_bytes(&buf, MMBUF__OBJ_SIZE, (const mmbyte_t*)"buffer data", 11);
    mu_assert(rc == 0, "buffer init should succeed");

    rc = mmstrstk_init_by_buf(&s, AS_MMBUF_C(&buf), 0);
    mu_assert(rc == 0, "string init from buffer should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&s)) == 11, "string should be 11 bytes");

    mmstrstk_uninit(&s);
    mmbufstk_uninit(&buf, MMBUF__OBJ_SIZE);
}

MU_TEST(test_string_init_by_buffer_offset)
{
    mmstrstk_t s;
    mmbufstk_t buf;
    mmbufstk_init_by_bytes(&buf, MMBUF__OBJ_SIZE, (const mmbyte_t*)"hello world", 11);

    int rc = mmstrstk_init_by_buf(&s, AS_MMBUF_C(&buf), 6);
    mu_assert(rc == 0, "string init from buffer with offset should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&s)) == 5, "offset 6 -> 'world' = 5 bytes");
    const mmbyte_t* d = MemeString_byteData(AS_MMSTR_C(&s));
    mu_assert(memcmp(d, "world", 5) == 0, "content should be 'world'");

    mmstrstk_uninit(&s);
    mmbufstk_uninit(&buf, MMBUF__OBJ_SIZE);
}

MU_TEST_SUITE(test_meme_string_convert)
{
    MU_RUN_TEST(test_mapping_convert_upper);
    MU_RUN_TEST(test_mapping_convert_skip);
    MU_RUN_TEST(test_from_uint16);
    MU_RUN_TEST(test_from_uint16_hex);
    MU_RUN_TEST(test_from_uint32);
    MU_RUN_TEST(test_from_uint64);
    MU_RUN_TEST(test_from_uint64_zero);
    MU_RUN_TEST(test_format_in_cstyle);
    MU_RUN_TEST(test_format_with_limit);
    MU_RUN_TEST(test_string_init_by_buffer);
    MU_RUN_TEST(test_string_init_by_buffer_offset);
}

#endif // TEST_MEME_STRING_CONVERT_H_INCLUDED
