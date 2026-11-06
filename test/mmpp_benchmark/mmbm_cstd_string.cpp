// Pure C standard library string benchmarks (string.h / stdlib.h only).
// Mirrors mmbm_string_comprehensive.cpp so results line up 1:1.
//
// C has no string type, so each operation is spelled the way C spells it:
//   ctor  = malloc + memcpy
//   copy  = malloc + memcpy
//   size  = strlen
//   find  = strchr / strstr
//   substr= malloc + memcpy
//   concat= malloc + memcpy
//
// NOTE: the malloc/free in ctor/copy/substr/concat is unavoidable in C --
// it is the C equivalent of constructing a std::string. It is kept, not elided.

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <cstring>
#include <cstdlib>

// Same test data as mmbm_string_comprehensive.cpp
static const char* short_str = "Hello!";
static const char* medium_str =
    "The quick brown fox jumps over the lazy dog. "
    "Pack my box with five dozen liquor jugs.";
static const char* large_str =
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
    "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
    "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris. "
    "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum. "
    "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia. "
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
    "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";

// ============================================================================
// CONSTRUCTION - Default
// C: heap-allocate a 1-byte buffer holding just the terminator.
static void CStd_DefaultCtor(benchmark::State& state) {
    for (auto _ : state) {
        char* s = (char*)malloc(1);
        s[0] = '\0';
        benchmark::DoNotOptimize(s);
        free(s);
    }
}
BENCHMARK(CStd_DefaultCtor)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// CONSTRUCTION - From C string (short)
static void CStd_CtorShortCStr(benchmark::State& state) {
    size_t len = strlen(short_str);
    for (auto _ : state) {
        char* s = (char*)malloc(len + 1);
        memcpy(s, short_str, len + 1);
        benchmark::DoNotOptimize(s);
        free(s);
    }
}
BENCHMARK(CStd_CtorShortCStr)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// CONSTRUCTION - From C string (medium)
static void CStd_CtorMediumCStr(benchmark::State& state) {
    size_t len = strlen(medium_str);
    for (auto _ : state) {
        char* s = (char*)malloc(len + 1);
        memcpy(s, medium_str, len + 1);
        benchmark::DoNotOptimize(s);
        free(s);
    }
}
BENCHMARK(CStd_CtorMediumCStr)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// CONSTRUCTION - From C string (large)
static void CStd_CtorLargeCStr(benchmark::State& state) {
    size_t len = strlen(large_str);
    for (auto _ : state) {
        char* s = (char*)malloc(len + 1);
        memcpy(s, large_str, len + 1);
        benchmark::DoNotOptimize(s);
        free(s);
    }
}
BENCHMARK(CStd_CtorLargeCStr)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// COPY CONSTRUCTION - Small
static void CStd_CopyCtorSmall(benchmark::State& state) {
    size_t len = strlen(short_str);
    char* src = (char*)malloc(len + 1);
    memcpy(src, short_str, len + 1);
    for (auto _ : state) {
        char* copy = (char*)malloc(len + 1);
        memcpy(copy, src, len + 1);
        benchmark::DoNotOptimize(copy);
        free(copy);
    }
    free(src);
}
BENCHMARK(CStd_CopyCtorSmall)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// COPY CONSTRUCTION - Large
static void CStd_CopyCtorLarge(benchmark::State& state) {
    size_t len = strlen(large_str);
    char* src = (char*)malloc(len + 1);
    memcpy(src, large_str, len + 1);
    for (auto _ : state) {
        char* copy = (char*)malloc(len + 1);
        memcpy(copy, src, len + 1);
        benchmark::DoNotOptimize(copy);
        free(copy);
    }
    free(src);
}
BENCHMARK(CStd_CopyCtorLarge)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// SIZE / LENGTH
static void CStd_Size(benchmark::State& state) {
    for (auto _ : state)
        benchmark::DoNotOptimize(strlen(medium_str));
}
BENCHMARK(CStd_Size)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// EMPTY CHECK
static void CStd_Empty(benchmark::State& state) {
    for (auto _ : state)
        benchmark::DoNotOptimize(short_str[0] == '\0');
}
BENCHMARK(CStd_Empty)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// DATA / C_STR
// C: the pointer is already the data.
static void CStd_Data(benchmark::State& state) {
    for (auto _ : state)
        benchmark::DoNotOptimize(medium_str);
}
BENCHMARK(CStd_Data)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// INDEX ACCESS
static void CStd_IndexAccess(benchmark::State& state) {
    size_t len = strlen(medium_str);
    for (auto _ : state) {
        for (size_t i = 0; i < len; i += 8)
            benchmark::DoNotOptimize(medium_str[i]);
    }
}
BENCHMARK(CStd_IndexAccess)->Iterations(MMBM_ITERATION_COUNT / 10);

// ============================================================================
// EQUALITY COMPARISON
static void CStd_Equal(benchmark::State& state) {
    for (auto _ : state)
        benchmark::DoNotOptimize(strcmp(medium_str, medium_str) == 0);
}
BENCHMARK(CStd_Equal)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// LEXICOGRAPHICAL COMPARE
static void CStd_Compare(benchmark::State& state) {
    const char* s1 = "Apple";
    const char* s2 = "Banana";
    for (auto _ : state)
        benchmark::DoNotOptimize(strcmp(s1, s2));
}
BENCHMARK(CStd_Compare)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// FIND SINGLE CHAR
static void CStd_FindChar(benchmark::State& state) {
    for (auto _ : state)
        benchmark::DoNotOptimize(strchr(medium_str, 'x'));
}
BENCHMARK(CStd_FindChar)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// FIND SUBSTRING
static void CStd_FindSubstring(benchmark::State& state) {
    for (auto _ : state)
        benchmark::DoNotOptimize(strstr(medium_str, "fox"));
}
BENCHMARK(CStd_FindSubstring)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// SUBSTR
static void CStd_Substr(benchmark::State& state) {
    for (auto _ : state) {
        char* result = (char*)malloc(21);
        memcpy(result, medium_str + 10, 20);
        result[20] = '\0';
        benchmark::DoNotOptimize(result);
        free(result);
    }
}
BENCHMARK(CStd_Substr)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// CONCATENATION
static void CStd_Concat(benchmark::State& state) {
    const char* s1 = "Hello, ";
    const char* s2 = "World!";
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    for (auto _ : state) {
        char* result = (char*)malloc(len1 + len2 + 1);
        memcpy(result, s1, len1);
        memcpy(result + len1, s2, len2 + 1);
        benchmark::DoNotOptimize(result);
        free(result);
    }
}
BENCHMARK(CStd_Concat)->Iterations(MMBM_ITERATION_COUNT);
