#ifndef TEST_MEME_STRING_OTHER_H_INCLUDED
#define TEST_MEME_STRING_OTHER_H_INCLUDED

#include <minunit.h>
#include "test_shared.h"
#include <meme/unsafe/string_view.h>

/* ==================== foreach ==================== */

typedef struct {
    int count;
    mmrune_t last_rune;
} foreach_ctx_t;

static mmflag_cbproc_t count_foreach(const mmrune_t* rune, void* user_data)
{
    foreach_ctx_t* ctx = (foreach_ctx_t*)user_data;
    ctx->count++;
    ctx->last_rune = *rune;
    return mmflag_cbproc_continue;
}

MU_TEST(test_foreach_basic)
{
    mmstrstk_t s;
    init_cstr(&s, "abc");
    foreach_ctx_t ctx = { 0 };
    mmint_t rc = MemeString_foreach(AS_MMSTR_C(&s), count_foreach, &ctx);
    mu_assert(rc == 3, "foreach should return total byte count");
    mu_assert(ctx.count == 3, "should iterate 3 runes");
    mmstrstk_uninit(&s);
}

MU_TEST(test_foreach_empty)
{
    mmstrstk_t s;
    mmstrstk_init(&s);
    foreach_ctx_t ctx = { 0 };
    mmint_t rc = MemeString_foreach(AS_MMSTR_C(&s), count_foreach, &ctx);
    mu_assert(rc == 0, "foreach on empty should succeed");
    mu_assert(ctx.count == 0, "should iterate 0 runes");
    mmstrstk_uninit(&s);
}

/* ==================== heap byte size ==================== */

MU_TEST(test_heap_byte_size)
{
    mmstrstk_t s;
    mmstrstk_init(&s);
    mmint_t heap_bytes = MemeString_getPrivateHeapByteSize(AS_MMSTR_C(&s));
    mu_assert(heap_bytes >= 0, "heap byte size should be >= 0");
    mmstrstk_uninit(&s);
}

MU_TEST(test_shared_heap_byte_size)
{
    mmstrstk_t s;
    mmstrstk_init(&s);
    mmint_t heap_bytes = MemeString_getSharedHeapByteSize(AS_MMSTR_C(&s));
    mu_assert(heap_bytes >= 0, "shared heap byte size should be >= 0");
    mmstrstk_uninit(&s);
}

MU_TEST(test_available_byte_capacity)
{
    mmstrstk_t s;
    mmstrstk_init(&s);
    mmint_t cap = MemeString_availableByteCapacity(AS_MMSTR_C(&s));
    mu_assert(cap >= 0, "available capacity should be >= 0");
    mmstrstk_uninit(&s);
}

MU_TEST(test_max_byte_size)
{
    mmstrstk_t s;
    mmstrstk_init(&s);
    mmint_t max = MemeString_maxByteSize(AS_MMSTR_C(&s));
    mu_assert(max >= 0, "max byte size should be >= 0");
    mmstrstk_uninit(&s);
}

MU_TEST(test_max_byte_capacity)
{
    mmstrstk_t s;
    mmstrstk_init(&s);
    mmint_t max = MemeString_maxByteCapacity(AS_MMSTR_C(&s));
    mu_assert(max >= 0, "max byte capacity should be >= 0");
    mmstrstk_uninit(&s);
}

/* ==================== MemeString create/destroy ==================== */

MU_TEST(test_string_create_destroy)
{
    mms_t s = NULL;
    int rc = MemeString_create(&s);
    mu_assert(rc == 0, "MemeString_create should succeed");
    mu_assert(s != NULL, "created string should not be NULL");

    rc = MemeString_destroy(&s);
    mu_assert(rc == 0, "MemeString_destroy should succeed");
    mu_assert(s == NULL, "after destroy, pointer should be NULL");
}

MU_TEST(test_string_reset_ptr)
{
    mms_t s = NULL;
    MemeString_create(&s);

    mmstrstk_t src;
    init_cstr(&src, "data");
    MemeString_assign(s, AS_MMSTR_C(&src));

    int rc = MemeString_reset(s);
    mu_assert(rc == 0, "MemeString_reset should succeed");

    mu_assert(MemeString_isEmpty(s) != 0, "after reset, string should be empty");

    MemeString_destroy(&s);
    mmstrstk_uninit(&src);
}

/* ==================== string view unsafe ==================== */

MU_TEST(test_string_view_init)
{
    const char* raw = "hello";
    mmstrstk_t view;
    int rc = MemeStringViewUnsafeStack_init(&view, MMSTR__OBJ_SIZE,
        (const uint8_t*)raw, 5);
    mu_assert(rc == 0, "view init should succeed");
    mmstr_strg_t st = MemeString_storageType(AS_MMSTR_C(&view));
    mu_assert(st == MemeString_UnsafeStorageType_view,
        "should be view storage");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&view)) == 5, "view byte size should be 5");
    mmstrstk_uninit(&view);
}

MU_TEST(test_string_view_from_string)
{
    mmstrstk_t s;
    init_cstr(&s, "hello world");

    mmstrstk_t view;
    int rc = MemeStringViewUnsafeStack_initByOther(&view, MMSTR__OBJ_SIZE, &s);
    mu_assert(rc == 0, "view from string should succeed");
    mu_assert(MemeString_storageType(AS_MMSTR_C(&view)) == MemeString_UnsafeStorageType_view,
        "should be view storage");

    mu_assert(MemeString_byteSize(AS_MMSTR_C(&view)) == MemeString_byteSize(AS_MMSTR_C(&s)),
        "view byte size should match source");
    mu_assert(memcmp(MemeString_byteData(AS_MMSTR_C(&view)), MemeString_byteData(AS_MMSTR_C(&s)),
        (size_t)MemeString_byteSize(AS_MMSTR_C(&s))) == 0, "view data should match source");

    mmstrstk_uninit(&view);
    mmstrstk_uninit(&s);
}

MU_TEST_SUITE(test_meme_string_other)
{
    MU_RUN_TEST(test_foreach_basic);
    MU_RUN_TEST(test_foreach_empty);
    MU_RUN_TEST(test_heap_byte_size);
    MU_RUN_TEST(test_shared_heap_byte_size);
    MU_RUN_TEST(test_available_byte_capacity);
    MU_RUN_TEST(test_max_byte_size);
    MU_RUN_TEST(test_max_byte_capacity);
    MU_RUN_TEST(test_string_create_destroy);
    MU_RUN_TEST(test_string_reset_ptr);
    MU_RUN_TEST(test_string_view_init);
    MU_RUN_TEST(test_string_view_from_string);
}

#endif // TEST_MEME_STRING_OTHER_H_INCLUDED
