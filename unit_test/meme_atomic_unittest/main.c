
#include <minunit.h>

#include <mego/thrd/threads.h>
#include <meme/conc/atomic.h>

const int ctest_mt_i32_add_cb1_once_value = 1000000;
int ctest_mt_i32_add_cb_1(void* _arg)
{
    mmconc_atomic_int32_t* atmc = (mmconc_atomic_int32_t*)_arg;
    for (int i = 0; i < 1000000; i++)
    {
        mmconc_atomic_i32_fetch_add(atmc, 1);
    }
    return 0;
}

const int64_t ctest_mt_i32_add_cb2_once_value = 2097152 * 256;
int ctest_mt_i32_add_cb_2(void* _arg)
{
    mmconc_atomic_int32_t* atmc = (mmconc_atomic_int32_t*)_arg;
    for (int i = 0; i < 2097152; i++)
    {
        mmconc_atomic_i32_fetch_add(atmc, 256);
    }
    return 0;
}

const int ctest_mt_i32_add_cb3_once_value = 2097152 * 255;
int ctest_mt_i32_add_cb_3(void* _arg)
{
    mmconc_atomic_int32_t* atmc = (mmconc_atomic_int32_t*)_arg;
    for (int i = 0; i < 2097152; i++)
    {
        mmconc_atomic_i32_fetch_add(atmc, 255);
    }
    return 0;
}

const int64_t ctest_mt_i32_ovf_add_cb1_once_value = 2000000 * 1000;
int ctest_mt_i32_ovf_add_cb_1(void* _arg)
{
    mmconc_atomic_int32_t* atmc = (mmconc_atomic_int32_t*)_arg;
    for (int i = 0; i < 2000000; i++)
    {
        mmconc_atomic_i32_fetch_add(atmc, 1000);
    }
    return 0;
}

const int64_t ctest_mt_i64_add_cb1_once_value = 1000000;
int ctest_mt_i64_add_cb_1(void* _arg)
{
    mmconc_atomic_int64_t* atmc = (mmconc_atomic_int64_t*)_arg;
    for (int i = 0; i < 1000000; i++)
    {
        mmconc_atomic_i64_fetch_add(atmc, 1);
    }
    return 0;
}

const int64_t ctest_mt_i64_ovf_add_cb1_once_value = 4000000 * 1000000000000;
int ctest_mt_i64_ovf_add_cb_1(void* _arg)
{
    mmconc_atomic_int64_t* atmc = (mmconc_atomic_int64_t*)_arg;
    for (int i = 0; i < 4000000; i++)
    {
        mmconc_atomic_i64_fetch_add(atmc, 1000000000000);
    }
    return 0;
}

const int ctest_mt_i8_ovf_add_cb1_once_value = 1000000;
int ctest_mt_i8_ovf_add_cb_1(void* _arg)
{
    mmconc_atomic_int8_t* atmc = (mmconc_atomic_int8_t*)_arg;
    for (int i = 0; i < 1000000; i++)
    {
        mmconc_atomic_i8_fetch_add(atmc, 1);
    }
    return 0;
}

const int ctest_mt_i16_ovf_add_cb1_once_value = 1000000;
int ctest_mt_i16_ovf_add_cb_1(void* _arg)
{
    mmconc_atomic_int16_t* atmc = (mmconc_atomic_int16_t*)_arg;
    for (int i = 0; i < 1000000; i++)
    {
        mmconc_atomic_i16_fetch_add(atmc, 1);
    }
    return 0;
}

