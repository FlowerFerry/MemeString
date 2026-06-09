
#ifndef TEST_MEME_STRING_BASICS_H_INCLUDED
#define TEST_MEME_STRING_BASICS_H_INCLUDED

#include <minunit.h>
#include <meme/string.h>
#include <meme/rune.h>
#include <string.h>

/* Cast helper: mmstrstk_t* → mmstr_cptr_t (= const struct _MemeString_t*).
   Both types are layout-compatible, so a pointer cast is safe. */
#define AS_MMSTR_C(ptr) ((mmstr_cptr_t)(void*)(ptr))

/* Helper: initialize a string from a C string literal */
static void init_from_cstr(mmstrstk_t* s, const char* cstr)
{
    int rc = mmstrstk_init_by_u8(s, cstr, (mmint_t)strlen(cstr));
    mu_assert(rc == 0, "mmstrstk_init_by_u8 should succeed");
}

MU_TEST(test_string_init_empty)
{
    mmstrstk_t s;
    int rc = mmstrstk_init(&s);
    mu_assert(rc == 0, "mmstrstk_init should succeed");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_init_by_u8)
{
    mmstrstk_t s;
    int rc = mmstrstk_init_by_u8(&s, "hello", 5);
    mu_assert(rc == 0, "mmstrstk_init_by_u8 should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&s)) == 5, "byteSize should be 5");
    mu_assert(memcmp(MemeString_byteData(AS_MMSTR_C(&s)), "hello", 5) == 0, "content should match");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_init_by_u8_null)
{
    mmstrstk_t s;
    int rc = mmstrstk_init_by_u8(&s, NULL, 0);
    mu_assert(rc == 0, "mmstrstk_init_by_u8(NULL) should succeed (empty string)");
    mu_assert(MemeString_isEmpty(AS_MMSTR_C(&s)) != 0, "string should be empty");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_init_by_other)
{
    mmstrstk_t s1, s2;
    init_from_cstr(&s1, "hello");
    int rc = mmstrstk_init_by_other(&s2, AS_MMSTR_C(&s1));
    mu_assert(rc == 0, "mmstrstk_init_by_other should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&s2)) == 5, "copied string byteSize should be 5");
    mmstrstk_uninit(&s1);
    mmstrstk_uninit(&s2);
}

MU_TEST(test_string_init_by_other_null)
{
    mmstrstk_t s;
    int rc = mmstrstk_init_by_other(&s, NULL);
    mu_assert(rc == 0, "mmstrstk_init_by_other(NULL) should succeed");
    mu_assert(MemeString_isEmpty(AS_MMSTR_C(&s)) != 0, "string should be empty");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_init_by_rune)
{
    mmstrstk_t s;
    /* U+0041 'A' as a rune */
    mmrune_t r = MemeRune_getInitObject();
    MemeRune_initByByte(&r, 'A');

    int rc = mmstrstk_init_by_rune(&s, 3, r);
    mu_assert(rc == 0, "mmstrstk_init_by_rune should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&s)) == 3, "byteSize should be 3 for 'AAA'");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_init_by_u16)
{
    mmstrstk_t s;
    const uint16_t u16[] = { 'h', 'e', 'l', 'l', 'o', 0 };
    int rc = mmstrstk_init_by_u16(&s, u16, 5);
    mu_assert(rc == 0, "mmstrstk_init_by_u16 should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&s)) == 5, "byteSize should be 5");
    mu_assert(memcmp(MemeString_byteData(AS_MMSTR_C(&s)), "hello", 5) == 0, "content should match");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_init_by_hex)
{
    mmstrstk_t s;
    const uint8_t data[] = { 0xDE, 0xAD, 0xBE };
    int rc = mmstrstk_init_by_hex(&s, NULL, 0, data, 3);
    mu_assert(rc == 0, "mmstrstk_init_by_hex should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&s)) == 6, "hex: 3 bytes -> 6 hex chars");
    const mmbyte_t* d = MemeString_byteData(AS_MMSTR_C(&s));
    mu_assert(d[0] == 'D' && d[1] == 'E' && d[2] == 'A' && d[3] == 'D' && d[4] == 'B' && d[5] == 'E',
        "hex content should be 'DEADBE'");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_init_by_hex_with_sep)
{
    mmstrstk_t s;
    const uint8_t data[] = { 0xDE, 0xAD, 0xBE };
    int rc = mmstrstk_init_by_hex(&s, (const mmbyte_t*)":", 1, data, 3);
    mu_assert(rc == 0, "mmstrstk_init_by_hex with separator should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&s)) == 8, "hex: 3 bytes -> 'DE:AD:BE' (8 chars)");
    const mmbyte_t* d = MemeString_byteData(AS_MMSTR_C(&s));
    mu_assert(d[0] == 'D' && d[1] == 'E' && d[2] == ':' && d[3] == 'A' && d[4] == 'D'
        && d[5] == ':' && d[6] == 'B' && d[7] == 'E', "hex content should be 'DE:AD:BE'");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_init_cond_copy)
{
    mmstrstk_t s1, s2;
    init_from_cstr(&s1, "hello");
    int rc = mmstrstk_init_cond_copy(&s2, AS_MMSTR_C(&s1));
    mu_assert(rc == 0, "mmstrstk_init_cond_copy should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&s2)) == 5, "copied string byteSize should be 5");
    mmstrstk_uninit(&s1);
    mmstrstk_uninit(&s2);
}

MU_TEST(test_string_c_str)
{
    mmstrstk_t s;
    init_from_cstr(&s, "hello");
    const char* cstr = MemeString_cStr(AS_MMSTR_C(&s));
    mu_assert(cstr != NULL, "cStr should not be NULL");
    mu_assert(strcmp(cstr, "hello") == 0, "cStr content should match");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_storage_type)
{
    mmstrstk_t s;
    mmstrstk_init(&s);
    mmstr_strg_t st = MemeString_storageType(AS_MMSTR_C(&s));
    mu_assert(st == MemeString_StorageType_small, "empty string should be small storage");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_byte_size)
{
    mmstrstk_t s;
    init_from_cstr(&s, "hello");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&s)) == 5, "byteSize should be 5");
    mu_assert(MemeString_runeSize(AS_MMSTR_C(&s)) == 5, "runeSize should be 5 for ASCII only");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_is_equal)
{
    mmstrstk_t s;
    init_from_cstr(&s, "hello");
    int result = 0;
    int rc = MemeString_isEqual(AS_MMSTR_C(&s), "hello", 5, &result);
    mu_assert(rc == 0, "isEqual should succeed");
    mu_assert(result != 0, "string should equal 'hello'");

    rc = MemeString_isEqual(AS_MMSTR_C(&s), "world", 5, &result);
    mu_assert(rc == 0, "isEqual should succeed");
    mu_assert(result == 0, "string should not equal 'world'");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_is_empty_nonempty)
{
    mmstrstk_t s;
    mmstrstk_init(&s);
    mu_assert(MemeString_isEmpty(AS_MMSTR_C(&s)) != 0, "empty string should be empty");
    mu_assert(MemeString_isNonempty(AS_MMSTR_C(&s)) == 0, "empty string should not be nonempty");
    mmstrstk_uninit(&s);

    init_from_cstr(&s, "hi");
    mu_assert(MemeString_isNonempty(AS_MMSTR_C(&s)) != 0, "non-empty string should be nonempty");
    mu_assert(MemeString_isEmpty(AS_MMSTR_C(&s)) == 0, "non-empty string should not be empty");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_assign)
{
    mmstrstk_t s;
    mmstrstk_init(&s);

    mmstrstk_t s2;
    init_from_cstr(&s2, "world");

    int rc = mmstrstk_assign(&s, AS_MMSTR_C(&s2));
    mu_assert(rc == 0, "assign should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&s)) == 5, "after assign byteSize should be 5");

    mmstrstk_uninit(&s);
    mmstrstk_uninit(&s2);
}

MU_TEST(test_string_assign_null)
{
    mmstrstk_t s;
    init_from_cstr(&s, "hello");

    int rc = mmstrstk_assign(&s, NULL);
    mu_assert(rc == 0, "assign(NULL) should succeed (reset to empty)");
    mu_assert(MemeString_isEmpty(AS_MMSTR_C(&s)) != 0, "after NULL assign, string should be empty");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_assign_by_utf8)
{
    mmstrstk_t s;
    mmstrstk_init(&s);
    int rc = mmstrstk_assign_by_utf8(&s, (const mmbyte_t*)"test", 4);
    mu_assert(rc == 0, "assign_by_utf8 should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&s)) == 4, "byteSize should be 4");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_reset)
{
    mmstrstk_t s;
    init_from_cstr(&s, "hello");
    int rc = mmstrstk_reset(&s);
    mu_assert(rc == 0, "reset should succeed");
    mu_assert(MemeString_isEmpty(AS_MMSTR_C(&s)) != 0, "after reset string should be empty");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_swap)
{
    mmstrstk_t a, b;
    init_from_cstr(&a, "hello");
    init_from_cstr(&b, "world");

    int rc = MemeString_swap((mmstr_ptr_t)(void*)&a, (mmstr_ptr_t)(void*)&b);
    mu_assert(rc == 0, "swap should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&a)) == 5, "after swap a byteSize should be 5");

    mmstrstk_uninit(&a);
    mmstrstk_uninit(&b);
}

MU_TEST(test_string_is_only_ascii)
{
    mmstrstk_t s;
    int result;
    init_from_cstr(&s, "hello123");
    int rc = MemeString_isOnlyAscii(AS_MMSTR_C(&s), &result);
    mu_assert(rc == 0, "isOnlyAscii should succeed");
    mu_assert(result != 0, "'hello123' should be only ASCII");
    mmstrstk_uninit(&s);

    /* UTF-8 multi-byte sequence (Chinese character) */
    init_from_cstr(&s, "\xe4\xb8\xad"); /* U+4E2D: 中 */
    rc = MemeString_isOnlyAscii(AS_MMSTR_C(&s), &result);
    mu_assert(rc == 0, "isOnlyAscii for Chinese should succeed");
    mu_assert(result == 0, "Chinese character should NOT be only ASCII");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_contains_only_ascii)
{
    mmstrstk_t s;
    int result;
    init_from_cstr(&s, "hello");
    int rc = MemeString_containsOnlyAscii(AS_MMSTR_C(&s), &result);
    mu_assert(rc == 0, "containsOnlyAscii should succeed");
    mu_assert(result != 0, "'hello' should contain only ASCII");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_front_back)
{
    mmstrstk_t s;
    init_from_cstr(&s, "hello");
    const mmbyte_t* f = MemeString_front(AS_MMSTR_C(&s));
    mu_assert(f != NULL && *f == 'h', "front should be 'h'");
    const mmbyte_t* b = MemeString_back(AS_MMSTR_C(&s));
    mu_assert(b != NULL && *b == 'o', "back should be 'o'");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_is_equal_other)
{
    mmstrstk_t a, b;
    init_from_cstr(&a, "hello");
    init_from_cstr(&b, "hello");
    int result;
    int rc = MemeString_isEqualWithOther(AS_MMSTR_C(&a), AS_MMSTR_C(&b), &result);
    mu_assert(rc == 0, "isEqualWithOther should succeed");
    mu_assert(result != 0, "'hello' == 'hello'");

    init_from_cstr(&b, "world");
    rc = MemeString_isEqualWithOther(AS_MMSTR_C(&a), AS_MMSTR_C(&b), &result);
    mu_assert(rc == 0, "isEqualWithOther should succeed");
    mu_assert(result == 0, "'hello' != 'world'");
    mmstrstk_uninit(&a);
    mmstrstk_uninit(&b);
}

MU_TEST(test_string_compare)
{
    mmstrstk_t a, b;
    init_from_cstr(&a, "abc");
    init_from_cstr(&b, "abd");
    int cmp = MemeString_compare(AS_MMSTR_C(&a), AS_MMSTR_C(&b));
    mu_assert(cmp < 0, "'abc' < 'abd'");

    cmp = MemeString_compare(AS_MMSTR_C(&b), AS_MMSTR_C(&a));
    mu_assert(cmp > 0, "'abd' > 'abc'");

    init_from_cstr(&b, "abc");
    cmp = MemeString_compare(AS_MMSTR_C(&a), AS_MMSTR_C(&b));
    mu_assert(cmp == 0, "'abc' == 'abc'");
    mmstrstk_uninit(&a);
    mmstrstk_uninit(&b);
}

MU_TEST(test_string_compare_by_utf8)
{
    mmstrstk_t a;
    init_from_cstr(&a, "abc");
    int cmp = MemeString_compareByUtf8bytes(AS_MMSTR_C(&a), (const mmbyte_t*)"abd", 3);
    mu_assert(cmp < 0, "'abc' < 'abd'");
    mmstrstk_uninit(&a);
}

MU_TEST(test_string_shared_storage_types)
{
    mmstrstk_t s;
    mmstrstk_init(&s);
    mu_assert(MemeString_isSharedStorageTypes(AS_MMSTR_C(&s)) == 0, "small storage is not shared");
    mmstrstk_uninit(&s);
}

MU_TEST(test_string_storage_limit)
{
    mmint_t small_limit = MemeStringOption_getStorageSmallLimit();
    mu_assert(small_limit > 0, "small storage limit should be > 0");

    mmint_t medium_limit = MemeStringOption_getStorageMediumLimit();
    mu_assert(medium_limit >= 0, "medium storage limit should be >= 0");
}

MU_TEST_SUITE(test_meme_string_basics)
{
    MU_RUN_TEST(test_string_init_empty);
    MU_RUN_TEST(test_string_init_by_u8);
    MU_RUN_TEST(test_string_init_by_u8_null);
    MU_RUN_TEST(test_string_init_by_other);
    MU_RUN_TEST(test_string_init_by_other_null);
    MU_RUN_TEST(test_string_init_by_rune);
    MU_RUN_TEST(test_string_init_by_u16);
    MU_RUN_TEST(test_string_init_by_hex);
    MU_RUN_TEST(test_string_init_by_hex_with_sep);
    MU_RUN_TEST(test_string_init_cond_copy);
    MU_RUN_TEST(test_string_c_str);
    MU_RUN_TEST(test_string_storage_type);
    MU_RUN_TEST(test_string_byte_size);
    MU_RUN_TEST(test_string_is_empty_nonempty);
    MU_RUN_TEST(test_string_assign);
    MU_RUN_TEST(test_string_assign_null);
    MU_RUN_TEST(test_string_assign_by_utf8);
    MU_RUN_TEST(test_string_reset);
    MU_RUN_TEST(test_string_swap);
    MU_RUN_TEST(test_string_is_only_ascii);
    MU_RUN_TEST(test_string_contains_only_ascii);
    MU_RUN_TEST(test_string_front_back);
    MU_RUN_TEST(test_string_is_equal_other);
    MU_RUN_TEST(test_string_compare);
    MU_RUN_TEST(test_string_compare_by_utf8);
    MU_RUN_TEST(test_string_shared_storage_types);
    MU_RUN_TEST(test_string_storage_limit);
}

#endif // TEST_MEME_STRING_BASICS_H_INCLUDED
