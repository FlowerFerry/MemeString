// Batch 6: rune, variant, varts, buffer_view, and the remaining string
// operations that had no benchmark.
//
// API surface verified against the headers first:
//   rune_def.hpp        : codepoint/char_size/is_ascii/is_multi/is_space/
//                         is_en_char/is_en_punct/is_ch_punct/valid/from_codepoint
//   variant_def.hpp     : is_null/is_type/type_id/set/try_get/get_or/reset/swap
//   varts_def.hpp       : timestamp/var/set_var/swap
//   buffer_view_def.hpp : data/size/index_of/find/starts_with/ends_with/compare
//   string_def.hpp      : count/cut_prefix/cut_suffix/trim_prefix/trim_suffix/
//                         trim_if_rune/rune_front/rune_back/u16char_size/
//                         rune_foreach/to_valid_utf8/mapping_convert/c_format
//
// rune, variant and varts have no std equivalent, so those cells are memepp-only.
// buffer_view maps onto std::string_view.

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/rune.hpp>
#include <memepp/variant.hpp>
#include <memepp/varts.hpp>
#include <memepp/buffer.hpp>
#include <memepp/buffer_view.hpp>

#include <string>
#include <string_view>
#include <vector>
#include <cstring>

namespace {

const char* const kAscii   = "The quick brown fox jumps over the lazy dog.";
const char  kUtf8[]        = "Hello \xE4\xB8\xAD\xE6\x96\x87 world \xE4\xB8\xAD\xE6\x96\x87 end";
const char* const kInvalid = "Hello \xFF\xFE world";   // invalid UTF-8

// buffer_view takes const uint8_t*, so const char* literals need a cast.
inline memepp::buffer_view make_bv(const char* s)
{
    return memepp::buffer_view(
        reinterpret_cast<const memepp::buffer_view::const_pointer>(s),
        static_cast<memepp::buffer_view::size_type>(std::strlen(s)));
}
inline const memepp::buffer_view::const_pointer bv_p(const char* s)
{
    return reinterpret_cast<const memepp::buffer_view::const_pointer>(s);
}


// ===========================================================================
// rune -- no std equivalent, memepp only
// ===========================================================================
static void Rune_CtorAscii(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(memepp::rune('A').codepoint());
}
BENCHMARK(Rune_CtorAscii)->Iterations(MMBM_ITERATION_COUNT);

static void Rune_CtorMulti(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(memepp::rune(0x4E2D).codepoint());
}
BENCHMARK(Rune_CtorMulti)->Iterations(MMBM_ITERATION_COUNT);

static void Rune_CharSize(benchmark::State& state) {
    const memepp::rune r(0x4E2D);
    for (auto _ : state) benchmark::DoNotOptimize(r.char_size());
}
BENCHMARK(Rune_CharSize)->Iterations(MMBM_ITERATION_COUNT);

static void Rune_Codepoint(benchmark::State& state) {
    const memepp::rune r(0x4E2D);
    for (auto _ : state) benchmark::DoNotOptimize(r.codepoint());
}
BENCHMARK(Rune_Codepoint)->Iterations(MMBM_ITERATION_COUNT);

static void Rune_Valid(benchmark::State& state) {
    const memepp::rune r(0x4E2D);
    for (auto _ : state) benchmark::DoNotOptimize(r.valid());
}
BENCHMARK(Rune_Valid)->Iterations(MMBM_ITERATION_COUNT);

static void Rune_IsAscii(benchmark::State& state) {
    const memepp::rune r('A');
    for (auto _ : state) benchmark::DoNotOptimize(r.is_ascii());
}
BENCHMARK(Rune_IsAscii)->Iterations(MMBM_ITERATION_COUNT);

static void Rune_IsMulti(benchmark::State& state) {
    const memepp::rune r(0x4E2D);
    for (auto _ : state) benchmark::DoNotOptimize(r.is_multi());
}
BENCHMARK(Rune_IsMulti)->Iterations(MMBM_ITERATION_COUNT);

static void Rune_IsSpace(benchmark::State& state) {
    const memepp::rune r(' ');
    for (auto _ : state) benchmark::DoNotOptimize(r.is_space());
}
BENCHMARK(Rune_IsSpace)->Iterations(MMBM_ITERATION_COUNT);

static void Rune_IsEnChar(benchmark::State& state) {
    const memepp::rune r('A');
    for (auto _ : state) benchmark::DoNotOptimize(r.is_en_char());
}
BENCHMARK(Rune_IsEnChar)->Iterations(MMBM_ITERATION_COUNT);

static void Rune_IsChPunct(benchmark::State& state) {
    const memepp::rune r(0x3002);   // U+3002 ideographic full stop
    for (auto _ : state) benchmark::DoNotOptimize(r.is_ch_punct());
}
BENCHMARK(Rune_IsChPunct)->Iterations(MMBM_ITERATION_COUNT);

static void Rune_FromCodepoint(benchmark::State& state) {
    for (auto _ : state)
        benchmark::DoNotOptimize(memepp::rune::from_codepoint(0x4E2D).codepoint());
}
BENCHMARK(Rune_FromCodepoint)->Iterations(MMBM_ITERATION_COUNT);

// C equivalent of is_ascii on a byte: value < 0x80
static void C_IsAsciiByte(benchmark::State& state) {
    const unsigned char c = 'A';
    for (auto _ : state) benchmark::DoNotOptimize(c < 0x80);
}
BENCHMARK(C_IsAsciiByte)->Iterations(MMBM_ITERATION_COUNT);

// ===========================================================================
// string rune-oriented accessors
// ===========================================================================
static void Str_RuneFront(benchmark::State& state) {
    memepp::string s(kUtf8);
    for (auto _ : state) benchmark::DoNotOptimize(s.rune_front().codepoint());
}
BENCHMARK(Str_RuneFront)->Iterations(MMBM_ITERATION_COUNT);

static void Str_RuneBack(benchmark::State& state) {
    memepp::string s(kUtf8);
    for (auto _ : state) benchmark::DoNotOptimize(s.rune_back().codepoint());
}
BENCHMARK(Str_RuneBack)->Iterations(MMBM_ITERATION_COUNT);

static void Str_U16CharSize(benchmark::State& state) {
    memepp::string s(kUtf8);
    for (auto _ : state) benchmark::DoNotOptimize(s.u16char_size());
}
BENCHMARK(Str_U16CharSize)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Str_RuneForeach(benchmark::State& state) {
    memepp::string s(kUtf8);
    for (auto _ : state) {
        std::uint32_t sum = 0;
        s.rune_foreach([&sum](const memepp::rune& r) {
            sum += r.codepoint() & 0xFF;
            return true;
        });
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(Str_RuneForeach)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Str_ToValidUtf8(benchmark::State& state) {
    memepp::string s(kInvalid);
    for (auto _ : state) benchmark::DoNotOptimize(s.to_valid_utf8().size());
}
BENCHMARK(Str_ToValidUtf8)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// string count / cut / trim variants
// ===========================================================================
static void Str_CountSub(benchmark::State& state) {
    memepp::string s("the cat sat on the mat with the hat");
    for (auto _ : state) benchmark::DoNotOptimize(s.count("the"));
}
BENCHMARK(Str_CountSub)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Str_CountRune(benchmark::State& state) {
    memepp::string s("the cat sat on the mat with the hat");
    const memepp::rune r('t');
    for (auto _ : state) benchmark::DoNotOptimize(s.count(r));
}
BENCHMARK(Str_CountRune)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Str_CountCI(benchmark::State& state) {
    memepp::string s("The cat sat on THE mat with the hat");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.count("the",
            memepp::case_sensitivity_t::all_insensitive));
}
BENCHMARK(Str_CountCI)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Str_CutPrefix(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) {
        auto r = s.cut_prefix("The quick ");
        benchmark::DoNotOptimize(std::get<1>(r));
    }
}
BENCHMARK(Str_CutPrefix)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Str_CutSuffix(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) {
        auto r = s.cut_suffix(" lazy dog.");
        benchmark::DoNotOptimize(std::get<1>(r));
    }
}
BENCHMARK(Str_CutSuffix)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Str_Cut(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) {
        auto r = s.cut(" brown ");
        benchmark::DoNotOptimize(std::get<2>(r));
    }
}
BENCHMARK(Str_Cut)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Str_TrimPrefix(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.trim_prefix("The quick ").size());
}
BENCHMARK(Str_TrimPrefix)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Str_TrimSuffix(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.trim_suffix(" lazy dog.").size());
}
BENCHMARK(Str_TrimSuffix)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Str_TrimIfRune(benchmark::State& state) {
    memepp::string s("      hello world      ");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.trim_if_rune(
            [](const memepp::rune& r) { return r.is_space(); }).size());
}
BENCHMARK(Str_TrimIfRune)->Iterations(MMBM_ITERATION_COUNT / 10);