MU_TEST(ctest_mt_int_add)
{
    mmconc_atomic_int32_t atmc32 = mmconc_atomic_i32_get_init(0);

    mgthrd_t thrd1;
    mgthrd_t thrd2;
    mgthrd_t thrd3;
    mgthrd_t thrd4;
    mgthrd_create(&thrd1, ctest_mt_i32_add_cb_1, &atmc32);
    mgthrd_create(&thrd2, ctest_mt_i32_add_cb_1, &atmc32);
    mgthrd_create(&thrd3, ctest_mt_i32_add_cb_1, &atmc32);
    mgthrd_create(&thrd4, ctest_mt_i32_add_cb_1, &atmc32);
    mgthrd_join(thrd1, NULL);
    mgthrd_join(thrd2, NULL);
    mgthrd_join(thrd3, NULL);
    mgthrd_join(thrd4, NULL);

    mu_assert(mmconc_atomic_i32_load(&atmc32) == 4 * ctest_mt_i32_add_cb1_once_value,
        "Error: mmconc_atomic_i32_load(&atmc) != 4 * ctest_mt_i32_add_cb1_once_value");

    mmconc_atomic_i32_store(&atmc32, 0);
    
    mu_assert(mmconc_atomic_i32_load(&atmc32) == 0,
        "Error: mmconc_atomic_i32_load(&atmc) != 0");

    mgthrd_create(&thrd1, ctest_mt_i32_add_cb_2, &atmc32);
    mgthrd_create(&thrd2, ctest_mt_i32_add_cb_2, &atmc32);
    mgthrd_create(&thrd3, ctest_mt_i32_add_cb_2, &atmc32);
    mgthrd_create(&thrd4, ctest_mt_i32_add_cb_2, &atmc32);
    mgthrd_join(thrd1, NULL);
    mgthrd_join(thrd2, NULL);
    mgthrd_join(thrd3, NULL);
    mgthrd_join(thrd4, NULL);

    mu_assert(mmconc_atomic_i32_load(&atmc32) == (int)(4 * ctest_mt_i32_add_cb2_once_value),
        "Error: mmconc_atomic_i32_load(&atmc) != (int)(4 * ctest_mt_i32_add_cb2_once_value)");

    mmconc_atomic_i32_store(&atmc32, 0);

    mu_assert(mmconc_atomic_i32_load(&atmc32) == 0,
        "Error: mmconc_atomic_i32_load(&atmc) != 0");

    mgthrd_create(&thrd1, ctest_mt_i32_add_cb_3, &atmc32);
    mgthrd_create(&thrd2, ctest_mt_i32_add_cb_3, &atmc32);
    mgthrd_create(&thrd3, ctest_mt_i32_add_cb_3, &atmc32);
    mgthrd_create(&thrd4, ctest_mt_i32_add_cb_3, &atmc32);
    mgthrd_join(thrd1, NULL);
    mgthrd_join(thrd2, NULL);
    mgthrd_join(thrd3, NULL);
    mgthrd_join(thrd4, NULL);

    mu_assert(mmconc_atomic_i32_load(&atmc32) == 4 * ctest_mt_i32_add_cb3_once_value,
        "Error: mmconc_atomic_i32_load(&atmc) != 4 * ctest_mt_i32_add_cb3_once_value");

    mmconc_atomic_int64_t atmc64 = mmconc_atomic_i64_get_init(0);
    
    mgthrd_create(&thrd1, ctest_mt_i64_add_cb_1, &atmc64);
    mgthrd_create(&thrd2, ctest_mt_i64_add_cb_1, &atmc64);
    mgthrd_create(&thrd3, ctest_mt_i64_add_cb_1, &atmc64);
    mgthrd_create(&thrd4, ctest_mt_i64_add_cb_1, &atmc64);
    mgthrd_join(thrd1, NULL);
    mgthrd_join(thrd2, NULL);
    mgthrd_join(thrd3, NULL);
    mgthrd_join(thrd4, NULL);

    mu_assert(mmconc_atomic_i64_load(&atmc64) == 4 * ctest_mt_i64_add_cb1_once_value,
        "Error: mmconc_atomic_i64_load(&atmc64) != 4 * ctest_mt_i64_add_cb1_once_value");
}

