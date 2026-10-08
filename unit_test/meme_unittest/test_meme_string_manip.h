#ifndef TEST_MEME_STRING_MANIP_H_INCLUDED
#define TEST_MEME_STRING_MANIP_H_INCLUDED

#include <minunit.h>
#include "test_shared.h"

/* ==================== concat_v2 ==================== */

MU_TEST(test_concat_v2_basic)
{
    mmstrstk_t a, b, out;
    init_cstr(&a, "abc");
    init_cstr(&b, "def");

    mgec_t rc = MemeStringStack_concat_v2(&a, &b, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "concat_v2 should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 6, "concat 'abc'+'def' = 6 bytes");
    const mmbyte_t* d = MemeString_byteData(AS_MMSTR_C(&out));
    mu_assert(memcmp(d, "abcdef", 6) == 0, "concat content should be 'abcdef'");

    mmstrstk_uninit(&a);
    mmstrstk_uninit(&b);
    mmstrstk_uninit(&out);
}

MU_TEST(test_concat_v2_already_inited)
{
    mmstrstk_t a, b, out;
    init_cstr(&a, "abc");
    init_cstr(&b, "def");
    init_cstr(&out, "xxx"); /* pre-initialized */

    mgec_t rc = MemeStringStack_concat_v2(&a, &b, &out, -1);
    mu_assert(rc == 0, "concat_v2 with pre-inited out should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 6, "concat content should be 6 bytes");

    mmstrstk_uninit(&a);
    mmstrstk_uninit(&b);
    mmstrstk_uninit(&out);
}

/* ==================== mid_v2 ==================== */

MU_TEST(test_mid_v2_basic)
{
    mmstrstk_t s, out;
    init_cstr(&s, "hello world");

    mgec_t rc = MemeStringStack_mid_v2(&s, 0, 5, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "mid_v2 should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "substring should be 5 bytes");
    const mmbyte_t* d = MemeString_byteData(AS_MMSTR_C(&out));
    mu_assert(memcmp(d, "hello", 5) == 0, "substring should be 'hello'");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_mid_v2_negative_count)
{
    mmstrstk_t s, out;
    init_cstr(&s, "hello world");

    mgec_t rc = MemeStringStack_mid_v2(&s, 6, -1, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "mid_v2 with negative count should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "substring from 6 should be 'world'");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_mid_v2_offset_beyond)
{
    mmstrstk_t s, out;
    init_cstr(&s, "hi");

    mgec_t rc = MemeStringStack_mid_v2(&s, 100, 5, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "mid_v2 with offset beyond should succeed (empty)");
    mu_assert(MemeString_isEmpty(AS_MMSTR_C(&out)) != 0, "empty substring expected");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

/* ==================== trimSpace_v2 ==================== */

MU_TEST(test_trim_space_basic)
{
    mmstrstk_t s, out;
    init_cstr(&s, "  hello  ");

    mgec_t rc = MemeStringStack_trimSpace_v2(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "trimSpace should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "trimmed should be 5 bytes");
    const mmbyte_t* d = MemeString_byteData(AS_MMSTR_C(&out));
    mu_assert(memcmp(d, "hello", 5) == 0, "trimmed should be 'hello'");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_trim_space_all_whitespace)
{
    mmstrstk_t s, out;
    init_cstr(&s, "   \t\n  ");

    mgec_t rc = MemeStringStack_trimSpace_v2(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "trimSpace of all whitespace should succeed");
    mu_assert(MemeString_isEmpty(AS_MMSTR_C(&out)) != 0, "trimmed all-whitespace should be empty");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_trim_space_empty)
{
    mmstrstk_t s, out;
    mmstrstk_init(&s);

    mgec_t rc = MemeStringStack_trimSpace_v2(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "trimSpace of empty should succeed");
    mu_assert(MemeString_isEmpty(AS_MMSTR_C(&out)) != 0, "trimmed empty should be empty");

    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_trim_left_space)
{
    mmstrstk_t s, out;
    init_cstr(&s, "  hello");

    mgec_t rc = MemeStringStack_trimLeftSpace_v2(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "trimLeftSpace should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "left trimmed should be 'hello'");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_trim_right_space)
{
    mmstrstk_t s, out;
    init_cstr(&s, "hello  ");

    mgec_t rc = MemeStringStack_trimRightSpace_v2(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "trimRightSpace should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "right trimmed should be 'hello'");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

/* ==================== trimByCuts ==================== */

MU_TEST(test_trim_by_cuts_basic)
{
    mmstrstk_t s, out;
    init_cstr(&s, "###hello###");
    mgec_t rc = MemeStringStack_trimByCuts_v2(&s, "#", 1, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "trimByCuts should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "trimmed by cuts should be 5 bytes = 'hello'");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

/* ==================== trimPrefix / trimSuffix ==================== */

MU_TEST(test_trim_prefix_match)
{
    mmstrstk_t s, out;
    init_cstr(&s, "prefix_hello");
    mgec_t rc = MemeStringStack_trimPrefix(&s, "prefix_", 7, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "trimPrefix should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "after prefix trim, should be 'hello'");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_trim_prefix_no_match)
{
    mmstrstk_t s, out;
    init_cstr(&s, "hello");
    mgec_t rc = MemeStringStack_trimPrefix(&s, "nope", 4, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "trimPrefix no match should succeed (full copy)");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "no match -> full copy");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_trim_suffix_match)
{
    mmstrstk_t s, out;
    init_cstr(&s, "hello_suffix");
    mgec_t rc = MemeStringStack_trimSuffix(&s, "_suffix", 7, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "trimSuffix should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "after suffix trim, should be 'hello'");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_trim_suffix_no_match)
{
    mmstrstk_t s, out;
    init_cstr(&s, "hello");
    mgec_t rc = MemeStringStack_trimSuffix(&s, "nope", 4, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "trimSuffix no match should succeed (full copy)");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "no match -> full copy");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

/* ==================== toEnUpper / toEnLower ==================== */

MU_TEST(test_to_en_upper_basic)
{
    mmstrstk_t s, out;
    init_cstr(&s, "hello");
    mgec_t rc = MemeStringStack_toEnUpper_v2(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "toEnUpper should succeed");
    const mmbyte_t* d = MemeString_byteData(AS_MMSTR_C(&out));
    mu_assert(memcmp(d, "HELLO", 5) == 0, "uppercased should be 'HELLO'");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_to_en_lower_basic)
{
    mmstrstk_t s, out;
    init_cstr(&s, "HELLO");
    mgec_t rc = MemeStringStack_toEnLower_v2(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "toEnLower should succeed");
    const mmbyte_t* d = MemeString_byteData(AS_MMSTR_C(&out));
    mu_assert(memcmp(d, "hello", 5) == 0, "lowercased should be 'hello'");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_to_en_upper_non_ascii_unaffected)
{
    mmstrstk_t s, out;
    init_cstr(&s, "\xe4\xb8\xad"); /* U+4E2D: 中 */
    mgec_t rc = MemeStringStack_toEnUpper_v2(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "toEnUpper of non-ASCII should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 3, "non-ASCII chars should be unchanged");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

/* ==================== getRepeat_v2 ==================== */

MU_TEST(test_get_repeat_basic)
{
    mmstrstk_t out;
    mgec_t rc = MemeStringStack_getRepeat_v2(&out, MMSTR__OBJ_SIZE, 3, "abc", 3);
    mu_assert(rc == 0, "getRepeat should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 9, "repeated 3x 'abc' = 9 bytes");
    const mmbyte_t* d = MemeString_byteData(AS_MMSTR_C(&out));
    mu_assert(memcmp(d, "abcabcabc", 9) == 0, "content should be 'abcabcabc'");
    mmstrstk_uninit(&out);
}

MU_TEST(test_get_repeat_zero_count)
{
    mmstrstk_t out;
    mgec_t rc = MemeStringStack_getRepeat_v2(&out, MMSTR__OBJ_SIZE, 0, "abc", 3);
    mu_assert(rc == 0, "getRepeat with 0 count should succeed (empty)");
    mu_assert(MemeString_isEmpty(AS_MMSTR_C(&out)) != 0, "zero repeats should be empty");
    mmstrstk_uninit(&out);
}

MU_TEST(test_get_repeat_neg_len)
{
    mmstrstk_t out;
    mgec_t rc = MemeStringStack_getRepeat_v2(&out, MMSTR__OBJ_SIZE, 2, "ab", -1);
    mu_assert(rc == 0, "getRepeat with negative len should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 4, "'ab'x2 = 4 bytes");
    mmstrstk_uninit(&out);
}

/* ==================== replace_v2 ==================== */

MU_TEST(test_replace_basic)
{
    mmstrstk_t s, out;
    init_cstr(&s, "a-b-c");
    mgec_t rc = MemeStringStack_replace_v2(&s, "-", 1, ",", 1, -1, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "replace should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "replaced 'a,b,c' = 5 bytes");
    const mmbyte_t* d = MemeString_byteData(AS_MMSTR_C(&out));
    mu_assert(memcmp(d, "a,b,c", 5) == 0, "content should be 'a,b,c'");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_replace_empty_pattern)
{
    mmstrstk_t s, out;
    init_cstr(&s, "hello");
    mgec_t rc = MemeStringStack_replace_v2(&s, "", 0, "x", 1, -1, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "replace with empty pattern should succeed (copy)");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "empty pattern -> full copy");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_replace_limited_count)
{
    mmstrstk_t s, out;
    init_cstr(&s, "a-b-c-d");
    mgec_t rc = MemeStringStack_replace_v2(&s, "-", 1, ",", 1, 2, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "replace with count limit should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 7, "2 replacements limited: a,b,c-d");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

/* ==================== toValidUtf8_v2 ==================== */

MU_TEST(test_to_valid_utf8_valid_input)
{
    mmstrstk_t s, out;
    init_cstr(&s, "hello");
    mgec_t rc = MemeStringStack_toValidUtf8_v2(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "toValidUtf8 with valid input should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "valid UTF-8 should be kept as-is");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_to_valid_utf8_empty)
{
    mmstrstk_t s, out;
    mmstrstk_init(&s);
    mgec_t rc = MemeStringStack_toValidUtf8_v2(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "toValidUtf8 with empty should succeed");
    mu_assert(MemeString_isEmpty(AS_MMSTR_C(&out)) != 0, "empty in -> empty out");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

/* ==================== reverse ==================== */

MU_TEST(test_reverse_ascii)
{
    mmstrstk_t s, out;
    init_cstr(&s, "abc");
    mgec_t rc = MemeStringStack_reverse(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "reverse ASCII should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 3, "reversed size == 3");
    mu_assert(memcmp(MemeString_byteData(AS_MMSTR_C(&out)), "cba", 3) == 0, "abc -> cba");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_reverse_chinese)
{
    mmstrstk_t s, out;
    /* 你好 -> 好你 */
    init_cstr(&s, "\xE4\xBD\xA0\xE5\xA5\xBD");
    mgec_t rc = MemeStringStack_reverse(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "reverse Chinese should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 6, "reversed size == 6");
    mu_assert(memcmp(MemeString_byteData(AS_MMSTR_C(&out)), "\xE5\xA5\xBD\xE4\xBD\xA0", 6) == 0, "你好 -> 好你");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_reverse_mixed)
{
    mmstrstk_t s, out;
    /* "a你b" -> "b你a" */
    init_cstr(&s, "a\xE4\xBD\xA0" "b");
    mgec_t rc = MemeStringStack_reverse(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "reverse mixed should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "reversed size == 5");
    mu_assert(memcmp(MemeString_byteData(AS_MMSTR_C(&out)), "b\xE4\xBD\xA0" "a", 5) == 0, "a你b -> b你a");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_reverse_emoji)
{
    mmstrstk_t s, out;
    /* 😀x -> x😀 */
    init_cstr(&s, "\xF0\x9F\x98\x80x");
    mgec_t rc = MemeStringStack_reverse(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "reverse emoji should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "reversed size == 5");
    mu_assert(memcmp(MemeString_byteData(AS_MMSTR_C(&out)), "x\xF0\x9F\x98\x80", 5) == 0, "😀x -> x😀");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_reverse_empty)
{
    mmstrstk_t s, out;
    mmstrstk_init(&s);
    mgec_t rc = MemeStringStack_reverse(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "reverse empty should succeed");
    mu_assert(MemeString_isEmpty(AS_MMSTR_C(&out)) != 0, "empty in -> empty out");
    mmstrstk_uninit(&s);
    mmstrstk_uninit(&out);
}

MU_TEST(test_reverse_invalid_utf8)
{
    mmstrstk_t s, out;
    const char bad[] = { 'a', (char)0x80, 'b' };
    MemeStringStack_initByU8bytes(&s, MMSTR__OBJ_SIZE, (const mmbyte_t*)bad, 3);
    mgec_t rc = MemeStringStack_reverse(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == MGEC__INVAL, "reverse invalid UTF-8 should return MGEC__INVAL");
    mu_assert(MemeString_isEmpty(AS_MMSTR_C(&out)) != 0, "invalid UTF-8 -> empty out");
    mmstrstk_uninit(&s);
}

MU_TEST(test_reverse_double)
{
    mmstrstk_t s, o1, o2;
    const char* in = "Hello\xE4\xBD\xA0\xE5\xA5\xBD\xF0\x9F\x98\x80!";
    int n = (int)strlen(in);
    MemeStringStack_initByU8bytes(&s, MMSTR__OBJ_SIZE, (const mmbyte_t*)in, n);
    mu_assert(MemeStringStack_reverse(&s, &o1, MMSTR__OBJ_SIZE) == 0, "first reverse");
    mu_assert(MemeStringStack_reverse(&o1, &o2, MMSTR__OBJ_SIZE) == 0, "second reverse");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&o2)) == n, "double reverse restores size");
    mu_assert(memcmp(MemeString_byteData(AS_MMSTR_C(&o2)), in, n) == 0, "double reverse restores content");
    mmstrstk_uninit(&o2);
    mmstrstk_uninit(&o1);
    mmstrstk_uninit(&s);
}

MU_TEST(test_reverse_medium)
{
    mmstrstk_t s, out;
    char in[121];
    int i;
    /* 40 x 3-byte rune = 120 bytes (medium storage) */
    for (i = 0; i < 40; ++i) {
        in[i*3]     = (char)(0xE4 + (i % 2));
        in[i*3 + 1] = (char)(0xB8 + (i % 4));
        in[i*3 + 2] = (char)(0x80 + (i % 8));
    }
    in[120] = '\0';
    MemeStringStack_initByU8bytes(&s, MMSTR__OBJ_SIZE, (const mmbyte_t*)in, 120);
    mgec_t rc = MemeStringStack_reverse(&s, &out, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "reverse medium storage should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 120, "medium reversed size == 120");
    /* verify first rune is now last */
    mu_assert(memcmp(MemeString_byteData(AS_MMSTR_C(&out)) + 117, in, 3) == 0, "first rune moved to end");
    mmstrstk_uninit(&out);
    mmstrstk_uninit(&s);
}

/* ==================== join ==================== */

MU_TEST(test_join_basic)
{
    mmstrstk_t items[3];
    init_cstr(&items[0], "a");
    init_cstr(&items[1], "b");
    init_cstr(&items[2], "c");

    mmstrstk_t out;
    mgec_t rc = MemeStringStack_join(&out, MMSTR__OBJ_SIZE, ",", 1, items, 3);
    mu_assert(rc == 0, "join should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "joined 'a,b,c' = 5 bytes");
    const mmbyte_t* d = MemeString_byteData(AS_MMSTR_C(&out));
    mu_assert(memcmp(d, "a,b,c", 5) == 0, "join content should be 'a,b,c'");

    mmstrstk_uninit(&items[0]);
    mmstrstk_uninit(&items[1]);
    mmstrstk_uninit(&items[2]);
    mmstrstk_uninit(&out);
}

MU_TEST(test_join_empty_array)
{
    mmstrstk_t out;
    mgec_t rc = MemeStringStack_join(&out, MMSTR__OBJ_SIZE, ",", 1, NULL, 0);
    mu_assert(rc == 0, "join of empty array should succeed");
    mu_assert(MemeString_isEmpty(AS_MMSTR_C(&out)) != 0, "empty array -> empty result");
    mmstrstk_uninit(&out);
}

/* ==================== writeBytes ==================== */

MU_TEST(test_write_bytes)
{
    mmstrstk_t s;
    init_cstr(&s, "hello");
    mmbyte_t buf[16];
    memset(buf, 0, sizeof(buf));
    mmint_t written = MemeString_writeBytes(AS_MMSTR_C(&s), 0, 5, buf);
    mu_assert(written == 0, "writeBytes should return 0 on success");
    mu_assert(memcmp(buf, "hello", 5) == 0, "buffer content should be 'hello'");
    mmstrstk_uninit(&s);
}

/* ==================== split (deprecated) ==================== */

MU_TEST(test_split_deprecated_basic)
{
    mmstrstk_t s;
    init_cstr(&s, "a,b,c");
    mmstrstk_t out[5];
    mmint_t out_count = 5;
    mmint_t search_index = 0;

    mmint_t rc = MemeString_split(AS_MMSTR_C(&s), ",", 1, MemeFlag_KeepEmptyParts,
        MemeFlag_CaseSensitive, out, &out_count, &search_index);
    mu_assert(rc == 0, "split should succeed");
    mu_assert(out_count == 3, "should get 3 parts");

    for (mmint_t i = 0; i < out_count; i++)
        mmstrstk_uninit(&out[i]);
    mmstrstk_uninit(&s);
}

MU_TEST(test_split_skip_empty)
{
    mmstrstk_t s;
    init_cstr(&s, "a,,b");
    mmstrstk_t out[5];
    mmint_t out_count = 5;
    mmint_t search_index = 0;

    mmint_t rc = MemeString_split(AS_MMSTR_C(&s), ",", 1, MemeFlag_SkipEmptyParts,
        MemeFlag_CaseSensitive, out, &out_count, &search_index);
    mu_assert(rc == 0, "split skip-empty should succeed");
    mu_assert(out_count == 2, "should get 2 parts (skipping empty)");

    for (mmint_t i = 0; i < out_count; i++)
        mmstrstk_uninit(&out[i]);
    mmstrstk_uninit(&s);
}

/* ==================== MemeStringStack_split ==================== */

MU_TEST(test_string_stack_split_basic)
{
    mmstrstk_t s;
    init_cstr(&s, "a,b,c");
    mmstrstk_t out[5];
    mmint_t out_count = 5;
    mmint_t search_index = 0;

    mmint_t rc = MemeStringStack_split(&s, ",", 1, MemeFlag_SkipEmptyParts,
        MemeFlag_CaseSensitive, out, MMSTR__OBJ_SIZE, &out_count, &search_index);
    mu_assert(rc == 0, "MemeStringStack_split should succeed");
    mu_assert(out_count == 3, "should get 3 parts");

    for (mmint_t i = 0; i < out_count; i++)
        mmstrstk_uninit(&out[i]);
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_stack_split_insensitive)
{
    mmstrstk_t s;
    init_cstr(&s, "Aa,Bb,Cc");
    mmstrstk_t out[5];
    mmint_t out_count = 5;
    mmint_t search_index = 0;

    /* Split by lowercase ',' — should work because comma has no case */
    mmint_t rc = MemeStringStack_split(&s, ",", 1, MemeFlag_SkipEmptyParts,
        MemeFlag_AllInsensitive, out, MMSTR__OBJ_SIZE, &out_count, &search_index);
    mu_assert(rc == 0, "MemeStringStack_split insensitive should succeed");
    mu_assert(out_count == 3, "should get 3 parts");

    for (mmint_t i = 0; i < out_count; i++)
        mmstrstk_uninit(&out[i]);
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_stack_split_insensitive_multi_byte_key)
{
    mmstrstk_t s;
    init_cstr(&s, "hello|WORLD|Hello|world");
    mmstrstk_t out[5];
    mmint_t out_count = 5;
    mmint_t search_index = 0;

    /* Split by "|" with case insensitive — matches same character */
    mmint_t rc = MemeStringStack_split(&s, "|", 1, MemeFlag_SkipEmptyParts,
        MemeFlag_AllInsensitive, out, MMSTR__OBJ_SIZE, &out_count, &search_index);
    mu_assert(rc == 0, "MemeStringStack_split insensitive with '|' should succeed");
    mu_assert(out_count == 4, "should get 4 parts");

    for (mmint_t i = 0; i < out_count; i++)
        mmstrstk_uninit(&out[i]);
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_stack_split_insensitive_letter_key)
{
    mmstrstk_t s;
    init_cstr(&s, "abcXdefXghi");
    mmstrstk_t out[5];
    mmint_t out_count = 5;
    mmint_t search_index = 0;

    /* Split by lowercase "x" with case insensitive — should match both "X" */
    mmint_t rc = MemeStringStack_split(&s, "x", 1, MemeFlag_SkipEmptyParts,
        MemeFlag_AllInsensitive, out, MMSTR__OBJ_SIZE, &out_count, &search_index);
    mu_assert(rc == 0, "MemeStringStack_split insensitive letter should succeed");
    mu_assert(out_count == 3, "should get 3 parts (split on X)");

    for (mmint_t i = 0; i < out_count; i++)
        mmstrstk_uninit(&out[i]);
    mmstrstk_uninit(&s);
}

MU_TEST_SUITE(test_meme_string_manip)
{
    MU_RUN_TEST(test_concat_v2_basic);
    MU_RUN_TEST(test_concat_v2_already_inited);
    MU_RUN_TEST(test_mid_v2_basic);
    MU_RUN_TEST(test_mid_v2_negative_count);
    MU_RUN_TEST(test_mid_v2_offset_beyond);
    MU_RUN_TEST(test_trim_space_basic);
    MU_RUN_TEST(test_trim_space_all_whitespace);
    MU_RUN_TEST(test_trim_space_empty);
    MU_RUN_TEST(test_trim_left_space);
    MU_RUN_TEST(test_trim_right_space);
    MU_RUN_TEST(test_trim_by_cuts_basic);
    MU_RUN_TEST(test_trim_prefix_match);
    MU_RUN_TEST(test_trim_prefix_no_match);
    MU_RUN_TEST(test_trim_suffix_match);
    MU_RUN_TEST(test_trim_suffix_no_match);
    MU_RUN_TEST(test_to_en_upper_basic);
    MU_RUN_TEST(test_to_en_lower_basic);
    MU_RUN_TEST(test_to_en_upper_non_ascii_unaffected);
    MU_RUN_TEST(test_get_repeat_basic);
    MU_RUN_TEST(test_get_repeat_zero_count);
    MU_RUN_TEST(test_get_repeat_neg_len);
    MU_RUN_TEST(test_replace_basic);
    MU_RUN_TEST(test_replace_empty_pattern);
    MU_RUN_TEST(test_replace_limited_count);
    MU_RUN_TEST(test_to_valid_utf8_valid_input);
    MU_RUN_TEST(test_to_valid_utf8_empty);
    MU_RUN_TEST(test_reverse_ascii);
    MU_RUN_TEST(test_reverse_chinese);
    MU_RUN_TEST(test_reverse_mixed);
    MU_RUN_TEST(test_reverse_emoji);
    MU_RUN_TEST(test_reverse_empty);
    MU_RUN_TEST(test_reverse_invalid_utf8);
    MU_RUN_TEST(test_reverse_double);
    MU_RUN_TEST(test_reverse_medium);
    MU_RUN_TEST(test_join_basic);
    MU_RUN_TEST(test_join_empty_array);
    MU_RUN_TEST(test_write_bytes);
    MU_RUN_TEST(test_split_deprecated_basic);
    MU_RUN_TEST(test_split_skip_empty);
    MU_RUN_TEST(test_string_stack_split_basic);
    MU_RUN_TEST(test_string_stack_split_insensitive);
    MU_RUN_TEST(test_string_stack_split_insensitive_multi_byte_key);
    MU_RUN_TEST(test_string_stack_split_insensitive_letter_key);
}

#endif // TEST_MEME_STRING_MANIP_H_INCLUDED
