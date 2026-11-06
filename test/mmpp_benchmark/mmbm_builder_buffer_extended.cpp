// Batch 5: string_builder and variable_buffer extended API.
//
// Everything below was verified against the headers before being called:
//   string_builder_def.hpp : prepend/release/generate/part_count/part_capacity/
//                            reserve_parts/clear/operator+=
//   variable_buffer_def.hpp: prepend/push_front/pop_front/slice/release/
//                            resize_and_overwrite/count
//
// std counterparts are std::string where the operation exists there; the
// builder's part_* accessors and variable_buffer::release have no std
// equivalent and only get a memepp column.

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <memepp/string_builder.hpp>
#include <memepp/variable_buffer.hpp>
#include <memepp/buffer.hpp>

#include <string>
#include <vector>
#include <cstring>

namespace {

const char* const kShort = "Hello!";
const char* const kMed   =
    "The quick brown fox jumps over the lazy dog. "
    "Pack my box with five dozen liquor jugs.";
const char* const kParts[4] = { "alpha", "bravo", "charlie", "delta" };

using vb_t = memepp::variable_buffer;

inline vb_t::const_pointer vb_p(const char* s)
{
    return reinterpret_cast<vb_t::const_pointer>(s);
}
inline vb_t make_vb(const char* s)
{
    return vb_t(vb_p(s), static_cast<vb_t::size_type>(std::strlen(s)));
}

// ===========================================================================
// string_builder::prepend
// ===========================================================================
static void Sb_PrependChained(benchmark::State& state) {
    for (auto _ : state) {
        memepp::string_builder sb;
        for (int i = 3; i >= 0; --i) sb.prepend(kParts[i]);
        benchmark::DoNotOptimize(sb.size());
    }
}
BENCHMARK(Sb_PrependChained)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_PrependChained(benchmark::State& state) {
    for (auto _ : state) {
        std::string s;
        for (int i = 3; i >= 0; --i) s.insert(0, kParts[i]);
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_PrependChained)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Sb_PrependOnce(benchmark::State& state) {
    for (auto _ : state) {
        memepp::string_builder sb;
        sb += memepp::string(kMed);
        sb.prepend(kShort);
        benchmark::DoNotOptimize(sb.size());
    }
}
BENCHMARK(Sb_PrependOnce)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_PrependOnce(benchmark::State& state) {
    for (auto _ : state) {
        std::string s(kMed);
        s.insert(0, kShort);
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_PrependOnce)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// string_builder::generate / release
// ===========================================================================
static void Sb_Generate(benchmark::State& state) {
    memepp::string_builder proto;
    proto += kMed;
    proto += " tail";
    for (auto _ : state) {
        memepp::string s = proto.generate();   // const: builder reusable
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(Sb_Generate)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Sb_Release(benchmark::State& state) {
    for (auto _ : state) {
        memepp::string_builder sb;
        sb += kMed;
        memepp::string s = sb.release();       // consumes the builder
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(Sb_Release)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_FlattenCopy(benchmark::State& state) {
    std::vector<std::string> parts = { kMed, " tail" };
    for (auto _ : state) {
        std::string s;
        for (const auto& p : parts) s += p;
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_FlattenCopy)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// string_builder part introspection
// ===========================================================================
static void Sb_PartCount(benchmark::State& state) {
    memepp::string_builder sb;
    for (int i = 0; i < 4; ++i) sb += kParts[i];
    for (auto _ : state) benchmark::DoNotOptimize(sb.part_count());
}
BENCHMARK(Sb_PartCount)->Iterations(MMBM_ITERATION_COUNT);

static void Sb_PartCapacity(benchmark::State& state) {
    memepp::string_builder sb;
    for (int i = 0; i < 4; ++i) sb += kParts[i];
    for (auto _ : state) benchmark::DoNotOptimize(sb.part_capacity());
}
BENCHMARK(Sb_PartCapacity)->Iterations(MMBM_ITERATION_COUNT);

static void Sb_ReserveParts(benchmark::State& state) {
    for (auto _ : state) {
        memepp::string_builder sb;
        sb.reserve_parts(16);
        for (int i = 0; i < 4; ++i) sb += kParts[i];
        benchmark::DoNotOptimize(sb.part_capacity());
    }
}
BENCHMARK(Sb_ReserveParts)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_Reserve(benchmark::State& state) {
    for (auto _ : state) {
        std::string s;
        s.reserve(16 * 5);
        for (int i = 0; i < 4; ++i) s += kParts[i];
        benchmark::DoNotOptimize(s.capacity());
    }
}
BENCHMARK(StdStr_Reserve)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// variable_buffer::prepend / push_front / pop_front
// ===========================================================================
static void Vb_Prepend(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = make_vb(kMed);
        vb.prepend(vb_p(kShort), static_cast<vb_t::size_type>(std::strlen(kShort)));
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(Vb_Prepend)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_InsertFront(benchmark::State& state) {
    for (auto _ : state) {
        std::string s(kMed);
        s.insert(0, kShort);
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_InsertFront)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vb_PrependChained(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = make_vb("");
        for (int i = 3; i >= 0; --i)
            vb.prepend(vb_p(kParts[i]),
                       static_cast<vb_t::size_type>(std::strlen(kParts[i])));
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(Vb_PrependChained)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vb_PushFront(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = make_vb(kShort);
        for (int i = 0; i < 16; ++i) vb.push_front(static_cast<vb_t::value_type>('x'));
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(Vb_PushFront)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_PushFrontEquiv(benchmark::State& state) {
    for (auto _ : state) {
        std::string s(kShort);
        for (int i = 0; i < 16; ++i) s.insert(0, 1, 'x');
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_PushFrontEquiv)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vb_PopFront(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = make_vb(kMed);
        for (int i = 0; i < 16; ++i) vb.pop_front();
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(Vb_PopFront)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_PopFrontEquiv(benchmark::State& state) {
    for (auto _ : state) {
        std::string s(kMed);
        for (int i = 0; i < 16; ++i) s.erase(0, 1);
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_PopFrontEquiv)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// variable_buffer::slice / resize_and_overwrite / release / count
// ===========================================================================
static void Vb_Slice(benchmark::State& state) {
    vb_t vb = make_vb(kMed);
    for (auto _ : state) {
        vb_t s = vb.slice(10, 40);
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(Vb_Slice)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_SubstrForSlice(benchmark::State& state) {
    std::string s(kMed);
    for (auto _ : state) benchmark::DoNotOptimize(s.substr(10, 40).size());
}
BENCHMARK(StdStr_SubstrForSlice)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vb_ResizeAndOverwrite(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = make_vb(kShort);
        vb.resize_and_overwrite(256);
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(Vb_ResizeAndOverwrite)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_ResizeForOverwrite(benchmark::State& state) {
    for (auto _ : state) {
        std::string s(kShort);
        s.resize(256);
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_ResizeForOverwrite)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vb_ReleaseToBuffer(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = make_vb(kMed);
        memepp::buffer b;
        vb.release(b);
        benchmark::DoNotOptimize(b.size());
    }
}
BENCHMARK(Vb_ReleaseToBuffer)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vb_ReleaseToString(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = make_vb(kMed);
        memepp::string s;
        vb.release(s);
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(Vb_ReleaseToString)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vb_CountValue(benchmark::State& state) {
    vb_t vb = make_vb(kMed);
    for (auto _ : state)
        benchmark::DoNotOptimize(vb.count(static_cast<vb_t::value_type>('o')));
}
BENCHMARK(Vb_CountValue)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_CountValue(benchmark::State& state) {
    std::string s(kMed);
    for (auto _ : state)
        benchmark::DoNotOptimize(static_cast<long>(std::count(s.begin(), s.end(), 'o')));
}
BENCHMARK(StdStr_CountValue)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vb_CountSub(benchmark::State& state) {
    vb_t vb = make_vb(kMed);
    for (auto _ : state)
        benchmark::DoNotOptimize(vb.count(vb_p("the"),
            static_cast<vb_t::size_type>(std::strlen("the"))));
}
BENCHMARK(Vb_CountSub)->Iterations(MMBM_ITERATION_COUNT / 10);

} // namespace