MU_TEST(ctest_mt_int_ovf_add)
{
    mmconc_atomic_int8_t atmc8 = mmconc_atomic_i8_get_init(0);
    
    mgthrd_t thrd1;
    mgthrd_t thrd2;
    mgthrd_t thrd3;
    mgthrd_t thrd4;
    mgthrd_create(&thrd1, ctest_mt_i8_ovf_add_cb_1, &atmc8);
    mgthrd_create(&thrd2, ctest_mt_i8_ovf_add_cb_1, &atmc8);
    mgthrd_create(&thrd3, ctest_mt_i8_ovf_add_cb_1, &atmc8);
    mgthrd_create(&thrd4, ctest_mt_i8_ovf_add_cb_1, &atmc8);
    
    mgthrd_join(thrd1, NULL);
    mgthrd_join(thrd2, NULL);
    mgthrd_join(thrd3, NULL);
    mgthrd_join(thrd4, NULL);

    mu_assert(mmconc_atomic_i8_load(&atmc8) == (int8_t)(4 * ctest_mt_i8_ovf_add_cb1_once_value),
        "Error: mmconc_atomic_i8_load(&atmc) != (int8_t)(4 * ctest_mt_i8_ovf_add_cb1_once_value)");

    mmconc_atomic_int16_t atmc16 = mmconc_atomic_i16_get_init(0);
    
    mgthrd_create(&thrd1, ctest_mt_i16_ovf_add_cb_1, &atmc16);
    mgthrd_create(&thrd2, ctest_mt_i16_ovf_add_cb_1, &atmc16);
    mgthrd_create(&thrd3, ctest_mt_i16_ovf_add_cb_1, &atmc16);
    mgthrd_create(&thrd4, ctest_mt_i16_ovf_add_cb_1, &atmc16);

    mgthrd_join(thrd1, NULL);
    mgthrd_join(thrd2, NULL);
    mgthrd_join(thrd3, NULL);
    mgthrd_join(thrd4, NULL);

    mu_assert(mmconc_atomic_i16_load(&atmc16) == (int16_t)(4 * ctest_mt_i16_ovf_add_cb1_once_value),
        "Error: mmconc_atomic_i16_load(&atmc) != (int16_t)(4 * ctest_mt_i16_ovf_add_cb1_once_value)");

    mmconc_atomic_int32_t atmc32 = mmconc_atomic_i32_get_init(0);
    
    mgthrd_create(&thrd1, ctest_mt_i32_ovf_add_cb_1, &atmc32);
    mgthrd_create(&thrd2, ctest_mt_i32_ovf_add_cb_1, &atmc32);
    mgthrd_create(&thrd3, ctest_mt_i32_ovf_add_cb_1, &atmc32);
    mgthrd_create(&thrd4, ctest_mt_i32_ovf_add_cb_1, &atmc32);

    mgthrd_join(thrd1, NULL);
    mgthrd_join(thrd2, NULL);
    mgthrd_join(thrd3, NULL);
    mgthrd_join(thrd4, NULL);
    
    mu_assert(mmconc_atomic_i32_load(&atmc32) == (int)(4 * ctest_mt_i32_ovf_add_cb1_once_value),
        "Error: mmconc_atomic_i32_load(&atmc) != (int)(4 * ctest_mt_i32_ovf_add_cb1_once_value)");

    mmconc_atomic_int64_t atmc64 = mmconc_atomic_i64_get_init(0);
    
    mgthrd_t thrd5;
    mgthrd_t thrd6;
    mgthrd_t thrd7;
    mgthrd_t thrd8;

    mgthrd_create(&thrd1, ctest_mt_i64_ovf_add_cb_1, &atmc64);
    mgthrd_create(&thrd2, ctest_mt_i64_ovf_add_cb_1, &atmc64);
    mgthrd_create(&thrd3, ctest_mt_i64_ovf_add_cb_1, &atmc64);
    mgthrd_create(&thrd4, ctest_mt_i64_ovf_add_cb_1, &atmc64);
    mgthrd_create(&thrd5, ctest_mt_i64_ovf_add_cb_1, &atmc64);
    mgthrd_create(&thrd6, ctest_mt_i64_ovf_add_cb_1, &atmc64);
    mgthrd_create(&thrd7, ctest_mt_i64_ovf_add_cb_1, &atmc64);
    mgthrd_create(&thrd8, ctest_mt_i64_ovf_add_cb_1, &atmc64);

    mgthrd_join(thrd1, NULL);
    mgthrd_join(thrd2, NULL);
    mgthrd_join(thrd3, NULL);
    mgthrd_join(thrd4, NULL);
    mgthrd_join(thrd5, NULL);
    mgthrd_join(thrd6, NULL);
    mgthrd_join(thrd7, NULL);
    mgthrd_join(thrd8, NULL);
    
    mu_assert(mmconc_atomic_i64_load(&atmc64) == (int64_t)(8 * ctest_mt_i64_ovf_add_cb1_once_value),
        "Error: mmconc_atomic_i64_load(&atmc) != (int64_t)(8 * ctest_mt_i64_ovf_add_cb1_once_value)");
    
}

