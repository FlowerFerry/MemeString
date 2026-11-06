// Comprehensive memepp::string benchmark vs std::string vs C API
// Covers: construction, copy, move, destruction, comparison, search, access

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <string>
#include <cstring>

// Test data generators
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
static void MemeStr_DefaultCtor(benchmark::State& state) {
    for (auto _ : state)
        memepp::string s;
}
BENCHMARK(MemeStr_DefaultCtor)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_DefaultCtor(benchmark::State& state) {
    for (auto _ : state)
        std::string s;
}
BENCHMARK(StdStr_DefaultCtor)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// CONSTRUCTION - From C string (short)
static void MemeStr_CtorShortCStr(benchmark::State& state) {
    for (auto _ : state)
        memepp::string s(short_str);
}
BENCHMARK(MemeStr_CtorShortCStr)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_CtorShortCStr(benchmark::State& state) {
    for (auto _ : state)
        std::string s(short_str);
}
BENCHMARK(StdStr_CtorShortCStr)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// CONSTRUCTION - From C string (medium)
static void MemeStr_CtorMediumCStr(benchmark::State& state) {
    for (auto _ : state)
        memepp::string s(medium_str);
}
BENCHMARK(MemeStr_CtorMediumCStr)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_CtorMediumCStr(benchmark::State& state) {
    for (auto _ : state)
        std::string s(medium_str);
}
BENCHMARK(StdStr_CtorMediumCStr)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// CONSTRUCTION - From C string (large)
static void MemeStr_CtorLargeCStr(benchmark::State& state) {
    for (auto _ : state)
        memepp::string s(large_str);
}
BENCHMARK(MemeStr_CtorLargeCStr)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_CtorLargeCStr(benchmark::State& state) {
    for (auto _ : state)
        std::string s(large_str);
}
BENCHMARK(StdStr_CtorLargeCStr)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// COPY CONSTRUCTION - Small
static void MemeStr_CopyCtorSmall(benchmark::State& state) {
    memepp::string src(short_str);
    for (auto _ : state)
        memepp::string copy(src);
}
BENCHMARK(MemeStr_CopyCtorSmall)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_CopyCtorSmall(benchmark::State& state) {
    std::string src(short_str);
    for (auto _ : state)
        std::string copy(src);
}
BENCHMARK(StdStr_CopyCtorSmall)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// COPY CONSTRUCTION - Large
static void MemeStr_CopyCtorLarge(benchmark::State& state) {
    memepp::string src(large_str);
    for (auto _ : state)
        memepp::string copy(src);
}
BENCHMARK(MemeStr_CopyCtorLarge)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_CopyCtorLarge(benchmark::State& state) {
    std::string src(large_str);
    for (auto _ : state)
        std::string copy(src);
}
BENCHMARK(StdStr_CopyCtorLarge)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// MOVE CONSTRUCTION - Small
static void MemeStr_MoveCtorSmall(benchmark::State& state) {
    for (auto _ : state) {
        memepp::string src(short_str);
        memepp::string moved(std::move(src));
        benchmark::DoNotOptimize(moved.data());
    }
}
BENCHMARK(MemeStr_MoveCtorSmall)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_MoveCtorSmall(benchmark::State& state) {
    for (auto _ : state) {
        std::string src(short_str);
        std::string moved(std::move(src));
        benchmark::DoNotOptimize(moved.data());
    }
}
BENCHMARK(StdStr_MoveCtorSmall)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// MOVE CONSTRUCTION - Large
static void MemeStr_MoveCtorLarge(benchmark::State& state) {
    for (auto _ : state) {
        memepp::string src(large_str);
        memepp::string moved(std::move(src));
        benchmark::DoNotOptimize(moved.data());
    }
}
BENCHMARK(MemeStr_MoveCtorLarge)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_MoveCtorLarge(benchmark::State& state) {
    for (auto _ : state) {
        std::string src(large_str);
        std::string moved(std::move(src));
        benchmark::DoNotOptimize(moved.data());
    }
}
BENCHMARK(StdStr_MoveCtorLarge)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// SIZE / LENGTH
static void MemeStr_Size(benchmark::State& state) {
    memepp::string s(medium_str);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.size());
}
BENCHMARK(MemeStr_Size)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Size(benchmark::State& state) {
    std::string s(medium_str);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.size());
}
BENCHMARK(StdStr_Size)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// EMPTY CHECK
static void MemeStr_Empty(benchmark::State& state) {
    memepp::string s(short_str);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.empty());
}
BENCHMARK(MemeStr_Empty)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Empty(benchmark::State& state) {
    std::string s(short_str);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.empty());
}
BENCHMARK(StdStr_Empty)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// DATA / C_STR
static void MemeStr_Data(benchmark::State& state) {
    memepp::string s(medium_str);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.data());
}
BENCHMARK(MemeStr_Data)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Data(benchmark::State& state) {
    std::string s(medium_str);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.data());
}
BENCHMARK(StdStr_Data)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// INDEX ACCESS
static void MemeStr_IndexAccess(benchmark::State& state) {
    memepp::string s(medium_str);
    auto sz = s.size();
    for (auto _ : state) {
        for (size_t i = 0; i < sz; i += 8)
            benchmark::DoNotOptimize(s[i]);
    }
}
BENCHMARK(MemeStr_IndexAccess)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_IndexAccess(benchmark::State& state) {
    std::string s(medium_str);
    auto sz = s.size();
    for (auto _ : state) {
        for (size_t i = 0; i < sz; i += 8)
            benchmark::DoNotOptimize(s[i]);
    }
}
BENCHMARK(StdStr_IndexAccess)->Iterations(MMBM_ITERATION_COUNT / 10);

