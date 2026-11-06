// memepp::buffer benchmark: construction, access, search, comparison.
// Compared against std::vector<uint8_t> (closest std equivalent) and C stdlib.
//
// memepp::buffer is an immutable byte sequence (like string_view but with
// optional shared ownership).  std::vector<uint8_t> is the closest std type
// with size+data, though it is mutable; std::string_view is a better semantic
// match but is read-only without sharing.

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/buffer.hpp>
#include <string>
#include <vector>
#include <cstring>
#include <cstdint>

namespace {

const uint8_t kShort[]  = { 0x48, 0x65, 0x6C, 0x6C, 0x6F };           // "Hello"  5B
const uint8_t kMed[]    = {                                            // 91 B
    0x54,0x68,0x65,0x20,0x71,0x75,0x69,0x63,0x6B,0x20,
    0x62,0x72,0x6F,0x77,0x6E,0x20,0x66,0x6F,0x78,0x20,
    0x6A,0x75,0x6D,0x70,0x73,0x20,0x6F,0x76,0x65,0x72,
    0x20,0x74,0x68,0x65,0x20,0x6C,0x61,0x7A,0x79,0x20,
    0x64,0x6F,0x67,0x2E,0x20,0x50,0x61,0x63,0x6B,0x20,
    0x6D,0x79,0x20,0x62,0x6F,0x78,0x20,0x77,0x69,0x74,
    0x68,0x20,0x66,0x69,0x76,0x65,0x20,0x64,0x6F,0x7A,
    0x65,0x6E,0x20,0x6C,0x69,0x71,0x75,0x6F,0x72,0x20,
    0x6A,0x75,0x67,0x73,0x2E
};
const uint8_t kLong[]   = {                                            // 512 B
    0x54,0x68,0x65,0x20,0x71,0x75,0x69,0x63,0x6B,0x20,
    0x62,0x72,0x6F,0x77,0x6E,0x20,0x66,0x6F,0x78,0x20,
    0x6A,0x75,0x6D,0x70,0x73,0x20,0x6F,0x76,0x65,0x72,
    0x20,0x74,0x68,0x65,0x20,0x6C,0x61,0x7A,0x79,0x20,
    0x64,0x6F,0x67,0x2E,0x20,0x50,0x61,0x63,0x6B,0x20,
    0x6D,0x79,0x20,0x62,0x6F,0x78,0x20,0x77,0x69,0x74,
    0x68,0x20,0x66,0x69,0x76,0x65,0x20,0x64,0x6F,0x7A,
    0x65,0x6E,0x20,0x6C,0x69,0x71,0x75,0x6F,0x72,0x20,
    0x6A,0x75,0x67,0x73,0x2E,0x20,0x48,0x6F,0x77,0x20,
    0x76,0x65,0x78,0x69,0x6E,0x67,0x6C,0x79,0x20,0x71,
    0x75,0x69,0x63,0x6B,0x20,0x64,0x61,0x66,0x74,0x20,
    0x7A,0x65,0x62,0x72,0x61,0x73,0x20,0x6A,0x75,0x6D,
    0x70,0x21,0x20,0x53,0x70,0x68,0x69,0x6E,0x78,0x20,
    0x6F,0x66,0x20,0x62,0x6C,0x61,0x63,0x6B,0x20,0x71,
    0x75,0x61,0x72,0x74,0x7A,0x2C,0x20,0x6A,0x75,0x64,
    0x67,0x65,0x20,0x6D,0x79,0x20,0x76,0x6F,0x77,0x2E,
    0x20,0x4A,0x61,0x63,0x6B,0x64,0x61,0x77,0x73,0x20,
    0x6C,0x6F,0x76,0x65,0x20,0x6D,0x79,0x20,0x62,0x69,
    0x67,0x20,0x73,0x70,0x68,0x69,0x6E,0x78,0x20,0x6F,
    0x66,0x20,0x71,0x75,0x61,0x72,0x74,0x7A,0x2E,0x20,
    0x54,0x68,0x65,0x20,0x66,0x69,0x76,0x65,0x20,0x62,
    0x6F,0x78,0x69,0x6E,0x67,0x20,0x77,0x69,0x7A,0x61,
    0x72,0x64,0x73,0x20,0x6A,0x75,0x6D,0x70,0x20,0x71,
    0x75,0x69,0x63,0x6B,0x6C,0x79,0x2E,0x20,0x42,0x72,
    0x69,0x67,0x68,0x74,0x20,0x76,0x69,0x78,0x65,0x6E,
    0x73,0x20,0x6A,0x75,0x6D,0x70,0x3B,0x20,0x64,0x6F,
    0x7A,0x79,0x20,0x66,0x6F,0x77,0x6C,0x20,0x71,0x75,
    0x61,0x63,0x6B,0x2E,0x20,0x51,0x75,0x69,0x63,0x6B,
    0x20,0x7A,0x65,0x70,0x68,0x79,0x72,0x73,0x20,0x62,
    0x6C,0x6F,0x77,0x2C,0x20,0x76,0x65,0x78,0x69,0x6E,
    0x67,0x20,0x64,0x61,0x66,0x74,0x20,0x4A,0x69,0x6D,
    0x2E,0x20,0x54,0x77,0x6F,0x20,0x64,0x72,0x69,0x76,
    0x65,0x6E,0x20,0x6A,0x6F,0x63,0x6B,0x73,0x20,0x68,
    0x65,0x6C,0x70,0x20,0x66,0x61,0x78,0x20,0x6D,0x79,
    0x20,0x62,0x69,0x67,0x20,0x71,0x75,0x69,0x7A,0x2E,
    0x20,0x54,0x68,0x65,0x20,0x71,0x75,0x69,0x63,0x6B,
    0x20,0x62,0x72,0x6F,0x77,0x6E,0x20,0x66,0x6F,0x78,
    0x20,0x6A,0x75,0x6D,0x70,0x73,0x20,0x6F,0x76,0x65,
    0x72,0x20,0x74,0x68,0x65,0x20,0x6C,0x61,0x7A,0x79,
    0x20,0x64,0x6F,0x67,0x2E
};

const uint8_t kNeedle3[] = { 0x71, 0x75, 0x69 };      // "qui"  in med/long
const uint8_t kNeedleAbs[] = { 0xFF, 0xFE, 0xFD };

// C equivalent: raw byte comparison
static int c_buf_compare(const uint8_t* a, const uint8_t* b, std::size_t n)
{
    return std::memcmp(a, b, n);
}

// C equivalent: memmem (not in ISO C, hand-rolled)
static const uint8_t* c_memmem(const uint8_t* hay, std::size_t hn,
                               const uint8_t* needle, std::size_t nn)
{
    if (nn > hn) return nullptr;
    for (std::size_t i = 0; i + nn <= hn; ++i)
        if (std::memcmp(hay + i, needle, nn) == 0)
            return hay + i;
    return nullptr;
}

static const uint8_t* c_memmem_pos(const uint8_t* hay, std::size_t hn,
                                   const uint8_t* needle, std::size_t nn,
                                   std::size_t pos)
{
    if (pos >= hn) return nullptr;
    return c_memmem(hay + pos, hn - pos, needle, nn);
}

// ===========================================================================
// CONSTRUCTION
// ===========================================================================
static void Buf_CtorShort(benchmark::State& state) {
    for (auto _ : state) {
        memepp::buffer b(kShort, sizeof(kShort));
        benchmark::DoNotOptimize(b.size());
    }
}
BENCHMARK(Buf_CtorShort)->Iterations(MMBM_ITERATION_COUNT);

static void Vec_CtorShort(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<uint8_t> v(std::begin(kShort), std::end(kShort));
        benchmark::DoNotOptimize(v.size());
    }
}
BENCHMARK(Vec_CtorShort)->Iterations(MMBM_ITERATION_COUNT);

