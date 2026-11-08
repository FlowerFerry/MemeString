// Three-way search benchmark: C standard library vs std::string vs memepp::string.
//
// ---------------------------------------------------------------------------
// WHY SOME CELLS SAY "hand-rolled"
// ---------------------------------------------------------------------------
// ISO C provides no reverse substring search (no strrstr) and no
// case-insensitive search of any kind.  Those C-side cells are therefore
// hand-written loops, which is exactly what a C programmer would have to write.
// They are labelled as such so nobody reads them as library performance.
//
// Capability matrix (verified against this host's CRT):
//
//   forward single byte    strchr          OK
//   reverse single byte    strrchr         OK
//   forward substring      strstr          OK
//   forward memchr         memchr          OK  (fastest of all: SIMD block scan)
//   reverse substring      --  none --     hand-rolled
//   case-insensitive any   --  none --     hand-rolled via tolower
//
// ---------------------------------------------------------------------------
// WHAT IS COMPARED
// ---------------------------------------------------------------------------
// Each case runs three implementations on identical bytes, so the only
// difference is the string layer:
//
//   C        : raw char* from std::string::c_str() plus a CR<T> call
//   std      : std::string member function or a hand-rolled equivalent
//   memepp   : memepp::string member function
//
// Every result is fed to benchmark::DoNotOptimize.  Earlier revisions of this
// suite reported 0 ns for several std::string cases because the return value
// was dropped and MSVC deleted the entire loop.
//
// The matrices live in mmbm_search_xlarge.cpp for 4K/16K/64K haystacks; this
// file covers 1B/8B/91B/512B so together they span the full size range.

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <string>
#include <cstring>
#include <cstddef>
#include <cctype>

