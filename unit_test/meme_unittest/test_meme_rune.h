#ifndef TEST_MEME_RUNE_H_INCLUDED
#define TEST_MEME_RUNE_H_INCLUDED

#include <minunit.h>
#include <meme/rune.h>
#include <string.h>

MU_TEST(test_rune_get_init)
{
    mmrune_t r = MemeRune_getInitObject();
    mu_assert(MemeRune_isEmpty(&r) != 0, "empty rune should be empty");
    mu_assert(MemeRune_isValid(&r) != 0, "empty rune should be valid");
    mu_assert(MemeRune_size(&r) == 0, "empty rune size should be 0");
}

MU_TEST(test_rune_init_by_byte)
{
    mmrune_t r = MemeRune_getInitObject();
    int rc = MemeRune_initByByte(&r, 'A');
    mu_assert(rc == 0, "initByByte should succeed");
    mu_assert(MemeRune_size(&r) == 1, "byte rune size should be 1");
    mu_assert(MemeRune_data(&r)[0] == 'A', "rune data should be 'A'");
}

MU_TEST(test_rune_init_by_utf8_2byte)
{
    /* U+00A9 © copyright sign: 0xC2 0xA9 */
    const mmbyte_t utf8[] = { 0xC2, 0xA9 };
    mmrune_t r = MemeRune_getInitObject();
    int rc = MemeRune_initByUtf8Bytes(&r, utf8, 2);
    mu_assert(rc == 0, "initByUtf8Bytes for 2-byte rune should succeed");
    mu_assert(MemeRune_size(&r) == 2, "2-byte rune size should be 2");
}

MU_TEST(test_rune_init_by_utf8_3byte)
{
    /* U+4E2D 中: 0xE4 0xB8 0xAD */
    const mmbyte_t utf8[] = { 0xE4, 0xB8, 0xAD };
    mmrune_t r = MemeRune_getInitObject();
    int rc = MemeRune_initByUtf8Bytes(&r, utf8, 3);
    mu_assert(rc == 0, "initByUtf8Bytes for 3-byte rune should succeed");
    mu_assert(MemeRune_size(&r) == 3, "3-byte rune size should be 3");
}

MU_TEST(test_rune_init_by_utf8_too_long)
{
    mmrune_t r = MemeRune_getInitObject();
    const mmbyte_t buf[10] = { 0 };
    int rc = MemeRune_initByUtf8Bytes(&r, buf, 10);
    mu_assert(rc != 0, "initByUtf8Bytes with > MAX_CHAR should fail");
}

MU_TEST(test_rune_init_by_other)
{
    mmrune_t r1 = MemeRune_getInitObject();
    MemeRune_initByByte(&r1, 'Z');
    mmrune_t r2 = MemeRune_getInitObject();
    int rc = MemeRune_initByOther(&r2, &r1);
    mu_assert(rc == 0, "initByOther should succeed");
    mu_assert(MemeRune_data(&r2)[0] == 'Z', "copied rune should be 'Z'");
}

MU_TEST(test_rune_reset)
{
    mmrune_t r = MemeRune_getInitObject();
    MemeRune_initByByte(&r, 'X');
    mu_assert(MemeRune_isEmpty(&r) == 0, "after init, rune should not be empty");
    int rc = MemeRune_reset(&r);
    mu_assert(rc == 0, "reset should succeed");
    mu_assert(MemeRune_isEmpty(&r) != 0, "after reset, rune should be empty");
}

MU_TEST(test_rune_assign)
{
    mmrune_t r1 = MemeRune_getInitObject();
    MemeRune_initByByte(&r1, 'Q');
    mmrune_t r2 = MemeRune_getInitObject();
    int rc = MemeRune_assign(&r2, &r1);
    mu_assert(rc == 0, "assign should succeed");
    mu_assert(MemeRune_data(&r2)[0] == 'Q', "assigned rune should be 'Q'");
}

MU_TEST(test_rune_swap)
{
    mmrune_t a = MemeRune_getInitObject();
    mmrune_t b = MemeRune_getInitObject();
    MemeRune_initByByte(&a, 'A');
    MemeRune_initByByte(&b, 'B');
    MemeRune_swap(&a, &b);
    mu_assert(MemeRune_data(&a)[0] == 'B', "after swap a should be 'B'");
    mu_assert(MemeRune_data(&b)[0] == 'A', "after swap b should be 'A'");
}

MU_TEST(test_rune_is_space)
{
    mmrune_t r = MemeRune_getInitObject();
    MemeRune_initByByte(&r, ' ');
    mu_assert(MemeRune_isSpace(&r) != 0, "space should be space");

    MemeRune_initByByte(&r, 'A');
    mu_assert(MemeRune_isSpace(&r) == 0, "'A' should not be space");
}

MU_TEST(test_rune_is_ch_punct)
{
    mmrune_t r = MemeRune_getInitObject();
    /* ，(U+FF0C fullwidth comma): 0xEF 0xBC 0x8C */
    const mmbyte_t comma_utf8[] = { 0xEF, 0xBC, 0x8C };
    MemeRune_initByUtf8Bytes(&r, comma_utf8, 3);
    mu_assert(MemeRune_isChPunct(&r) != 0, "fullwidth comma should be Chinese punctuation");
}

MU_TEST(test_rune_resize)
{
    mmrune_t r = MemeRune_getInitObject();
    MemeRune_initByByte(&r, 'A');
    int rc = MemeRune_resize(&r, 0);
    mu_assert(rc == 0, "resize to 0 should succeed");
    mu_assert(MemeRune_isEmpty(&r) != 0, "after resize to 0, rune should be empty");
}

MU_TEST(test_rune_is_multi_char)
{
    mmrune_t r = MemeRune_getInitObject();
    MemeRune_initByByte(&r, 'A');
    mu_assert(MemeRune_isMulitChar(&r) == 0, "'A' is not multi-char");
}

MU_TEST_SUITE(test_meme_rune)
{
    MU_RUN_TEST(test_rune_get_init);
    MU_RUN_TEST(test_rune_init_by_byte);
    MU_RUN_TEST(test_rune_init_by_utf8_2byte);
    MU_RUN_TEST(test_rune_init_by_utf8_3byte);
    MU_RUN_TEST(test_rune_init_by_utf8_too_long);
    MU_RUN_TEST(test_rune_init_by_other);
    MU_RUN_TEST(test_rune_reset);
    MU_RUN_TEST(test_rune_assign);
    MU_RUN_TEST(test_rune_swap);
    MU_RUN_TEST(test_rune_is_space);
    MU_RUN_TEST(test_rune_is_ch_punct);
    MU_RUN_TEST(test_rune_resize);
    MU_RUN_TEST(test_rune_is_multi_char);
}

#endif // TEST_MEME_RUNE_H_INCLUDED