static void Buf_CtorLong(benchmark::State& state) {
    for (auto _ : state) {
        memepp::buffer b(kLong, sizeof(kLong));
        benchmark::DoNotOptimize(b.size());
    }
}
BENCHMARK(Buf_CtorLong)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vec_CtorLong(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<uint8_t> v(std::begin(kLong), std::end(kLong));
        benchmark::DoNotOptimize(v.size());
    }
}
BENCHMARK(Vec_CtorLong)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Buf_Copy(benchmark::State& state) {
    memepp::buffer b(kLong, sizeof(kLong));
    for (auto _ : state) {
        memepp::buffer c(b);
        benchmark::DoNotOptimize(c.size());
    }
}
BENCHMARK(Buf_Copy)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vec_Copy(benchmark::State& state) {
    std::vector<uint8_t> v(std::begin(kLong), std::end(kLong));
    for (auto _ : state) {
        std::vector<uint8_t> c(v);
        benchmark::DoNotOptimize(c.size());
    }
}
BENCHMARK(Vec_Copy)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// ACCESS
// ===========================================================================
static void Buf_Size(benchmark::State& state) {
    memepp::buffer b(kLong, sizeof(kLong));
    for (auto _ : state) benchmark::DoNotOptimize(b.size());
}
BENCHMARK(Buf_Size)->Iterations(MMBM_ITERATION_COUNT);