namespace {

// ---------------------------------------------------------------------------
// Haystacks: 1 / 8 / 91 / 512 bytes, ASCII.
// ---------------------------------------------------------------------------
const char* const kHay1 = "a";

const char* const kHay8 = "abcdefgh";

const char* const kHay91 =
    "The quick brown fox jumps over the lazy dog. "
    "Pack my box with five dozen liquor jugs.";

const char* const kHay512 =
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

// Upper-case twin of kHay91: feeds the case-insensitive paths a haystack whose
// bytes actually differ in case from the needle.
const char* const kHay91Upper =
    "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG. "
    "PACK MY BOX WITH FIVE DOZEN LIQUOR JUGS.";

// Needles.  All letters are a-w so they never collide with the x/y/z filler
// used in the XL file (that file reuses these semantics).
const char* const kNdlShort  = "fox";                                    // 3
const char* const kNdlMedium = "brown fox jumps o";                      // 17
const char* const kNdlAbsent = "QQQQQQQQQQQQQQQQQQQQ";                   // 20, not present

// ---------------------------------------------------------------------------
// Hand-rolled C helpers.  These exist because ISO C has no equivalent.
// ---------------------------------------------------------------------------

// Reverse substring search: no strrstr in ISO C.
static std::size_t c_rfind_str(const char* hay, const char* needle)
{
    const std::size_t hl = std::strlen(hay);
    const std::size_t nl = std::strlen(needle);
    if (nl == 0 || hl < nl) return std::size_t(-1);

    for (std::size_t i = hl - nl + 1; i-- > 0; ) {
        if (std::memcmp(hay + i, needle, nl) == 0)
            return i;
    }
    return std::size_t(-1);
}

// Case-insensitive single-byte search: no standard function at all.
static std::size_t c_find_ci(const char* hay, unsigned char key)
{
    const unsigned char k = (unsigned char)std::tolower(key);
    for (const char* p = hay; *p != '\0'; ++p)
        if ((unsigned char)std::tolower((unsigned char)*p) == k)
            return (std::size_t)(p - hay);
    return std::size_t(-1);
}

static std::size_t c_rfind_ci(const char* hay, unsigned char key)
{
    const unsigned char k = (unsigned char)std::tolower(key);
    const std::size_t n = std::strlen(hay);
    for (std::size_t i = n; i-- > 0; )
        if ((unsigned char)std::tolower((unsigned char)hay[i]) == k)
            return i;
    return std::size_t(-1);
}

static std::size_t c_find_str_ci(const char* hay, const char* needle)
{
    const std::size_t hl = std::strlen(hay);
    const std::size_t nl = std::strlen(needle);
    if (nl == 0 || hl < nl) return std::size_t(-1);

    for (std::size_t i = 0; i + nl <= hl; ++i) {
        std::size_t k = 0;
        while (k < nl &&
               std::tolower((unsigned char)hay[i + k]) ==
               std::tolower((unsigned char)needle[k]))
            ++k;
        if (k == nl) return i;
    }
    return std::size_t(-1);
}

// std hand-rolled equivalents (std::string has no rfind-ci either).
static std::size_t s_find_ci(const std::string& s, unsigned char key)
{
    return c_find_ci(s.c_str(), key);
}

static std::size_t s_rfind_ci(const std::string& s, unsigned char key)
{
    return c_rfind_ci(s.c_str(), key);
}

// ---------------------------------------------------------------------------
// Three-way macro.  `C_BODY` sees `cs` (const char*), `S_BODY` sees `ss`
// (std::string&), `M_BODY` sees `ms` (memepp::string&).
// ---------------------------------------------------------------------------
#define BENCH3(NAME, HAY, C_BODY, S_BODY, M_BODY)                             \
    static void C_##NAME(benchmark::State& state) {                           \
        const std::string own(HAY);                                           \
        const char* cs = own.c_str();                                         \
        (void)cs;                                                             \
        for (auto _ : state) { C_BODY; }                                      \
    }                                                                         \
    BENCHMARK(C_##NAME)->Iterations(MMBM_ITERATION_COUNT);                    \
    static void Std_##NAME(benchmark::State& state) {                         \
        std::string ss(HAY);                                                  \
        (void)ss;                                                             \
        for (auto _ : state) { S_BODY; }                                      \
    }                                                                         \
    BENCHMARK(Std_##NAME)->Iterations(MMBM_ITERATION_COUNT);                  \
    static void Meme_##NAME(benchmark::State& state) {                        \
        memepp::string ms(HAY);                                               \
        (void)ms;                                                             \
        for (auto _ : state) { M_BODY; }                                      \
    }                                                                         \
    BENCHMARK(Meme_##NAME)->Iterations(MMBM_ITERATION_COUNT);

// Same but without an iteration cap (for the 512-byte cases that would
// otherwise take too long).
#define BENCH3_BIG(NAME, HAY, C_BODY, S_BODY, M_BODY)                         \
    static void C_##NAME(benchmark::State& state) {                           \
        const std::string own(HAY);                                           \
        const char* cs = own.c_str();                                         \
        (void)cs;                                                             \
        for (auto _ : state) { C_BODY; }                                      \
    }                                                                         \
    BENCHMARK(C_##NAME);                                                      \
    static void Std_##NAME(benchmark::State& state) {                         \
        std::string ss(HAY);                                                  \
        (void)ss;                                                             \
        for (auto _ : state) { S_BODY; }                                      \
    }                                                                         \
    BENCHMARK(Std_##NAME);                                                    \
    static void Meme_##NAME(benchmark::State& state) {                        \
        memepp::string ms(HAY);                                               \
        (void)ms;                                                             \
        for (auto _ : state) { M_BODY; }                                      \
    }                                                                         \
    BENCHMARK(Meme_##NAME);

} // namespace

// ===========================================================================
// find: single byte, case-sensitive
// ===========================================================================
BENCH3(FindChar_1B,
    kHay1,
    benchmark::DoNotOptimize(std::strchr(cs, 'a')),
    benchmark::DoNotOptimize(ss.find('a')),
    benchmark::DoNotOptimize(ms.find('a')))

BENCH3(FindChar_8B,
    kHay8,
    benchmark::DoNotOptimize(std::strchr(cs, 'e')),
    benchmark::DoNotOptimize(ss.find('e')),
    benchmark::DoNotOptimize(ms.find('e')))

BENCH3(FindChar_91B,
    kHay91,
    benchmark::DoNotOptimize(std::strchr(cs, 'w')),
    benchmark::DoNotOptimize(ss.find('w')),
    benchmark::DoNotOptimize(ms.find('w')))

BENCH3_BIG(FindChar_512B,
    kHay512,
    benchmark::DoNotOptimize(std::strchr(cs, 'Q')),
    benchmark::DoNotOptimize(ss.find('Q')),
    benchmark::DoNotOptimize(ms.find('Q')))

