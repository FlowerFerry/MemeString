#ifndef MEGO_TEST_CSTR_ALLOC_H_INCLUDED
#define MEGO_TEST_CSTR_ALLOC_H_INCLUDED

#include <mego/mem/cstr_alloc_if_no_end_zero.h>
#include <minunit.h>

#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/* ===========================================================
 *  mgmem__cstr_alloc_if_no_end_zero
 * =========================================================== */

MU_TEST(ut_cstr_alloc_null_src)
{
    const char *out = NULL;
    mgec_t ec = mgmem__cstr_alloc_if_no_end_zero(NULL, 3, &out, NULL, 0);
    mu_assert(ec == MGEC__INVAL, "NULL src should return MGEC__INVAL");
}

MU_TEST(ut_cstr_alloc_null_out)
{
    const char *src = "hello";
    mgec_t ec = mgmem__cstr_alloc_if_no_end_zero(src, 5, NULL, NULL, 0);
    mu_assert(ec == MGEC__INVAL, "NULL out should return MGEC__INVAL");
}

MU_TEST(ut_cstr_alloc_neg_slen_no_alloc)
{
    /* slen < 0, must_alloc == 0 → return src directly, olen = strlen */
    const char *src = "hello";
    const char *out = NULL;
    mmint_t olen = 0;
    mgec_t ec = mgmem__cstr_alloc_if_no_end_zero(src, -1, &out, &olen, 0);
    mu_assert(ec == 0, "neg slen no_alloc should succeed");
    mu_assert(out == src, "neg slen no_alloc: out should be src pointer");
    mu_assert(olen == 5, "neg slen no_alloc: olen should be strlen(src)");
}

MU_TEST(ut_cstr_alloc_neg_slen_must_alloc)
{
    /* slen < 0, must_alloc == 1 → allocate a fresh copy */
    const char *src = "hello";
    const char *out = NULL;
    mmint_t olen = 0;
    mgec_t ec = mgmem__cstr_alloc_if_no_end_zero(src, -1, &out, &olen, 1);
    mu_assert(ec == 0, "neg slen must_alloc should succeed");
    mu_assert(out != NULL, "neg slen must_alloc: out should not be NULL");
    mu_assert(out != src, "neg slen must_alloc: out should be a new allocation");
    mu_assert(olen == 5, "neg slen must_alloc: olen should be 5");
    mu_assert(strcmp(out, src) == 0, "neg slen must_alloc: content mismatch");
    mu_assert(out[olen] == '\0', "neg slen must_alloc: missing null terminator");
    free((void*)out);
}

MU_TEST(ut_cstr_alloc_terminated_no_alloc)
{
    /* src[slen] == '\0' and must_alloc == 0 → return src */
    const char *src = "world";
    const char *out = NULL;
    mmint_t olen = 0;
    mgec_t ec = mgmem__cstr_alloc_if_no_end_zero(src, 5, &out, &olen, 0);
    mu_assert(ec == 0, "terminated no_alloc should succeed");
    mu_assert(out == src, "terminated no_alloc: out should be src pointer");
    mu_assert(olen == 5, "terminated no_alloc: olen mismatch");
}

MU_TEST(ut_cstr_alloc_not_terminated_no_alloc)
{
    /* src[slen] != '\0' and must_alloc == 0 → allocate */
    char buf[8] = {'h', 'e', 'l', 'l', 'o', 'X', 'Y', 'Z'}; /* no '\0' at [5] */
    const char *out = NULL;
    mmint_t olen = 0;
    mgec_t ec = mgmem__cstr_alloc_if_no_end_zero(buf, 5, &out, &olen, 0);
    mu_assert(ec == 0, "not terminated should succeed");
    mu_assert(out != NULL, "not terminated: out should not be NULL");
    mu_assert(out != (const char*)buf, "not terminated: should allocate new buffer");
    mu_assert(olen == 5, "not terminated: olen mismatch");
    mu_assert(strncmp(out, "hello", 5) == 0, "not terminated: content mismatch");
    mu_assert(out[5] == '\0', "not terminated: missing null terminator");
    free((void*)out);
}