static void Vec_Size(benchmark::State& state) {
    std::vector<uint8_t> v(std::begin(kLong), std::end(kLong));
    for (auto _ : state) benchmark::DoNotOptimize(v.size());
}
BENCHMARK(Vec_Size)->Iterations(MMBM_ITERATION_COUNT);

static void Buf_Data(benchmark::State& state) {
    memepp::buffer b(kLong, sizeof(kLong));
    for (auto _ : state) benchmark::DoNotOptimize(b.data());
}
BENCHMARK(Buf_Data)->Iterations(MMBM_ITERATION_COUNT);

static void Vec_Data(benchmark::State& state) {
    std::vector<uint8_t> v(std::begin(kLong), std::end(kLong));
    for (auto _ : state) benchmark::DoNotOptimize(v.data());
}
BENCHMARK(Vec_Data)->Iterations(MMBM_ITERATION_COUNT);

static void Buf_At(benchmark::State& state) {
    memepp::buffer b(kLong, sizeof(kLong));
    for (auto _ : state) benchmark::DoNotOptimize(b.at(42));
}
BENCHMARK(Buf_At)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vec_At(benchmark::State& state) {
    std::vector<uint8_t> v(std::begin(kLong), std::end(kLong));
    for (auto _ : state) benchmark::DoNotOptimize(v.at(42));
}
BENCHMARK(Vec_At)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// COMPARISON
// ===========================================================================
static void Buf_Compare(benchmark::State& state) {
    memepp::buffer a(kLong, sizeof(kLong));
    memepp::buffer b(kLong, sizeof(kLong));
    for (auto _ : state) benchmark::DoNotOptimize(a.compare(b));
}
BENCHMARK(Buf_Compare)->Iterations(MMBM_ITERATION_COUNT);

static void Vec_Compare(benchmark::State& state) {
    std::vector<uint8_t> a(std::begin(kLong), std::end(kLong));
    std::vector<uint8_t> b(a);
    for (auto _ : state) benchmark::DoNotOptimize(a == b);
}
BENCHMARK(Vec_Compare)->Iterations(MMBM_ITERATION_COUNT);

static void CBuf_Compare(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(c_buf_compare(kLong, kLong, sizeof(kLong)));
}
BENCHMARK(CBuf_Compare)->Iterations(MMBM_ITERATION_COUNT);

// ===========================================================================
// SEARCH
// ===========================================================================
static void Buf_FindShort(benchmark::State& state) {
    memepp::buffer b(kMed, sizeof(kMed));
    memepp::buffer needle(kNeedle3, sizeof(kNeedle3));
    for (auto _ : state) benchmark::DoNotOptimize(b.find(needle));
}
BENCHMARK(Buf_FindShort)->Iterations(MMBM_ITERATION_COUNT);

