// Final coverage pass: std/C counterparts for the operators and containers that
// were still one-sided, plus the API members that had never been called.
//
// API surface verified against the headers:
//   string_def.hpp       : bytes/length/native_handle/rbegin/rend/rune_begin/
//                          cbegin/cend/rune_end/to_rune_iterator/assign/
//                          capacity/max_size/reset/swap
//   string_span_def.hpp  : to_string_ref
//   buffer_span_def.hpp  : to_variable_buffer
//   rune_def.hpp         : is_en_upper/is_en_lower/is_en_punct
//   variant_def.hpp      : is_type/convert_or/get_or
//   varts_def.hpp        : is_ts_valid/ms_part/time_part/set_timestamp/
//                          set_userdata/userdata/make_now
//   string_span_fwd / fwd : mm_span / mm_bspan free functions

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/string_span.hpp>
#include <memepp/buffer_span.hpp>
#include <memepp/buffer.hpp>
#include <memepp/variable_buffer.hpp>
#include <memepp/rune.hpp>
#include <memepp/variant.hpp>
#include <memepp/varts.hpp>

#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <cstring>

namespace {

const char* kA = "Hello World!";
const char* kB = "Hello Apple!";    // differs at '/' -> 'A'
const char* kAscii =
    "The quick brown fox jumps over the lazy dog. Pack my box with five dozen.";
const char* kUtf8 = "Hello \xE4\xB8\xAD\xE6\x96\x87 world end";

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
// 1. std counterparts for the string relational operators
// ===========================================================================
static void MemeStr_OpNe_StrStr2(benchmark::State& state) {
    memepp::string a(kA), b(kB);
    for (auto _ : state) benchmark::DoNotOptimize(a != b);
}
BENCHMARK(MemeStr_OpNe_StrStr2)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_OpNe(benchmark::State& state) {
    std::string a(kA), b(kB);
    for (auto _ : state) benchmark::DoNotOptimize(a != b);
}
BENCHMARK(StdStr_OpNe)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_OpLt2(benchmark::State& state) {
    memepp::string a(kB), b(kA);
    for (auto _ : state) benchmark::DoNotOptimize(a < b);
}
BENCHMARK(MemeStr_OpLt2)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_OpLt(benchmark::State& state) {
    std::string a(kB), b(kA);
    for (auto _ : state) benchmark::DoNotOptimize(a < b);
}
BENCHMARK(StdStr_OpLt)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_OpGt(benchmark::State& state) {
    memepp::string a(kA), b(kB);
    for (auto _ : state) benchmark::DoNotOptimize(a > b);
}
BENCHMARK(MemeStr_OpGt)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_OpGt(benchmark::State& state) {
    std::string a(kA), b(kB);
    for (auto _ : state) benchmark::DoNotOptimize(a > b);
}
BENCHMARK(StdStr_OpGt)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_OpLe2(benchmark::State& state) {
    memepp::string a(kB), b(kA);
    for (auto _ : state) benchmark::DoNotOptimize(a <= b);
}
BENCHMARK(MemeStr_OpLe2)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_OpLe(benchmark::State& state) {
    std::string a(kB), b(kA);
    for (auto _ : state) benchmark::DoNotOptimize(a <= b);
}
BENCHMARK(StdStr_OpLe)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_OpGe(benchmark::State& state) {
    memepp::string a(kA), b(kB);
    for (auto _ : state) benchmark::DoNotOptimize(a >= b);
}
BENCHMARK(MemeStr_OpGe)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_OpGe(benchmark::State& state) {
    std::string a(kA), b(kB);
    for (auto _ : state) benchmark::DoNotOptimize(a >= b);
}
BENCHMARK(StdStr_OpGe)->Iterations(MMBM_ITERATION_COUNT);

// C counterparts for > and >= (C_OpLt / C_OpLe already exist)
static void C_OpGt(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(std::strcmp(kA, kB) > 0);
}
BENCHMARK(C_OpGt)->Iterations(MMBM_ITERATION_COUNT);

static void C_OpGe(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(std::strcmp(kA, kB) >= 0);
}
BENCHMARK(C_OpGe)->Iterations(MMBM_ITERATION_COUNT);

// ===========================================================================
// 2. operator+(char) / operator+(string_view) std counterparts
// ===========================================================================
static void MemeSb_PlusChar(benchmark::State& state) {
    for (auto _ : state) {
        memepp::string_builder sb;
        sb += memepp::string(kA);
        sb += '!';
        benchmark::DoNotOptimize(sb.size());
    }
}
BENCHMARK(MemeSb_PlusChar)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_PlusChar(benchmark::State& state) {
    for (auto _ : state) {
        std::string s(kA);
        s += '!';
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_PlusChar)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_PlusView(benchmark::State& state) {
    for (auto _ : state) {
        std::string s(kA);
        s.append(std::string_view(kB));
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(StdStr_PlusView)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// 3. join / split std counterparts
// ===========================================================================
static void MemeStr_JoinVecView2(benchmark::State& state) {
    memepp::string s(kAscii);
    std::vector<memepp::string_view> parts = { "alpha","bravo","charlie","delta" };
    for (auto _ : state) benchmark::DoNotOptimize(s.join(parts).size());
}
BENCHMARK(MemeStr_JoinVecView2)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_JoinVecView(benchmark::State& state) {
    std::string sep(", ");
    std::vector<std::string_view> parts = { "alpha","bravo","charlie","delta" };
    for (auto _ : state) {
        std::string out;
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i) out += sep;
            out += parts[i];
        }
        benchmark::DoNotOptimize(out.size());
    }
}
BENCHMARK(StdStr_JoinVecView)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_SplitMulti2(benchmark::State& state) {
    memepp::string s("alpha::bravo::charlie::delta::echo");
    for (auto _ : state) {
        std::vector<memepp::string_view> out;
        s.split("::", memepp::split_behavior_t::skip_empty_parts, std::back_inserter(out));
        benchmark::DoNotOptimize(out.size());
    }
}
BENCHMARK(MemeStr_SplitMulti2)->Iterations(MMBM_ITERATION_COUNT / 100);

static void StdStr_SplitMulti(benchmark::State& state) {
    std::string s("alpha::bravo::charlie::delta::echo");
    for (auto _ : state) {
        std::vector<std::string_view> out;
        std::string_view sv(s);
        size_t start = 0;
        for (;;) {
            size_t at = sv.find("::", start);
            if (at == std::string_view::npos) { out.push_back(sv.substr(start)); break; }
            out.push_back(sv.substr(start, at - start));
            start = at + 2;
        }
        benchmark::DoNotOptimize(out.size());
    }
}
BENCHMARK(StdStr_SplitMulti)->Iterations(MMBM_ITERATION_COUNT / 100);

static void MemeStr_SplitKeepEmpty2(benchmark::State& state) {
    memepp::string s("alpha,,charlie,,echo");
    for (auto _ : state) {
        std::vector<memepp::string_view> out;
        s.split(",", memepp::split_behavior_t::keep_empty_parts, std::back_inserter(out));
        benchmark::DoNotOptimize(out.size());
    }
}
BENCHMARK(MemeStr_SplitKeepEmpty2)->Iterations(MMBM_ITERATION_COUNT / 100);

static void StdStr_SplitKeepEmpty(benchmark::State& state) {
    std::string s("alpha,,charlie,,echo");
    for (auto _ : state) {
        std::vector<std::string_view> out;
        std::string_view sv(s);
        size_t start = 0;
        for (;;) {
            size_t at = sv.find(',', start);
            if (at == std::string_view::npos) { out.push_back(sv.substr(start)); break; }
            out.push_back(sv.substr(start, at - start));   // empty parts kept
            start = at + 1;
        }
        benchmark::DoNotOptimize(out.size());
    }
}
BENCHMARK(StdStr_SplitKeepEmpty)->Iterations(MMBM_ITERATION_COUNT / 100);

// ===========================================================================
// 4. trim_if std counterpart (find_if)
// ===========================================================================
static void MemeStr_TrimIfSpace2(benchmark::State& state) {
    memepp::string s("      hello world      ");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.trim_if(
            [](memepp::string::value_type c) { return c == ' '; }).size());
}
BENCHMARK(MemeStr_TrimIfSpace2)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_TrimIfEquiv(benchmark::State& state) {
    std::string s("      hello world      ");
    auto is_sp = [](char c) { return c == ' '; };
    for (auto _ : state) {
        auto b = std::find_if_not(s.begin(), s.end(), is_sp);
        auto e = std::find_if_not(s.rbegin(), s.rend(), is_sp).base();
        benchmark::DoNotOptimize(static_cast<size_t>(e - b));
    }
}
BENCHMARK(StdStr_TrimIfEquiv)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// 5. buffer operators vs std::vector
// ===========================================================================
static void Buf_OpEq2(benchmark::State& state) {
    memepp::buffer a(reinterpret_cast<const memepp::buffer::const_pointer>(kAscii),
                     static_cast<memepp::buffer::size_type>(std::strlen(kAscii)));
    memepp::buffer b(a);
    for (auto _ : state) benchmark::DoNotOptimize(a == b);
}
BENCHMARK(Buf_OpEq2)->Iterations(MMBM_ITERATION_COUNT);

static void Vec_OpEq(benchmark::State& state) {
    std::vector<uint8_t> a(kAscii, kAscii + std::strlen(kAscii));
    std::vector<uint8_t> b(a);
    for (auto _ : state) benchmark::DoNotOptimize(a == b);
}
BENCHMARK(Vec_OpEq)->Iterations(MMBM_ITERATION_COUNT);

static void Buf_OpNe2(benchmark::State& state) {
    memepp::buffer a(reinterpret_cast<const memepp::buffer::const_pointer>(kAscii),
                     static_cast<memepp::buffer::size_type>(20));
    memepp::buffer b(reinterpret_cast<const memepp::buffer::const_pointer>(kAscii),
                     static_cast<memepp::buffer::size_type>(40));
    for (auto _ : state) benchmark::DoNotOptimize(a != b);
}
BENCHMARK(Buf_OpNe2)->Iterations(MMBM_ITERATION_COUNT);

static void Vec_OpNe(benchmark::State& state) {
    std::vector<uint8_t> a(kAscii, kAscii + 20);
    std::vector<uint8_t> b(kAscii, kAscii + 40);
    for (auto _ : state) benchmark::DoNotOptimize(a != b);
}
BENCHMARK(Vec_OpNe)->Iterations(MMBM_ITERATION_COUNT);

static void Buf_OpLt2(benchmark::State& state) {
    memepp::buffer a(reinterpret_cast<const memepp::buffer::const_pointer>(kAscii),
                     static_cast<memepp::buffer::size_type>(20));
    memepp::buffer b(reinterpret_cast<const memepp::buffer::const_pointer>(kAscii),
                     static_cast<memepp::buffer::size_type>(40));
    for (auto _ : state) benchmark::DoNotOptimize(a < b);
}
BENCHMARK(Buf_OpLt2)->Iterations(MMBM_ITERATION_COUNT);

static void Vec_OpLt(benchmark::State& state) {
    std::vector<uint8_t> a(kAscii, kAscii + 20);
    std::vector<uint8_t> b(kAscii, kAscii + 40);
    for (auto _ : state) benchmark::DoNotOptimize(a < b);
}
BENCHMARK(Vec_OpLt)->Iterations(MMBM_ITERATION_COUNT);

// ===========================================================================
// 6. C counterparts for variable_buffer mutators
// ===========================================================================
static void Vb_Remove(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = make_vb(kAscii);
        vb.remove(10, 20);
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(Vb_Remove)->Iterations(MMBM_ITERATION_COUNT / 10);

static void C_MemmoveRemove(benchmark::State& state) {
    const size_t n = std::strlen(kAscii);
    for (auto _ : state) {
        char* buf = static_cast<char*>(std::malloc(n + 1));
        std::memcpy(buf, kAscii, n + 1);
        // erase [10, 30): memmove the tail 20 bytes left
        std::memmove(buf + 10, buf + 30, n - 30 + 1);
        benchmark::DoNotOptimize(std::strlen(buf));
        std::free(buf);
    }
}
BENCHMARK(C_MemmoveRemove)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vb_PrependC(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = make_vb(kAscii);
        vb.prepend(vb_p("PRE"), static_cast<vb_t::size_type>(3));
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(Vb_PrependC)->Iterations(MMBM_ITERATION_COUNT / 10);

static void C_MemmovePrepend(benchmark::State& state) {
    const size_t n = std::strlen(kAscii);
    for (auto _ : state) {
        char* buf = static_cast<char*>(std::malloc(n + 4));
        std::memcpy(buf + 3, kAscii, n + 1);
        std::memcpy(buf, "PRE", 3);
        benchmark::DoNotOptimize(std::strlen(buf));
        std::free(buf);
    }
}
BENCHMARK(C_MemmovePrepend)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vb_PushFrontC(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = make_vb(kAscii);
        vb.push_front(static_cast<vb_t::value_type>('x'));
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(Vb_PushFrontC)->Iterations(MMBM_ITERATION_COUNT / 10);

static void C_MemmovePushFront(benchmark::State& state) {
    const size_t n = std::strlen(kAscii);
    for (auto _ : state) {
        char* buf = static_cast<char*>(std::malloc(n + 2));
        std::memcpy(buf + 1, kAscii, n + 1);
        buf[0] = 'x';
        benchmark::DoNotOptimize(std::strlen(buf));
        std::free(buf);
    }
}
BENCHMARK(C_MemmovePushFront)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vb_PopFrontC(benchmark::State& state) {
    for (auto _ : state) {
        vb_t vb = make_vb(kAscii);
        vb.pop_front();
        benchmark::DoNotOptimize(vb.size());
    }
}
BENCHMARK(Vb_PopFrontC)->Iterations(MMBM_ITERATION_COUNT / 10);

static void C_MemmovePopFront(benchmark::State& state) {
    const size_t n = std::strlen(kAscii);
    for (auto _ : state) {
        char* buf = static_cast<char*>(std::malloc(n + 1));
        std::memcpy(buf, kAscii, n + 1);
        std::memmove(buf, buf + 1, n);
        benchmark::DoNotOptimize(std::strlen(buf));
        std::free(buf);
    }
}
BENCHMARK(C_MemmovePopFront)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Vb_SliceC(benchmark::State& state) {
    vb_t vb = make_vb(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(vb.slice(10, 40).size());
}
BENCHMARK(Vb_SliceC)->Iterations(MMBM_ITERATION_COUNT / 10);

static void C_SliceC(benchmark::State& state) {
    const char* s = kAscii;
    for (auto _ : state) {
        char* out = static_cast<char*>(std::malloc(41));
        std::memcpy(out, s + 10, 40);
        out[40] = '\0';
        benchmark::DoNotOptimize(std::strlen(out));
        std::free(out);
    }
}
BENCHMARK(C_SliceC)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// 7. string_builder::prepend C counterpart
// ===========================================================================
static void Sb_PrependC(benchmark::State& state) {
    for (auto _ : state) {
        memepp::string_builder sb;
        sb += kAscii;
        sb.prepend("PRE");
        benchmark::DoNotOptimize(sb.size());
    }
}
BENCHMARK(Sb_PrependC)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// 8. string::swap / reset / assign / move-ctor C counterparts
// ===========================================================================
static void MemeStr_Swap2(benchmark::State& state) {
    memepp::string a(kA), b(kB);
    for (auto _ : state) {
        a.swap(b);
        benchmark::DoNotOptimize(a.size());
    }
}
BENCHMARK(MemeStr_Swap2)->Iterations(MMBM_ITERATION_COUNT);

static void C_SwapPointers(benchmark::State& state) {
    const char* p = kA;
    const char* q = kB;
    for (auto _ : state) {
        const char* t = p; p = q; q = t;
        benchmark::DoNotOptimize(p);
    }
}
BENCHMARK(C_SwapPointers)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_Reset2(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) {
        s.reset();
        s = memepp::string(kAscii);
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(MemeStr_Reset2)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_Assign2(benchmark::State& state) {
    memepp::string s;
    for (auto _ : state) {
        s = memepp::string(kAscii);
        benchmark::DoNotOptimize(s.size());
    }
}
BENCHMARK(MemeStr_Assign2)->Iterations(MMBM_ITERATION_COUNT / 10);

static void C_MallocFreeAssign(benchmark::State& state) {
    const size_t n = std::strlen(kAscii);
    for (auto _ : state) {
        void* p = std::malloc(n + 1);
        std::memcpy(p, kAscii, n + 1);
        benchmark::DoNotOptimize(p);
        std::free(p);
    }
}
BENCHMARK(C_MallocFreeAssign)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// 9. iterator C counterparts (member-function forms)
// ===========================================================================
static void MemeStr_IterFwd2(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) {
        size_t sum = 0;
        for (auto it = s.cbegin(); it != s.cend(); ++it) sum += *it;
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(MemeStr_IterFwd2)->Iterations(MMBM_ITERATION_COUNT / 10);

static void C_PtrIterFwd(benchmark::State& state) {
    const char* p = kAscii;
    const size_t n = std::strlen(kAscii);
    for (auto _ : state) {
        size_t sum = 0;
        for (const char* it = p; it != p + n; ++it) sum += static_cast<unsigned char>(*it);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(C_PtrIterFwd)->Iterations(MMBM_ITERATION_COUNT / 10);

static void MemeStr_IterRev2(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) {
        size_t sum = 0;
        for (auto it = s.rbegin(); it != s.rend(); ++it) sum += *it;
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(MemeStr_IterRev2)->Iterations(MMBM_ITERATION_COUNT / 10);

static void C_PtrIterRev(benchmark::State& state) {
    const char* p = kAscii;
    const size_t n = std::strlen(kAscii);
    for (auto _ : state) {
        size_t sum = 0;
        for (const char* it = p + n; it != p; ) sum += static_cast<unsigned char>(*--it);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(C_PtrIterRev)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// 10. previously uncalled members
// ===========================================================================
static void Str_Bytes(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(s.bytes());
}
BENCHMARK(Str_Bytes)->Iterations(MMBM_ITERATION_COUNT);

static void Str_Length(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(s.length());
}
BENCHMARK(Str_Length)->Iterations(MMBM_ITERATION_COUNT);

static void Str_NativeHandle(benchmark::State& state) {
    memepp::string s(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(static_cast<const void*>(&s.native_handle()));
}
BENCHMARK(Str_NativeHandle)->Iterations(MMBM_ITERATION_COUNT);

static void Str_RuneBeginEnd(benchmark::State& state) {
    memepp::string s(kUtf8);
    for (auto _ : state) {
        int n = 0;
        for (auto it = s.rune_begin(); it != s.rune_end(); ++it) ++n;
        benchmark::DoNotOptimize(n);
    }
}
BENCHMARK(Str_RuneBeginEnd)->Iterations(MMBM_ITERATION_COUNT / 100);

static void Str_ToRuneIterator(benchmark::State& state) {
    memepp::string s(kUtf8);
    for (auto _ : state) benchmark::DoNotOptimize(s.to_rune_iterator(6).to_rune().codepoint());
}
BENCHMARK(Str_ToRuneIterator)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Span_ToStringRef(benchmark::State& state) {
    memepp::string_span sp(kAscii);
    for (auto _ : state) benchmark::DoNotOptimize(sp.to_string_ref().size());
}
BENCHMARK(Span_ToStringRef)->Iterations(MMBM_ITERATION_COUNT / 10);

static void BSpan_ToVariableBuffer(benchmark::State& state) {
    memepp::buffer_span sp(reinterpret_cast<const memepp::buffer_span::const_pointer>(kAscii),
                           static_cast<memepp::buffer_span::size_type>(std::strlen(kAscii)));
    for (auto _ : state) benchmark::DoNotOptimize(sp.to_variable_buffer().size());
}
BENCHMARK(BSpan_ToVariableBuffer)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Rune_IsEnUpper(benchmark::State& state) {
    const memepp::rune r('A');
    for (auto _ : state) benchmark::DoNotOptimize(r.is_en_upper());
}
BENCHMARK(Rune_IsEnUpper)->Iterations(MMBM_ITERATION_COUNT);

static void Rune_IsEnLower(benchmark::State& state) {
    const memepp::rune r('a');
    for (auto _ : state) benchmark::DoNotOptimize(r.is_en_lower());
}
BENCHMARK(Rune_IsEnLower)->Iterations(MMBM_ITERATION_COUNT);

static void Rune_IsEnPunct(benchmark::State& state) {
    const memepp::rune r('!');
    for (auto _ : state) benchmark::DoNotOptimize(r.is_en_punct());
}
BENCHMARK(Rune_IsEnPunct)->Iterations(MMBM_ITERATION_COUNT);

static void Var_IsType(benchmark::State& state) {
    memepp::variant v(int64_t(42));
    for (auto _ : state)
        benchmark::DoNotOptimize(v.is_type(v.type_id()));
}
BENCHMARK(Var_IsType)->Iterations(MMBM_ITERATION_COUNT);

static void Var_ConvertOr(benchmark::State& state) {
    memepp::string src("12345");
    memepp::variant v(src);
    for (auto _ : state) benchmark::DoNotOptimize(v.convert_or<std::string>("-1"));
}
BENCHMARK(Var_ConvertOr)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Var_GetOr2(benchmark::State& state) {
    memepp::variant v(int64_t(42));
    for (auto _ : state) benchmark::DoNotOptimize(v.get_or<int64_t>(-1));
}
BENCHMARK(Var_GetOr2)->Iterations(MMBM_ITERATION_COUNT);

static void Varts_IsTsValid(benchmark::State& state) {
    memepp::variant v(int64_t(1));
    memepp::varts vs(v, mgu_timestamp_t{ 1234567890 });
    for (auto _ : state) benchmark::DoNotOptimize(vs.is_ts_valid());
}
BENCHMARK(Varts_IsTsValid)->Iterations(MMBM_ITERATION_COUNT);

static void Varts_TimePart(benchmark::State& state) {
    memepp::variant v(int64_t(1));
    memepp::varts vs(v, mgu_timestamp_t{ 1234567890 });
    for (auto _ : state) benchmark::DoNotOptimize(vs.time_part());
}
BENCHMARK(Varts_TimePart)->Iterations(MMBM_ITERATION_COUNT);

static void Varts_MsPart(benchmark::State& state) {
    memepp::variant v(int64_t(1));
    memepp::varts vs(v, mgu_timestamp_t{ 1234567890 });
    for (auto _ : state) benchmark::DoNotOptimize(vs.ms_part());
}
BENCHMARK(Varts_MsPart)->Iterations(MMBM_ITERATION_COUNT);

static void Varts_Userdata(benchmark::State& state) {
    memepp::variant v(int64_t(1));
    memepp::varts vs(v, mgu_timestamp_t{ 1234567890 }, 7);
    for (auto _ : state) benchmark::DoNotOptimize(vs.userdata());
}
BENCHMARK(Varts_Userdata)->Iterations(MMBM_ITERATION_COUNT);

static void Varts_SetTimestamp(benchmark::State& state) {
    memepp::variant v(int64_t(1));
    memepp::varts vs(v, mgu_timestamp_t{ 0 });
    for (auto _ : state) {
        vs.set_timestamp(mgu_timestamp_t{ 1234567890 });
        benchmark::DoNotOptimize(vs.timestamp());
    }
}
BENCHMARK(Varts_SetTimestamp)->Iterations(MMBM_ITERATION_COUNT);

static void Varts_SetUserdata(benchmark::State& state) {
    memepp::variant v(int64_t(1));
    memepp::varts vs(v, mgu_timestamp_t{ 0 });
    for (auto _ : state) {
        vs.set_userdata(7);
        benchmark::DoNotOptimize(vs.userdata());
    }
}
BENCHMARK(Varts_SetUserdata)->Iterations(MMBM_ITERATION_COUNT);

static void Varts_MakeNow(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(memepp::varts::make_now().timestamp());
}
BENCHMARK(Varts_MakeNow)->Iterations(MMBM_ITERATION_COUNT / 10);

static void Varts_MakeNowVariant(benchmark::State& state) {
    memepp::variant v(int64_t(42));
    for (auto _ : state)
        benchmark::DoNotOptimize(memepp::varts::make_now(v).timestamp());
}
BENCHMARK(Varts_MakeNowVariant)->Iterations(MMBM_ITERATION_COUNT / 10);

} // namespace