// mapping_convert's callback takes memepp::rune& -- NOT a byte.  It operates on
// whole runes: mutate r to substitute it, or return a negative value to drop
// it.  The declaration in string_tmpimpl.hpp:20 constrains _Func with
//   std::is_invocable_v<_Func, memepp::rune&>
// so a byte-based lambda fails to instantiate.
static void Str_MappingConvertUpper(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.mapping_convert([](memepp::rune& r) {
            if (r.is_en_lower()) r = memepp::rune(static_cast<char>(r.codepoint() - 32));
            return 0;
        }).size());
    }
}
BENCHMARK(Str_MappingConvertUpper)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Str_MappingConvertDelete(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.mapping_convert([](memepp::rune& r) {
            return r.is_space() ? -1 : 0;
        }).size());
    }
}
BENCHMARK(Str_MappingConvertDelete)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// c_format
// ===========================================================================
static void Str_CFormat(benchmark::State& state) {
    for (auto _ : state)
        benchmark::DoNotOptimize(
            memepp::c_format(size_t(128), "%d-%s-%05.2f", 42, "abc", 3.14159).size());
}
BENCHMARK(Str_CFormat)->Iterations(MMBM_ITERATION_COUNT / 10);

static void C_Snprintf(benchmark::State& state) {
    for (auto _ : state) {
        char buf[128];
        std::snprintf(buf, sizeof(buf), "%d-%s-%05.2f", 42, "abc", 3.14159);
        benchmark::DoNotOptimize(buf[0]);
    }
}
BENCHMARK(C_Snprintf)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// variant -- no std equivalent, memepp only
// ===========================================================================
static void Var_CtorInt(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(memepp::variant(int64_t(42)).is_null());
}
BENCHMARK(Var_CtorInt)->Iterations(MMBM_ITERATION_COUNT);

