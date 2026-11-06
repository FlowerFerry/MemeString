// import_from_dll / export_into_dll — real data transfer across the C++ layer.
//
// The base templates in dll.hpp return empty objects; the specialisations for
// string (string_tmpimpl.hpp), buffer (buffer_tmpimpl.hpp), variant, and varts
// do the actual work.  This file tests the string specialisations, which are
// the most commonly used.
//
// Each benchmark constructs a source object, crosses it into the plain C stack
// handle (the "DLL boundary"), then crosses it back.  The round-trip measures
// the full cost of the bridge in both directions.

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <memepp/buffer.hpp>
#include <memepp/dll.hpp>
#include <string>
#include <cstring>

namespace {

const char* kShort = "Hello!";
const char* kLong  =
    "The quick brown fox jumps over the lazy dog. "
    "Pack my box with five dozen liquor jugs. "
    "How vexingly quick daft zebras jump! "
    "Sphinx of black quartz, judge my vow. ";

// ---------------------------------------------------------------------------
// STRING round-trip
// ---------------------------------------------------------------------------

// Host → DLL → Host (const& → copy or share)
static void Dll_Roundtrip_String_Const_Short(benchmark::State& state) {
    memepp::string src(kShort);
    for (auto _ : state) {
        mmstrstk_t stk = memepp::export_into_dll<mmstrstk_t>(src, MMSTR__OBJ_SIZE);
        memepp::string out = memepp::import_from_dll<memepp::string>(stk, MMSTR__OBJ_SIZE);
        mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE);
        benchmark::DoNotOptimize(out.data());
    }
}
BENCHMARK(Dll_Roundtrip_String_Const_Short)->Iterations(MMBM_ITERATION_COUNT);

static void Dll_Roundtrip_String_Const_Long(benchmark::State& state) {
    memepp::string src(kLong);
    for (auto _ : state) {
        mmstrstk_t stk = memepp::export_into_dll<mmstrstk_t>(src, MMSTR__OBJ_SIZE);
        memepp::string out = memepp::import_from_dll<memepp::string>(stk, MMSTR__OBJ_SIZE);
        mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE);
        benchmark::DoNotOptimize(out.data());
    }
}
BENCHMARK(Dll_Roundtrip_String_Const_Long)->Iterations(MMBM_ITERATION_COUNT / 10);

// Host → DLL (&& → move / transfer ownership)
static void Dll_Export_Move_Short(benchmark::State& state) {
    for (auto _ : state) {
        memepp::string src(kShort);
        mmstrstk_t stk = memepp::export_into_dll<mmstrstk_t>(std::move(src), MMSTR__OBJ_SIZE);
        benchmark::DoNotOptimize(reinterpret_cast<const unsigned char*>(&stk)[0]);
        mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE);
    }
}
BENCHMARK(Dll_Export_Move_Short)->Iterations(MMBM_ITERATION_COUNT);

static void Dll_Export_Move_Long(benchmark::State& state) {
    for (auto _ : state) {
        memepp::string src(kLong);
        mmstrstk_t stk = memepp::export_into_dll<mmstrstk_t>(std::move(src), MMSTR__OBJ_SIZE);
        benchmark::DoNotOptimize(reinterpret_cast<const unsigned char*>(&stk)[0]);
        mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE);
    }
}
BENCHMARK(Dll_Export_Move_Long)->Iterations(MMBM_ITERATION_COUNT / 10);

// DLL → Host (rvalue import uninit's source)
static void Dll_Import_Move_Short(benchmark::State& state) {
    for (auto _ : state) {
        mmstrstk_t stk;
        MemeStringStack_initByU8bytes(&stk, MMSTR__OBJ_SIZE,
            (const uint8_t*)kShort, static_cast<mmint_t>(std::strlen(kShort)));
        memepp::string out = memepp::import_from_dll<memepp::string>(std::move(stk), MMSTR__OBJ_SIZE);
        benchmark::DoNotOptimize(out.data());
    }
}
BENCHMARK(Dll_Import_Move_Short)->Iterations(MMBM_ITERATION_COUNT);

static void Dll_Import_Move_Long(benchmark::State& state) {
    for (auto _ : state) {
        mmstrstk_t stk;
        MemeStringStack_initByU8bytes(&stk, MMSTR__OBJ_SIZE,
            (const uint8_t*)kLong, static_cast<mmint_t>(std::strlen(kLong)));
        memepp::string out = memepp::import_from_dll<memepp::string>(std::move(stk), MMSTR__OBJ_SIZE);
        benchmark::DoNotOptimize(out.data());
    }
}
BENCHMARK(Dll_Import_Move_Long)->Iterations(MMBM_ITERATION_COUNT / 10);