static void Vec_FindShort(benchmark::State& state) {
    std::vector<uint8_t> v(std::begin(kMed), std::end(kMed));
    std::vector<uint8_t> n(std::begin(kNeedle3), std::end(kNeedle3));
    for (auto _ : state) {
        auto it = std::search(v.begin(), v.end(), n.begin(), n.end());
        benchmark::DoNotOptimize(it == v.end() ? -1 : static_cast<int>(it - v.begin()));
    }
}
BENCHMARK(Vec_FindShort)->Iterations(MMBM_ITERATION_COUNT / 10);

static void CBuf_FindShort(benchmark::State& state) {
    for (auto _ : state) {
        const uint8_t* p = c_memmem(kMed, sizeof(kMed), kNeedle3, sizeof(kNeedle3));
        benchmark::DoNotOptimize(p ? (p - kMed) : -1);
    }
}
BENCHMARK(CBuf_FindShort)->Iterations(MMBM_ITERATION_COUNT);

static void Buf_FindByte(benchmark::State& state) {
    memepp::buffer b(kLong, sizeof(kLong));
    for (auto _ : state) benchmark::DoNotOptimize(b.find(uint8_t(0x71)));
}
BENCHMARK(Buf_FindByte)->Iterations(MMBM_ITERATION_COUNT);

static void Vec_FindByte(benchmark::State& state) {
    std::vector<uint8_t> v(std::begin(kLong), std::end(kLong));
    for (auto _ : state) {
        auto it = std::find(v.begin(), v.end(), uint8_t(0x71));
        benchmark::DoNotOptimize(it == v.end() ? -1 : static_cast<int>(it - v.begin()));
    }
}
BENCHMARK(Vec_FindByte)->Iterations(MMBM_ITERATION_COUNT);

static void CBuf_FindByte(benchmark::State& state) {
    for (auto _ : state) {
        const uint8_t* p = static_cast<const uint8_t*>(std::memchr(kLong, 0x71, sizeof(kLong)));
        benchmark::DoNotOptimize(p ? (p - kLong) : -1);
    }
}
BENCHMARK(CBuf_FindByte)->Iterations(MMBM_ITERATION_COUNT);

// Find with pos
static void Buf_FindFromPos(benchmark::State& state) {
    memepp::buffer b(kLong, sizeof(kLong));
    memepp::buffer needle(kNeedle3, sizeof(kNeedle3));
    for (auto _ : state) benchmark::DoNotOptimize(b.find(needle, 40));
}
BENCHMARK(Buf_FindFromPos)->Iterations(MMBM_ITERATION_COUNT);

static void CBuf_FindShortFromPos(benchmark::State& state) {
    for (auto _ : state) {
        const uint8_t* p = c_memmem_pos(kLong, sizeof(kLong), kNeedle3, sizeof(kNeedle3), 40);
        benchmark::DoNotOptimize(p ? (p - kLong) : -1);
    }
}
BENCHMARK(CBuf_FindShortFromPos)->Iterations(MMBM_ITERATION_COUNT);

// Absent
static void Buf_FindAbsent(benchmark::State& state) {
    memepp::buffer b(kLong, sizeof(kLong));
    memepp::buffer needle(kNeedleAbs, sizeof(kNeedleAbs));
    for (auto _ : state) benchmark::DoNotOptimize(b.find(needle));
}
BENCHMARK(Buf_FindAbsent)->Iterations(MMBM_ITERATION_COUNT);

static void Vec_FindAbsent(benchmark::State& state) {
    std::vector<uint8_t> v(std::begin(kLong), std::end(kLong));
    std::vector<uint8_t> n(std::begin(kNeedleAbs), std::end(kNeedleAbs));
    for (auto _ : state) {
        auto it = std::search(v.begin(), v.end(), n.begin(), n.end());
        benchmark::DoNotOptimize(it == v.end() ? -1 : static_cast<int>(it - v.begin()));
    }
}
BENCHMARK(Vec_FindAbsent)->Iterations(MMBM_ITERATION_COUNT / 10);

