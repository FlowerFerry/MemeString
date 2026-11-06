// Remaining zero-coverage classes: buffer_span, multi_buffer_view,
// const_rune_iterator, errc, and convert::self.
//
// API surface read from the headers before use:
//   buffer_span_def.hpp  : size/data/slice/first/last/find/rfind/contains/
//                          starts_with/ends_with/compare/to_buffer/to_string/
//                          remove_prefix/remove_suffix
//   multi_buffer_view.hpp: push_back/push_front/pop_back/pop_front/append/
//                          prepend/insert/erase/slice/size/at/view_count/
//                          views_capacity/reserve_views/to_buffer/compact/split_at
//   rune_iterator.hpp    : construct/++/--/*/to_rune/==
//   errc.hpp             : errc()/set_errc/get_errc (thread_local int)
//   convert/self.hpp     : from/view/into for string, string_view, mmstrstk_t
//
// std counterparts:
//   buffer_span         -> std::span<const uint8_t> is C++20; this build is
//                          C++17, so only a raw-pointer loop is comparable.
//   multi_buffer_view   -> no std equivalent (it is a scatter/gather view).
//   const_rune_iterator -> no std equivalent (UTF-8 aware).
//   errc                -> no equivalent (thread-local error slot).

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/buffer_span.hpp>
#include <memepp/multi_buffer_view.hpp>
#include <memepp/buffer_view.hpp>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/string_span.hpp>
#include <memepp/buffer.hpp>
#include <memepp/variable_buffer.hpp>
#include <memepp/rune.hpp>
#include <memepp/rune_iterator.hpp>
#include <memepp/errc.hpp>
#include <memepp/convert/self.hpp>

#include <string>
#include <string_view>
#include <vector>
#include <cstring>

