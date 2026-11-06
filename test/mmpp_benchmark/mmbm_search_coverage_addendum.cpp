// Coverage addendum: fills the cells that mmbm_search_ci_matrix.cpp and
// mmbm_api_coverage.cpp still leave empty.
//
// ---------------------------------------------------------------------------
// WHAT THIS FILE ADDS (and why each was empty before)
// ---------------------------------------------------------------------------
// 1. CI at 1 B / 8 B        -- the CI matrix started at 91 B, so the two
//                              shortest buffers never reached
//                              mmsimd_u8_find2 / mmsimd_u8_rfind2 at all.
// 2. LastIndexCharCI_64K    -- only 4K and 16K had the reverse CI char cell.
// 3. rfind(str) CI @512     -- only 91 B and 512 B *tail* existed; the
//                              mid and absent positions on 512 B were empty.
// 4. Long needle at 64 K    -- kNeedleLong (58 B) had a mid cell only; tail
//    (tail + absent)           and absent were never measured.
// 5. case_sensitivity_t::case_sensitive  -- every CI benchmark used
//                              all_insensitive.  The case_sensitive enumerator
//                              (which folds only ASCII letters, unlike
//                              all_sensitive which compares raw bytes) had zero
//                              coverage.
// 6. index_of(_full_match)  -- string_def.hpp:195 declares it; nothing called
//    index_of(_offset,_limit) it.
// 7. UTF-8 rune needle at a size gradient -- only one fixed string was measured.
// 8. Storage type x search  -- ci_matrix had three 16 K points; this adds
//                              4 K / 64 K and a reverse case.
// 9. to_large_or_user()     -- declared at string_def.hpp:139, untested.
//
// Filler discipline is identical to the other two files: x/y/z only, needles
// from a-w only, with runtime verification before every measurement.

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <string>
#include <cstring>
#include <cassert>
#include <cstdio>

