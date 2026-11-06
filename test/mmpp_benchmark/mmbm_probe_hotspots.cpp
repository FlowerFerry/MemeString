// Probe: isolate WHERE the cost is for the slow memepp::string operations.
// Each probe pairs with a mmbm_string_* counterpart so the delta is attributable.

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <algorithm>
#include <string>
#include <cstring>
#include <cstdlib>

extern "C" {
#include <meme/string.h>
#include <meme/utf/u8rune.h>
}

static const char* g_hay =
    "The quick brown fox jumps over the lazy dog. "
    "Pack my box with five dozen liquor jugs.";

// ---------------------------------------------------------------------------
// P1. Cost of one cross-DLL call, measured on a trivial exported accessor.
//     MemeString_maxByteSize is exported and does almost nothing.
//     This is the floor for ANY memepp operation that crosses the boundary.
static void Probe_DllCall_maxByteSize(benchmark::State& state) {
    mmstrstk_t stk;
    MemeStringStack_initByU8bytes(&stk, MMSTR__OBJ_SIZE, (const mmbyte_t*)g_hay, -1);
    mmstr_cptr_t p = memepp::to_pointer(stk);
    for (auto _ : state)
        benchmark::DoNotOptimize(MemeString_maxByteSize(p));
    MemeStringStack_unInit(&stk, MMSTR__OBJ_SIZE);
}
BENCHMARK(Probe_DllCall_maxByteSize)->Iterations(MMBM_ITERATION_COUNT);

// P2. byteSize alone (exported, switches on storage type)
static void Probe_DllCall_byteSize(benchmark::State& state) {
    mmstrstk_t stk;
    MemeStringStack_initByU8bytes(&stk, MMSTR__OBJ_SIZE, (const mmbyte_t*)g_hay, -1);
    mmstr_cptr_t p = memepp::to_pointer(stk);
    for (auto _ : state)
        benchmark::DoNotOptimize(MemeString_byteSize(p));
    MemeStringStack_unInit(&stk, MMSTR__OBJ_SIZE);
}
BENCHMARK(Probe_DllCall_byteSize)->Iterations(MMBM_ITERATION_COUNT);

// P3. One exported MemeString_at (what operator[] costs at the C layer)
static void Probe_DllCall_at(benchmark::State& state) {
    mmstrstk_t stk;
    MemeStringStack_initByU8bytes(&stk, MMSTR__OBJ_SIZE, (const mmbyte_t*)g_hay, -1);
    mmstr_cptr_t p = memepp::to_pointer(stk);
    for (auto _ : state)
        benchmark::DoNotOptimize(MemeString_at(p, 5));
    MemeStringStack_unInit(&stk, MMSTR__OBJ_SIZE);
}
BENCHMARK(Probe_DllCall_at)->Iterations(MMBM_ITERATION_COUNT);

// P4. Direct byte read on the same data (no call at all)
static void Probe_DirectByteRead(benchmark::State& state) {
    mmstrstk_t stk;
    MemeStringStack_initByU8bytes(&stk, MMSTR__OBJ_SIZE, (const mmbyte_t*)g_hay, -1);
    const char* raw = MemeString_cStr(memepp::to_pointer(stk));
    for (auto _ : state)
        benchmark::DoNotOptimize(raw[5]);
    MemeStringStack_unInit(&stk, MMSTR__OBJ_SIZE);
}
BENCHMARK(Probe_DirectByteRead)->Iterations(MMBM_ITERATION_COUNT);

// ---------------------------------------------------------------------------
// P5. find: short needle (3B) -- Boyer-Moore table build dominates
static void Probe_find_short_needle(benchmark::State& state) {
    memepp::string s(g_hay);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.find("fox"));
}
BENCHMARK(Probe_find_short_needle)->Iterations(MMBM_ITERATION_COUNT);

// P6. find: long needle (40B) -- same table build, but scan amortizes it.
//     If P6 ~= P5, the cost is the fixed table build, not the scan.
static void Probe_find_long_needle(benchmark::State& state) {
    memepp::string s(g_hay);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.find("jumps over the lazy dog. Pack my box"));
}
BENCHMARK(Probe_find_long_needle)->Iterations(MMBM_ITERATION_COUNT);

// P7. find: needle NOT present, forces a full scan of the haystack
static void Probe_find_absent_needle(benchmark::State& state) {
    memepp::string s(g_hay);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.find("ZZZZZZ"));
}
BENCHMARK(Probe_find_absent_needle)->Iterations(MMBM_ITERATION_COUNT);

// P8. std::string find on identical data for reference
static void Probe_std_find_short(benchmark::State& state) {
    std::string s(g_hay);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.find("fox"));
}
BENCHMARK(Probe_std_find_short)->Iterations(MMBM_ITERATION_COUNT);

static void Probe_std_find_long(benchmark::State& state) {
    std::string s(g_hay);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.find("jumps over the lazy dog. Pack my box"));
}
BENCHMARK(Probe_std_find_long)->Iterations(MMBM_ITERATION_COUNT);

// ---------------------------------------------------------------------------
// P9. reverse: count the passes. memepp reverse = copy + validate + 2 reverses.
//     Compare against the raw building blocks it uses internally.
static void Probe_reverse_full(benchmark::State& state) {
    memepp::string s("Hello World! The quick brown fox.");
    for (auto _ : state) {
        memepp::string r = s.reverse();
        benchmark::DoNotOptimize(r.data());
    }
}
BENCHMARK(Probe_reverse_full)->Iterations(MMBM_ITERATION_COUNT);