// ============================================================================
// EQUALITY COMPARISON
static void MemeStr_Equal(benchmark::State& state) {
    memepp::string s1(medium_str);
    memepp::string s2(medium_str);
    for (auto _ : state)
        benchmark::DoNotOptimize(s1 == s2);
}
BENCHMARK(MemeStr_Equal)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Equal(benchmark::State& state) {
    std::string s1(medium_str);
    std::string s2(medium_str);
    for (auto _ : state)
        benchmark::DoNotOptimize(s1 == s2);
}
BENCHMARK(StdStr_Equal)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// LEXICOGRAPHICAL COMPARE
static void MemeStr_Compare(benchmark::State& state) {
    memepp::string s1("Apple");
    memepp::string s2("Banana");
    for (auto _ : state)
        benchmark::DoNotOptimize(s1.compare(s2));
}
BENCHMARK(MemeStr_Compare)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Compare(benchmark::State& state) {
    std::string s1("Apple");
    std::string s2("Banana");
    for (auto _ : state)
        benchmark::DoNotOptimize(s1.compare(s2));
}
BENCHMARK(StdStr_Compare)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// FIND SINGLE CHAR
static void MemeStr_FindChar(benchmark::State& state) {
    memepp::string s(medium_str);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.find('x'));
}
BENCHMARK(MemeStr_FindChar)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_FindChar(benchmark::State& state) {
    std::string s(medium_str);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.find('x'));
}
BENCHMARK(StdStr_FindChar)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// FIND SUBSTRING
static void MemeStr_FindSubstring(benchmark::State& state) {
    memepp::string s(medium_str);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.find("fox"));
}
BENCHMARK(MemeStr_FindSubstring)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_FindSubstring(benchmark::State& state) {
    std::string s(medium_str);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.find("fox"));
}
BENCHMARK(StdStr_FindSubstring)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// SUBSTR
static void MemeStr_Substr(benchmark::State& state) {
    memepp::string s(medium_str);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.substr(10, 20));
}
BENCHMARK(MemeStr_Substr)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Substr(benchmark::State& state) {
    std::string s(medium_str);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.substr(10, 20));
}
BENCHMARK(StdStr_Substr)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// CONCATENATION
static void MemeStr_Concat(benchmark::State& state) {
    memepp::string s1("Hello, ");
    memepp::string s2("World!");
    for (auto _ : state)
        benchmark::DoNotOptimize(s1 + s2);
}
BENCHMARK(MemeStr_Concat)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Concat(benchmark::State& state) {
    std::string s1("Hello, ");
    std::string s2("World!");
    for (auto _ : state)
        benchmark::DoNotOptimize(s1 + s2);
}
BENCHMARK(StdStr_Concat)->Iterations(MMBM_ITERATION_COUNT);
