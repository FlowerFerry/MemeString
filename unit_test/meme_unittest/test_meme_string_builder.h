#ifndef TEST_MEME_STRING_BUILDER_H_INCLUDED
#define TEST_MEME_STRING_BUILDER_H_INCLUDED

#include <minunit.h>
#include "test_shared.h"
#include <meme/string_builder.h>

MU_TEST(test_builder_init)
{
    mmsbldrstk_t builder;
    int rc = MemeStringBuilderStack_init(&builder, MMSBLDR__OBJ_SIZE);
    mu_assert(rc == 0, "builder init should succeed");
    MemeStringBuilderStack_unInit(&builder, MMSBLDR__OBJ_SIZE);
}

MU_TEST(test_builder_init_by_other)
{
    mmsbldrstk_t b1, b2;
    MemeStringBuilderStack_init(&b1, MMSBLDR__OBJ_SIZE);

    int rc = MemeStringBuilderStack_initByOther(&b2, MMSBLDR__OBJ_SIZE, AS_MMBLDR_C(&b1));
    mu_assert(rc == 0, "builder init by other should succeed");
    MemeStringBuilderStack_unInit(&b1, MMSBLDR__OBJ_SIZE);
    MemeStringBuilderStack_unInit(&b2, MMSBLDR__OBJ_SIZE);
}

MU_TEST(test_builder_swap)
{
    mmsbldrstk_t a, b;
    MemeStringBuilderStack_init(&a, MMSBLDR__OBJ_SIZE);
    MemeStringBuilderStack_init(&b, MMSBLDR__OBJ_SIZE);

    int rc = MemeStringBuilderStack_swap(&a, &b, MMSBLDR__OBJ_SIZE);
    mu_assert(rc == 0, "builder swap should succeed");
    MemeStringBuilderStack_unInit(&a, MMSBLDR__OBJ_SIZE);
    MemeStringBuilderStack_unInit(&b, MMSBLDR__OBJ_SIZE);
}

MU_TEST_SUITE(test_meme_string_builder)
{
    MU_RUN_TEST(test_builder_init);
    MU_RUN_TEST(test_builder_init_by_other);
    MU_RUN_TEST(test_builder_swap);
}

#endif // TEST_MEME_STRING_BUILDER_H_INCLUDED
