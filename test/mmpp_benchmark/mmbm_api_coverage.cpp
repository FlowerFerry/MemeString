// Previously-uncovered API surface, benchmarked against its nearest equivalent.
//
// ---------------------------------------------------------------------------
// API NAME MAPPING (verified against the headers, not assumed)
// ---------------------------------------------------------------------------
// Several names in the coverage request do not exist in this library.  Each one
// is mapped below to the API that actually provides the behaviour, or marked
// MISSING when there is no equivalent at all.  Nothing in this file calls an
// API that was not read from a header first.
//
//   requested name            actual memepp API                       header
//   ----------------------    ------------------------------------    ------------------
//   to_std_string()           memepp::to<std::string>() / mm_to<>     convert/std/string.hpp
//   from_std_string()         memepp::from(std::string) / mm_from()   convert/std/string.hpp
//   to_string_view()          memepp::string_view(s) ctor            string_view_def.hpp
//                             (mm_view() has no convert::view<T>
//                              specialisation for memepp::string)
//   append/insert/remove/     variable_buffer (the mutable container;
//   push_back/pop_back/         memepp::string itself is immutable)   variable_buffer_def.hpp
//   clear/reserve/resize
//   operator+=                string_builder::operator+=              string_builder_def.hpp
//   find_first_of / _last_of  string_span::find_first_of(...)         string_span_def.hpp
//   find_first_not_of / ...   string_span::find_first_not_of(...)     string_span_def.hpp
//   compare(pos,len,str)      string_span::compare(p1,c1,...)         string_span_def.hpp
//   at()                      string / string_view / string_span      string_def.hpp
//   begin/end/rbegin/...      string / string_view / string_span      string_def.hpp
//   hash()                    std::hash<memepp::string>               hash/std/hash.hpp
//   from_number<T>()          memepp::to_string(intN_t, radix)        to_string_def.hpp
//   from_hexadecimals()       memepp::from_hexadecimals()             string_def.hpp
//   byte_size()/max_byte_size string::size() / max_size()             string_def.hpp
//   is_small/is_large/...     string::storage_type()                  string_def.hpp
//   trim(charset)             string::trim_if(predicate)              string_def.hpp
//   substr(pos)               string::substr(pos) (count defaults)    string_def.hpp
//
//   MISSING, no equivalent:
//     operator<=>            -- use compare(); no three-way operator exists
//     operator<< (ostream)   -- no stream insertion operator
//     get_allocator()        -- memepp::string has no allocator concept
//     to_number<T>()         -- no string->number parse on the string class;
//                              memepp::to_string is number->string only
//     string::append/insert/ -- memepp::string is immutable by design; the
//     remove/clear/resize      mutable type is variable_buffer

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/string_span.hpp>
#include <memepp/string_builder.hpp>
#include <memepp/variable_buffer.hpp>
#include <memepp/to_string.hpp>
#include <memepp/convert/std/string.hpp>
#include <memepp/convert/common.hpp>
#include <memepp/hash/std/hash.hpp>

#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include <cctype>
#include <cstring>