MU_TEST(ut_cstr_alloc_terminated_must_alloc)
{
    /* must_alloc == 1 even if already terminated → still allocates */
    const char *src = "hi";
    const char *out = NULL;
    mmint_t olen = 0;
    mgec_t ec = mgmem__cstr_alloc_if_no_end_zero(src, 2, &out, &olen, 1);
    mu_assert(ec == 0, "must_alloc=1 should succeed");
    mu_assert(out != NULL, "must_alloc=1: out should not be NULL");
    mu_assert(out != src, "must_alloc=1: should allocate even if terminated");
    mu_assert(olen == 2, "must_alloc=1: olen mismatch");
    mu_assert(strcmp(out, "hi") == 0, "must_alloc=1: content mismatch");
    free((void*)out);
}

MU_TEST(ut_cstr_alloc_olen_null)
{
    /* _olen == NULL should not crash */
    const char *src = "abc";
    const char *out = NULL;
    mgec_t ec = mgmem__cstr_alloc_if_no_end_zero(src, 3, &out, NULL, 0);
    mu_assert(ec == 0, "olen=NULL should succeed");
    mu_assert(out == src, "olen=NULL: out should be src");
}

/* ===========================================================
 *  mgmem__wcstr_alloc_if_no_end_zero
 * =========================================================== */

MU_TEST(ut_wcstr_alloc_null_src)
{
    const wchar_t *out = NULL;
    mgec_t ec = mgmem__wcstr_alloc_if_no_end_zero(NULL, 3, &out, NULL, 0);
    mu_assert(ec == MGEC__INVAL, "wcstr NULL src should return MGEC__INVAL");
}

MU_TEST(ut_wcstr_alloc_null_out)
{
    const wchar_t *src = L"hello";
    mgec_t ec = mgmem__wcstr_alloc_if_no_end_zero(src, 5, NULL, NULL, 0);
    mu_assert(ec == MGEC__INVAL, "wcstr NULL out should return MGEC__INVAL");
}

MU_TEST(ut_wcstr_alloc_neg_slen_no_alloc)
{
    const wchar_t *src = L"hello";
    const wchar_t *out = NULL;
    mmint_t olen = 0;
    mgec_t ec = mgmem__wcstr_alloc_if_no_end_zero(src, -1, &out, &olen, 0);
    mu_assert(ec == 0, "wcstr neg slen no_alloc should succeed");
    mu_assert(out == src, "wcstr neg slen no_alloc: out should be src pointer");
    mu_assert(olen == 5, "wcstr neg slen no_alloc: olen should be wcslen(src)");
}

MU_TEST(ut_wcstr_alloc_neg_slen_must_alloc)
{
    const wchar_t *src = L"hello";
    const wchar_t *out = NULL;
    mmint_t olen = 0;
    mgec_t ec = mgmem__wcstr_alloc_if_no_end_zero(src, -1, &out, &olen, 1);
    mu_assert(ec == 0, "wcstr neg slen must_alloc should succeed");
    mu_assert(out != NULL, "wcstr neg slen must_alloc: out should not be NULL");
    mu_assert(out != src, "wcstr neg slen must_alloc: should allocate new buffer");
    mu_assert(olen == 5, "wcstr neg slen must_alloc: olen mismatch");
    mu_assert(wcscmp(out, src) == 0, "wcstr neg slen must_alloc: content mismatch");
    free((void*)out);
}

MU_TEST(ut_wcstr_alloc_terminated_no_alloc)
{
    const wchar_t *src = L"world";
    const wchar_t *out = NULL;
    mmint_t olen = 0;
    mgec_t ec = mgmem__wcstr_alloc_if_no_end_zero(src, 5, &out, &olen, 0);
    mu_assert(ec == 0, "wcstr terminated no_alloc should succeed");
    mu_assert(out == src, "wcstr terminated no_alloc: out should be src");
    mu_assert(olen == 5, "wcstr terminated no_alloc: olen mismatch");
}