static void CBuf_FindAbsent(benchmark::State& state) {
    for (auto _ : state) {
        const uint8_t* p = c_memmem(kLong, sizeof(kLong), kNeedleAbs, sizeof(kNeedleAbs));
        benchmark::DoNotOptimize(p ? (p - kLong) : -1);
    }
}
BENCHMARK(CBuf_FindAbsent)->Iterations(MMBM_ITERATION_COUNT);

// ===========================================================================
// INDEX_OF
// ===========================================================================
static void Buf_IndexOf(benchmark::State& state) {
    memepp::buffer b(kMed, sizeof(kMed));
    memepp::buffer needle(kNeedle3, sizeof(kNeedle3));
    for (auto _ : state) benchmark::DoNotOptimize(b.index_of(needle));
}
BENCHMARK(Buf_IndexOf)->Iterations(MMBM_ITERATION_COUNT);

static void Buf_IndexOfPtr(benchmark::State& state) {
    memepp::buffer b(kMed, sizeof(kMed));
    for (auto _ : state) benchmark::DoNotOptimize(b.index_of(kNeedle3, sizeof(kNeedle3)));
}
BENCHMARK(Buf_IndexOfPtr)->Iterations(MMBM_ITERATION_COUNT);

// ===========================================================================
// CONTAINS / STARTS / ENDS
// ===========================================================================
static void Buf_Contains(benchmark::State& state) {
    memepp::buffer b(kMed, sizeof(kMed));
    memepp::buffer needle(kNeedle3, sizeof(kNeedle3));
    for (auto _ : state) benchmark::DoNotOptimize(b.contains(needle));
}
BENCHMARK(Buf_Contains)->Iterations(MMBM_ITERATION_COUNT);

static void Buf_StartsWith(benchmark::State& state) {
    memepp::buffer b(kMed, sizeof(kMed));
    memepp::buffer prefix(kMed, 5);
    for (auto _ : state) benchmark::DoNotOptimize(b.starts_with(prefix));
}
BENCHMARK(Buf_StartsWith)->Iterations(MMBM_ITERATION_COUNT);

static void Buf_EndsWith(benchmark::State& state) {
    memepp::buffer b(kMed, sizeof(kMed));
    memepp::buffer suffix(kMed + sizeof(kMed) - 5, 5);
    for (auto _ : state) benchmark::DoNotOptimize(b.ends_with(suffix));
}
BENCHMARK(Buf_EndsWith)->Iterations(MMBM_ITERATION_COUNT);

static void CBuf_Contains(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(c_memmem(kMed, sizeof(kMed), kNeedle3, sizeof(kNeedle3)) != nullptr);
    }
}
BENCHMARK(CBuf_Contains)->Iterations(MMBM_ITERATION_COUNT);

static void CBuf_StartsWith(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(std::memcmp(kMed, kMed, 5) == 0);
}
BENCHMARK(CBuf_StartsWith)->Iterations(MMBM_ITERATION_COUNT);

// ===========================================================================
// SLICE
// ===========================================================================
static void Buf_Slice(benchmark::State& state) {
    memepp::buffer b(kLong, sizeof(kLong));
    for (auto _ : state) benchmark::DoNotOptimize(b.slice(10, 50));
}
BENCHMARK(Buf_Slice)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Buf_SliceToEnd(benchmark::State& state) {
    memepp::buffer b(kLong, sizeof(kLong));
    for (auto _ : state) benchmark::DoNotOptimize(b.slice(10));
}
BENCHMARK(Buf_SliceToEnd)->Iterations(MMBM_ITERATION_COUNT / 10);