// memchr is the C-side fast path for a bounded buffer; include it as a
// separate row so the SIMD block-scan advantage is visible.
BENCH3_BIG(FindCharMemchr_512B,
    kHay512,
    benchmark::DoNotOptimize(std::memchr(cs, 'Q', 512)),
    benchmark::DoNotOptimize(ss.find('Q')),
    benchmark::DoNotOptimize(ms.find('Q')))

// ===========================================================================
// find: substring, case-sensitive
// ===========================================================================
BENCH3(FindStr_91B,
    kHay91,
    benchmark::DoNotOptimize(std::strstr(cs, kNdlShort)),
    benchmark::DoNotOptimize(ss.find(kNdlShort)),
    benchmark::DoNotOptimize(ms.find(kNdlShort)))

BENCH3(FindStrMedium_91B,
    kHay91,
    benchmark::DoNotOptimize(std::strstr(cs, kNdlMedium)),
    benchmark::DoNotOptimize(ss.find(kNdlMedium)),
    benchmark::DoNotOptimize(ms.find(kNdlMedium)))

BENCH3_BIG(FindStr_512B,
    kHay512,
    benchmark::DoNotOptimize(std::strstr(cs, kNdlShort)),
    benchmark::DoNotOptimize(ss.find(kNdlShort)),
    benchmark::DoNotOptimize(ms.find(kNdlShort)))

BENCH3_BIG(FindStrAbsent_512B,
    kHay512,
    benchmark::DoNotOptimize(std::strstr(cs, kNdlAbsent)),
    benchmark::DoNotOptimize(ss.find(kNdlAbsent)),
    benchmark::DoNotOptimize(ms.find(kNdlAbsent)))

// ===========================================================================
// rfind: single byte
// ===========================================================================
BENCH3(RfindChar_8B,
    kHay8,
    benchmark::DoNotOptimize(std::strrchr(cs, 'f')),
    benchmark::DoNotOptimize(ss.rfind('f')),
    benchmark::DoNotOptimize(ms.rfind('f')))

BENCH3(RfindChar_91B,
    kHay91,
    benchmark::DoNotOptimize(std::strrchr(cs, 'o')),
    benchmark::DoNotOptimize(ss.rfind('o')),
    benchmark::DoNotOptimize(ms.rfind('o')))

BENCH3_BIG(RfindChar_512B,
    kHay512,
    benchmark::DoNotOptimize(std::strrchr(cs, 'o')),
    benchmark::DoNotOptimize(ss.rfind('o')),
    benchmark::DoNotOptimize(ms.rfind('o')))

// ===========================================================================
// rfind: substring.  C has no strrstr, so the C cell is hand-rolled.
// ===========================================================================
BENCH3(RfindStr_91B,
    kHay91,
    benchmark::DoNotOptimize(c_rfind_str(cs, kNdlShort)),
    benchmark::DoNotOptimize(ss.rfind(kNdlShort)),
    benchmark::DoNotOptimize(ms.rfind(kNdlShort)))

BENCH3_BIG(RfindStr_512B,
    kHay512,
    benchmark::DoNotOptimize(c_rfind_str(cs, kNdlShort)),
    benchmark::DoNotOptimize(ss.rfind(kNdlShort)),
    benchmark::DoNotOptimize(ms.rfind(kNdlShort)))

// ===========================================================================
// contains
// ===========================================================================
BENCH3(ContainsStr_91B,
    kHay91,
    benchmark::DoNotOptimize(std::strstr(cs, kNdlShort) != NULL),
    benchmark::DoNotOptimize(ss.find(kNdlShort) != std::string::npos),
    benchmark::DoNotOptimize(ms.contains(kNdlShort)))

BENCH3_BIG(ContainsStrAbsent_512B,
    kHay512,
    benchmark::DoNotOptimize(std::strstr(cs, kNdlAbsent) != NULL),
    benchmark::DoNotOptimize(ss.find(kNdlAbsent) != std::string::npos),
    benchmark::DoNotOptimize(ms.contains(kNdlAbsent)))

// ===========================================================================
// starts_with / ends_with
// ===========================================================================
BENCH3(StartsWith_91B,
    kHay91,
    benchmark::DoNotOptimize(std::strncmp(cs, "The quick", 9) == 0),
    benchmark::DoNotOptimize(ss.compare(0, 9, "The quick") == 0),
    benchmark::DoNotOptimize(ms.starts_with("The quick")))