int ctest_mt_u8_ovf_add_cb_1(void* _arg)
{
    mmconc_atomic_uint8_t* atmc = (mmconc_atomic_uint8_t*)_arg;
    for (int i = 0; i < 1000000; i++)
    {
        mmconc_atomic_u8_fetch_add(atmc, 1);
    }
    return 0;
}

int ctest_mt_u16_ovf_add_cb_1(void* _arg)
{
    mmconc_atomic_uint16_t* atmc = (mmconc_atomic_uint16_t*)_arg;
    for (int i = 0; i < 1000000; i++)
    {
        mmconc_atomic_u16_fetch_add(atmc, 1);
    }
    return 0;
}

MU_TEST(ctest_mt_uint_ovf_add)
{
    mmconc_atomic_uint8_t atmc8 = mmconc_atomic_u8_get_init(0);
    
    mgthrd_t thrd1;
    mgthrd_t thrd2;
    mgthrd_t thrd3;
    mgthrd_t thrd4;
    mgthrd_create(&thrd1, ctest_mt_u8_ovf_add_cb_1, &atmc8);
    mgthrd_create(&thrd2, ctest_mt_u8_ovf_add_cb_1, &atmc8);
    mgthrd_create(&thrd3, ctest_mt_u8_ovf_add_cb_1, &atmc8);
    mgthrd_create(&thrd4, ctest_mt_u8_ovf_add_cb_1, &atmc8);
    
    mgthrd_join(thrd1, NULL);
    mgthrd_join(thrd2, NULL);
    mgthrd_join(thrd3, NULL);
    mgthrd_join(thrd4, NULL);

    mu_assert(mmconc_atomic_u8_load(&atmc8) == 0,
        "Error: mmconc_atomic_u8_load(&atmc) != 0");

    mmconc_atomic_uint16_t atmc16 = mmconc_atomic_u16_get_init(0);
    
    mgthrd_create(&thrd1, ctest_mt_u16_ovf_add_cb_1, &atmc16);
    mgthrd_create(&thrd2, ctest_mt_u16_ovf_add_cb_1, &atmc16);
    mgthrd_create(&thrd3, ctest_mt_u16_ovf_add_cb_1, &atmc16);
    mgthrd_create(&thrd4, ctest_mt_u16_ovf_add_cb_1, &atmc16);
    
    mgthrd_join(thrd1, NULL);
    mgthrd_join(thrd2, NULL);
    mgthrd_join(thrd3, NULL);
    mgthrd_join(thrd4, NULL);

    mu_assert(mmconc_atomic_u16_load(&atmc16) == 0x900,
        "Error: mmconc_atomic_u16_load(&atmc) != 0x900");
}

//#ifdef __cplusplus
//extern "C" {
//#endif
//    void ctest_multithrd_add();
//#ifdef __cplusplus
//}
//#endif


MU_TEST_SUITE(ctest_suite) {
    MU_RUN_TEST(ctest_mt_int_add);
    MU_RUN_TEST(ctest_mt_int_ovf_add);
    MU_RUN_TEST(ctest_mt_uint_ovf_add);
}

int main() {
    MU_RUN_SUITE(ctest_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