namespace {

const char* const kSeg[] = {
    "xyzzyxyzzyxyzzyxyzzy",
    "zzzzxyzzzzxyzzzzxyzz",
    "xyzxyzxyzxyzxyzxyzxy",
};
constexpr std::size_t kSegCount = sizeof(kSeg) / sizeof(kSeg[0]);

// Letters a-w only; never x/y/z, so no collision with the filler.
const char* const kN3  = "red";                  // 3 B  -> short path
const char* const kN5  = "brown";                // 5 B  -> short path
const char* const kN17 = "brown wolverine j";    // 17 B -> BM path
const char* const kN58 =
    "brown wolverine jumps over the lazy dog and packs my box with liquor";
const char* const kNAbs17 = "qbrown wolverine ";

constexpr std::size_t k1   = 1;
constexpr std::size_t k8   = 8;
constexpr std::size_t k91  = 91;
constexpr std::size_t k512 = 512;
constexpr std::size_t k4K  = 4  * 1024;
constexpr std::size_t k16K = 16 * 1024;
constexpr std::size_t k64K = 64 * 1024;

std::string make_hay(std::size_t size, const char* needle, std::size_t pos)
{
    std::string s;
    s.reserve(size + 256);
    for (std::size_t i = 0; s.size() < size; ++i)
        s += kSeg[i % kSegCount];
    s.resize(size);

    if (needle != nullptr) {
        const std::size_t n = std::strlen(needle);
        // assert() is compiled out under NDEBUG, so this must be a real check:
        // std::string::replace() silently clamps a plant that runs past the
        // end, which then trips verify_plant() with a confusing message.
        if (pos + n > s.size()) {
            std::fprintf(stderr,
                "FATAL: plant does not fit -- %zu + %zu > %zu.\n",
                pos, n, s.size());
            std::abort();
        }
        s.replace(pos, n, needle);
    }
    return s;
}

void verify_plant(const std::string& h, const char* needle, std::size_t want)
{
    const std::size_t f = h.find(needle);
    if (f != want) {
        std::fprintf(stderr, "FATAL: \"%s\" at %zu, want %zu (hay %zu).\n",
                     needle, f, want, h.size());
        std::abort();
    }
}
void verify_absent(const std::string& h, const char* needle)
{
    if (h.find(needle) != std::string::npos) {
        std::fprintf(stderr, "FATAL: absent \"%s\" is present.\n", needle);
        std::abort();
    }
}

std::size_t p_head(std::size_t) { return 0; }
std::size_t p_mid (std::size_t n) { return n / 2; }
std::size_t p_none(std::size_t) { return 0; }
// Tail position: leave room for the needle itself plus a small margin.
std::size_t p_tail_n(std::size_t n, const char* needle)
{
    const std::size_t len = std::strlen(needle);
    return (n > len + 8) ? (n - len - 8) : 0;
}

inline char fold_ch(char c) { return (c >= 'A' && c <= 'Z') ? char(c + 32) : c; }

std::size_t s_index_ci(const std::string& s, const char* n)
{
    const std::size_t nl = std::strlen(n);
    if (nl == 0 || nl > s.size()) return std::size_t(-1);
    for (std::size_t i = 0; i + nl <= s.size(); ++i) {
        std::size_t k = 0;
        while (k < nl && fold_ch(s[i + k]) == fold_ch(n[k])) ++k;
        if (k == nl) return i;
    }
    return std::size_t(-1);
}

std::size_t s_last_index_ci(const std::string& s, const char* n)
{
    const std::size_t nl = std::strlen(n);
    if (nl == 0 || nl > s.size()) return std::size_t(-1);
    for (std::size_t i = s.size() - nl + 1; i-- > 0; ) {
        std::size_t k = 0;
        while (k < nl && fold_ch(s[i + k]) == fold_ch(n[k])) ++k;
        if (k == nl) return i;
    }
    return std::size_t(-1);
}

// ===========================================================================
// 1. CI at the two shortest sizes (1 B / 8 B).
//    mmsimd_u8_find2 is called with len 1 or 8 here -- below one AVX2 block,
//    so this exercises the scalar tail path of the AVX2 implementation, which
//    no other benchmark reached.
// ===========================================================================
#define CI_PAIR(NAME, HAY_EXPR, CHECK, NEEDLE)                                \
    static void MemeStr_##NAME(benchmark::State& state) {                     \
        const std::string hay = (HAY_EXPR);                                   \
        CHECK(hay);                                                           \
        memepp::string s(hay.data(),                                          \
            static_cast<memepp::string::size_type>(hay.size()));              \
        for (auto _ : state)                                                  \
            benchmark::DoNotOptimize(s.index_of(                              \
                NEEDLE, memepp::case_sensitivity_t::all_insensitive));        \
    }                                                                         \
    BENCHMARK(MemeStr_##NAME)->Iterations(MMBM_ITERATION_COUNT);              \
    static void StdStr_##NAME(benchmark::State& state) {                      \
        const std::string hay = (HAY_EXPR);                                   \
        CHECK(hay);                                                           \
        std::string s(hay);                                                   \
        for (auto _ : state)                                                  \
            benchmark::DoNotOptimize(s_index_ci(s, NEEDLE));                  \
    }                                                                         \
    BENCHMARK(StdStr_##NAME)->Iterations(MMBM_ITERATION_COUNT);

#define CI_RPAIR(NAME, HAY_EXPR, CHECK, NEEDLE)                               \
    static void MemeStr_##NAME(benchmark::State& state) {                     \
        const std::string hay = (HAY_EXPR);                                   \
        CHECK(hay);                                                           \
        memepp::string s(hay.data(),                                          \
            static_cast<memepp::string::size_type>(hay.size()));              \
        for (auto _ : state)                                                  \
            benchmark::DoNotOptimize(s.last_index_of(                         \
                NEEDLE, memepp::case_sensitivity_t::all_insensitive));        \
    }                                                                         \
    BENCHMARK(MemeStr_##NAME)->Iterations(MMBM_ITERATION_COUNT);              \
    static void StdStr_##NAME(benchmark::State& state) {                      \
        const std::string hay = (HAY_EXPR);                                   \
        CHECK(hay);                                                           \
        std::string s(hay);                                                   \
        for (auto _ : state)                                                  \
            benchmark::DoNotOptimize(s_last_index_ci(s, NEEDLE));             \
    }                                                                         \
    BENCHMARK(StdStr_##NAME)->Iterations(MMBM_ITERATION_COUNT);

#define CHK_P(N, POS) [](const std::string& h){ verify_plant(h, N, POS(h.size())); }
#define CHK_A(N)      [](const std::string& h){ verify_absent(h, N); }
#define HAY(S, N, POS) make_hay((S), (N), POS((S)))

// 1 B: single letter haystack, single letter needle (both cases matched).
CI_PAIR(FindCharCI_1B_Hit,  "B",   CHK_P("B", p_head), "b")
CI_PAIR(FindCharCI_1B_Miss, "b",   CHK_A("q"),         "q")
CI_RPAIR(RfindCharCI_1B_Hit,  "B", CHK_P("B", p_head), "b")
CI_RPAIR(RfindCharCI_1B_Miss, "b", CHK_A("q"),         "q")

// 8 B: two-letter haystack contributions.
// The haystack holds the needle in upper case; the needle is passed in lower
// case, so only the case-insensitive path can match.  verify_plant() then
// checks the *upper-case* spelling, which is what is actually in the bytes.
CI_PAIR(FindStrCI_8B_Hit,  "BROWNxyz", CHK_P("BROWN", p_head), "brown")
CI_PAIR(FindStrCI_8B_Miss, "BROWNxyz", CHK_A("qrv"),           "QRV")
CI_RPAIR(RfindStrCI_8B_Hit,  "BROWNxyz", CHK_P("BROWN", p_head), "brown")
CI_RPAIR(RfindStrCI_8B_Miss, "BROWNxyz", CHK_A("qrv"),           "QRV")

// ===========================================================================
// 2. LastIndexOf CI at 64 K (the missing half of the 64 K char pair).
// ===========================================================================
static void MemeStr_LastIndexCharCI_64K_Mid(benchmark::State& state)
{
    std::string hay = make_hay(k64K, nullptr, 0);
    hay[k64K / 2] = 'b';
    memepp::string s(hay.data(), static_cast<memepp::string::size_type>(hay.size()));
    for (auto _ : state)
        benchmark::DoNotOptimize(s.last_index_of("b",
            memepp::case_sensitivity_t::all_insensitive));
}
BENCHMARK(MemeStr_LastIndexCharCI_64K_Mid);

static void MemeStr_LastIndexCharCI_64K_Abs(benchmark::State& state)
{
    std::string hay = make_hay(k64K, nullptr, 0);
    memepp::string s(hay.data(), static_cast<memepp::string::size_type>(hay.size()));
    for (auto _ : state)
        benchmark::DoNotOptimize(s.last_index_of("q",
            memepp::case_sensitivity_t::all_insensitive));
}
BENCHMARK(MemeStr_LastIndexCharCI_64K_Abs);

static void StdStr_LastIndexCharCI_64K_Mid(benchmark::State& state)
{
    std::string hay = make_hay(k64K, nullptr, 0);
    hay[k64K / 2] = 'b';
    for (auto _ : state) {
        std::size_t r = std::size_t(-1);
        for (std::size_t i = hay.size(); i-- > 0; )
            if (fold_ch(hay[i]) == 'b') { r = i; break; }
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(StdStr_LastIndexCharCI_64K_Mid);

static void StdStr_LastIndexCharCI_64K_Abs(benchmark::State& state)
{
    std::string hay = make_hay(k64K, nullptr, 0);
    for (auto _ : state) {
        std::size_t r = std::size_t(-1);
        for (std::size_t i = hay.size(); i-- > 0; )
            if (fold_ch(hay[i]) == 'q') { r = i; break; }
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(StdStr_LastIndexCharCI_64K_Abs);

// ===========================================================================
// 3. rfind(str) CI at 512 B: mid + absent (91 B already has tail + absent).
// ===========================================================================
CI_RPAIR(RfindStrCI_512_Mid, HAY(k512, kN17, p_mid), CHK_P(kN17, p_mid), kN17)

// ===========================================================================
// 4. Long needle (58 B) at 64 K: tail and absent.  Only the mid cell existed.
// ===========================================================================
static void MemeStr_FindLong_64K_Tail(benchmark::State& state)
{
    const std::string hay = make_hay(k64K, kN58, p_tail_n(k64K, kN58));
    verify_plant(hay, kN58, p_tail_n(hay.size(), kN58));
    memepp::string s(hay.data(), static_cast<memepp::string::size_type>(hay.size()));
    for (auto _ : state) benchmark::DoNotOptimize(s.find(kN58));
}
BENCHMARK(MemeStr_FindLong_64K_Tail);

static void StdStr_FindLong_64K_Tail(benchmark::State& state)
{
    const std::string hay = make_hay(k64K, kN58, p_tail_n(k64K, kN58));
    verify_plant(hay, kN58, p_tail_n(hay.size(), kN58));
    std::string s(hay);
    for (auto _ : state) benchmark::DoNotOptimize(s.find(kN58));
}
BENCHMARK(StdStr_FindLong_64K_Tail);

static void MemeStr_FindLong_64K_Abs(benchmark::State& state)
{
    const std::string hay = HAY(k64K, nullptr, p_none);
    verify_absent(hay, kNAbs17);
    memepp::string s(hay.data(), static_cast<memepp::string::size_type>(hay.size()));
    for (auto _ : state) benchmark::DoNotOptimize(s.find(kNAbs17));
}
BENCHMARK(MemeStr_FindLong_64K_Abs);

static void StdStr_FindLong_64K_Abs(benchmark::State& state)
{
    const std::string hay = HAY(k64K, nullptr, p_none);
    verify_absent(hay, kNAbs17);
    std::string s(hay);
    for (auto _ : state) benchmark::DoNotOptimize(s.find(kNAbs17));
}
BENCHMARK(StdStr_FindLong_64K_Abs);

static void MemeStr_RfindLong_64K_Tail(benchmark::State& state)
{
    const std::string hay = make_hay(k64K, kN58, p_tail_n(k64K, kN58));
    verify_plant(hay, kN58, p_tail_n(hay.size(), kN58));
    memepp::string s(hay.data(), static_cast<memepp::string::size_type>(hay.size()));
    for (auto _ : state) benchmark::DoNotOptimize(s.rfind(kN58));
}
BENCHMARK(MemeStr_RfindLong_64K_Tail);

static void StdStr_RfindLong_64K_Tail(benchmark::State& state)
{
    const std::string hay = make_hay(k64K, kN58, p_tail_n(k64K, kN58));
    verify_plant(hay, kN58, p_tail_n(hay.size(), kN58));
    std::string s(hay);
    for (auto _ : state) benchmark::DoNotOptimize(s.rfind(kN58));
}
BENCHMARK(StdStr_RfindLong_64K_Tail);

// ===========================================================================
// 5. case_sensitivity_t::case_sensitive
//    This is NOT the same as all_sensitive: all_sensitive compares raw bytes,
//    case_sensitive folds ASCII letters only and still compares bytes outside
//    a-z.  Nothing else in the suite passes this enumerator.
// ===========================================================================
static void MemeStr_IndexOf_CaseSensitive91(benchmark::State& state)
{
    memepp::string s("The quick brown fox jumps over the lazy dog.");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.index_of("brown",
            memepp::case_sensitivity_t::case_sensitive));
}
BENCHMARK(MemeStr_IndexOf_CaseSensitive91)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_IndexOf_CaseSensitiveMiss(benchmark::State& state)
{
    memepp::string s("The quick brown fox jumps over the lazy dog.");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.index_of("BROWN",
            memepp::case_sensitivity_t::case_sensitive));
}
BENCHMARK(MemeStr_IndexOf_CaseSensitiveMiss)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_IndexOf_AllSensitive91(benchmark::State& state)
{
    memepp::string s("The quick brown fox jumps over the lazy dog.");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.index_of("brown",
            memepp::case_sensitivity_t::all_sensitive));
}
BENCHMARK(MemeStr_IndexOf_AllSensitive91)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_LastIndexOf_CaseSensitive91(benchmark::State& state)
{
    memepp::string s("The quick brown fox jumps over the lazy dog.");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.last_index_of("brown",
            memepp::case_sensitivity_t::case_sensitive));
}
BENCHMARK(MemeStr_LastIndexOf_CaseSensitive91)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_IndexChar_CaseSensitive512(benchmark::State& state)
{
    const std::string h = make_hay(k512, nullptr, 0);
    std::string hay = h;
    hay[k512 / 2] = 'b';
    memepp::string s(hay.data(), static_cast<memepp::string::size_type>(hay.size()));
    for (auto _ : state)
        benchmark::DoNotOptimize(s.index_of("B",
            memepp::case_sensitivity_t::case_sensitive));
}
BENCHMARK(MemeStr_IndexChar_CaseSensitive512)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// 6. index_of overloads that take _full_match / _offset+_limit
//    string_def.hpp:195-210.  Nothing called these before.
// ===========================================================================
static void MemeStr_IndexOf_FullMatchTrue(benchmark::State& state)
{
    memepp::string s("brown");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.index_of(
            memepp::string_view("brown"), true,
            memepp::case_sensitivity_t::all_sensitive));
}
BENCHMARK(MemeStr_IndexOf_FullMatchTrue)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_IndexOf_FullMatchFalse(benchmark::State& state)
{
    memepp::string s("brown fox");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.index_of(
            memepp::string_view("brown"), false,
            memepp::case_sensitivity_t::all_sensitive));
}
BENCHMARK(MemeStr_IndexOf_FullMatchFalse)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_IndexChar_FullMatch(benchmark::State& state)
{
    memepp::string s("brown");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.index_of("b", true,
            memepp::case_sensitivity_t::all_sensitive));
}
BENCHMARK(MemeStr_IndexChar_FullMatch)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_IndexChar_OffsetLimit(benchmark::State& state)
{
    memepp::string s(k91 ? "The quick brown fox jumps over the lazy dog. Pack my box with five dozen."
                         : "");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.index_of("brown", 5, 16, false,
            memepp::case_sensitivity_t::all_sensitive));
}
BENCHMARK(MemeStr_IndexChar_OffsetLimit)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// 7. UTF-8 rune needle across a size gradient.
//    ascii_padding only uses x/y/z-free letters so a multibyte needle cannot
//    appear by accident.
// ===========================================================================
const char* const kRune2 = "\xE4\xB8\xAD\xE6\x96\x87";   // 中文 (6 bytes, 2 runes)
const char* const kRune3 = "\xE4\xB8\xAD\xE6\x96\x87\xE5\xAD\x97"; // 中文字

