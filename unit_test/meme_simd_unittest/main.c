
#include <minunit.h>
#include <meme/simd/simd.h>

MU_TEST(ctest_mmsimd_i8_add_001) 
{
    int8_t a[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    int8_t b[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    int8_t c[15] = { 0 };
    int8_t d[15] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30 };
    mmsimd_i8_add(a, b, c, 15);
    mu_assert(memcmp(c, d, 15) == 0, "Error: memcmp(c, d, 16) == 0 failed");
}

MU_TEST(ctest_mmsimd_i8_add_002)
{
    int8_t a[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 
        17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
    int8_t b[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
        17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
    int8_t c[32] = { 0 };
    int8_t d[32] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32,
        34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64 };
    mmsimd_i8_add(a, b, c, 32);
    mu_assert(memcmp(c, d, 32) == 0, "Error: memcmp(c, d, 32) == 0 failed");
}

MU_TEST(ctest_mmsimd_i8_add_003)
{
    int8_t a[63];
    int8_t b[63];
    int8_t c[63] = { 0 };
    int8_t d[63];

    for (int i = 0; i < 63; ++i) {
        a[i] = (int8_t)i + 1;
    }

    for (int i = 0; i < 63; ++i) {
        b[i] = (int8_t)(63 - i);
    }

    for (int i = 0; i < 63; ++i) {
        d[i] = 64;
    }

    mmsimd_i8_add(a, b, c, 63);
    mu_assert(memcmp(c, d, 63) == 0, "Error: memcmp(c, d, 63) == 0 failed");
}

MU_TEST(ctest_mmsimd_i8_add_004)
{
    int8_t a[65] = { 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
        77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93,
        94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
        109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
        123, 124, 125, 126, 127 };
    int8_t b[65] = { 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 
        77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 
        94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 
        109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 
        123, 124, 125, 126, 127 };
    int8_t c[65] = { 0 };
    int8_t d[65] = { 126, 128, 130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 
        150, 152, 154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 
        180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200, 202, 204, 206, 208, 
        210, 212, 214, 216, 218, 220, 222, 224, 226, 228, 230, 232, 234, 236, 238, 
        240, 242, 244, 246, 248, 250, 252, 254 };
    
    mmsimd_i8_add(a, b, c, 65);
    mu_assert(memcmp(c, d, 65) == 0, "Error: memcmp(c, d, 65) == 0 failed");
}

MU_TEST(ctest_mmsimd_u8_add_001)
{
    uint8_t a[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    uint8_t b[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    uint8_t c[15] = { 0 };
    uint8_t d[15] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30 };
    mmsimd_u8_add(a, b, c, 15);
    mu_assert(memcmp(c, d, 15) == 0, "Error: memcmp(c, d, 16) == 0 failed");
} 

MU_TEST(ctest_mmsimd_u8_add_002)
{
    uint8_t a[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 
        17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
    uint8_t b[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
        17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
    uint8_t c[32] = { 0 };
    uint8_t d[32] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32,
        34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64 };
    mmsimd_u8_add(a, b, c, 32);
    mu_assert(memcmp(c, d, 32) == 0, "Error: memcmp(c, d, 32) == 0 failed");
}

MU_TEST(ctest_mmsimd_u8_add_003)
{
    uint8_t a[63];
    uint8_t b[63];
    uint8_t c[63] = { 0 };
    uint8_t d[63];

    for (int i = 0; i < 63; ++i) {
        a[i] = (uint8_t)i + 1;
    }

    for (int i = 0; i < 63; ++i) {
        b[i] = (uint8_t)(63 - i);
    }

    for (int i = 0; i < 63; ++i) {
        d[i] = 64;
    }

    mmsimd_u8_add(a, b, c, 63);
    mu_assert(memcmp(c, d, 63) == 0, "Error: memcmp(c, d, 63) == 0 failed");
}

MU_TEST(ctest_mmsimd_u8_add_004)
{
    uint8_t a[65];
    uint8_t b[65];
    uint8_t c[65] = { 0 };
    uint8_t d[65];

    for (int i = 0; i < 65; ++i) {
        a[i] = (uint8_t)i + 63;
    }

    for (int i = 0; i < 65; ++i) {
        b[i] = (uint8_t)i + 63;
    }

    for (int i = 0; i < 65; ++i) {
        d[i] = 2 * i + 126;
    }

    mmsimd_u8_add(a, b, c, 65);

    mu_assert(memcmp(c, d, 65) == 0, "Error: memcmp(c, d, 65) == 0 failed");

}

MU_TEST(ctest_mmsimd_i8_to_i16_001)
{
    int8_t  a[65];
    int16_t b[65];

    for (int i = 0; i < 65; ++i) {
        a[i] = (int8_t)i + 63;
    }

    mmsimd_i8_to_i16(a, b, 65);
    for (int i = 0; i < 65; ++i) {
        mu_assert(b[i] == a[i], "Error: b[i] == a[i] failed");
    }
}

MU_TEST(ctest_mmsimd_i16_fill_001)
{
    int16_t a[65];
    int16_t b = 1;
    int16_t c[65];

    for (int i = 0; i < 65; ++i) {
        c[i] = 1;
    }
    
    mmsimd_i16_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(int16_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(int16_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_i16_fill_002)
{
    int16_t a[65];
    int16_t b = -1;
    int16_t c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = -1;
    }

    mmsimd_i16_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(int16_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(int16_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_i16_fill_003)
{
    int16_t a[65];
    int16_t b = 0;
    int16_t c[65] = { 0 };

    mmsimd_i16_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(int16_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(int16_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_u16_fill_001)
{
    uint16_t a[65];
    uint16_t b = UINT16_MAX;
    uint16_t c[65];
    memset(c, 0xFF, 65 * sizeof(uint16_t));

    mmsimd_u16_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(uint16_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(uint16_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_u16_fill_002)
{
    uint16_t a[65];
    uint16_t b = 0;
    uint16_t c[65] = { 0 };

    mmsimd_u16_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(uint16_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(uint16_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_u16_fill_003)
{
    uint16_t a[65];
    uint16_t b = 1;
    uint16_t c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = 1;
    }

    mmsimd_u16_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(uint16_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(uint16_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_i32_fill_001)
{
    int32_t a[65];
    int32_t b = 0;
    int32_t c[65] = { 0 };

    mmsimd_i32_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(int32_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(int32_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_i32_fill_002)
{
    int32_t a[65];
    int32_t b = 1;
    int32_t c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = 1;
    }

    mmsimd_i32_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(int32_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(int32_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_i32_fill_003)
{
    int32_t a[65];
    int32_t b = INT32_MIN;
    int32_t c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = INT32_MIN;
    }

    mmsimd_i32_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(int32_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(int32_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_i32_fill_004)
{
    int32_t a[65];
    int32_t b = INT32_MAX;
    int32_t c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = INT32_MAX;
    }

    mmsimd_i32_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(int32_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(int32_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_u32_fill_001)
{
    uint32_t a[65];
    uint32_t b = 0;
    uint32_t c[65] = { 0 };

    mmsimd_u32_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(uint32_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(uint32_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_u32_fill_002)
{
    uint32_t a[65];
    uint32_t b = 1;
    uint32_t c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = 1;
    }

    mmsimd_u32_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(uint32_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(uint32_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_u32_fill_003)
{
    uint32_t a[65];
    uint32_t b = UINT32_MAX;
    uint32_t c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = UINT32_MAX;
    }

    mmsimd_u32_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(uint32_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(uint32_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_i64_fill_001)
{
    int64_t a[65];
    int64_t b = 0;
    int64_t c[65] = { 0 };

    mmsimd_i64_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(int64_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(int64_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_i64_fill_002)
{
    int64_t a[65];
    int64_t b = 1;
    int64_t c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = 1;
    }

    mmsimd_i64_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(int64_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(int64_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_i64_fill_003)
{
    int64_t a[65];
    int64_t b = INT64_MIN;
    int64_t c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = INT64_MIN;
    }

    mmsimd_i64_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(int64_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(int64_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_i64_fill_004)
{
    int64_t a[65];
    int64_t b = INT64_MAX;
    int64_t c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = INT64_MAX;
    }

    mmsimd_i64_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(int64_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(int64_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_u64_fill_001)
{
    uint64_t a[65];
    uint64_t b = 0;
    uint64_t c[65] = { 0 };

    mmsimd_u64_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(uint64_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(uint64_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_u64_fill_002)
{
    uint64_t a[65];
    uint64_t b = 1;
    uint64_t c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = 1;
    }

    mmsimd_u64_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(uint64_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(uint64_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_u64_fill_003)
{
    uint64_t a[65];
    uint64_t b = UINT64_MAX;
    uint64_t c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = UINT64_MAX;
    }

    mmsimd_u64_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(uint64_t)) == 0, "Error: memcmp(a, c, 65 * sizeof(uint64_t)) == 0 failed");
}

MU_TEST(ctest_mmsimd_f32_fill_001)
{
    float a[65];
    float b = 0.0f;
    float c[65] = { 0.0f };

    mmsimd_f32_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(float)) == 0, "Error: memcmp(a, c, 65 * sizeof(float)) == 0 failed");
}

#include <float.h>

MU_TEST(ctest_mmsimd_f32_fill_002)
{
    float a[65];
    float b = 1.1f;
    float c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = 1.1f;
    }

    mmsimd_f32_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(float)) == 0, "Error: memcmp(a, c, 65 * sizeof(float)) == 0 failed");
}

MU_TEST(ctest_mmsimd_f32_fill_003)
{
    float a[65];
    float b = -1.1f;
    float c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = -1.1f;
    }

    mmsimd_f32_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(float)) == 0, "Error: memcmp(a, c, 65 * sizeof(float)) == 0 failed");
}

MU_TEST(ctest_mmsimd_f32_fill_004)
{
    float a[65];
    float b = FLT_MAX;
    float c[65];

    for (int i = 0; i < 65; ++i) {
        c[i] = FLT_MAX;
    }

    mmsimd_f32_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(float)) == 0, "Error: memcmp(a, c, 65 * sizeof(float)) == 0 failed");
}

MU_TEST(ctest_mmsimd_f64_fill_001)
{
    double a[65];
    double b = 0.0;
    double c[65] = { 0.0 };

    mmsimd_f64_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(double)) == 0, "Error: memcmp(a, c, 65 * sizeof(double)) == 0 failed");
}

MU_TEST(ctest_mmsimd_f64_fill_002)
{
    double a[65];
    double b = 1.1;
    double c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = 1.1;
    }

    mmsimd_f64_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(double)) == 0, "Error: memcmp(a, c, 65 * sizeof(double)) == 0 failed");
}

MU_TEST(ctest_mmsimd_f64_fill_003)
{
    double a[65];
    double b = -1.1;
    double c[65];
    
    for (int i = 0; i < 65; ++i) {
        c[i] = -1.1;
    }

    mmsimd_f64_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(double)) == 0, "Error: memcmp(a, c, 65 * sizeof(double)) == 0 failed");
}

MU_TEST(ctest_mmsimd_f64_fill_004)
{
    double a[65];
    double b = DBL_MAX;
    double c[65];

    for (int i = 0; i < 65; ++i) {
        c[i] = DBL_MAX;
    }

    mmsimd_f64_fill(a, 65, b);

    mu_assert(memcmp(a, c, 65 * sizeof(double)) == 0, "Error: memcmp(a, c, 65 * sizeof(double)) == 0 failed");
}

MU_TEST_SUITE(ctest_suite) {
    MU_RUN_TEST(ctest_mmsimd_i8_add_001);
    MU_RUN_TEST(ctest_mmsimd_i8_add_002);
    MU_RUN_TEST(ctest_mmsimd_i8_add_003);
    MU_RUN_TEST(ctest_mmsimd_i8_add_004);
    MU_RUN_TEST(ctest_mmsimd_u8_add_001);
    MU_RUN_TEST(ctest_mmsimd_u8_add_002);
    MU_RUN_TEST(ctest_mmsimd_u8_add_003);
    MU_RUN_TEST(ctest_mmsimd_u8_add_004);

    MU_RUN_TEST(ctest_mmsimd_i8_to_i16_001);

    MU_RUN_TEST(ctest_mmsimd_i16_fill_001);
    MU_RUN_TEST(ctest_mmsimd_i16_fill_002);
    MU_RUN_TEST(ctest_mmsimd_i16_fill_003);
    MU_RUN_TEST(ctest_mmsimd_u16_fill_001);
    MU_RUN_TEST(ctest_mmsimd_u16_fill_002);
    MU_RUN_TEST(ctest_mmsimd_u16_fill_003);
    MU_RUN_TEST(ctest_mmsimd_i32_fill_001);
    MU_RUN_TEST(ctest_mmsimd_i32_fill_002);
    MU_RUN_TEST(ctest_mmsimd_i32_fill_003);
    MU_RUN_TEST(ctest_mmsimd_i32_fill_004);
    MU_RUN_TEST(ctest_mmsimd_u32_fill_001);
    MU_RUN_TEST(ctest_mmsimd_u32_fill_002);
    MU_RUN_TEST(ctest_mmsimd_u32_fill_003);
    MU_RUN_TEST(ctest_mmsimd_i64_fill_001);
    MU_RUN_TEST(ctest_mmsimd_i64_fill_002);
    MU_RUN_TEST(ctest_mmsimd_i64_fill_003);
    MU_RUN_TEST(ctest_mmsimd_i64_fill_004);
    MU_RUN_TEST(ctest_mmsimd_u64_fill_001);
    MU_RUN_TEST(ctest_mmsimd_u64_fill_002);
    MU_RUN_TEST(ctest_mmsimd_u64_fill_003);
    MU_RUN_TEST(ctest_mmsimd_f32_fill_001);
    MU_RUN_TEST(ctest_mmsimd_f32_fill_002);
    MU_RUN_TEST(ctest_mmsimd_f32_fill_003);
    MU_RUN_TEST(ctest_mmsimd_f32_fill_004);
    MU_RUN_TEST(ctest_mmsimd_f64_fill_001);
    MU_RUN_TEST(ctest_mmsimd_f64_fill_002);
    MU_RUN_TEST(ctest_mmsimd_f64_fill_003);
    MU_RUN_TEST(ctest_mmsimd_f64_fill_004);
}

int main() {
    MU_RUN_SUITE(ctest_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