static void Var_CtorDouble(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(memepp::variant(3.14159).is_null());
}
BENCHMARK(Var_CtorDouble)->Iterations(MMBM_ITERATION_COUNT);

static void Var_CtorString(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(memepp::variant(s).is_null());
}
BENCHMARK(Var_CtorString)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Var_Copy(benchmark::State& state) {
    memepp::variant v(int64_t(42));
    for (auto _ : state) benchmark::DoNotOptimize(memepp::variant(v).is_null());
}
BENCHMARK(Var_Copy)->Iterations(MMBM_ITERATION_COUNT);

static void Var_CopyString(benchmark::State& state) {
    memepp::string s(kAscii);
    memepp::variant v(s);
    for (auto _ : state) benchmark::DoNotOptimize(memepp::variant(v).is_null());
}
BENCHMARK(Var_CopyString)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Var_TryGetInt(benchmark::State& state) {
    memepp::variant v(int64_t(42));
    for (auto _ : state) {
        int64_t out = 0;
        benchmark::DoNotOptimize(v.try_get(out));
    }
}
BENCHMARK(Var_TryGetInt)->Iterations(MMBM_ITERATION_COUNT);

static void Var_GetOrInt(benchmark::State& state) {
    memepp::variant v(int64_t(42));
    for (auto _ : state) benchmark::DoNotOptimize(v.get_or<int64_t>(-1));
}
BENCHMARK(Var_GetOrInt)->Iterations(MMBM_ITERATION_COUNT);

static void Var_TryConvertString(benchmark::State& state) {
    memepp::string s("12345");
    memepp::variant v(s);
    for (auto _ : state) {
        std::string out;
        benchmark::DoNotOptimize(v.try_convert(out));
    }
}
BENCHMARK(Var_TryConvertString)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Var_IsNull(benchmark::State& state) {
    memepp::variant v;
    for (auto _ : state) benchmark::DoNotOptimize(v.is_null());
}
BENCHMARK(Var_IsNull)->Iterations(MMBM_ITERATION_COUNT);

static void Var_TypeId(benchmark::State& state) {
    memepp::variant v(int64_t(42));
    for (auto _ : state) benchmark::DoNotOptimize(v.type_id());
}
BENCHMARK(Var_TypeId)->Iterations(MMBM_ITERATION_COUNT);

