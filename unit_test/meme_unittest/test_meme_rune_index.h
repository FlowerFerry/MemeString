#ifndef TEST_MEME_RUNE_INDEX_H_INCLUDED
#define TEST_MEME_RUNE_INDEX_H_INCLUDED

#include <minunit.h>
#include <meme/rune.h>
#include <meme/utf/u8rune.h>

MU_TEST(test_rune_index_is_space_ascii)
{
    /* ASCII space: 0x20, size 1 */
    const mmbyte_t space[] = { 0x20 };
    int result = MemeRuneIndex_isSpace(space, 1);
    mu_assert(result != 0, "space should be space");

    const mmbyte_t tab[] = { 0x09 };
    result = MemeRuneIndex_isSpace(tab, 1);
    mu_assert(result != 0, "tab should be space");
}

MU_TEST(test_rune_index_is_space_non_ascii)
{
    /* U+3000 ideographic space: 0xE3 0x80 0x80 */
    const mmbyte_t ideo_space[] = { 0xE3, 0x80, 0x80 };
    int result = MemeRuneIndex_isSpace(ideo_space, 3);
    mu_assert(result != 0, "ideographic space should be space");

    /* U+00A0 no-break space: 0xC2 0xA0 */
    const mmbyte_t nbsp[] = { 0xC2, 0xA0 };
    result = MemeRuneIndex_isSpace(nbsp, 2);
    mu_assert(result != 0, "no-break space should be space");
}

MU_TEST(test_rune_index_is_space_not)
{
    const mmbyte_t letter[] = { 'A' };
    int result = MemeRuneIndex_isSpace(letter, 1);
    mu_assert(result == 0, "'A' should not be space");
}

MU_TEST(test_rune_index_is_ch_punct)
{
    /* 。 (U+3002): 0xE3 0x80 0x82 */
    const mmbyte_t ch_period[] = { 0xE3, 0x80, 0x82 };
    int result = MemeRuneIndex_isChPunct(ch_period, 3);
    mu_assert(result != 0, "Chinese period should be Chinese punctuation");

    /* non-punctuation: U+4E2D 中 */
    const mmbyte_t chinese_char[] = { 0xE4, 0xB8, 0xAD };
    result = MemeRuneIndex_isChPunct(chinese_char, 3);
    mu_assert(result == 0, "Chinese char 中 should not be punctuation");
}

MU_TEST(test_rune_index_is_fullwidth_digit)
{
    /* １ (U+FF11 fullwidth digit 1): 0xEF 0xBC 0x91 */
    const mmbyte_t fw_one[] = { 0xEF, 0xBC, 0x91 };
    int result = MemeRuneIndex_isFullWidthDigit(fw_one, 3);
    mu_assert(result != 0, "fullwidth digit 1 should be fullwidth digit");

    /* ASCII '1' */
    const mmbyte_t ascii_one[] = { '1' };
    result = MemeRuneIndex_isFullWidthDigit(ascii_one, 1);
    mu_assert(result == 0, "ASCII '1' should not be fullwidth digit");
}

MU_TEST(test_rune_index_is_chinese_digit)
{
    /* 一 (U+4E00): 0xE4 0xB8 0x80 */
    const mmbyte_t yi[] = { 0xE4, 0xB8, 0x80 };
    int result = MemeRuneIndex_isChineseDigit(yi, 3);
    mu_assert(result != 0, "U+4E00 should be Chinese digit");

    /* 二 (U+4E8C): 0xE4 0xBA 0x8C */
    const mmbyte_t er[] = { 0xE4, 0xBA, 0x8C };
    result = MemeRuneIndex_isChineseDigit(er, 3);
    mu_assert(result != 0, "U+4E8C should be Chinese digit");
}

MU_TEST(test_mmruneidx_is_space)
{
    const mmbyte_t space[] = { 0x20 };
    int result = mmruneidx_is_space(space, 1);
    mu_assert(result != 0, "mmruneidx_is_space should identify space");
}

MU_TEST_SUITE(test_meme_rune_index)
{
    MU_RUN_TEST(test_rune_index_is_space_ascii);
    MU_RUN_TEST(test_rune_index_is_space_non_ascii);
    MU_RUN_TEST(test_rune_index_is_space_not);
    MU_RUN_TEST(test_rune_index_is_ch_punct);
    MU_RUN_TEST(test_rune_index_is_fullwidth_digit);
    MU_RUN_TEST(test_rune_index_is_chinese_digit);
    MU_RUN_TEST(test_mmruneidx_is_space);
}

#endif // TEST_MEME_RUNE_INDEX_H_INCLUDED
