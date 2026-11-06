// Case-insensitive + large-haystack search matrix.
//
// ---------------------------------------------------------------------------
// WHY THIS FILE EXISTS
// ---------------------------------------------------------------------------
// mmbm_search_3way.cpp covers CI only at 91B/512B, and mmbm_search_xlarge.cpp
// covers only case-SENSITIVE substring search at 4K/16K/64K.  The intersection
// "case-insensitive x large haystack" was completely empty, which means these
// code paths had never been measured:
//
//   MemeImpl_SearchByBoyerMooreWithSensitivity   (CI BM, large haystack)
//   MemeImpl_ReverseSearchByBoyerMooreWithSensitivity
//   __MemeImpl_shortForwardSearch  CI branch     (mmsimd_u8_find2)
//   __MemeImpl_shortReverseSearch  CI branch     (mmsimd_u8_rfind2)
//
// Also empty before this file:
//   * single-byte needle at 4K/16K/64K  (mmsimd_u8_find / mmsimd_u8_rfind path,
//     a different code path from the BM path exercised by 3B/17B/58B needles)
//   * the head/mid/tail/absent position dimension below 1 KiB
//
// ---------------------------------------------------------------------------
// FILLER CORRECTNESS
// ---------------------------------------------------------------------------
// Same discipline as mmbm_search_xlarge.cpp: filler uses only x / y / z, and
// every needle here uses only letters from a-w (in either case), so a needle
// can never occur accidentally in the filler.  verify_plant() / verify_absent()
// assert that at runtime and abort() if it is violated.
//
// CI is safe under this rule: folding 'A'-'W' maps onto 'a'-'w', and none of
// those bytes appear in the filler either.

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <string>
#include <cstring>
#include <cassert>
#include <cstdio>