static void Var_Swap(benchmark::State& state) {
    memepp::variant a(int64_t(1)), b(int64_t(2));
    for (auto _ : state) {
        a.swap(b);
        benchmark::DoNotOptimize(a.type_id());
    }
}
BENCHMARK(Var_Swap)->Iterations(MMBM_ITERATION_COUNT);

static void Var_Reset(benchmark::State& state) {
    memepp::variant v(int64_t(42));
    for (auto _ : state) {
        v.reset();
        v = int64_t(42);
        benchmark::DoNotOptimize(v.is_null());
    }
}
BENCHMARK(Var_Reset)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// varts -- variant + timestamp + user data, memepp only
// ===========================================================================
static void Varts_CtorVariant(benchmark::State& state) {
    memepp::variant v(int64_t(42));
    for (auto _ : state) benchmark::DoNotOptimize(memepp::varts(v).timestamp());
}
BENCHMARK(Varts_CtorVariant)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Varts_CtorTimestamp(benchmark::State& state) {
    for (auto _ : state)
        benchmark::DoNotOptimize(memepp::varts(mgu_timestamp_t{ 1234567890 }).timestamp());
}
BENCHMARK(Varts_CtorTimestamp)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Varts_Timestamp(benchmark::State& state) {
    memepp::variant v(int64_t(42));
    memepp::varts vs(v, mgu_timestamp_t{ 1234567890 });
    for (auto _ : state) benchmark::DoNotOptimize(vs.timestamp());
}
BENCHMARK(Varts_Timestamp)->Iterations(MMBM_ITERATION_COUNT);

static void Varts_VarAccess(benchmark::State& state) {
    memepp::variant v(int64_t(42));
    memepp::varts vs(v, mgu_timestamp_t{ 1234567890 });
    for (auto _ : state) benchmark::DoNotOptimize(vs.var().type_id());
}
BENCHMARK(Varts_VarAccess)->Iterations(MMBM_ITERATION_COUNT);

static void Varts_Copy(benchmark::State& state) {
    memepp::variant v(int64_t(42));
    memepp::varts vs(v, mgu_timestamp_t{ 1234567890 });
    for (auto _ : state) {
        memepp::varts copy(vs);
        benchmark::DoNotOptimize(copy.timestamp());
    }
}
BENCHMARK(Varts_Copy)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// buffer_view vs std::string_view
// ===========================================================================
static void BufView_Ctor(benchmark::State& state) {
    memepp::buffer b(reinterpret_cast<const memepp::buffer::const_pointer>(kAscii),
                     static_cast<memepp::buffer::size_type>(std::strlen(kAscii)));
    for (auto _ : state) benchmark::DoNotOptimize(memepp::buffer_view(b).size());
}
BENCHMARK(BufView_Ctor)->Iterations(MMBM_ITERATION_COUNT);

static void StdView_CtorForBufView(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(std::string_view(kAscii).size());
}
BENCHMARK(StdView_CtorForBufView)->Iterations(MMBM_ITERATION_COUNT);

static void BufView_Size(benchmark::State& state) {
    memepp::buffer_view bv = make_bv(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(bv.size());
}
BENCHMARK(BufView_Size)->Iterations(MMBM_ITERATION_COUNT);

static void BufView_Find(benchmark::State& state) {
    memepp::buffer_view bv = make_bv(kAscii);
    memepp::buffer_view needle = make_bv("brown");
    for (auto _ : state) benchmark::DoNotOptimize(bv.find(needle));
}
BENCHMARK(BufView_Find)->Iterations(MMBM_ITERATION_COUNT);

static void StdView_FindForBufView(benchmark::State& state) {
    std::string_view sv(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(sv.find("brown"));
}
BENCHMARK(StdView_FindForBufView)->Iterations(MMBM_ITERATION_COUNT);

static void BufView_IndexOf(benchmark::State& state) {
    memepp::buffer_view bv = make_bv(kAscii);
    memepp::buffer_view needle = make_bv("brown");
    for (auto _ : state) benchmark::DoNotOptimize(bv.index_of(needle));
}
BENCHMARK(BufView_IndexOf)->Iterations(MMBM_ITERATION_COUNT);

} // namespace