MU_TEST(ut_wcstr_alloc_not_terminated_no_alloc)
{
    wchar_t buf[8] = {L'h', L'e', L'l', L'l', L'o', L'X', L'Y', L'Z'};
    const wchar_t *out = NULL;
    mmint_t olen = 0;
    mgec_t ec = mgmem__wcstr_alloc_if_no_end_zero(buf, 5, &out, &olen, 0);
    mu_assert(ec == 0, "wcstr not terminated should succeed");
    mu_assert(out != NULL, "wcstr not terminated: out should not be NULL");
    mu_assert(out != (const wchar_t*)buf, "wcstr not terminated: should allocate");
    mu_assert(olen == 5, "wcstr not terminated: olen mismatch");
    mu_assert(wcsncmp(out, L"hello", 5) == 0, "wcstr not terminated: content mismatch");
    mu_assert(out[5] == L'\0', "wcstr not terminated: missing null terminator");
    free((void*)out);
}

MU_TEST(ut_wcstr_alloc_must_alloc)
{
    const wchar_t *src = L"hi";
    const wchar_t *out = NULL;
    mmint_t olen = 0;
    mgec_t ec = mgmem__wcstr_alloc_if_no_end_zero(src, 2, &out, &olen, 1);
    mu_assert(ec == 0, "wcstr must_alloc=1 should succeed");
    mu_assert(out != src, "wcstr must_alloc=1: should allocate even if terminated");
    mu_assert(olen == 2, "wcstr must_alloc=1: olen mismatch");
    mu_assert(wcscmp(out, L"hi") == 0, "wcstr must_alloc=1: content mismatch");
    free((void*)out);
}

/* ===========================================================
 *  mgmem__free_if_ptr_not_equal
 * =========================================================== */

MU_TEST(ut_free_if_not_equal_null_new)
{
    /* new == NULL → must not free, must not crash */
    const char *src = "data";
    mgmem__free_if_ptr_not_equal(src, NULL);
    mu_check(1); /* reaching here is success */
}

MU_TEST(ut_free_if_not_equal_same_ptr)
{
    /* new == src → must not free (src is not heap-allocated here) */
    const char *src = "data";
    mgmem__free_if_ptr_not_equal(src, (void*)src);
    mu_check(1);
}

MU_TEST(ut_free_if_not_equal_different_ptr)
{
    /* new != src → should free the new allocation without crashing */
    char *heap = (char*)malloc(8);
    mu_assert(heap != NULL, "malloc failed in free_if_not_equal test");
    strcpy(heap, "test");
    mgmem__free_if_ptr_not_equal("src", heap);
    mu_check(1); /* no crash = success */
}

/* ---- suite ---- */

MU_TEST_SUITE(test_cstr_alloc)
{
    MU_RUN_TEST(ut_cstr_alloc_null_src);
    MU_RUN_TEST(ut_cstr_alloc_null_out);
    MU_RUN_TEST(ut_cstr_alloc_neg_slen_no_alloc);
    MU_RUN_TEST(ut_cstr_alloc_neg_slen_must_alloc);
    MU_RUN_TEST(ut_cstr_alloc_terminated_no_alloc);
    MU_RUN_TEST(ut_cstr_alloc_not_terminated_no_alloc);
    MU_RUN_TEST(ut_cstr_alloc_terminated_must_alloc);
    MU_RUN_TEST(ut_cstr_alloc_olen_null);

    MU_RUN_TEST(ut_wcstr_alloc_null_src);
    MU_RUN_TEST(ut_wcstr_alloc_null_out);
    MU_RUN_TEST(ut_wcstr_alloc_neg_slen_no_alloc);
    MU_RUN_TEST(ut_wcstr_alloc_neg_slen_must_alloc);
    MU_RUN_TEST(ut_wcstr_alloc_terminated_no_alloc);
    MU_RUN_TEST(ut_wcstr_alloc_not_terminated_no_alloc);
    MU_RUN_TEST(ut_wcstr_alloc_must_alloc);

    MU_RUN_TEST(ut_free_if_not_equal_null_new);
    MU_RUN_TEST(ut_free_if_not_equal_same_ptr);
    MU_RUN_TEST(ut_free_if_not_equal_different_ptr);
}

#endif /* MEGO_TEST_CSTR_ALLOC_H_INCLUDED */