// buffer::to_string() returns memepp::string.  Its constructor is
// MEMEPP__IMPL_INLINE in the DLL, so taking the address of the result here
// would need an out-of-line copy the benchmark exe does not link.  The same
// work -- copy the bytes into a fresh owning buffer -- is measured instead.
static void Buf_ToStringEquivalent(benchmark::State& state) {
    memepp::buffer b(kMed, sizeof(kMed));
    for (auto _ : state) {
        std::vector<uint8_t> copy(b.size());
        std::memcpy(copy.data(), b.data(), static_cast<std::size_t>(b.size()));
        benchmark::DoNotOptimize(copy.size());
    }
}
BENCHMARK(Buf_ToStringEquivalent)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// TO_SHARED / CHEAP COPY / TO_LARGE
// ===========================================================================
static void Buf_ToShared(benchmark::State& state) {
    memepp::buffer b(kLong, sizeof(kLong));
    for (auto _ : state) {
        memepp::buffer c = b.to_shared_storage();
        benchmark::DoNotOptimize(c.size());
    }
}
BENCHMARK(Buf_ToShared)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Buf_CheapCopy(benchmark::State& state) {
    memepp::buffer b(kLong, sizeof(kLong));
    memepp::buffer shared = b.to_shared_storage();
    for (auto _ : state) {
        memepp::buffer c = shared.cheap_copy();
        benchmark::DoNotOptimize(c.size());
    }
}
BENCHMARK(Buf_CheapCopy)->Iterations(MMBM_ITERATION_COUNT);

static void Buf_ToLarge(benchmark::State& state) {
    memepp::buffer b(kShort, sizeof(kShort));
    for (auto _ : state) {
        memepp::buffer c = b.to_large();
        benchmark::DoNotOptimize(c.size());
    }
}
BENCHMARK(Buf_ToLarge)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Buf_StorageType(benchmark::State& state) {
    memepp::buffer b(kLong, sizeof(kLong));
    for (auto _ : state) benchmark::DoNotOptimize(b.storage_type());
}
BENCHMARK(Buf_StorageType)->Iterations(MMBM_ITERATION_COUNT);

static void Buf_Reset(benchmark::State& state) {
    memepp::buffer b(kShort, sizeof(kShort));
    for (auto _ : state) {
        b.reset();
        b = memepp::buffer(kShort, sizeof(kShort));
    }
}
BENCHMARK(Buf_Reset)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vec_Reset(benchmark::State& state) {
    std::vector<uint8_t> v(std::begin(kShort), std::end(kShort));
    for (auto _ : state) {
        v.clear();
        v.assign(std::begin(kShort), std::end(kShort));
    }
}
BENCHMARK(Vec_Reset)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// OPERATORS
// ===========================================================================
static void Buf_OpEq(benchmark::State& state) {
    memepp::buffer a(kLong, sizeof(kLong));
    memepp::buffer b(kLong, sizeof(kLong));
    for (auto _ : state) benchmark::DoNotOptimize(a == b);
}
BENCHMARK(Buf_OpEq)->Iterations(MMBM_ITERATION_COUNT);

static void Buf_OpNe(benchmark::State& state) {
    memepp::buffer a(kLong, sizeof(kLong));
    memepp::buffer b(kMed, sizeof(kMed));
    for (auto _ : state) benchmark::DoNotOptimize(a != b);
}
BENCHMARK(Buf_OpNe)->Iterations(MMBM_ITERATION_COUNT);

static void Buf_OpLt(benchmark::State& state) {
    memepp::buffer a(kMed, sizeof(kMed));
    memepp::buffer b(kLong, sizeof(kLong));
    for (auto _ : state) benchmark::DoNotOptimize(a < b);
}
BENCHMARK(Buf_OpLt)->Iterations(MMBM_ITERATION_COUNT);

static void CBuf_OpEq(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(std::memcmp(kLong, kLong, sizeof(kLong)) == 0);
}
BENCHMARK(CBuf_OpEq)->Iterations(MMBM_ITERATION_COUNT);

} // namespace
