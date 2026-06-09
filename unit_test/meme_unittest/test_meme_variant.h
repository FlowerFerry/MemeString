#ifndef TEST_MEME_VARIANT_H_INCLUDED
#define TEST_MEME_VARIANT_H_INCLUDED

#include <minunit.h>
#include "test_shared.h"
#include <meme/variant.h>

MU_TEST(test_variant_init)
{
    mmvarstk_t v;
    mgec_t rc = mmvarstk_init(&v, MMVAR__OBJ_SIZE);
    mu_assert(rc == 0, "variant init should succeed");
    mu_assert(MemeVariantStack_isNull(&v, MMVAR__OBJ_SIZE) != 0, "variant should be null after init");
    mmvarstk_uninit(&v, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_init_by_int64)
{
    mmvarstk_t v;
    mgec_t rc = mmvarstk_init_by_int64(&v, MMVAR__OBJ_SIZE, 42);
    mu_assert(rc == 0, "variant init by int64 should succeed");
    mu_assert(MemeVariantStack_isInt64(&v, MMVAR__OBJ_SIZE) != 0, "should be int64 type");

    int64_t val;
    MemeVariantStack_getInt64(&v, MMVAR__OBJ_SIZE, &val);
    mu_assert(val == 42, "value should be 42");
    mmvarstk_uninit(&v, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_init_by_uint64)
{
    mmvarstk_t v;
    mgec_t rc = mmvarstk_init_by_uint64(&v, MMVAR__OBJ_SIZE, 123);
    mu_assert(rc == 0, "variant init by uint64 should succeed");
    mu_assert(MemeVariantStack_isUInt64(&v, MMVAR__OBJ_SIZE) != 0, "should be uint64 type");

    uint64_t val;
    MemeVariantStack_getUInt64(&v, MMVAR__OBJ_SIZE, &val);
    mu_assert(val == 123, "value should be 123");
    mmvarstk_uninit(&v, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_init_by_double)
{
    mmvarstk_t v;
    mgec_t rc = mmvarstk_init_by_double(&v, MMVAR__OBJ_SIZE, 3.14);
    mu_assert(rc == 0, "variant init by double should succeed");
    mu_assert(MemeVariantStack_isDouble(&v, MMVAR__OBJ_SIZE) != 0, "should be double type");

    double val;
    MemeVariantStack_getDouble(&v, MMVAR__OBJ_SIZE, &val);
    mu_assert(val > 3.13 && val < 3.15, "value should be ~3.14");
    mmvarstk_uninit(&v, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_init_by_byte)
{
    mmvarstk_t v;
    mgec_t rc = mmvarstk_init_by_byte(&v, MMVAR__OBJ_SIZE, 0xAB);
    mu_assert(rc == 0, "variant init by byte should succeed");
    mu_assert(MemeVariantStack_isByte(&v, MMVAR__OBJ_SIZE) != 0, "should be byte type");

    mmbyte_t val;
    MemeVariantStack_getByte(&v, MMVAR__OBJ_SIZE, &val);
    mu_assert(val == 0xAB, "value should be 0xAB");
    mmvarstk_uninit(&v, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_init_by_char)
{
    mmvarstk_t v;
    mgec_t rc = mmvarstk_init_by_char(&v, MMVAR__OBJ_SIZE, 'Z');
    mu_assert(rc == 0, "variant init by char should succeed");
    mu_assert(MemeVariantStack_isChar(&v, MMVAR__OBJ_SIZE) != 0, "should be char type");

    char val;
    MemeVariantStack_getChar(&v, MMVAR__OBJ_SIZE, &val);
    mu_assert(val == 'Z', "value should be 'Z'");
    mmvarstk_uninit(&v, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_init_by_string)
{
    mmvarstk_t v;
    mmstrstk_t s;
    init_cstr(&s, "hello");
    mgec_t rc = mmvarstk_init_by_str(&v, MMVAR__OBJ_SIZE, AS_MMSTR_C(&s));
    mu_assert(rc == 0, "variant init by string should succeed");
    mu_assert(MemeVariantStack_isString(&v, MMVAR__OBJ_SIZE) != 0, "should be string type");

    mmstrstk_t out;
    MemeVariantStack_getString(&v, MMVAR__OBJ_SIZE, &out, 0);
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&out)) == 5, "string variant should be 5 bytes");
    mmstrstk_uninit(&out);
    mmstrstk_uninit(&s);
    mmvarstk_uninit(&v, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_init_by_rune)
{
    mmrune_t r = MemeRune_getInitObject();
    MemeRune_initByByte(&r, 'A');

    mmvarstk_t v;
    mgec_t rc = mmvarstk_init_by_rune(&v, MMVAR__OBJ_SIZE, &r);
    mu_assert(rc == 0, "variant init by rune should succeed");
    mu_assert(MemeVariantStack_isRune(&v, MMVAR__OBJ_SIZE) != 0, "should be rune type");

    mmrune_t out;
    MemeVariantStack_getRune(&v, MMVAR__OBJ_SIZE, &out);
    mu_assert(MemeRune_data(&out)[0] == 'A', "rune value should be 'A'");
    mmvarstk_uninit(&v, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_init_by_other)
{
    mmvarstk_t v1, v2;
    mmvarstk_init_by_int64(&v1, MMVAR__OBJ_SIZE, 99);
    mgec_t rc = MemeVariantStack_initByOther(&v2, MMVAR__OBJ_SIZE, AS_MMVAR_C(&v1));
    mu_assert(rc == 0, "variant init by other should succeed");

    int64_t val;
    MemeVariantStack_getInt64(&v2, MMVAR__OBJ_SIZE, &val);
    mu_assert(val == 99, "value should be 99");
    mmvarstk_uninit(&v1, MMVAR__OBJ_SIZE);
    mmvarstk_uninit(&v2, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_init_by_move)
{
    mmvarstk_t v1, v2;
    mmvarstk_init_by_int64(&v1, MMVAR__OBJ_SIZE, 77);
    mgec_t rc = MemeVariantStack_initByMove(&v2, MMVAR__OBJ_SIZE, &v1);
    mu_assert(rc == 0, "variant init by move should succeed");

    int64_t val;
    MemeVariantStack_getInt64(&v2, MMVAR__OBJ_SIZE, &val);
    mu_assert(val == 77, "value should be 77 after move");
    mmvarstk_uninit(&v1, MMVAR__OBJ_SIZE);
    mmvarstk_uninit(&v2, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_assign)
{
    mmvarstk_t v1, v2;
    mmvarstk_init(&v1, MMVAR__OBJ_SIZE);
    mmvarstk_init_by_int64(&v2, MMVAR__OBJ_SIZE, 55);

    mgec_t rc = MemeVariantStack_assign(&v1, MMVAR__OBJ_SIZE, AS_MMVAR_C(&v2));
    mu_assert(rc == 0, "variant assign should succeed");

    mu_assert(MemeVariantStack_isInt64(&v1, MMVAR__OBJ_SIZE) != 0, "should be int64 after assign");
    int64_t val;
    MemeVariantStack_getInt64(&v1, MMVAR__OBJ_SIZE, &val);
    mu_assert(val == 55, "value should be 55");
    mmvarstk_uninit(&v1, MMVAR__OBJ_SIZE);
    mmvarstk_uninit(&v2, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_swap)
{
    mmvarstk_t a, b;
    mmvarstk_init_by_int64(&a, MMVAR__OBJ_SIZE, 1);
    mmvarstk_init_by_int64(&b, MMVAR__OBJ_SIZE, 2);

    mgec_t rc = MemeVariantStack_swap(&a, &b, MMVAR__OBJ_SIZE);
    mu_assert(rc == 0, "variant swap should succeed");

    int64_t va, vb;
    MemeVariantStack_getInt64(&a, MMVAR__OBJ_SIZE, &va);
    MemeVariantStack_getInt64(&b, MMVAR__OBJ_SIZE, &vb);
    mu_assert(va == 2 && vb == 1, "values should be swapped");
    mmvarstk_uninit(&a, MMVAR__OBJ_SIZE);
    mmvarstk_uninit(&b, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_reset)
{
    mmvarstk_t v;
    mmvarstk_init_by_int64(&v, MMVAR__OBJ_SIZE, 10);
    mgec_t rc = MemeVariantStack_reset(&v, MMVAR__OBJ_SIZE);
    mu_assert(rc == 0, "variant reset should succeed");
    mu_assert(MemeVariantStack_isNull(&v, MMVAR__OBJ_SIZE) != 0, "after reset, should be null");
    mmvarstk_uninit(&v, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_set_null)
{
    mmvarstk_t v;
    mmvarstk_init_by_int64(&v, MMVAR__OBJ_SIZE, 10);
    mgec_t rc = MemeVariantStack_setNull(&v, MMVAR__OBJ_SIZE);
    mu_assert(rc == 0, "setNull should succeed");
    mu_assert(MemeVariantStack_isNull(&v, MMVAR__OBJ_SIZE) != 0, "should be null after setNull");
    mmvarstk_uninit(&v, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_setters_and_getters)
{
    mmvarstk_t v;
    mmvarstk_init(&v, MMVAR__OBJ_SIZE);

    MemeVariantStack_setByte(&v, MMVAR__OBJ_SIZE, 0x42);
    mu_assert(MemeVariantStack_isByte(&v, MMVAR__OBJ_SIZE) != 0, "should be byte");
    mmbyte_t b;
    MemeVariantStack_getByte(&v, MMVAR__OBJ_SIZE, &b);
    mu_assert(b == 0x42, "byte should be 0x42");

    MemeVariantStack_setDouble(&v, MMVAR__OBJ_SIZE, 2.71);
    mu_assert(MemeVariantStack_isDouble(&v, MMVAR__OBJ_SIZE) != 0, "should be double");
    double d;
    MemeVariantStack_getDouble(&v, MMVAR__OBJ_SIZE, &d);
    mu_assert(d > 2.70 && d < 2.72, "double should be ~2.71");

    mmvarstk_uninit(&v, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_conv_to_int64)
{
    mmvarstk_t v;
    mmvarstk_init_by_int64(&v, MMVAR__OBJ_SIZE, 42);
    mmint_t val;
    mgec_t rc = MemeVariantStack_convToInt(&v, MMVAR__OBJ_SIZE, &val);
    mu_assert(rc == 0, "convToInt should succeed");
    mu_assert(val == 42, "converted value should be 42");
    mmvarstk_uninit(&v, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_is_type)
{
    mmvarstk_t v;
    mmvarstk_init_by_int64(&v, MMVAR__OBJ_SIZE, 100);
    mu_assert(MemeVariantStack_isType(&v, MMVAR__OBJ_SIZE, MMMETA_TYPID__INT64) != 0,
        "isType should identify INT64");
    mu_assert(MemeVariantStack_isType(&v, MMVAR__OBJ_SIZE, MMMETA_TYPID__NULL) == 0,
        "isType should reject NULL type");
    mmvarstk_uninit(&v, MMVAR__OBJ_SIZE);
}

MU_TEST(test_variant_init_cond_convert)
{
    mmvarstk_t v1, v2;
    mmvarstk_init_by_int64(&v1, MMVAR__OBJ_SIZE, 99);
    mgec_t rc = MemeVariantStack_initAndConditionalConvert(&v2, MMVAR__OBJ_SIZE, AS_MMVAR_C(&v1));
    mu_assert(rc == 0, "initAndConditionalConvert should succeed");
    mu_assert(MemeVariantStack_isInt64(&v2, MMVAR__OBJ_SIZE) != 0, "should be int64");
    mmvarstk_uninit(&v1, MMVAR__OBJ_SIZE);
    mmvarstk_uninit(&v2, MMVAR__OBJ_SIZE);
}

MU_TEST_SUITE(test_meme_variant)
{
    MU_RUN_TEST(test_variant_init);
    MU_RUN_TEST(test_variant_init_by_int64);
    MU_RUN_TEST(test_variant_init_by_uint64);
    MU_RUN_TEST(test_variant_init_by_double);
    MU_RUN_TEST(test_variant_init_by_byte);
    MU_RUN_TEST(test_variant_init_by_char);
    MU_RUN_TEST(test_variant_init_by_string);
    MU_RUN_TEST(test_variant_init_by_rune);
    MU_RUN_TEST(test_variant_init_by_other);
    MU_RUN_TEST(test_variant_init_by_move);
    MU_RUN_TEST(test_variant_assign);
    MU_RUN_TEST(test_variant_swap);
    MU_RUN_TEST(test_variant_reset);
    MU_RUN_TEST(test_variant_set_null);
    MU_RUN_TEST(test_variant_setters_and_getters);
    MU_RUN_TEST(test_variant_conv_to_int64);
    MU_RUN_TEST(test_variant_is_type);
    MU_RUN_TEST(test_variant_init_cond_convert);
}

#endif // TEST_MEME_VARIANT_H_INCLUDED