// Shared-storage round-trip: const& export shares refcount, import deep-copies
static void Dll_Roundtrip_String_Shared(benchmark::State& state) {
    memepp::string src(kLong);
    src = src.to_shared_storage();  // force large/user so export shares
    for (auto _ : state) {
        mmstrstk_t stk = memepp::export_into_dll<mmstrstk_t>(src, MMSTR__OBJ_SIZE);
        memepp::string out = memepp::import_from_dll<memepp::string>(stk, MMSTR__OBJ_SIZE);
        mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE);
        benchmark::DoNotOptimize(out.data());
    }
}
BENCHMARK(Dll_Roundtrip_String_Shared)->Iterations(MMBM_ITERATION_COUNT / 10);

// ---------------------------------------------------------------------------
// BUFFER round-trip
// ---------------------------------------------------------------------------
static uint8_t make_byte(std::size_t i) { return static_cast<uint8_t>(i & 0xFF); }

static memepp::buffer make_buf(std::size_t n) {
    std::vector<uint8_t> data(n);
    for (std::size_t i = 0; i < n; ++i) data[i] = make_byte(i);
    return memepp::buffer(data.data(), static_cast<mmint_t>(n));
}

static void Dll_Roundtrip_Buffer_Const_Short(benchmark::State& state) {
    memepp::buffer src = make_buf(6);
    for (auto _ : state) {
        mmbufstk_t stk = memepp::export_into_dll<mmbufstk_t>(src, MMSTR__OBJ_SIZE);
        memepp::buffer out = memepp::import_from_dll<memepp::buffer>(stk, MMSTR__OBJ_SIZE);
        MemeBufferStack_unInit(&stk, MMSTR__OBJ_SIZE);
        benchmark::DoNotOptimize(out.size());
    }
}
BENCHMARK(Dll_Roundtrip_Buffer_Const_Short)->Iterations(MMBM_ITERATION_COUNT);

static void Dll_Roundtrip_Buffer_Const_Long(benchmark::State& state) {
    memepp::buffer src = make_buf(400);
    for (auto _ : state) {
        mmbufstk_t stk = memepp::export_into_dll<mmbufstk_t>(src, MMSTR__OBJ_SIZE);
        memepp::buffer out = memepp::import_from_dll<memepp::buffer>(stk, MMSTR__OBJ_SIZE);
        MemeBufferStack_unInit(&stk, MMSTR__OBJ_SIZE);
        benchmark::DoNotOptimize(out.size());
    }
}
BENCHMARK(Dll_Roundtrip_Buffer_Const_Long)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Dll_Roundtrip_Buffer_Shared(benchmark::State& state) {
    memepp::buffer src = make_buf(400);
    src = src.to_shared_storage();
    for (auto _ : state) {
        mmbufstk_t stk = memepp::export_into_dll<mmbufstk_t>(src, MMSTR__OBJ_SIZE);
        memepp::buffer out = memepp::import_from_dll<memepp::buffer>(stk, MMSTR__OBJ_SIZE);
        MemeBufferStack_unInit(&stk, MMSTR__OBJ_SIZE);
        benchmark::DoNotOptimize(out.size());
    }
}
BENCHMARK(Dll_Roundtrip_Buffer_Shared)->Iterations(MMBM_ITERATION_COUNT / 10);

// ---------------------------------------------------------------------------
// std::string ↔ DLL boundary (to show that std::string has no such bridge)
// ---------------------------------------------------------------------------
static void Std_Direct_Copy_Short(benchmark::State& state) {
    std::string src(kShort);
    for (auto _ : state) {
        std::string out(src);
        benchmark::DoNotOptimize(out.data());
    }
}
BENCHMARK(Std_Direct_Copy_Short)->Iterations(MMBM_ITERATION_COUNT);

static void Std_Direct_Copy_Long(benchmark::State& state) {
    std::string src(kLong);
    for (auto _ : state) {
        std::string out(src);
        benchmark::DoNotOptimize(out.data());
    }
}
BENCHMARK(Std_Direct_Copy_Long)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Std_Direct_Move(benchmark::State& state) {
    for (auto _ : state) {
        std::string src(kLong);
        std::string out(std::move(src));
        benchmark::DoNotOptimize(out.data());
    }
}
BENCHMARK(Std_Direct_Move)->Iterations(MMBM_ITERATION_COUNT / 10);

} // namespace