namespace {

const char* const kShort = "Hello World!";

// variable_buffer::const_pointer is const MemeByte_t* (i.e. const uint8_t*),
// so every const char* literal needs an explicit cast.
using vb_t = memepp::variable_buffer;
inline vb_t vb_from(const char* s)
{
    return vb_t(reinterpret_cast<vb_t::const_pointer>(s),
                static_cast<vb_t::size_type>(std::strlen(s)));
}
inline vb_t::const_pointer vb_p(const char* s)
{
    return reinterpret_cast<vb_t::const_pointer>(s);
}
const char* const kMed   =
    "The quick brown fox jumps over the lazy dog. "
    "Pack my box with five dozen liquor jugs.";
const char* const kLong  =
    "The quick brown fox jumps over the lazy dog. "
    "Pack my box with five dozen liquor jugs. "
    "How vexingly quick daft zebras jump! "
    "Sphinx of black quartz, judge my vow. "
    "Jackdaws love my big sphinx of quartz. "
    "The five boxing wizards jump quickly. "
    "Bright vixens jump; dozy fowl quack. "
    "Quick zephyrs blow, vexing daft Jim. "
    "Two driven jocks help fax my big quiz. "
    "The quick brown fox jumps over the lazy dog. ";

// ===========================================================================
// std::string <-> memepp::string conversion
// (requested as to_std_string / from_std_string; the real names are
//  memepp::to<std::string>() and memepp::from(std::string))
// ===========================================================================
static void MemeStr_ToStdShort(benchmark::State& state) {
    memepp::string s(kShort);
    for (auto _ : state)
        benchmark::DoNotOptimize(memepp::to<std::string>(s));
}
BENCHMARK(MemeStr_ToStdShort)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_CopyShort(benchmark::State& state) {
    std::string s(kShort);
    for (auto _ : state)
        benchmark::DoNotOptimize(std::string(s));
}
BENCHMARK(StdStr_CopyShort)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_ToStdLong(benchmark::State& state) {
    memepp::string s(kLong);
    for (auto _ : state)
        benchmark::DoNotOptimize(memepp::to<std::string>(s));
}
BENCHMARK(MemeStr_ToStdLong)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_CopyLong(benchmark::State& state) {
    std::string s(kLong);
    for (auto _ : state)
        benchmark::DoNotOptimize(std::string(s));
}
BENCHMARK(StdStr_CopyLong)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_FromStdShort(benchmark::State& state) {
    std::string s(kShort);
    for (auto _ : state)
        benchmark::DoNotOptimize(memepp::from(s));
}
BENCHMARK(MemeStr_FromStdShort)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_FromStdMove(benchmark::State& state) {
    std::string src(kLong);
    for (auto _ : state) {
        std::string s(src);                       // fresh source each round
        benchmark::DoNotOptimize(memepp::from(std::move(s)));
    }
}
BENCHMARK(MemeStr_FromStdMove)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_FromCStrLong(benchmark::State& state) {
    std::string src(kLong);
    for (auto _ : state) {
        std::string s(src);
        benchmark::DoNotOptimize(std::string(std::move(s)));
    }
}
BENCHMARK(StdStr_FromCStrLong)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// memepp::string -> string_view  (requested as to_string_view)
// ===========================================================================
static void MemeStr_ToViewCtor(benchmark::State& state) {
    memepp::string s(kMed);
    for (auto _ : state)
        benchmark::DoNotOptimize(memepp::string_view(s).size());
}
BENCHMARK(MemeStr_ToViewCtor)->Iterations(MMBM_ITERATION_COUNT);

// mm_view()/memepp::view() need a convert::view<T> specialisation, and there is
// none for memepp::string itself (only for std::string).  The string_view
// constructor above is the supported route, so no helper variant is measured.

static void StdStr_ToStdView(benchmark::State& state) {
    std::string s(kMed);
    for (auto _ : state)
        benchmark::DoNotOptimize(std::string_view(s).size());
}
BENCHMARK(StdStr_ToStdView)->Iterations(MMBM_ITERATION_COUNT);

// ===========================================================================
// Mutable container: memepp::variable_buffer vs std::string
// (memepp::string is immutable; variable_buffer is the append/insert/remove
//  type, so it is the correct counterpart for std::string's modifiers)
// ===========================================================================
static void MemeVb_AppendCStr(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = vb_from(kMed);
        vb.append(vb_p(kShort), static_cast<vb_t::size_type>(std::strlen(kShort)));
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(MemeVb_AppendCStr)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_AppendCStr(benchmark::State& state) {
    for (auto _ : state) {
        std::string s(kMed);
        s.append(kShort);
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_AppendCStr)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeVb_PushBack(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = vb_from(kShort);
        for (int i = 0; i < 16; ++i) vb.push_back('x');
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(MemeVb_PushBack)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_PushBack(benchmark::State& state) {
    for (auto _ : state) {
        std::string s(kShort);
        for (int i = 0; i < 16; ++i) s.push_back('x');
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_PushBack)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeVb_Insert(benchmark::State& state) {
    memepp::string ins("INSERTED");
    for (auto _ : state) {
        vb_t vb = vb_from(kMed);
        vb.insert(static_cast<vb_t::size_type>(10), ins);
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(MemeVb_Insert)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_Insert(benchmark::State& state) {
    std::string ins("INSERTED");
    for (auto _ : state) {
        std::string s(kMed);
        s.insert(10, ins);
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_Insert)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeVb_Remove(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = vb_from(kMed);
        vb.remove(10, 20);
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(MemeVb_Remove)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_Erase(benchmark::State& state) {
    for (auto _ : state) {
        std::string s(kMed);
        s.erase(10, 20);
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_Erase)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeVb_Resize(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = vb_from(kShort);
        vb.resize(200, 'z');
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(MemeVb_Resize)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_Resize(benchmark::State& state) {
    for (auto _ : state) {
        std::string s(kShort);
        s.resize(200, 'z');
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_Resize)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeVb_Reserve(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = vb_from(kShort);
        vb.reserve(4096);
        benchmark::DoNotOptimize(vb.capacity());
    }
}
BENCHMARK(MemeVb_Reserve)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_Reserve(benchmark::State& state) {
    for (auto _ : state) {
        std::string s(kShort);
        s.reserve(4096);
        benchmark::DoNotOptimize(s.capacity());
    }
}
BENCHMARK(StdStr_Reserve)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeVb_Clear(benchmark::State& state) {
    vb_t vb = vb_from(kMed);
    for (auto _ : state) {
        vb.clear();
        vb.append(vb_p(kMed), static_cast<vb_t::size_type>(std::strlen(kMed)));
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(MemeVb_Clear)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_Clear(benchmark::State& state) {
    std::string s(kMed);
    for (auto _ : state) {
        s.clear();
        s.append(kMed);
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_Clear)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeVb_PopBack(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = vb_from(kMed);
        for (int i = 0; i < 16; ++i) vb.pop_back();
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(MemeVb_PopBack)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_PopBack(benchmark::State& state) {
    for (auto _ : state) {
        std::string s(kMed);
        for (int i = 0; i < 16; ++i) s.pop_back();
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_PopBack)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// string_builder::operator+=  (the only operator+= in the library)
// ===========================================================================
static void MemeSb_PlusEq(benchmark::State& state) {
    for (auto _ : state) {
        memepp::string_builder sb;
        sb += memepp::string(kShort);
        sb += kShort;
        sb += ' ';
        benchmark::DoNotOptimize(sb.size());
    }
}
BENCHMARK(MemeSb_PlusEq)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_PlusEq(benchmark::State& state) {
    for (auto _ : state) {
        std::string s;
        s += std::string(kShort);
        s += kShort;
        s += ' ';
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_PlusEq)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// find_first_of / find_last_of / find_first_not_of / find_last_not_of
// memepp::string_span vs std::string_view (both C++17, same contract)
// ===========================================================================
const char* const kSet3 = "aei";
const char* const kSet8 = "aeiouwy";

#define SPAN_PAIR(NAME, HAY, CALL_M, CALL_S)                                  \
    static void MemeSpan_##NAME(benchmark::State& state) {                    \
        memepp::string_span sp(HAY);                                          \
        for (auto _ : state) { CALL_M; }                                      \
    }                                                                         \
    BENCHMARK(MemeSpan_##NAME)->Iterations(MMBM_ITERATION_COUNT);             \
    static void StdView_##NAME(benchmark::State& state) {                     \
        std::string_view sv(HAY);                                             \
        for (auto _ : state) { CALL_S; }                                      \
    }                                                                         \
    BENCHMARK(StdView_##NAME)->Iterations(MMBM_ITERATION_COUNT);

SPAN_PAIR(FirstOf_3, kMed,
    benchmark::DoNotOptimize(sp.find_first_of(kSet3)),
    benchmark::DoNotOptimize(sv.find_first_of(kSet3)))
SPAN_PAIR(FirstOf_7, kMed,
    benchmark::DoNotOptimize(sp.find_first_of(kSet8)),
    benchmark::DoNotOptimize(sv.find_first_of(kSet8)))
SPAN_PAIR(LastOf_3, kMed,
    benchmark::DoNotOptimize(sp.find_last_of(kSet3)),
    benchmark::DoNotOptimize(sv.find_last_of(kSet3)))
SPAN_PAIR(LastOf_7, kLong,
    benchmark::DoNotOptimize(sp.find_last_of(kSet8)),
    benchmark::DoNotOptimize(sv.find_last_of(kSet8)))
SPAN_PAIR(FirstNotOf_Space, "          abcdefgh",
    benchmark::DoNotOptimize(sp.find_first_not_of(' ')),
    benchmark::DoNotOptimize(sv.find_first_not_of(' ')))
SPAN_PAIR(FirstNotOf_3, kMed,
    benchmark::DoNotOptimize(sp.find_first_not_of(kSet3)),
    benchmark::DoNotOptimize(sv.find_first_not_of(kSet3)))
SPAN_PAIR(LastNotOf_Space, "abcdefgh          ",
    benchmark::DoNotOptimize(sp.find_last_not_of(' ')),
    benchmark::DoNotOptimize(sv.find_last_not_of(' ')))
SPAN_PAIR(LastNotOf_3, kMed,
    benchmark::DoNotOptimize(sp.find_last_not_of(kSet3)),
    benchmark::DoNotOptimize(sv.find_last_not_of(kSet3)))
SPAN_PAIR(FirstOf_Absent, kMed,
    benchmark::DoNotOptimize(sp.find_first_of("QQQQ")),
    benchmark::DoNotOptimize(sv.find_first_of("QQQQ")))
SPAN_PAIR(LastOf_Absent, kMed,
    benchmark::DoNotOptimize(sp.find_last_of("QQQQ")),
    benchmark::DoNotOptimize(sv.find_last_of("QQQQ")))

// ===========================================================================
// compare: multi-argument (string_span) and single (string)
// ===========================================================================
SPAN_PAIR(CompareFull, kMed,
    benchmark::DoNotOptimize(sp.compare(memepp::string_span(kMed))),
    benchmark::DoNotOptimize(sv.compare(std::string_view(kMed))))
SPAN_PAIR(CompareSub, kMed,
    benchmark::DoNotOptimize(sp.compare(10, 20, memepp::string_span(kMed))),
    benchmark::DoNotOptimize(sv.compare(10, 20, std::string_view(kMed))))
SPAN_PAIR(CompareCStr, kMed,
    benchmark::DoNotOptimize(sp.compare(kMed)),
    benchmark::DoNotOptimize(sv.compare(kMed)))
SPAN_PAIR(CompareSubCStr, kMed,
    benchmark::DoNotOptimize(sp.compare(10, 20, kMed)),
    benchmark::DoNotOptimize(sv.compare(10, 20, kMed)))

static void MemeStr_Compare(benchmark::State& state) {
    memepp::string a(kMed), b(kMed);
    for (auto _ : state)
        benchmark::DoNotOptimize(a.compare(b));
}
BENCHMARK(MemeStr_Compare)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Compare(benchmark::State& state) {
    std::string a(kMed), b(kMed);
    for (auto _ : state)
        benchmark::DoNotOptimize(a.compare(b));
}
BENCHMARK(StdStr_Compare)->Iterations(MMBM_ITERATION_COUNT);

// ===========================================================================
// at() -- bounds-checked access
// ===========================================================================
static void MemeStr_At(benchmark::State& state) {
    memepp::string s(kMed);
    std::size_t i = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.at(i));
        i = (i + 37) % s.size();
    }
}
BENCHMARK(MemeStr_At)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_At(benchmark::State& state) {
    std::string s(kMed);
    std::size_t i = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.at(i));
        i = (i + 37) % s.size();
    }
}
BENCHMARK(StdStr_At)->Iterations(MMBM_ITERATION_COUNT);

static void MemeSpan_At(benchmark::State& state) {
    memepp::string_span sp(kMed);
    std::size_t i = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(sp.at(i));
        i = (i + 37) % sp.size();
    }
}
BENCHMARK(MemeSpan_At)->Iterations(MMBM_ITERATION_COUNT);

// ===========================================================================
// Iterators
// ===========================================================================
static void MemeStr_IterForward(benchmark::State& state) {
    memepp::string s(kMed);
    for (auto _ : state) {
        std::size_t sum = 0;
        for (auto it = s.cbegin(), e = s.cend(); it != e; ++it) sum += *it;
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(MemeStr_IterForward)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_IterForward(benchmark::State& state) {
    std::string s(kMed);
    for (auto _ : state) {
        std::size_t sum = 0;
        for (auto it = s.cbegin(), e = s.cend(); it != e; ++it) sum += *it;
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(StdStr_IterForward)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_IterReverse(benchmark::State& state) {
    memepp::string s(kMed);
    for (auto _ : state) {
        std::size_t sum = 0;
        for (auto it = s.crbegin(), e = s.crend(); it != e; ++it) sum += *it;
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(MemeStr_IterReverse)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_IterReverse(benchmark::State& state) {
    std::string s(kMed);
    for (auto _ : state) {
        std::size_t sum = 0;
        for (auto it = s.crbegin(), e = s.crend(); it != e; ++it) sum += *it;
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(StdStr_IterReverse)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeSpan_IterForward(benchmark::State& state) {
    memepp::string_span sp(kMed);
    for (auto _ : state) {
        std::size_t sum = 0;
        for (auto it = sp.cbegin(), e = sp.cend(); it != e; ++it) sum += *it;
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(MemeSpan_IterForward)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdView_IterForward(benchmark::State& state) {
    std::string_view sv(kMed);
    for (auto _ : state) {
        std::size_t sum = 0;
        for (auto it = sv.cbegin(), e = sv.cend(); it != e; ++it) sum += *it;
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(StdView_IterForward)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// Hash (std::hash specialisation)
// ===========================================================================
static void MemeStr_Hash(benchmark::State& state) {
    memepp::string s(kMed);
    for (auto _ : state)
        benchmark::DoNotOptimize(std::hash<memepp::string>{}(s));
}
BENCHMARK(MemeStr_Hash)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Hash(benchmark::State& state) {
    std::string s(kMed);
    for (auto _ : state)
        benchmark::DoNotOptimize(std::hash<std::string>{}(s));
}
BENCHMARK(StdStr_Hash)->Iterations(MMBM_ITERATION_COUNT);

static void MemeView_Hash(benchmark::State& state) {
    memepp::string_view sv(kMed);
    for (auto _ : state)
        benchmark::DoNotOptimize(std::hash<memepp::string_view>{}(sv));
}
BENCHMARK(MemeView_Hash)->Iterations(MMBM_ITERATION_COUNT);

// ===========================================================================
// Number <-> string
// (requested as from_number<T>() / to_number<T>(); the real API is
//  memepp::to_string(intN_t, radix).  There is NO string->number parse on
//  memepp::string, so the reverse direction has no memepp cell.)
// ===========================================================================
static void MemeStr_ToStringI32(benchmark::State& state) {
    for (auto _ : state)
        benchmark::DoNotOptimize(memepp::to_string(std::int32_t(1234567890)));
}
BENCHMARK(MemeStr_ToStringI32)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_ToStringI32(benchmark::State& state) {
    for (auto _ : state)
        benchmark::DoNotOptimize(std::to_string(std::int32_t(1234567890)));
}
BENCHMARK(StdStr_ToStringI32)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_ToStringI64Hex(benchmark::State& state) {
    for (auto _ : state)
        benchmark::DoNotOptimize(memepp::to_string(std::int64_t(0x1234567890ABCDEF), 16));
}
BENCHMARK(MemeStr_ToStringI64Hex)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_ToStringI64(benchmark::State& state) {
    for (auto _ : state)
        benchmark::DoNotOptimize(std::to_string(std::int64_t(0x1234567890ABCDEF)));
}
BENCHMARK(StdStr_ToStringI64)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_FromHexadecimals(benchmark::State& state) {
    const std::uint8_t bytes[16] = {
        0xDE,0xAD,0xBE,0xEF,0x01,0x23,0x45,0x67,
        0x89,0xAB,0xCD,0xEF,0xFE,0xDC,0xBA,0x98 };
    for (auto _ : state)
        benchmark::DoNotOptimize(memepp::from_hexadecimals(bytes, sizeof(bytes)));
}
BENCHMARK(MemeStr_FromHexadecimals)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// join / split variants
// ===========================================================================
static void MemeStr_JoinVecStr(benchmark::State& state) {
    std::vector<memepp::string> v = {"alpha","bravo","charlie","delta","echo"};
    memepp::string sep(", ");
    for (auto _ : state)
        benchmark::DoNotOptimize(sep.join(v));
}
BENCHMARK(MemeStr_JoinVecStr)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_JoinVecView(benchmark::State& state) {
    std::vector<memepp::string_view> v = {"alpha","bravo","charlie","delta","echo"};
    memepp::string sep(", ");
    for (auto _ : state)
        benchmark::DoNotOptimize(sep.join(v));
}
BENCHMARK(MemeStr_JoinVecView)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_JoinVec(benchmark::State& state) {
    std::vector<std::string> v = {"alpha","bravo","charlie","delta","echo"};
    for (auto _ : state) {
        std::string out;
        for (std::size_t i = 0; i < v.size(); ++i) {
            if (i) out += ", ";
            out += v[i];
        }
        benchmark::DoNotOptimize(out.size());
    }
}
BENCHMARK(StdStr_JoinVec)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_SplitChar(benchmark::State& state) {
    memepp::string s("alpha,bravo,charlie,delta,echo");
    for (auto _ : state) {
        std::vector<memepp::string_view> out;
        s.split(",", memepp::split_behavior_t::skip_empty_parts, std::back_inserter(out));
        benchmark::DoNotOptimize(out.size());
    }
}
BENCHMARK(MemeStr_SplitChar)->Iterations(MMBM_ITERATION_COUNT / 100);

static void MemeStr_SplitMulti(benchmark::State& state) {
    memepp::string s("alpha::bravo::charlie::delta::echo");
    for (auto _ : state) {
        std::vector<memepp::string_view> out;
        s.split("::", memepp::split_behavior_t::skip_empty_parts, std::back_inserter(out));
        benchmark::DoNotOptimize(out.size());
    }
}
BENCHMARK(MemeStr_SplitMulti)->Iterations(MMBM_ITERATION_COUNT / 100);

static void MemeStr_SplitKeepEmpty(benchmark::State& state) {
    memepp::string s("alpha,,charlie,,echo");
    for (auto _ : state) {
        std::vector<memepp::string_view> out;
        s.split(",", memepp::split_behavior_t::keep_empty_parts, std::back_inserter(out));
        benchmark::DoNotOptimize(out.size());
    }
}
BENCHMARK(MemeStr_SplitKeepEmpty)->Iterations(MMBM_ITERATION_COUNT / 100);

static void StdStr_SplitChar(benchmark::State& state) {
    std::string s("alpha,bravo,charlie,delta,echo");
    for (auto _ : state) {
        std::vector<std::string_view> out;
        std::size_t start = 0;
        for (;;) {
            const std::size_t at = s.find(',', start);
            if (at == std::string::npos) { out.push_back(std::string_view(s).substr(start)); break; }
            out.push_back(std::string_view(s).substr(start, at - start));
            start = at + 1;
        }
        benchmark::DoNotOptimize(out.size());
    }
}
BENCHMARK(StdStr_SplitChar)->Iterations(MMBM_ITERATION_COUNT / 100);

// ===========================================================================
// trim with a custom character set (trim_if takes a predicate)
// ===========================================================================
static void MemeStr_TrimIfSpace(benchmark::State& state) {
    memepp::string s("      hello world      ");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.trim_if([](memepp::string::value_type c){ return c == ' '; }));
}
BENCHMARK(MemeStr_TrimIfSpace)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_TrimIfPunct(benchmark::State& state) {
    memepp::string s("!!!hello world???");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.trim_if([](memepp::string::value_type c){
            return c == '!' || c == '?'; }));
}
BENCHMARK(MemeStr_TrimIfPunct)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_TrimSpace(benchmark::State& state) {
    std::string s("      hello world      ");
    for (auto _ : state) {
        const std::size_t b = s.find_first_not_of(' ');
        const std::size_t e = s.find_last_not_of(' ');
        benchmark::DoNotOptimize(s.substr(b, e - b + 1));
    }
}
BENCHMARK(StdStr_TrimSpace)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// Capacity / introspection accessors
// (requested as byte_size / max_byte_size / is_small / is_shared; the real
//  names are size() / max_size() / storage_type())
// ===========================================================================
static void MemeStr_Size(benchmark::State& state) {
    memepp::string s(kMed);
    for (auto _ : state) benchmark::DoNotOptimize(s.size());
}
BENCHMARK(MemeStr_Size)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_MaxSize(benchmark::State& state) {
    memepp::string s(kMed);
    for (auto _ : state) benchmark::DoNotOptimize(s.max_size());
}
BENCHMARK(MemeStr_MaxSize)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_StorageType(benchmark::State& state) {
    memepp::string s(kMed);
    for (auto _ : state) benchmark::DoNotOptimize(s.storage_type());
}
BENCHMARK(MemeStr_StorageType)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Size(benchmark::State& state) {
    std::string s(kMed);
    for (auto _ : state) benchmark::DoNotOptimize(s.size());
}
BENCHMARK(StdStr_Size)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_MaxSize(benchmark::State& state) {
    std::string s(kMed);
    for (auto _ : state) benchmark::DoNotOptimize(s.max_size());
}
BENCHMARK(StdStr_MaxSize)->Iterations(MMBM_ITERATION_COUNT);

// ===========================================================================
// substr(pos) -- single-argument form (count defaulted)
// ===========================================================================
static void MemeStr_SubstrPos(benchmark::State& state) {
    memepp::string s(kMed);
    for (auto _ : state) benchmark::DoNotOptimize(s.substr(40));
}
BENCHMARK(MemeStr_SubstrPos)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_SubstrPosCount(benchmark::State& state) {
    memepp::string s(kMed);
    for (auto _ : state) benchmark::DoNotOptimize(s.substr(40, 30));
}
BENCHMARK(MemeStr_SubstrPosCount)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_SubstrPos(benchmark::State& state) {
    std::string s(kMed);
    for (auto _ : state) benchmark::DoNotOptimize(s.substr(40));
}
BENCHMARK(StdStr_SubstrPos)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_SubstrPosCount(benchmark::State& state) {
    std::string s(kMed);
    for (auto _ : state) benchmark::DoNotOptimize(s.substr(40, 30));
}
BENCHMARK(StdStr_SubstrPosCount)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// find(str, pos) / rfind(str, pos) -- offset forms
// ===========================================================================
static void MemeStr_FindFromPos(benchmark::State& state) {
    memepp::string s(kLong);
    for (auto _ : state) benchmark::DoNotOptimize(s.find("fox", 40));
}
BENCHMARK(MemeStr_FindFromPos)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_FindFromPos(benchmark::State& state) {
    std::string s(kLong);
    for (auto _ : state) benchmark::DoNotOptimize(s.find("fox", 40));
}
BENCHMARK(StdStr_FindFromPos)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_RfindToPos(benchmark::State& state) {
    memepp::string s(kLong);
    for (auto _ : state) benchmark::DoNotOptimize(s.rfind("fox", 300));
}
BENCHMARK(MemeStr_RfindToPos)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_RfindToPos(benchmark::State& state) {
    std::string s(kLong);
    for (auto _ : state) benchmark::DoNotOptimize(s.rfind("fox", 300));
}
BENCHMARK(StdStr_RfindToPos)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_FindCharFromPos(benchmark::State& state) {
    memepp::string s(kLong);
    for (auto _ : state) benchmark::DoNotOptimize(s.find('q', 200));
}
BENCHMARK(MemeStr_FindCharFromPos)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_FindCharFromPos(benchmark::State& state) {
    std::string s(kLong);
    for (auto _ : state) benchmark::DoNotOptimize(s.find('q', 200));
}
BENCHMARK(StdStr_FindCharFromPos)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_RfindCharToPos(benchmark::State& state) {
    memepp::string s(kLong);
    for (auto _ : state) benchmark::DoNotOptimize(s.rfind('q', 200));
}
BENCHMARK(MemeStr_RfindCharToPos)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_RfindCharToPos(benchmark::State& state) {
    std::string s(kLong);
    for (auto _ : state) benchmark::DoNotOptimize(s.rfind('q', 200));
}
BENCHMARK(StdStr_RfindCharToPos)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// UTF-8 rune needle -- no std equivalent, memepp only
// ===========================================================================
static void MemeStr_FindRune(benchmark::State& state) {
    memepp::string s("Hello \xE4\xB8\xAD\xE6\x96\x87 world \xE4\xB8\xAD\xE6\x96\x87 end");
    const memepp::rune target(0x4E2D);          // U+4E2D
    for (auto _ : state) benchmark::DoNotOptimize(s.find(target));
}
BENCHMARK(MemeStr_FindRune)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_RfindRune(benchmark::State& state) {
    memepp::string s("Hello \xE4\xB8\xAD\xE6\x96\x87 world \xE4\xB8\xAD\xE6\x96\x87 end");
    const memepp::rune target(0x4E2D);
    for (auto _ : state) benchmark::DoNotOptimize(s.rfind(target));
}
BENCHMARK(MemeStr_RfindRune)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_IndexRuneCI(benchmark::State& state) {
    memepp::string s("Hello \xE4\xB8\xAD\xE6\x96\x87 world \xE4\xB8\xAD\xE6\x96\x87 end");
    const memepp::string needle("\xE4\xB8\xAD\xE6\x96\x87");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.index_of(needle, memepp::case_sensitivity_t::all_insensitive));
}
BENCHMARK(MemeStr_IndexRuneCI)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_RuneSizeLong(benchmark::State& state) {
    memepp::string s("Hello \xE4\xB8\xAD\xE6\x96\x87 world \xE4\xB8\xAD\xE6\x96\x87 end");
    for (auto _ : state) benchmark::DoNotOptimize(s.rune_size());
}
BENCHMARK(MemeStr_RuneSizeLong)->Iterations(MMBM_ITERATION_COUNT);

} // namespace