namespace {

const char* const kAscii =
    "The quick brown fox jumps over the lazy dog. "
    "Pack my box with five dozen liquor jugs.";
const char* const kUtf8 =
    "Hello \xE4\xB8\xAD\xE6\x96\x87 world \xE4\xB8\xAD\xE6\x96\x87 end";

inline const uint8_t* u8(const char* s)
{
    return reinterpret_cast<const uint8_t*>(s);
}
inline memepp::buffer_span span_of(const char* s)
{
    return memepp::buffer_span(u8(s), static_cast<memepp::buffer_span::size_type>(std::strlen(s)));
}

// ===========================================================================
// buffer_span
// ===========================================================================
static void BSpan_Ctor(benchmark::State& state) {
    for (auto _ : state)
        benchmark::DoNotOptimize(span_of(kAscii).size());
}
BENCHMARK(BSpan_Ctor)->Iterations(MMBM_ITERATION_COUNT);

static void C_PtrPairCtor(benchmark::State& state) {
    for (auto _ : state) {
        const uint8_t* b = u8(kAscii);
        const uint8_t* e = b + std::strlen(kAscii);
        benchmark::DoNotOptimize(e - b);
    }
}
BENCHMARK(C_PtrPairCtor)->Iterations(MMBM_ITERATION_COUNT);

static void BSpan_Size(benchmark::State& state) {
    auto sp = span_of(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(sp.size());
}
BENCHMARK(BSpan_Size)->Iterations(MMBM_ITERATION_COUNT);

static void BSpan_At(benchmark::State& state) {
    auto sp = span_of(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(sp[42]);
}
BENCHMARK(BSpan_At)->Iterations(MMBM_ITERATION_COUNT);

static void BSpan_FrontBack(benchmark::State& state) {
    auto sp = span_of(kAscii);
    for (auto _ : state) {
        benchmark::DoNotOptimize(sp.front());
        benchmark::DoNotOptimize(sp.back());
    }
}
BENCHMARK(BSpan_FrontBack)->Iterations(MMBM_ITERATION_COUNT);

static void BSpan_FindSub(benchmark::State& state) {
    auto sp = span_of(kAscii);
    auto nd = span_of("brown");
    for (auto _ : state) benchmark::DoNotOptimize(sp.find(nd));
}
BENCHMARK(BSpan_FindSub)->Iterations(MMBM_ITERATION_COUNT);

static void C_MemmemForBSpan(benchmark::State& state) {
    const uint8_t* h = u8(kAscii);
    const size_t hn = std::strlen(kAscii);
    for (auto _ : state) {
        const uint8_t* p = nullptr;
        for (size_t i = 0; i + 5 <= hn; ++i)
            if (std::memcmp(h + i, "brown", 5) == 0) { p = h + i; break; }
        benchmark::DoNotOptimize(p ? (p - h) : -1);
    }
}
BENCHMARK(C_MemmemForBSpan)->Iterations(MMBM_ITERATION_COUNT);

static void BSpan_FindByte(benchmark::State& state) {
    auto sp = span_of(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(sp.find(static_cast<uint8_t>('q')));
}
BENCHMARK(BSpan_FindByte)->Iterations(MMBM_ITERATION_COUNT);

static void C_MemchrForBSpan(benchmark::State& state) {
    const uint8_t* h = u8(kAscii);
    const size_t hn = std::strlen(kAscii);
    for (auto _ : state) {
        const uint8_t* p = static_cast<const uint8_t*>(std::memchr(h, 'q', hn));
        benchmark::DoNotOptimize(p ? (p - h) : -1);
    }
}
BENCHMARK(C_MemchrForBSpan)->Iterations(MMBM_ITERATION_COUNT);

static void BSpan_RfindSub(benchmark::State& state) {
    auto sp = span_of(kAscii);
    auto nd = span_of("the");
    for (auto _ : state) benchmark::DoNotOptimize(sp.rfind(nd));
}
BENCHMARK(BSpan_RfindSub)->Iterations(MMBM_ITERATION_COUNT);

static void BSpan_Contains(benchmark::State& state) {
    auto sp = span_of(kAscii);
    auto nd = span_of("fox");
    for (auto _ : state) benchmark::DoNotOptimize(sp.contains(nd));
}
BENCHMARK(BSpan_Contains)->Iterations(MMBM_ITERATION_COUNT);

static void BSpan_StartsEndsWith(benchmark::State& state) {
    auto sp = span_of(kAscii);
    auto pre = span_of("The quick");
    auto suf = span_of("liquor jugs.");
    for (auto _ : state) {
        benchmark::DoNotOptimize(sp.starts_with(pre));
        benchmark::DoNotOptimize(sp.ends_with(suf));
    }
}
BENCHMARK(BSpan_StartsEndsWith)->Iterations(MMBM_ITERATION_COUNT);

static void BSpan_Compare(benchmark::State& state) {
    auto a = span_of(kAscii);
    auto b = span_of(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(a.compare(b));
}
BENCHMARK(BSpan_Compare)->Iterations(MMBM_ITERATION_COUNT);

static void C_MemcmpForBSpan(benchmark::State& state) {
    const char* s = kAscii;
    size_t n = std::strlen(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(std::memcmp(s, s, n));
}
BENCHMARK(C_MemcmpForBSpan)->Iterations(MMBM_ITERATION_COUNT);

static void BSpan_CompareSub(benchmark::State& state) {
    auto a = span_of(kAscii);
    auto b = span_of(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(a.compare(4, 15, b));
}
BENCHMARK(BSpan_CompareSub)->Iterations(MMBM_ITERATION_COUNT);

static void BSpan_Slice(benchmark::State& state) {
    auto sp = span_of(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(sp.slice(10, 40).size());
}
BENCHMARK(BSpan_Slice)->Iterations(MMBM_ITERATION_COUNT);

static void BSpan_FirstLast(benchmark::State& state) {
    auto sp = span_of(kAscii);
    for (auto _ : state) {
        benchmark::DoNotOptimize(sp.first(20).size());
        benchmark::DoNotOptimize(sp.last(20).size());
    }
}
BENCHMARK(BSpan_FirstLast)->Iterations(MMBM_ITERATION_COUNT);

static void BSpan_RemovePrefix(benchmark::State& state) {
    for (auto _ : state) {
        auto sp = span_of(kAscii);
        sp.remove_prefix(10);
        benchmark::DoNotOptimize(sp.size());
    }
}
BENCHMARK(BSpan_RemovePrefix)->Iterations(MMBM_ITERATION_COUNT);

static void BSpan_RemoveSuffix(benchmark::State& state) {
    for (auto _ : state) {
        auto sp = span_of(kAscii);
        sp.remove_suffix(10);
        benchmark::DoNotOptimize(sp.size());
    }
}
BENCHMARK(BSpan_RemoveSuffix)->Iterations(MMBM_ITERATION_COUNT);

static void BSpan_ToBuffer(benchmark::State& state) {
    auto sp = span_of(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(sp.to_buffer().size());
}
BENCHMARK(BSpan_ToBuffer)->Iterations(MMBM_ITERATION_COUNT / 10);

static void BSpan_Iterate(benchmark::State& state) {
    auto sp = span_of(kAscii);
    for (auto _ : state) {
        std::size_t sum = 0;
        for (auto i = 0; i < sp.size(); ++i) sum += sp[i];
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BSpan_Iterate)->Iterations(MMBM_ITERATION_COUNT / 10);

static void C_PtrIterate(benchmark::State& state) {
    const uint8_t* p = u8(kAscii);
    const size_t n = std::strlen(kAscii);
    for (auto _ : state) {
        std::size_t sum = 0;
        for (size_t i = 0; i < n; ++i) sum += p[i];
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(C_PtrIterate)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// multi_buffer_view
// ===========================================================================
static void Mbv_PushBack(benchmark::State& state) {
    std::string_view v1(kAscii, 20);
    for (auto _ : state) {
        memepp::multi_buffer_view m;
        for (int i = 0; i < 4; ++i) m.push_back(v1);
        benchmark::DoNotOptimize(m.size());
    }
}
BENCHMARK(Mbv_PushBack)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Mbv_PushFront(benchmark::State& state) {
    std::string_view v1(kAscii, 20);
    for (auto _ : state) {
        memepp::multi_buffer_view m;
        for (int i = 0; i < 4; ++i) m.push_front(v1);
        benchmark::DoNotOptimize(m.size());
    }
}
BENCHMARK(Mbv_PushFront)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Mbv_PopBack(benchmark::State& state) {
    std::string_view v1(kAscii, 20);
    for (auto _ : state) {
        memepp::multi_buffer_view m;
        for (int i = 0; i < 4; ++i) m.push_back(v1);
        for (int i = 0; i < 4; ++i) m.pop_back();
        benchmark::DoNotOptimize(m.size());
    }
}
BENCHMARK(Mbv_PopBack)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Mbv_PopFront(benchmark::State& state) {
    std::string_view v1(kAscii, 20);
    for (auto _ : state) {
        memepp::multi_buffer_view m;
        for (int i = 0; i < 4; ++i) m.push_back(v1);
        for (int i = 0; i < 4; ++i) m.pop_front();
        benchmark::DoNotOptimize(m.size());
    }
}
BENCHMARK(Mbv_PopFront)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Mbv_Append(benchmark::State& state) {
    std::string_view v1(kAscii, 20);
    for (auto _ : state) {
        memepp::multi_buffer_view m;
        m.append(v1);
        benchmark::DoNotOptimize(m.size());
    }
}
BENCHMARK(Mbv_Append)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Mbv_Prepend(benchmark::State& state) {
    std::string_view v1(kAscii, 20);
    memepp::multi_buffer_view base;
    base.push_back(std::string_view(kAscii, 40));
    for (auto _ : state) {
        memepp::multi_buffer_view m(base);
        m.prepend(v1);
        benchmark::DoNotOptimize(m.size());
    }
}
BENCHMARK(Mbv_Prepend)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Mbv_Insert(benchmark::State& state) {
    std::string_view v1(kAscii, 20);
    memepp::multi_buffer_view base;
    base.push_back(std::string_view(kAscii, 40));
    for (auto _ : state) {
        memepp::multi_buffer_view m(base);
        m.insert(10, v1);
        benchmark::DoNotOptimize(m.size());
    }
}
BENCHMARK(Mbv_Insert)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Mbv_Erase(benchmark::State& state) {
    for (auto _ : state) {
        memepp::multi_buffer_view m;
        m.push_back(std::string_view(kAscii, 100));
        m.erase(10, 20);
        benchmark::DoNotOptimize(m.size());
    }
}
BENCHMARK(Mbv_Erase)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Mbv_Size(benchmark::State& state) {
    memepp::multi_buffer_view m;
    m.push_back(std::string_view(kAscii, 40));
    for (auto _ : state) benchmark::DoNotOptimize(m.size());
}
BENCHMARK(Mbv_Size)->Iterations(MMBM_ITERATION_COUNT);

static void Mbv_ViewCount(benchmark::State& state) {
    memepp::multi_buffer_view m;
    for (int i = 0; i < 4; ++i) m.push_back(std::string_view(kAscii, 10));
    for (auto _ : state) benchmark::DoNotOptimize(m.view_count());
}
BENCHMARK(Mbv_ViewCount)->Iterations(MMBM_ITERATION_COUNT);

static void Mbv_ViewsCapacity(benchmark::State& state) {
    memepp::multi_buffer_view m;
    for (int i = 0; i < 4; ++i) m.push_back(std::string_view(kAscii, 10));
    for (auto _ : state) benchmark::DoNotOptimize(m.views_capacity());
}
BENCHMARK(Mbv_ViewsCapacity)->Iterations(MMBM_ITERATION_COUNT);

static void Mbv_ReserveViews(benchmark::State& state) {
    for (auto _ : state) {
        memepp::multi_buffer_view m;
        m.reserve_views(32);
        benchmark::DoNotOptimize(m.views_capacity());
    }
}
BENCHMARK(Mbv_ReserveViews)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Mbv_At(benchmark::State& state) {
    memepp::multi_buffer_view m;
    m.push_back(std::string_view(kAscii, 100));
    for (auto _ : state) benchmark::DoNotOptimize(m.at(42));
}
BENCHMARK(Mbv_At)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Mbv_ToBuffer(benchmark::State& state) {
    memepp::multi_buffer_view m;
    for (int i = 0; i < 4; ++i) m.push_back(std::string_view(kAscii, 20));
    for (auto _ : state) benchmark::DoNotOptimize(m.to_buffer().size());
}
BENCHMARK(Mbv_ToBuffer)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Mbv_Slice(benchmark::State& state) {
    memepp::multi_buffer_view m;
    for (int i = 0; i < 4; ++i) m.push_back(std::string_view(kAscii, 20));
    for (auto _ : state) benchmark::DoNotOptimize(m.slice(10, 40).size());
}
BENCHMARK(Mbv_Slice)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Mbv_Iterate(benchmark::State& state) {
    memepp::multi_buffer_view m;
    for (int i = 0; i < 4; ++i) m.push_back(std::string_view(kAscii, 20));
    for (auto _ : state) {
        std::size_t sum = 0;
        for (auto it = m.begin(), e = m.end(); it != e; ++it) sum += *it;
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(Mbv_Iterate)->Iterations(MMBM_ITERATION_COUNT / 10);

// std baseline: the same scatter/gather as one contiguous string
static void StdStr_GatherLoop(benchmark::State& state) {
    std::string contig;
    for (int i = 0; i < 4; ++i) contig.append(kAscii, 20);
    for (auto _ : state) {
        std::size_t sum = 0;
        for (char c : contig) sum += static_cast<unsigned char>(c);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(StdStr_GatherLoop)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// const_rune_iterator
// ===========================================================================
static void RuneIter_Construct(benchmark::State& state) {
    const uint8_t* b = u8(kUtf8);
    const uint8_t* e = b + std::strlen(kUtf8);
    for (auto _ : state)
        benchmark::DoNotOptimize(memepp::const_rune_iterator(b, e).to_rune().codepoint());
}
BENCHMARK(RuneIter_Construct)->Iterations(MMBM_ITERATION_COUNT / 10);

static void RuneIter_Increment(benchmark::State& state) {
    const uint8_t* b = u8(kUtf8);
    const uint8_t* e = b + std::strlen(kUtf8);
    for (auto _ : state) {
        memepp::const_rune_iterator it(b, e);
        int n = 0;
        for (; it != memepp::const_rune_iterator(e, e) && n < 32; ++it, ++n) {}
        benchmark::DoNotOptimize(n);
    }
}
BENCHMARK(RuneIter_Increment)->Iterations(MMBM_ITERATION_COUNT / 100);

// operator* returns a byte reference, not a rune; to_rune() is the decoder.
static void RuneIter_DerefByte(benchmark::State& state) {
    const uint8_t* b = u8(kUtf8);
    const uint8_t* e = b + std::strlen(kUtf8);
    memepp::const_rune_iterator it(b, e);
    for (auto _ : state) benchmark::DoNotOptimize(*it);
}
BENCHMARK(RuneIter_DerefByte)->Iterations(MMBM_ITERATION_COUNT / 10);

static void RuneIter_ToRune(benchmark::State& state) {
    const uint8_t* b = u8(kUtf8);
    const uint8_t* e = b + std::strlen(kUtf8);
    memepp::const_rune_iterator it(b, e);
    for (auto _ : state) benchmark::DoNotOptimize(it.to_rune().codepoint());
}
BENCHMARK(RuneIter_ToRune)->Iterations(MMBM_ITERATION_COUNT / 10);

// C baseline: decode UTF-8 by hand
static void C_Utf8Decode(benchmark::State& state) {
    const uint8_t* p = u8(kUtf8);
    const size_t n = std::strlen(kUtf8);
    for (auto _ : state) {
        size_t i = 0;
        uint32_t cp = 0;
        if (p[0] < 0x80) { cp = p[0]; i = 1; }
        else if ((p[0] & 0xE0) == 0xC0) { cp = ((p[0] & 0x1F) << 6) | (p[1] & 0x3F); i = 2; }
        else if ((p[0] & 0xF0) == 0xE0) { cp = ((p[0] & 0x0F) << 12) | ((p[1] & 0x3F) << 6) | (p[2] & 0x3F); i = 3; }
        else { cp = ((p[0] & 0x07) << 18) | ((p[1] & 0x3F) << 12) | ((p[2] & 0x3F) << 6) | (p[3] & 0x3F); i = 4; }
        benchmark::DoNotOptimize(cp + i + n);
    }
}
BENCHMARK(C_Utf8Decode)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// errc -- thread_local error slot touched by every failing API call
// ===========================================================================
static void Errc_SetGet(benchmark::State& state) {
    for (auto _ : state) {
        memepp::set_errc(42);
        benchmark::DoNotOptimize(memepp::get_errc());
    }
}
BENCHMARK(Errc_SetGet)->Iterations(MMBM_ITERATION_COUNT);

static void Errc_Pointer(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(memepp::errc());
}
BENCHMARK(Errc_Pointer)->Iterations(MMBM_ITERATION_COUNT);

static void C_ThreadLocalInt(benchmark::State& state) {
    static thread_local int slot = 0;
    for (auto _ : state) {
        slot = 42;
        benchmark::DoNotOptimize(slot);
    }
}
BENCHMARK(C_ThreadLocalInt)->Iterations(MMBM_ITERATION_COUNT);

// ===========================================================================
// convert::self -- zero-dependency wrappers
// ===========================================================================
static void Conv_FromStringIdentity(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(memepp::from(s).size());
}
BENCHMARK(Conv_FromStringIdentity)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Conv_FromStringMove(benchmark::State& state) {
    for (auto _ : state) {
        memepp::string s(kAscii);
        benchmark::DoNotOptimize(memepp::from(std::move(s)).size());
    }
}
BENCHMARK(Conv_FromStringMove)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Conv_ViewString(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(memepp::view(s).size());
}
BENCHMARK(Conv_ViewString)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Conv_IntoStringFromString(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(memepp::into<memepp::string>(s).size());
}
BENCHMARK(Conv_IntoStringFromString)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Conv_IntoStringFromView(benchmark::State& state) {
    memepp::string s(kAscii);
    memepp::string_view sv(s);
    for (auto _ : state) benchmark::DoNotOptimize(memepp::into<memepp::string>(sv).size());
}
BENCHMARK(Conv_IntoStringFromView)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Conv_IntoViewFromString(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(memepp::into<memepp::string_view>(s).size());
}
BENCHMARK(Conv_IntoViewFromString)->Iterations(MMBM_ITERATION_COUNT);

static void Conv_FromMmstk(benchmark::State& state) {
    mmstrstk_t stk;
    MemeStringStack_initByU8bytes(&stk, MMSTR__OBJ_SIZE,
        u8(kAscii), static_cast<mmint_t>(std::strlen(kAscii)));
    for (auto _ : state) benchmark::DoNotOptimize(memepp::from(stk).size());
    mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE);
}
BENCHMARK(Conv_FromMmstk)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Conv_ViewMmstk(benchmark::State& state) {
    mmstrstk_t stk;
    MemeStringStack_initByU8bytes(&stk, MMSTR__OBJ_SIZE,
        u8(kAscii), static_cast<mmint_t>(std::strlen(kAscii)));
    for (auto _ : state) benchmark::DoNotOptimize(memepp::view(stk).size());
    mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE);
}
BENCHMARK(Conv_ViewMmstk)->Iterations(MMBM_ITERATION_COUNT);

} // namespace