// P10. Just the allocation+copy that reverse starts with (initByU8bytes)
static void Probe_reverse_alloc_copy_only(benchmark::State& state) {
    const char* src = "Hello World! The quick brown fox.";
    size_t n = strlen(src);
    for (auto _ : state) {
        mmstrstk_t out;
        MemeStringStack_initByU8bytes(&out, MMSTR__OBJ_SIZE, (const mmbyte_t*)src, (mmint_t)n);
        MemeStringStack_unInit(&out, MMSTR__OBJ_SIZE);
    }
}
BENCHMARK(Probe_reverse_alloc_copy_only)->Iterations(MMBM_ITERATION_COUNT);

// P11. std::reverse on the same length for reference
static void Probe_std_reverse(benchmark::State& state) {
    std::string s("Hello World! The quick brown fox.");
    for (auto _ : state) {
        std::string r = s;
        std::reverse(r.begin(), r.end());
        benchmark::DoNotOptimize(r.data());
    }
}
BENCHMARK(Probe_std_reverse)->Iterations(MMBM_ITERATION_COUNT);

// ---------------------------------------------------------------------------
// P12. concat: memepp goes through an exported call; measure the C API direct
static void Probe_concat_capi(benchmark::State& state) {
    mmstrstk_t a, b;
    MemeStringStack_initByU8bytes(&a, MMSTR__OBJ_SIZE, (const mmbyte_t*)"Hello, ", -1);
    MemeStringStack_initByU8bytes(&b, MMSTR__OBJ_SIZE, (const mmbyte_t*)"World!", -1);
    for (auto _ : state) {
        mmstrstk_t out;
        MemeStringStack_concat_v2(&a, &b, &out, MMSTR__OBJ_SIZE);
        MemeStringStack_unInit(&out, MMSTR__OBJ_SIZE);
    }
    MemeStringStack_unInit(&a, MMSTR__OBJ_SIZE);
    MemeStringStack_unInit(&b, MMSTR__OBJ_SIZE);
}
BENCHMARK(Probe_concat_capi)->Iterations(MMBM_ITERATION_COUNT);

// P13. memepp concat through the C++ wrapper: delta vs P12 = wrapper cost
static void Probe_concat_wrapper(benchmark::State& state) {
    memepp::string a("Hello, ");
    memepp::string b("World!");
    for (auto _ : state) {
        memepp::string r = a.concat(b);
        benchmark::DoNotOptimize(r.data());
    }
}
BENCHMARK(Probe_concat_wrapper)->Iterations(MMBM_ITERATION_COUNT);

// P14. std::string concat, same operands
static void Probe_std_concat(benchmark::State& state) {
    std::string a("Hello, ");
    std::string b("World!");
    for (auto _ : state) {
        std::string r = a + b;
        benchmark::DoNotOptimize(r.data());
    }
}
BENCHMARK(Probe_std_concat)->Iterations(MMBM_ITERATION_COUNT);

// ---------------------------------------------------------------------------
// P15. operator[] chain: shows the per-access call cost directly
static void Probe_index_memepp(benchmark::State& state) {
    memepp::string s(g_hay);
    auto n = s.size();
    for (auto _ : state)
        for (size_t i = 0; i < n; ++i)
            benchmark::DoNotOptimize(s[i]);
}
BENCHMARK(Probe_index_memepp)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Probe_index_std(benchmark::State& state) {
    std::string s(g_hay);
    auto n = s.size();
    for (auto _ : state)
        for (size_t i = 0; i < n; ++i)
            benchmark::DoNotOptimize(s[i]);
}
BENCHMARK(Probe_index_std)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Probe_index_rawptr(benchmark::State& state) {
    mmstrstk_t stk;
    MemeStringStack_initByU8bytes(&stk, MMSTR__OBJ_SIZE, (const mmbyte_t*)g_hay, -1);
    const char* raw = MemeString_cStr(memepp::to_pointer(stk));
    size_t n = strlen(raw);
    for (auto _ : state)
        for (size_t i = 0; i < n; ++i)
            benchmark::DoNotOptimize(raw[i]);
    MemeStringStack_unInit(&stk, MMSTR__OBJ_SIZE);
}
BENCHMARK(Probe_index_rawptr)->Iterations(MMBM_ITERATION_COUNT / 10);

// ---------------------------------------------------------------------------
// P16. copy: small (inline SSO) vs large (refcounted) -- the design payoff
static void Probe_copy_small(benchmark::State& state) {
    memepp::string src("Hello!");
    for (auto _ : state) {
        memepp::string c(src);
        benchmark::DoNotOptimize(c.data());
    }
}
BENCHMARK(Probe_copy_small)->Iterations(MMBM_ITERATION_COUNT);

static void Probe_copy_large_shared(benchmark::State& state) {
    memepp::string src = memepp::string(g_hay).to_shared_storage();
    for (auto _ : state) {
        memepp::string c(src);
        benchmark::DoNotOptimize(c.data());
    }
}
BENCHMARK(Probe_copy_large_shared)->Iterations(MMBM_ITERATION_COUNT);

static void Probe_std_copy_small(benchmark::State& state) {
    std::string src("Hello!");
    for (auto _ : state) {
        std::string c(src);
        benchmark::DoNotOptimize(c.data());
    }
}
BENCHMARK(Probe_std_copy_small)->Iterations(MMBM_ITERATION_COUNT);

static void Probe_std_copy_large(benchmark::State& state) {
    std::string src(g_hay);
    for (auto _ : state) {
        std::string c(src);
        benchmark::DoNotOptimize(c.data());
    }
}
BENCHMARK(Probe_std_copy_large)->Iterations(MMBM_ITERATION_COUNT);