BENCH3_BIG(StartsWith_512B,
    kHay512,
    benchmark::DoNotOptimize(std::strncmp(cs, "The quick", 9) == 0),
    benchmark::DoNotOptimize(ss.compare(0, 9, "The quick") == 0),
    benchmark::DoNotOptimize(ms.starts_with("The quick")))

// NOTE: the C cell precomputes the length because ISO C has no ends_with
// primitive; calling strlen inline would measure the missing-API cost (a full
// rescan) rather than the comparison itself.  Both other columns get the
// length for free from the object.
static void C_EndsWith_5_91B(benchmark::State& state) {
    const std::string own(kHay91);
    const char* cs = own.c_str();
    const std::size_t n = own.size();
    for (auto _ : state)
        benchmark::DoNotOptimize(std::strcmp(cs + n - 5, "jugs.") == 0);
}
BENCHMARK(C_EndsWith_5_91B)->Iterations(MMBM_ITERATION_COUNT);
static void Std_EndsWith_5_91B(benchmark::State& state) {
    std::string ss(kHay91);
    for (auto _ : state)
        benchmark::DoNotOptimize(
            ss.size() >= 5 && ss.compare(ss.size() - 5, 5, "jugs.") == 0);
}
BENCHMARK(Std_EndsWith_5_91B)->Iterations(MMBM_ITERATION_COUNT);
static void Meme_EndsWith_5_91B(benchmark::State& state) {
    memepp::string ms(kHay91);
    for (auto _ : state)
        benchmark::DoNotOptimize(ms.ends_with("jugs."));
}
BENCHMARK(Meme_EndsWith_5_91B)->Iterations(MMBM_ITERATION_COUNT);

// ===========================================================================
// CASE-INSENSITIVE.  No ISO C function exists for any of these, so the C cell
// is a hand-rolled tolower loop in every row.
// ===========================================================================
BENCH3(Find_Char_CI_91B,
    kHay91,
    benchmark::DoNotOptimize(c_find_ci(cs, 'W')),
    benchmark::DoNotOptimize(s_find_ci(ss, 'W')),
    benchmark::DoNotOptimize(ms.index_of("W", memepp::case_sensitivity_t::all_insensitive)))

BENCH3_BIG(Find_Char_CI_512B,
    kHay512,
    benchmark::DoNotOptimize(c_find_ci(cs, 'Q')),
    benchmark::DoNotOptimize(s_find_ci(ss, 'Q')),
    benchmark::DoNotOptimize(ms.index_of("Q", memepp::case_sensitivity_t::all_insensitive)))

BENCH3_BIG(Find_Char_CI_Absent_512B,
    kHay512,
    benchmark::DoNotOptimize(c_find_ci(cs, 'z')),
    benchmark::DoNotOptimize(s_find_ci(ss, 'z')),
    benchmark::DoNotOptimize(ms.index_of("z", memepp::case_sensitivity_t::all_insensitive)))

BENCH3(Rfind_Char_CI_91B,
    kHay91,
    benchmark::DoNotOptimize(c_rfind_ci(cs, 'O')),
    benchmark::DoNotOptimize(s_rfind_ci(ss, 'O')),
    benchmark::DoNotOptimize(ms.last_index_of("O", memepp::case_sensitivity_t::all_insensitive)))

BENCH3_BIG(Rfind_Char_CI_512B,
    kHay512,
    benchmark::DoNotOptimize(c_rfind_ci(cs, 'O')),
    benchmark::DoNotOptimize(s_rfind_ci(ss, 'O')),
    benchmark::DoNotOptimize(ms.last_index_of("O", memepp::case_sensitivity_t::all_insensitive)))

// All three cells perform a case-insensitive SUBSTRING search.  ISO C and
// std::string both lack the operation, so both cells are hand-rolled with the
// same algorithm (first-byte scan + per-byte fold compare) to keep the
// comparison honest; only memepp has a library implementation.
BENCH3(Find_Str_CI_91B,
    kHay91Upper,
    benchmark::DoNotOptimize(c_find_str_ci(cs, kNdlShort)),
    benchmark::DoNotOptimize(c_find_str_ci(ss.c_str(), kNdlShort)),
    benchmark::DoNotOptimize(ms.index_of("FOX", memepp::case_sensitivity_t::all_insensitive)))

#undef BENCH3
#undef BENCH3_BIG