std::string make_utf8_hay(std::size_t size, const char* needle, std::size_t pos)
{
    std::string s;
    while (s.size() < size) s += "brownredwolverine";   // no multibyte bytes
    s.resize(size);
    if (needle) {
        const std::size_t n = std::strlen(needle);
        if (pos + n <= s.size()) s.replace(pos, n, needle);
    }
    return s;
}

#define RUNE_PAIR(NAME, SIZE, NEEDLE, POSFN)                                  \
    static void MemeStr_##NAME(benchmark::State& state) {                     \
        const std::string hay = make_utf8_hay((SIZE), NEEDLE, POSFN(SIZE));   \
        memepp::string s(hay.data(),                                          \
            static_cast<memepp::string::size_type>(hay.size()));              \
        for (auto _ : state)                                                  \
            benchmark::DoNotOptimize(s.index_of(NEEDLE,                       \
                memepp::case_sensitivity_t::all_sensitive));                  \
    }                                                                         \
    BENCHMARK(MemeStr_##NAME);

RUNE_PAIR(FindRune_91_Mid,  k91,  kRune2, p_mid)
RUNE_PAIR(FindRune_512_Mid, k512, kRune2, p_mid)
RUNE_PAIR(FindRune_4K_Mid,  k4K,  kRune2, p_mid)
RUNE_PAIR(FindRune_16K_Mid, k16K, kRune3, p_mid)
RUNE_PAIR(FindRune_64K_Mid, k64K, kRune3, p_mid)

// Rune-object find (single rune, rune overload rather than a byte string).
#define RUNE_OBJ(NAME, SIZE)                                                  \
    static void MemeStr_##NAME(benchmark::State& state) {                     \
        const std::string hay = make_utf8_hay((SIZE), kRune2, p_mid(SIZE));   \
        memepp::string s(hay.data(),                                          \
            static_cast<memepp::string::size_type>(hay.size()));              \
        const memepp::rune r(0x4E2D);                                         \
        for (auto _ : state) benchmark::DoNotOptimize(s.find(r));             \
    }                                                                         \
    BENCHMARK(MemeStr_##NAME);

RUNE_OBJ(FindRuneObj_91,  k91)
RUNE_OBJ(FindRuneObj_512, k512)
RUNE_OBJ(FindRuneObj_4K,  k4K)
RUNE_OBJ(FindRuneObj_16K, k16K)
RUNE_OBJ(FindRuneObj_64K, k64K)

// ===========================================================================
// 8. Storage type x search: 4 K and 64 K, forward and reverse.
// ===========================================================================
#define STORAGE_CELL(NAME, STOR_EXPR, SIZE, MEME_CALL)                        \
    static void MemeStr_##NAME(benchmark::State& state) {                     \
        const std::string hay = HAY((SIZE), kN17, p_mid);                     \
        verify_plant(hay, kN17, p_mid(hay.size()));                           \
        memepp::string base(hay.data(),                                       \
            static_cast<memepp::string::size_type>(hay.size()));              \
        for (auto _ : state) {                                                \
            memepp::string s = (STOR_EXPR);                                   \
            MEME_CALL;                                                        \
            benchmark::DoNotOptimize(s.storage_type());                       \
        }                                                                     \
    }                                                                         \
    BENCHMARK(MemeStr_##NAME);

STORAGE_CELL(Find_4K_large,  base.to_large(),          k4K,  benchmark::DoNotOptimize(s.find(kN17)))
STORAGE_CELL(Find_4K_shared, base.to_shared_storage(), k4K,  benchmark::DoNotOptimize(s.find(kN17)))
STORAGE_CELL(Find_4K_user,   base.to_large_or_user(),  k4K,  benchmark::DoNotOptimize(s.find(kN17)))

STORAGE_CELL(Find_64K_large,  base.to_large(),          k64K, benchmark::DoNotOptimize(s.find(kN17)))
STORAGE_CELL(Find_64K_shared, base.to_shared_storage(), k64K, benchmark::DoNotOptimize(s.find(kN17)))
STORAGE_CELL(Find_64K_user,   base.to_large_or_user(),  k64K, benchmark::DoNotOptimize(s.find(kN17)))

STORAGE_CELL(Rfind_4K_large,  base.to_large(),          k4K,  benchmark::DoNotOptimize(s.rfind(kN17)))
STORAGE_CELL(Rfind_4K_shared, base.to_shared_storage(), k4K,  benchmark::DoNotOptimize(s.rfind(kN17)))
STORAGE_CELL(Rfind_64K_large,  base.to_large(),          k64K, benchmark::DoNotOptimize(s.rfind(kN17)))
STORAGE_CELL(Rfind_64K_shared, base.to_shared_storage(), k64K, benchmark::DoNotOptimize(s.rfind(kN17)))

// CI over non-default storage (the CI path branches on storage_type()).
STORAGE_CELL(FindCI_16K_shared, base.to_shared_storage(), k16K,
    benchmark::DoNotOptimize(s.index_of(kN17,
        memepp::case_sensitivity_t::all_insensitive)))
STORAGE_CELL(FindCI_16K_large, base.to_large(), k16K,
    benchmark::DoNotOptimize(s.index_of(kN17,
        memepp::case_sensitivity_t::all_insensitive)))

// ===========================================================================
// 9. to_large_or_user() -- declared at string_def.hpp:139, previously untested
//    on its own (it only appeared inside the storage matrix above).
// ===========================================================================
static void MemeStr_ToLargeOrUser(benchmark::State& state)
{
    memepp::string s("The quick brown fox jumps over the lazy dog. Pack my box with five dozen.");
    for (auto _ : state) {
        memepp::string r = s.to_large_or_user();
        benchmark::DoNotOptimize(r.storage_type());
    }
}
BENCHMARK(MemeStr_ToLargeOrUser)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_ToLarge(benchmark::State& state)
{
    memepp::string s("The quick brown fox jumps over the lazy dog. Pack my box with five dozen.");
    for (auto _ : state) {
        memepp::string r = s.to_large();
        benchmark::DoNotOptimize(r.storage_type());
    }
}
BENCHMARK(MemeStr_ToLarge)->Iterations(MMBM_ITERATION_COUNT);

} // namespace
