#ifndef TEST_MEME_VARIABLE_BUFFER_H_INCLUDED
#define TEST_MEME_VARIABLE_BUFFER_H_INCLUDED

#include <minunit.h>
#include "test_shared.h"

MU_TEST(test_varbuf_init_empty)
{
    mmvbstk_t vb;
    int rc = MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "MemeVariableBufferStack_init should succeed");
    mu_assert(MemeVariableBuffer_isEmpty(AS_MMVB_C(&vb)) != 0, "empty varbuf should be empty");
    mu_assert(MemeVariableBuffer_size(AS_MMVB_C(&vb)) == 0, "empty varbuf size should be 0");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_init_by_bytes)
{
    mmvbstk_t vb;
    const mmbyte_t data[] = { 'h', 'e', 'l', 'l', 'o' };
    int rc = MemeVariableBufferStack_initByBytes(&vb, MMSTR__OBJ_SIZE, data, 5);
    mu_assert(rc == 0, "initByBytes should succeed");
    mu_assert(MemeVariableBuffer_size(AS_MMVB_C(&vb)) == 5, "size should be 5");
    const mmbyte_t* d = MemeVariableBuffer_data(AS_MMVB_C(&vb));
    mu_assert(memcmp(d, "hello", 5) == 0, "data should be 'hello'");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_init_by_other)
{
    mmvbstk_t vb1, vb2;
    MemeVariableBufferStack_initByBytes(&vb1, MMSTR__OBJ_SIZE, (const mmbyte_t*)"data", 4);
    int rc = MemeVariableBufferStack_initByOther(&vb2, MMSTR__OBJ_SIZE, &vb1);
    mu_assert(rc == 0, "initByOther should succeed");
    mu_assert(MemeVariableBuffer_size(AS_MMVB_C(&vb2)) == 4, "size should be 4");
    MemeVariableBufferStack_unInit(&vb1, MMSTR__OBJ_SIZE);
    MemeVariableBufferStack_unInit(&vb2, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_init_with_repeat)
{
    mmvbstk_t vb;
    int rc = MemeVariableBufferStack_initWithRepeatBytes(&vb, MMSTR__OBJ_SIZE, 5, 'A');
    mu_assert(rc == 0, "initWithRepeatBytes should succeed");
    mu_assert(MemeVariableBuffer_size(AS_MMVB_C(&vb)) == 5, "size should be 5");
    const mmbyte_t* d = MemeVariableBuffer_data(AS_MMVB_C(&vb));
    for (mmint_t i = 0; i < 5; i++)
        mu_assert(d[i] == 'A', "all bytes should be 'A'");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_assign)
{
    mmvbstk_t vb1, vb2;
    MemeVariableBufferStack_init(&vb1, MMSTR__OBJ_SIZE);
    MemeVariableBufferStack_initByBytes(&vb2, MMSTR__OBJ_SIZE, (const mmbyte_t*)"assign", 6);

    int rc = MemeVariableBufferStack_assign(&vb1, MMSTR__OBJ_SIZE, &vb2);
    mu_assert(rc == 0, "assign should succeed");
    mu_assert(MemeVariableBuffer_size(AS_MMVB_C(&vb1)) == 6, "size after assign should be 6");
    MemeVariableBufferStack_unInit(&vb1, MMSTR__OBJ_SIZE);
    MemeVariableBufferStack_unInit(&vb2, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_append_with_byte)
{
    mmvbstk_t vb;
    MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
    mmint_t rc = MemeVariableBuffer_appendWithByte(AS_MMVB(&vb), 'X');
    mu_assert(rc == 0, "appendWithByte should succeed");
    mu_assert(MemeVariableBuffer_size(AS_MMVB_C(&vb)) == 1, "size should be 1");
    mu_assert(MemeVariableBuffer_data(AS_MMVB_C(&vb))[0] == 'X', "byte should be 'X'");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_append_with_bytes)
{
    mmvbstk_t vb;
    MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
    mmint_t rc = MemeVariableBuffer_appendWithBytes(AS_MMVB(&vb), (const mmbyte_t*)"hello", 5);
    mu_assert(rc == 0, "appendWithBytes should succeed");
    mu_assert(MemeVariableBuffer_size(AS_MMVB_C(&vb)) == 5, "size should be 5");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_append_with_other)
{
    mmvbstk_t vb1, vb2;
    MemeVariableBufferStack_initByBytes(&vb1, MMSTR__OBJ_SIZE, (const mmbyte_t*)"hello", 5);
    MemeVariableBufferStack_init(&vb2, MMSTR__OBJ_SIZE);
    mmint_t rc = MemeVariableBuffer_appendWithOther(AS_MMVB(&vb2), AS_MMVB_C(&vb1));
    mu_assert(rc == 0, "appendWithOther should succeed");
    mu_assert(MemeVariableBuffer_size(AS_MMVB_C(&vb2)) == 5, "size after append should be 5");
    MemeVariableBufferStack_unInit(&vb1, MMSTR__OBJ_SIZE);
    MemeVariableBufferStack_unInit(&vb2, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_append_with_repeat_bytes)
{
    mmvbstk_t vb;
    MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
    mmint_t rc = MemeVariableBuffer_appendWithRepeatBytes(AS_MMVB(&vb), 3, 'Z');
    mu_assert(rc == 0, "appendWithRepeatBytes should succeed");
    mu_assert(MemeVariableBuffer_size(AS_MMVB_C(&vb)) == 3, "size should be 3");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_insert_with_bytes)
{
    mmvbstk_t vb;
    MemeVariableBufferStack_initByBytes(&vb, MMSTR__OBJ_SIZE, (const mmbyte_t*)"ac", 2);
    mmint_t rc = MemeVariableBuffer_insertWithBytes(AS_MMVB(&vb), 1, (const mmbyte_t*)"XY", 2);
    mu_assert(rc == 0, "insertWithBytes should succeed");
    mu_assert(MemeVariableBuffer_size(AS_MMVB_C(&vb)) == 4, "size should be 4");
    const mmbyte_t* d = MemeVariableBuffer_data(AS_MMVB_C(&vb));
    mu_assert(memcmp(d, "aXYc", 4) == 0, "data should be 'aXYc'");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_clear)
{
    mmvbstk_t vb;
    MemeVariableBufferStack_initByBytes(&vb, MMSTR__OBJ_SIZE, (const mmbyte_t*)"data", 4);
    mmint_t rc = MemeVariableBuffer_clear(AS_MMVB(&vb));
    mu_assert(rc == 0, "clear should succeed");
    mu_assert(MemeVariableBuffer_size(AS_MMVB_C(&vb)) == 0, "size after clear should be 0");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_resize)
{
    mmvbstk_t vb;
    MemeVariableBufferStack_initByBytes(&vb, MMSTR__OBJ_SIZE, (const mmbyte_t*)"hello", 5);
    mmint_t rc = MemeVariableBuffer_resize(AS_MMVB(&vb), 3);
    mu_assert(rc == 0, "resize should succeed");
    mu_assert(MemeVariableBuffer_size(AS_MMVB_C(&vb)) == 3, "size after resize should be 3");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_resize_with_byte)
{
    mmvbstk_t vb;
    MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
    mmint_t rc = MemeVariableBuffer_resizeWithByte(AS_MMVB(&vb), 5, 'X');
    mu_assert(rc == 0, "resizeWithByte should succeed");
    mu_assert(MemeVariableBuffer_size(AS_MMVB_C(&vb)) == 5, "size should be 5");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_remove)
{
    mmvbstk_t vb;
    MemeVariableBufferStack_initByBytes(&vb, MMSTR__OBJ_SIZE, (const mmbyte_t*)"abcdef", 6);
    mmint_t rc = MemeVariableBuffer_remove(AS_MMVB(&vb), 1, 3);
    mu_assert(rc == 0, "remove should succeed");
    mu_assert(MemeVariableBuffer_size(AS_MMVB_C(&vb)) == 3, "size after remove should be 3");
    const mmbyte_t* d = MemeVariableBuffer_data(AS_MMVB_C(&vb));
    mu_assert(memcmp(d, "aef", 3) == 0, "data should be 'aef'");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_reserve)
{
    mmvbstk_t vb;
    MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
    mmint_t rc = MemeVariableBuffer_reserve(AS_MMVB(&vb), 100);
    mu_assert(rc == 0, "reserve should succeed");
    mu_check(MemeVariableBuffer_availableByteCapacity(AS_MMVB_C(&vb)) >= 100);
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_self_chop)
{
    mmvbstk_t vb;
    MemeVariableBufferStack_initByBytes(&vb, MMSTR__OBJ_SIZE, (const mmbyte_t*)"hello world", 11);
    mmint_t rc = MemeVariableBuffer_selfChop(AS_MMVB(&vb), 6);
    mu_assert(rc == 0, "selfChop should succeed");
    mu_assert(MemeVariableBuffer_size(AS_MMVB_C(&vb)) == 17, "size after chop should be 17");
    const mmbyte_t* d = MemeVariableBuffer_data(AS_MMVB_C(&vb));
    mu_assert(memcmp(d, "hello world", 11) == 0, "data should start with 'hello world'");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_index_of_byte)
{
    mmvbstk_t vb;
    MemeVariableBufferStack_initByBytes(&vb, MMSTR__OBJ_SIZE, (const mmbyte_t*)"hello", 5);
    mmint_t pos = MemeVariableBuffer_indexOfWithByte(AS_MMVB_C(&vb), 0, 'l');
    mu_assert(pos == 2, "'l' should be at index 2");
    pos = MemeVariableBuffer_indexOfWithByte(AS_MMVB_C(&vb), 0, 'x');
    mu_assert(pos < 0, "'x' should not be found");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_is_equal_other)
{
    mmvbstk_t a, b;
    int result;
    MemeVariableBufferStack_initByBytes(&a, MMSTR__OBJ_SIZE, (const mmbyte_t*)"data", 4);
    MemeVariableBufferStack_initByBytes(&b, MMSTR__OBJ_SIZE, (const mmbyte_t*)"data", 4);
    int rc = MemeVariableBuffer_isEqualWithOther(AS_MMVB_C(&a), AS_MMVB_C(&b), &result);
    mu_assert(rc == 0, "isEqualWithOther should succeed");
    mu_assert(result != 0, "equal should return true");
    MemeVariableBufferStack_unInit(&a, MMSTR__OBJ_SIZE);
    MemeVariableBufferStack_unInit(&b, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_release_to_string)
{
    mmvbstk_t vb;
    mmstrstk_t s;
    MemeVariableBufferStack_initByBytes(&vb, MMSTR__OBJ_SIZE, (const mmbyte_t*)"hello", 5);
    mmint_t rc = MemeVariableBuffer_releaseToString(AS_MMVB(&vb), &s, MMSTR__OBJ_SIZE);
    mu_assert(rc == 0, "releaseToString should succeed");
    mu_assert(MemeString_byteSize(AS_MMSTR_C(&s)) == 5, "string after release should be 5 bytes");
    mmstrstk_uninit(&s);
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_split)
{
    mmvbstk_t vb;
    MemeVariableBufferStack_initByBytes(&vb, MMSTR__OBJ_SIZE, (const mmbyte_t*)"a|b|c", 5);
    mmvbstk_t out[5];
    mmint_t out_count = 5;
    mmint_t search_index = 0;

    mmint_t rc = MemeVariableBuffer_split(AS_MMVB_C(&vb), (const mmbyte_t*)"|", 1,
        MemeFlag_KeepEmptyParts, out, &out_count, &search_index);
    mu_assert(rc == 0, "MemeVariableBuffer_split should succeed");
    mu_assert(out_count == 3, "should get 3 parts");

    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
    for (mmint_t i = 0; i < out_count; i++)
        MemeVariableBufferStack_unInit(&out[i], MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_storage_type)
{
    mmvbstk_t vb;
    MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
    MemeVariableBuffer_Storage_t st = MemeVariableBuffer_storageType(AS_MMVB_C(&vb));
    mu_assert(st != 0, "storage type should be valid (non-zero)");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_at)
{
    mmvbstk_t vb;
    MemeVariableBufferStack_initByBytes(&vb, MMSTR__OBJ_SIZE, (const mmbyte_t*)"hello", 5);
    const mmbyte_t* p = MemeVariableBuffer_constAt(AS_MMVB_C(&vb), 1);
    mu_assert(p != NULL && *p == 'e', "at index 1 should be 'e'");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST(test_varbuf_data_not_const)
{
    mmvbstk_t vb;
    MemeVariableBufferStack_initByBytes(&vb, MMSTR__OBJ_SIZE, (const mmbyte_t*)"test", 4);
    mmbyte_t* p = MemeVariableBuffer_dataWithNotConst(AS_MMVB(&vb));
    mu_assert(p != NULL, "non-const data should not be NULL");
    p[0] = 'T';
    mu_assert(MemeVariableBuffer_data(AS_MMVB_C(&vb))[0] == 'T', "modified byte should be 'T'");
    MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
}

MU_TEST_SUITE(test_meme_variable_buffer)
{
    MU_RUN_TEST(test_varbuf_init_empty);
    MU_RUN_TEST(test_varbuf_init_by_bytes);
    MU_RUN_TEST(test_varbuf_init_by_other);
    MU_RUN_TEST(test_varbuf_init_with_repeat);
    MU_RUN_TEST(test_varbuf_assign);
    MU_RUN_TEST(test_varbuf_append_with_byte);
    MU_RUN_TEST(test_varbuf_append_with_bytes);
    MU_RUN_TEST(test_varbuf_append_with_other);
    MU_RUN_TEST(test_varbuf_append_with_repeat_bytes);
    MU_RUN_TEST(test_varbuf_insert_with_bytes);
    MU_RUN_TEST(test_varbuf_clear);
    MU_RUN_TEST(test_varbuf_resize);
    MU_RUN_TEST(test_varbuf_resize_with_byte);
    MU_RUN_TEST(test_varbuf_remove);
    MU_RUN_TEST(test_varbuf_reserve);
    MU_RUN_TEST(test_varbuf_self_chop);
    MU_RUN_TEST(test_varbuf_index_of_byte);
    MU_RUN_TEST(test_varbuf_is_equal_other);
    MU_RUN_TEST(test_varbuf_release_to_string);
    MU_RUN_TEST(test_varbuf_split);
    MU_RUN_TEST(test_varbuf_storage_type);
    MU_RUN_TEST(test_varbuf_at);
    MU_RUN_TEST(test_varbuf_data_not_const);
}

#endif // TEST_MEME_VARIABLE_BUFFER_H_INCLUDED