namespace {

// ---------------------------------------------------------------------------
// Filler and needles
// ---------------------------------------------------------------------------
const char* const kSeg[] = {
    "xyzzyxyzzyxyzzyxyzzy",
    "zzzzxyzzzzxyzzzzxyzz",
    "xyzxyzxyzxyzxyzxyzxy",
};
constexpr std::size_t kSegCount = sizeof(kSeg) / sizeof(kSeg[0]);

// Needles: letters a-w only, never x/y/z, so they cannot collide with filler
// under either case-sensitivity mode.
const char* const kCiNeedle3  = "red";                    // 3 B  -> short path
const char* const kCiNeedle5  = "brown";                  // 5 B  -> short path
const char* const kCiNeedle17 = "brown wolverine j";      // 17 B -> BM path
const char* const kCiNeedleUp = "BROWN WOLVERINE J";      // 17 B, other case

// Absent needles: contain q, never present in filler.
const char* const kCiAbsent17 = "qbrown wolverine ";
const char* const kCiAbsent3  = "qrv";

// Single-byte CI needles.  string::index_of takes a C string, not a char,
// so the one-byte form is passed as a NUL-terminated literal.
const char  kCiCharHit  = 'b';    // planted
const char  kCiCharMiss = 'q';    // never present
const char* const kCiCharHitStr  = "b";
const char* const kCiCharMissStr = "q";

constexpr std::size_t k91  = 91;
constexpr std::size_t k512 = 512;
constexpr std::size_t k4K  = 4  * 1024;
constexpr std::size_t k16K = 16 * 1024;
constexpr std::size_t k64K = 64 * 1024;

// ---------------------------------------------------------------------------
// Haystack builder + runtime invariants
// ---------------------------------------------------------------------------
std::string make_hay(std::size_t size, const char* needle, std::size_t pos)
{
    std::string s;
    s.reserve(size + 128);
    for (std::size_t i = 0; s.size() < size; ++i)
        s += kSeg[i % kSegCount];
    s.resize(size);

    if (needle != nullptr) {
        const std::size_t n = std::strlen(needle);
        assert(pos + n <= s.size());
        s.replace(pos, n, needle);
    }
    return s;
}

std::string make_hay_ch(std::size_t size, char ch, std::size_t pos)
{
    std::string s = make_hay(size, nullptr, 0);
    s[pos] = ch;
    return s;
}

void verify_plant(const std::string& hay, const char* needle, std::size_t want)
{
    const std::size_t found = hay.find(needle);
    if (found != want) {
        std::fprintf(stderr,
            "FATAL: plant invariant broken -- \"%s\" first at %zu, expected %zu "
            "(haystack %zu bytes).  Filler collides with the needle.\n",
            needle, found, want, hay.size());
        std::abort();
    }
}

void verify_absent(const std::string& hay, const char* needle)
{
    if (hay.find(needle) != std::string::npos) {
        std::fprintf(stderr, "FATAL: absent needle \"%s\" is present.\n", needle);
        std::abort();
    }
}

void verify_char_plant(const std::string& hay, char ch, std::size_t want)
{
    const std::size_t found = hay.find(ch);
    if (found != want) {
        std::fprintf(stderr,
            "FATAL: char plant broken -- '%c' first at %zu, expected %zu.\n",
            ch, found, want);
        std::abort();
    }
}

void verify_char_absent(const std::string& hay, char ch)
{
    if (hay.find(ch) != std::string::npos) {
        std::fprintf(stderr, "FATAL: absent char '%c' is present.\n", ch);
        std::abort();
    }
}

// ---------------------------------------------------------------------------
// Position functions
// ---------------------------------------------------------------------------
std::size_t pos_head(std::size_t n) { return 8; }
std::size_t pos_mid (std::size_t n) { return n / 2; }
std::size_t pos_tail(std::size_t n) { return n - 40; }
std::size_t pos_none(std::size_t n) { return 0; }   // haystack with no needle

// ---------------------------------------------------------------------------
// Hand-rolled std::string equivalents.
// std::string has no case-insensitive search at all, so every CI std cell is
// hand-written -- exactly what a caller would have to do.  It folds with the
// same ASCII-only rule the library uses, so the comparison is algorithmic
// rather than locale-dependent.
// ---------------------------------------------------------------------------
inline char fold_ch(char c)
{
    return (c >= 'A' && c <= 'Z') ? static_cast<char>(c + 32) : c;
}

std::size_t s_index_of_ci(const std::string& s, const char* needle)
{
    const std::size_t n = s.size(), nl = std::strlen(needle);
    if (nl == 0 || nl > n) return static_cast<std::size_t>(-1);
    for (std::size_t i = 0; i + nl <= n; ++i) {
        std::size_t k = 0;
        while (k < nl && fold_ch(s[i + k]) == fold_ch(needle[k])) ++k;
        if (k == nl) return i;
    }
    return static_cast<std::size_t>(-1);
}

std::size_t s_last_index_of_ci(const std::string& s, const char* needle)
{
    const std::size_t n = s.size(), nl = std::strlen(needle);
    if (nl == 0 || nl > n) return static_cast<std::size_t>(-1);
    for (std::size_t i = n - nl + 1; i-- > 0; ) {
        std::size_t k = 0;
        while (k < nl && fold_ch(s[i + k]) == fold_ch(needle[k])) ++k;
        if (k == nl) return i;
    }
    return static_cast<std::size_t>(-1);
}

std::size_t s_index_of_ci_ch(const std::string& s, char ch)
{
    const char k = fold_ch(ch);
    for (std::size_t i = 0; i < s.size(); ++i)
        if (fold_ch(s[i]) == k) return i;
    return static_cast<std::size_t>(-1);
}

std::size_t s_last_index_of_ci_ch(const std::string& s, char ch)
{
    const char k = fold_ch(ch);
    for (std::size_t i = s.size(); i-- > 0; )
        if (fold_ch(s[i]) == k) return i;
    return static_cast<std::size_t>(-1);
}

// ---------------------------------------------------------------------------
// CI substring matrix: index_of / last_index_of, all_insensitive.
// ---------------------------------------------------------------------------
#define CI_PAIR(NAME, HAY_EXPR, CHECK, NEEDLE)                                \
    static void MemeStr_##NAME(benchmark::State& state) {                     \
        const std::string hay = (HAY_EXPR);                                   \
        CHECK(hay);                                                           \
        memepp::string s(hay.data(),                                          \
            static_cast<memepp::string::size_type>(hay.size()));              \
        for (auto _ : state) {                                                \
            benchmark::DoNotOptimize(                                         \
                s.index_of(NEEDLE, memepp::case_sensitivity_t::all_insensitive)); \
        }                                                                     \
    }                                                                         \
    BENCHMARK(MemeStr_##NAME);                                                \
    static void StdStr_##NAME(benchmark::State& state) {                      \
        const std::string hay = (HAY_EXPR);                                   \
        CHECK(hay);                                                           \
        std::string s(hay);                                                   \
        for (auto _ : state)                                                  \
            benchmark::DoNotOptimize(s_index_of_ci(s, NEEDLE));               \
    }                                                                         \
    BENCHMARK(StdStr_##NAME);

#define CI_RPAIR(NAME, HAY_EXPR, CHECK, NEEDLE)                               \
    static void MemeStr_##NAME(benchmark::State& state) {                     \
        const std::string hay = (HAY_EXPR);                                   \
        CHECK(hay);                                                           \
        memepp::string s(hay.data(),                                          \
            static_cast<memepp::string::size_type>(hay.size()));              \
        for (auto _ : state) {                                                \
            benchmark::DoNotOptimize(                                         \
                s.last_index_of(NEEDLE, memepp::case_sensitivity_t::all_insensitive)); \
        }                                                                     \
    }                                                                         \
    BENCHMARK(MemeStr_##NAME);                                                \
    static void StdStr_##NAME(benchmark::State& state) {                      \
        const std::string hay = (HAY_EXPR);                                   \
        CHECK(hay);                                                           \
        std::string s(hay);                                                   \
        for (auto _ : state)                                                  \
            benchmark::DoNotOptimize(s_last_index_of_ci(s, NEEDLE));          \
    }                                                                         \
    BENCHMARK(StdStr_##NAME);

#define CHK_P(NEEDLE, POSFN) \
    [](const std::string& h) { verify_plant(h, NEEDLE, POSFN(h.size())); }
#define CHK_A(NEEDLE) \
    [](const std::string& h) { verify_absent(h, NEEDLE); }
#define HAY(SIZE, NEEDLE, POSFN) make_hay((SIZE), (NEEDLE), POSFN((SIZE)))

// --- index_of CI, 17-byte needle (exercises the CI Boyer-Moore path) -------
CI_PAIR(FindStrCI_91_Head,  HAY(k91,  kCiNeedle17, pos_head),  CHK_P(kCiNeedle17, pos_head),  kCiNeedle17)
CI_PAIR(FindStrCI_91_Mid,   HAY(k91,  kCiNeedle17, pos_mid),   CHK_P(kCiNeedle17, pos_mid),   kCiNeedle17)
CI_PAIR(FindStrCI_91_Abs,   HAY(k91,  nullptr,     0),         CHK_A(kCiAbsent17),            kCiAbsent17)

CI_PAIR(FindStrCI_512_Head, HAY(k512, kCiNeedle17, pos_head),  CHK_P(kCiNeedle17, pos_head),  kCiNeedle17)
CI_PAIR(FindStrCI_512_Mid,  HAY(k512, kCiNeedle17, pos_mid),   CHK_P(kCiNeedle17, pos_mid),   kCiNeedle17)
CI_PAIR(FindStrCI_512_Tail, HAY(k512, kCiNeedle17, pos_tail),  CHK_P(kCiNeedle17, pos_tail),  kCiNeedle17)
CI_PAIR(FindStrCI_512_Abs,  HAY(k512, nullptr,     0),         CHK_A(kCiAbsent17),            kCiAbsent17)

CI_PAIR(FindStrCI_4K_Mid,   HAY(k4K,  kCiNeedle17, pos_mid),   CHK_P(kCiNeedle17, pos_mid),   kCiNeedle17)
CI_PAIR(FindStrCI_4K_Tail,  HAY(k4K,  kCiNeedle17, pos_tail),  CHK_P(kCiNeedle17, pos_tail),  kCiNeedle17)
CI_PAIR(FindStrCI_4K_Abs,   HAY(k4K,  nullptr,     0),         CHK_A(kCiAbsent17),            kCiAbsent17)

CI_PAIR(FindStrCI_16K_Mid,  HAY(k16K, kCiNeedle17, pos_mid),   CHK_P(kCiNeedle17, pos_mid),   kCiNeedle17)
CI_PAIR(FindStrCI_16K_Tail, HAY(k16K, kCiNeedle17, pos_tail),  CHK_P(kCiNeedle17, pos_tail),  kCiNeedle17)
CI_PAIR(FindStrCI_16K_Abs,  HAY(k16K, nullptr,     0),         CHK_A(kCiAbsent17),            kCiAbsent17)

CI_PAIR(FindStrCI_64K_Mid,  HAY(k64K, kCiNeedle17, pos_mid),   CHK_P(kCiNeedle17, pos_mid),   kCiNeedle17)
CI_PAIR(FindStrCI_64K_Tail, HAY(k64K, kCiNeedle17, pos_tail),  CHK_P(kCiNeedle17, pos_tail),  kCiNeedle17)
CI_PAIR(FindStrCI_64K_Abs,  HAY(k64K, nullptr,     0),         CHK_A(kCiAbsent17),            kCiAbsent17)

// --- index_of CI, 3-byte needle (exercises the short path + find2) ---------
CI_PAIR(FindShortCI_4K_Mid,  HAY(k4K,  kCiNeedle3, pos_mid),  CHK_P(kCiNeedle3, pos_mid),  kCiNeedle3)
CI_PAIR(FindShortCI_4K_Abs,  HAY(k4K,  nullptr,    0),        CHK_A(kCiAbsent3),           kCiAbsent3)
CI_PAIR(FindShortCI_16K_Mid, HAY(k16K, kCiNeedle3, pos_mid),  CHK_P(kCiNeedle3, pos_mid),  kCiNeedle3)
CI_PAIR(FindShortCI_16K_Abs, HAY(k16K, nullptr,    0),        CHK_A(kCiAbsent3),           kCiAbsent3)
CI_PAIR(FindShortCI_64K_Mid, HAY(k64K, kCiNeedle3, pos_mid),  CHK_P(kCiNeedle3, pos_mid),  kCiNeedle3)
CI_PAIR(FindShortCI_64K_Abs, HAY(k64K, nullptr,    0),        CHK_A(kCiAbsent3),           kCiAbsent3)

// --- last_index_of CI (exercises mmsimd_u8_rfind2 / reverse BM) ------------
CI_RPAIR(RfindStrCI_91_Tail,  HAY(k91,  kCiNeedle17, pos_tail), CHK_P(kCiNeedle17, pos_tail), kCiNeedle17)
CI_RPAIR(RfindStrCI_91_Abs,   HAY(k91,  nullptr,     0),        CHK_A(kCiAbsent17),           kCiAbsent17)

CI_RPAIR(RfindStrCI_512_Tail, HAY(k512, kCiNeedle17, pos_tail), CHK_P(kCiNeedle17, pos_tail), kCiNeedle17)
CI_RPAIR(RfindStrCI_512_Abs,  HAY(k512, nullptr,     0),        CHK_A(kCiAbsent17),           kCiAbsent17)

CI_RPAIR(RfindStrCI_4K_Tail,  HAY(k4K,  kCiNeedle17, pos_tail), CHK_P(kCiNeedle17, pos_tail), kCiNeedle17)
CI_RPAIR(RfindStrCI_4K_Mid,   HAY(k4K,  kCiNeedle17, pos_mid),  CHK_P(kCiNeedle17, pos_mid),  kCiNeedle17)
CI_RPAIR(RfindStrCI_4K_Abs,   HAY(k4K,  nullptr,     0),        CHK_A(kCiAbsent17),           kCiAbsent17)

CI_RPAIR(RfindStrCI_16K_Tail, HAY(k16K, kCiNeedle17, pos_tail), CHK_P(kCiNeedle17, pos_tail), kCiNeedle17)
CI_RPAIR(RfindStrCI_16K_Mid,  HAY(k16K, kCiNeedle17, pos_mid),  CHK_P(kCiNeedle17, pos_mid),  kCiNeedle17)
CI_RPAIR(RfindStrCI_16K_Abs,  HAY(k16K, nullptr,     0),        CHK_A(kCiAbsent17),           kCiAbsent17)

CI_RPAIR(RfindStrCI_64K_Tail, HAY(k64K, kCiNeedle17, pos_tail), CHK_P(kCiNeedle17, pos_tail), kCiNeedle17)
CI_RPAIR(RfindStrCI_64K_Mid,  HAY(k64K, kCiNeedle17, pos_mid),  CHK_P(kCiNeedle17, pos_mid),  kCiNeedle17)
CI_RPAIR(RfindStrCI_64K_Abs,  HAY(k64K, nullptr,     0),        CHK_A(kCiAbsent17),           kCiAbsent17)

CI_RPAIR(RfindShortCI_4K_Tail,  HAY(k4K,  kCiNeedle5, pos_tail), CHK_P(kCiNeedle5, pos_tail), kCiNeedle5)
CI_RPAIR(RfindShortCI_16K_Tail, HAY(k16K, kCiNeedle5, pos_tail), CHK_P(kCiNeedle5, pos_tail), kCiNeedle5)
CI_RPAIR(RfindShortCI_64K_Tail, HAY(k64K, kCiNeedle5, pos_tail), CHK_P(kCiNeedle5, pos_tail), kCiNeedle5)
CI_RPAIR(RfindShortCI_64K_Abs,  HAY(k64K, nullptr,    0),        CHK_A(kCiAbsent3),           kCiAbsent3)

// ---------------------------------------------------------------------------
// CI single byte at large sizes (mmsimd_u8_find2 / mmsimd_u8_rfind2 char path)
// ---------------------------------------------------------------------------
#define CI_CHAR_PAIR(NAME, HAY_EXPR, CHECK, CH, MEME_CALL, STD_CALL)          \
    static void MemeStr_##NAME(benchmark::State& state) {                     \
        const std::string hay = (HAY_EXPR);                                   \
        CHECK(hay);                                                           \
        memepp::string s(hay.data(),                                          \
            static_cast<memepp::string::size_type>(hay.size()));              \
        for (auto _ : state) { MEME_CALL; }                                   \
    }                                                                         \
    BENCHMARK(MemeStr_##NAME);                                                \
    static void StdStr_##NAME(benchmark::State& state) {                      \
        const std::string hay = (HAY_EXPR);                                   \
        CHECK(hay);                                                           \
        std::string s(hay);                                                   \
        for (auto _ : state) { STD_CALL; }                                    \
    }                                                                         \
    BENCHMARK(StdStr_##NAME);

#define CHK_CP(CH, POSFN) \
    [](const std::string& h) { verify_char_plant(h, CH, POSFN(h.size())); }
#define CHK_CA(CH) \
    [](const std::string& h) { verify_char_absent(h, CH); }
#define HAYC(SIZE, CH, POSFN) make_hay_ch((SIZE), (CH), POSFN((SIZE)))

CI_CHAR_PAIR(IndexCharCI_4K_Mid,  HAYC(k4K,  kCiCharHit, pos_mid),
    CHK_CP(kCiCharHit, pos_mid), kCiCharHit,
    benchmark::DoNotOptimize(s.index_of(kCiCharHitStr, memepp::case_sensitivity_t::all_insensitive)),
    benchmark::DoNotOptimize(s_index_of_ci_ch(s, kCiCharHit)))
CI_CHAR_PAIR(IndexCharCI_4K_Abs,  HAYC(k4K,  kCiCharHit, pos_mid),
    CHK_CA(kCiCharMiss), kCiCharMiss,
    benchmark::DoNotOptimize(s.index_of(kCiCharMissStr, memepp::case_sensitivity_t::all_insensitive)),
    benchmark::DoNotOptimize(s_index_of_ci_ch(s, kCiCharMiss)))
CI_CHAR_PAIR(IndexCharCI_16K_Mid, HAYC(k16K, kCiCharHit, pos_mid),
    CHK_CP(kCiCharHit, pos_mid), kCiCharHit,
    benchmark::DoNotOptimize(s.index_of(kCiCharHitStr, memepp::case_sensitivity_t::all_insensitive)),
    benchmark::DoNotOptimize(s_index_of_ci_ch(s, kCiCharHit)))
CI_CHAR_PAIR(IndexCharCI_16K_Abs, HAYC(k16K, kCiCharHit, pos_mid),
    CHK_CA(kCiCharMiss), kCiCharMiss,
    benchmark::DoNotOptimize(s.index_of(kCiCharMissStr, memepp::case_sensitivity_t::all_insensitive)),
    benchmark::DoNotOptimize(s_index_of_ci_ch(s, kCiCharMiss)))
CI_CHAR_PAIR(LastIndexCharCI_4K_Mid,  HAYC(k4K,  kCiCharHit, pos_mid),
    CHK_CP(kCiCharHit, pos_mid), kCiCharHit,
    benchmark::DoNotOptimize(s.last_index_of(kCiCharHitStr, memepp::case_sensitivity_t::all_insensitive)),
    benchmark::DoNotOptimize(s_last_index_of_ci_ch(s, kCiCharHit)))
CI_CHAR_PAIR(LastIndexCharCI_16K_Mid, HAYC(k16K, kCiCharHit, pos_mid),
    CHK_CP(kCiCharHit, pos_mid), kCiCharHit,
    benchmark::DoNotOptimize(s.last_index_of(kCiCharHitStr, memepp::case_sensitivity_t::all_insensitive)),
    benchmark::DoNotOptimize(s_last_index_of_ci_ch(s, kCiCharHit)))

// ---------------------------------------------------------------------------
// Case-SENSITIVE single byte at large sizes.
// mmsimd_u8_find / mmsimd_u8_rfind were never measured above 512 B; that is a
// different code path from the BM substring path.
// ---------------------------------------------------------------------------
#define CHAR_PAIR(NAME, HAY_EXPR, CHECK, CH, MEME_CALL, STD_CALL)             \
    CI_CHAR_PAIR(NAME, HAY_EXPR, CHECK, CH, MEME_CALL, STD_CALL)

#define STD_FIND(CH)     benchmark::DoNotOptimize(s.find(CH))
#define STD_RFIND(CH)    benchmark::DoNotOptimize(s.rfind(CH))
#define MEME_FIND(CH)    benchmark::DoNotOptimize(s.find(CH))
#define MEME_RFIND(CH)   benchmark::DoNotOptimize(s.rfind(CH))

CHAR_PAIR(FindChar_4K_Mid,  HAYC(k4K,  kCiCharHit, pos_mid), CHK_CP(kCiCharHit, pos_mid), kCiCharHit, MEME_FIND(kCiCharHit), STD_FIND(kCiCharHit))
CHAR_PAIR(FindChar_4K_Abs,  HAYC(k4K,  kCiCharHit, pos_mid), CHK_CA(kCiCharMiss),         kCiCharMiss, MEME_FIND(kCiCharMiss), STD_FIND(kCiCharMiss))
CHAR_PAIR(FindChar_16K_Mid, HAYC(k16K, kCiCharHit, pos_mid), CHK_CP(kCiCharHit, pos_mid), kCiCharHit, MEME_FIND(kCiCharHit), STD_FIND(kCiCharHit))
CHAR_PAIR(FindChar_16K_Abs, HAYC(k16K, kCiCharHit, pos_mid), CHK_CA(kCiCharMiss),         kCiCharMiss, MEME_FIND(kCiCharMiss), STD_FIND(kCiCharMiss))
CHAR_PAIR(FindChar_64K_Mid, HAYC(k64K, kCiCharHit, pos_mid), CHK_CP(kCiCharHit, pos_mid), kCiCharHit, MEME_FIND(kCiCharHit), STD_FIND(kCiCharHit))
CHAR_PAIR(FindChar_64K_Abs, HAYC(k64K, kCiCharHit, pos_mid), CHK_CA(kCiCharMiss),         kCiCharMiss, MEME_FIND(kCiCharMiss), STD_FIND(kCiCharMiss))

CHAR_PAIR(RfindChar_4K_Tail,  HAYC(k4K,  kCiCharHit, pos_tail), CHK_CP(kCiCharHit, pos_tail), kCiCharHit, MEME_RFIND(kCiCharHit), STD_RFIND(kCiCharHit))
CHAR_PAIR(RfindChar_16K_Tail, HAYC(k16K, kCiCharHit, pos_tail), CHK_CP(kCiCharHit, pos_tail), kCiCharHit, MEME_RFIND(kCiCharHit), STD_RFIND(kCiCharHit))
CHAR_PAIR(RfindChar_64K_Tail, HAYC(k64K, kCiCharHit, pos_tail), CHK_CP(kCiCharHit, pos_tail), kCiCharHit, MEME_RFIND(kCiCharHit), STD_RFIND(kCiCharHit))
CHAR_PAIR(RfindChar_64K_Abs,  HAYC(k64K, kCiCharHit, pos_tail), CHK_CA(kCiCharMiss),          kCiCharMiss, MEME_RFIND(kCiCharMiss), STD_RFIND(kCiCharMiss))

// ---------------------------------------------------------------------------
// Case-sensitive substring at 4K (the one xlarge size with no rfind coverage)
// ---------------------------------------------------------------------------
#define SUB_PAIR(NAME, HAY_EXPR, CHECK, MEME_CALL, STD_CALL)                  \
    static void MemeStr_##NAME(benchmark::State& state) {                     \
        const std::string hay = (HAY_EXPR);                                   \
        CHECK(hay);                                                           \
        memepp::string s(hay.data(),                                          \
            static_cast<memepp::string::size_type>(hay.size()));              \
        for (auto _ : state) { MEME_CALL; }                                   \
    }                                                                         \
    BENCHMARK(MemeStr_##NAME);                                                \
    static void StdStr_##NAME(benchmark::State& state) {                      \
        const std::string hay = (HAY_EXPR);                                   \
        CHECK(hay);                                                           \
        std::string s(hay);                                                   \
        for (auto _ : state) { STD_CALL; }                                    \
    }                                                                         \
    BENCHMARK(StdStr_##NAME);

SUB_PAIR(FindMedium_4K_Head, HAY(k4K, kCiNeedle17, pos_head), CHK_P(kCiNeedle17, pos_head),
    benchmark::DoNotOptimize(s.find(kCiNeedle17)), benchmark::DoNotOptimize(s.find(kCiNeedle17)))
SUB_PAIR(FindMedium_4K_Tail, HAY(k4K, kCiNeedle17, pos_tail), CHK_P(kCiNeedle17, pos_tail),
    benchmark::DoNotOptimize(s.find(kCiNeedle17)), benchmark::DoNotOptimize(s.find(kCiNeedle17)))
SUB_PAIR(RfindMedium_4K_Tail, HAY(k4K, kCiNeedle17, pos_tail), CHK_P(kCiNeedle17, pos_tail),
    benchmark::DoNotOptimize(s.rfind(kCiNeedle17)), benchmark::DoNotOptimize(s.rfind(kCiNeedle17)))
SUB_PAIR(RfindMedium_4K_Mid, HAY(k4K, kCiNeedle17, pos_mid), CHK_P(kCiNeedle17, pos_mid),
    benchmark::DoNotOptimize(s.rfind(kCiNeedle17)), benchmark::DoNotOptimize(s.rfind(kCiNeedle17)))
SUB_PAIR(RfindMedium_4K_Abs, HAY(k4K, nullptr, pos_none), CHK_A(kCiAbsent17),
    benchmark::DoNotOptimize(s.rfind(kCiAbsent17)), benchmark::DoNotOptimize(s.rfind(kCiAbsent17)))
SUB_PAIR(RfindShort_4K_Tail, HAY(k4K, kCiNeedle5, pos_tail), CHK_P(kCiNeedle5, pos_tail),
    benchmark::DoNotOptimize(s.rfind(kCiNeedle5)), benchmark::DoNotOptimize(s.rfind(kCiNeedle5)))
SUB_PAIR(RfindShort_4K_Abs, HAY(k4K, nullptr, pos_none), CHK_A(kCiAbsent3),
    benchmark::DoNotOptimize(s.rfind(kCiAbsent3)), benchmark::DoNotOptimize(s.rfind(kCiAbsent3)))

// ---------------------------------------------------------------------------
// Position dimension below 1 KiB (previously only "present" was measured).
// ---------------------------------------------------------------------------
SUB_PAIR(FindShort_91_Head,  HAY(k91,  kCiNeedle5, pos_head), CHK_P(kCiNeedle5, pos_head),
    benchmark::DoNotOptimize(s.find(kCiNeedle5)), benchmark::DoNotOptimize(s.find(kCiNeedle5)))
SUB_PAIR(FindShort_91_Mid,   HAY(k91,  kCiNeedle5, pos_mid),  CHK_P(kCiNeedle5, pos_mid),
    benchmark::DoNotOptimize(s.find(kCiNeedle5)), benchmark::DoNotOptimize(s.find(kCiNeedle5)))
SUB_PAIR(FindShort_91_Tail,  HAY(k91,  kCiNeedle5, pos_tail), CHK_P(kCiNeedle5, pos_tail),
    benchmark::DoNotOptimize(s.find(kCiNeedle5)), benchmark::DoNotOptimize(s.find(kCiNeedle5)))
SUB_PAIR(FindShort_91_Abs,   HAY(k91,  nullptr, 0),           CHK_A(kCiAbsent3),
    benchmark::DoNotOptimize(s.find(kCiAbsent3)), benchmark::DoNotOptimize(s.find(kCiAbsent3)))

SUB_PAIR(FindShort_512_Head, HAY(k512, kCiNeedle5, pos_head), CHK_P(kCiNeedle5, pos_head),
    benchmark::DoNotOptimize(s.find(kCiNeedle5)), benchmark::DoNotOptimize(s.find(kCiNeedle5)))
SUB_PAIR(FindShort_512_Mid,  HAY(k512, kCiNeedle5, pos_mid),  CHK_P(kCiNeedle5, pos_mid),
    benchmark::DoNotOptimize(s.find(kCiNeedle5)), benchmark::DoNotOptimize(s.find(kCiNeedle5)))
SUB_PAIR(FindShort_512_Tail, HAY(k512, kCiNeedle5, pos_tail), CHK_P(kCiNeedle5, pos_tail),
    benchmark::DoNotOptimize(s.find(kCiNeedle5)), benchmark::DoNotOptimize(s.find(kCiNeedle5)))
SUB_PAIR(FindShort_512_Abs,  HAY(k512, nullptr, pos_none),           CHK_A(kCiAbsent3),
    benchmark::DoNotOptimize(s.find(kCiAbsent3)), benchmark::DoNotOptimize(s.find(kCiAbsent3)))

// ---------------------------------------------------------------------------
// Storage-type dimension: the same search on shared (large) and view storage.
// All other search benchmarks use whatever storage the constructor picked.
// ---------------------------------------------------------------------------
#define MEME_STORAGE_PAIR(NAME, STOR_EXPR)                                    \
    static void MemeStr_##NAME(benchmark::State& state) {                     \
        const std::string hay = HAY(k16K, kCiNeedle17, pos_mid);              \
        verify_plant(hay, kCiNeedle17, pos_mid(hay.size()));                  \
        memepp::string base(hay.data(),                                       \
            static_cast<memepp::string::size_type>(hay.size()));              \
        for (auto _ : state) {                                                \
            memepp::string s = (STOR_EXPR);                                   \
            benchmark::DoNotOptimize(s.find(kCiNeedle17));                    \
            benchmark::DoNotOptimize(s.storage_type());                       \
        }                                                                     \
    }                                                                         \
    BENCHMARK(MemeStr_##NAME);

MEME_STORAGE_PAIR(Find_16K_Large, base.to_large())
MEME_STORAGE_PAIR(Find_16K_Shared, base.to_shared_storage())

// string_storage_t has no 'view' enumerator; the non-owning view type is
// memepp::string_view, which has its own find().
static void MemeStr_Find_16K_View(benchmark::State& state) {
    const std::string hay = HAY(k16K, kCiNeedle17, pos_mid);
    verify_plant(hay, kCiNeedle17, pos_mid(hay.size()));
    memepp::string base(hay.data(), static_cast<memepp::string::size_type>(hay.size()));
    memepp::string_view sv(base);
    for (auto _ : state)
        benchmark::DoNotOptimize(sv.find(kCiNeedle17));
}
BENCHMARK(MemeStr_Find_16K_View);

} // namespace
