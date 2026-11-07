// Reusable-searcher benchmark: memepp::searcher vs one-shot search APIs.
//
// ---------------------------------------------------------------------------
// WHAT THIS FILE MEASURES
// ---------------------------------------------------------------------------
// memepp::searcher pre-processes a needle ONCE (fast-scan setup or Boyer-Moore
// tables) and reuses that work across many find() calls.  Every other search
// benchmark in this suite rebuilds whatever state it needs on each call, so
// none of them can show the amortisation the searcher exists for.  This file
// fixes that:
//
//   Srch_*        : build once, search 16 haystacks (the amortised pattern)
//   SrchSingle_*  : build + search exactly once (the break-even point)
//   SrchCtor_*    : construction cost alone, vs the C++17 std::boyer_moore_searcher
//   SrchMove_* / SrchCopy_* / SrchSwap_* / SrchDensity_* / SrchValid_* :
//                   the rest of the searcher API, which has no ISO C or
//                   std::string equivalent at all
//   SrchFindOV_*  : the find() overloads, which differ in how the text length
//                   is obtained (strlen for the const char* form)
//
// ---------------------------------------------------------------------------
// THE CELLS
// ---------------------------------------------------------------------------
//   C      : strstr() per haystack.  The only ISO C substring search; it
//            re-scans the needle every call, i.e. the "no reusable state"
//            baseline.  For case-insensitive there is no ISO C function at
//            all, so that cell is a hand-rolled fold loop.
//   std    : std::boyer_moore_searcher (<functional>, C++17) built once and
//            applied per haystack through std::search -- the closest
//            standard-library analogue of memepp::searcher.  std::string::find
//            is used only in SrchSingle_*, where it is the per-call baseline.
//   memepp : memepp::searcher built once, find() per haystack.
//
// std has NO case-insensitive searcher, so the CI rows have no std cell.
//
// ---------------------------------------------------------------------------
// FILLER / NEEDLE CONSTRUCTION (no accidental hits)
// ---------------------------------------------------------------------------
// Haystack filler uses ONLY the alphabet {x, y, z, p, a, r}.  Every planted
// needle uses letters strictly OUTSIDE that set, so a needle can only match
// where it was deliberately planted:
//
//   filler        x y z p a r
//   needle 5/17   b c d e f g h i              (8 letters)
//   needle 58     b c d e f g h i j k l m n o q s t u v w   (20 letters)
//
// Absent needles use '#' and '@', which occur in NO haystack at all (filler is
// letters, planted needles are letters), so absence is guaranteed rather than
// merely unlikely.
//
// Each needle gets its OWN haystack set.  Planting several needles into one
// haystack would let a later plant overwrite an earlier one and silently
// destroy the planted match.
//
// The 58B needle crosses MEME_IMPL_SHORT_NEEDLE_MAX (32) and therefore
// exercises the Boyer-Moore path, where table building is expensive and
// amortisation matters most.  5B and 17B stay on the fast-scan path.

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/searcher.hpp>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>

#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------
constexpr int kNHay = 16;

unsigned g_state = 0x6d2b79f5u;
unsigned rnd(void)
{
    g_state ^= g_state << 13;
    g_state ^= g_state >> 17;
    g_state ^= g_state << 5;
    return g_state;
}

std::string make_hay(std::size_t n)
{
    static const char alpha[] = "xyzpar";
    std::string s;
    s.resize(n);
    for (std::size_t i = 0; i < n; ++i)
        s[i] = alpha[rnd() % 6u];
    return s;
}

std::string make_needle(std::size_t n)
{
    // 20 letters, disjoint from the filler alphabet {x,y,z,p,a,r}.
    static const char wide_alpha[] = "bcdefghijklmnoqstuvw";
    std::string s;
    s.resize(n);
    if (n <= 17)
    {
        for (std::size_t i = 0; i < n; ++i)
            s[i] = static_cast<char>('b' + (rnd() % 8u));   // b..i
    }
    else
    {
        for (std::size_t i = 0; i < n; ++i)
            s[i] = wide_alpha[rnd() % 20u];
    }
    return s;
}

// Absent needle: '#' and '@' appear in no haystack, planted or filler.
std::string make_absent(std::size_t n)
{
    std::string s;
    s.resize(n);
    for (std::size_t i = 0; i < n; ++i)
        s[i] = (i % 3u == 0u) ? '@' : '#';
    return s;
}

std::string upper_of(const std::string& s)
{
    std::string r(s);
    for (char& c : r)
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return r;
}

MG_CAPI_INLINE const MemeByte_t* UB(const std::string& _s)
{
    return reinterpret_cast<const MemeByte_t*>(_s.data());
}

// One haystack set per needle: kNHay haystacks, each with the needle planted
// at a known random position.
struct DataSet
{
    std::vector<std::string> hay512;
    std::vector<std::string> hay4k;
    std::vector<std::string> hay16k;
    std::vector<std::size_t> pos512;
    std::vector<std::size_t> pos4k;
    std::vector<std::size_t> pos16k;
    std::string              needle;

    void build(const std::string& ndl)
    {
        needle = ndl;
        for (int i = 0; i < kNHay; ++i)
        {
            auto one = [&](std::size_t len, std::vector<std::size_t>& out)
            {
                std::string h = make_hay(len);
                const std::size_t span = len - ndl.size() - 8u;
                const std::size_t p    = 4u + (rnd() % span);
                h.replace(p, ndl.size(), ndl);
                out.push_back(p);
                return h;
            };
            hay512.push_back(one(512,   pos512));
            hay4k.push_back(one(4096,  pos4k));
            hay16k.push_back(one(16384, pos16k));
        }
    }
};

DataSet g_s5, g_s17, g_s58;

// Plain filler (no planted needle) for the absent-needle cases.
std::vector<std::string> g_plain512, g_plain16k;
std::string g_abs5, g_abs17, g_abs58;

void init_data(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    g_s5 .build(make_needle(5));
    g_s17.build(make_needle(17));
    g_s58.build(make_needle(58));

    g_abs5  = make_absent(5);
    g_abs17 = make_absent(17);
    g_abs58 = make_absent(58);

    for (int i = 0; i < kNHay; ++i)
    {
        g_plain512.push_back(make_hay(512));
        g_plain16k.push_back(make_hay(16384));
    }
}

// ---------------------------------------------------------------------------
// C cell: strstr per haystack (ISO C has nothing reusable)
// ---------------------------------------------------------------------------
mmint_t c_find_all(const std::vector<std::string>& hay, const char* ndl)
{
    mmint_t acc = 0;
    for (const std::string& h : hay)
    {
        const char* p = std::strstr(h.c_str(), ndl);
        acc += (p == nullptr) ? -1 : static_cast<mmint_t>(p - h.c_str());
    }
    return acc;
}

// Hand-rolled case-insensitive substring search: what ISO C forces a caller to
// write, since the standard has no strcasestr.
const char* c_strcasestr(const char* s, const char* ndl)
{
    const std::size_t n = std::strlen(ndl);
    if (n == 0) return s;
    for (; *s; ++s)
    {
        std::size_t i = 0;
        for (; i < n; ++i)
        {
            const unsigned char a = static_cast<unsigned char>(s[i]);
            const unsigned char b = static_cast<unsigned char>(ndl[i]);
            const unsigned char fa = (a >= 'A' && a <= 'Z') ? a + 32 : a;
            const unsigned char fb = (b >= 'A' && b <= 'Z') ? b + 32 : b;
            if (fa != fb) break;
        }
        if (i == n) return s;
    }
    return nullptr;
}

mmint_t c_find_all_ci(const std::vector<std::string>& hay, const char* ndl)
{
    mmint_t acc = 0;
    for (const std::string& h : hay)
    {
        const char* p = c_strcasestr(h.c_str(), ndl);
        acc += (p == nullptr) ? -1 : static_cast<mmint_t>(p - h.c_str());
    }
    return acc;
}

// ---------------------------------------------------------------------------
// std cell: std::boyer_moore_searcher built once, applied per haystack
// ---------------------------------------------------------------------------
template <typename BMS>
mmint_t std_find_all(BMS& bm, const std::vector<std::string>& hay)
{
    mmint_t acc = 0;
    for (const std::string& h : hay)
    {
        const std::string_view sv(h);
        const auto it = std::search(sv.begin(), sv.end(), bm);
        acc += (it == sv.end()) ? -1
                                : static_cast<mmint_t>(it - sv.begin());
    }
    return acc;
}

// ---------------------------------------------------------------------------
// memepp cell: searcher built once, find() per haystack
// ---------------------------------------------------------------------------
mmint_t meme_find_all(const memepp::searcher& s,
                      const std::vector<std::string>& hay)
{
    mmint_t acc = 0;
    for (const std::string& h : hay)
        acc += s.find(memepp::string_view(h.data(), h.size()));
    return acc;
}

// ---------------------------------------------------------------------------
// Correctness gate: every cell must agree with the planted position.
// ---------------------------------------------------------------------------
void verify(const DataSet& ds, const char* tag)
{
    const std::vector<const std::vector<std::string>*> sets =
        { &ds.hay512, &ds.hay4k, &ds.hay16k };
    const std::vector<const std::vector<std::size_t>*> poss =
        { &ds.pos512, &ds.pos4k, &ds.pos16k };

    memepp::searcher ms(UB(ds.needle), static_cast<mmint_t>(ds.needle.size()));

    for (std::size_t k = 0; k < sets.size(); ++k)
    {
        const std::vector<std::string>& hay = *sets[k];
        const std::vector<std::size_t>& pos = *poss[k];

        for (std::size_t i = 0; i < hay.size(); ++i)
        {
            const std::string& h = hay[i];
            const std::size_t want = pos[i];

            if (h.find(ds.needle) != want)
            {
                std::fprintf(stderr, "verify %s: plant lost\n", tag);
                std::abort();
            }

            const char* p = std::strstr(h.c_str(), ds.needle.c_str());
            if (p == nullptr || static_cast<std::size_t>(p - h.c_str()) != want)
            {
                std::fprintf(stderr, "verify %s: C strstr mismatch\n", tag);
                std::abort();
            }

            const std::string_view sv(h);
            std::boyer_moore_searcher bm(ds.needle.begin(), ds.needle.end());
            const auto it = std::search(sv.begin(), sv.end(), bm);
            if (it == sv.end() ||
                static_cast<std::size_t>(it - sv.begin()) != want)
            {
                std::fprintf(stderr, "verify %s: std BM searcher mismatch\n", tag);
                std::abort();
            }

            if (ms.find(memepp::string_view(h.data(), h.size()))
                != static_cast<mmint_t>(want))
            {
                std::fprintf(stderr, "verify %s: memepp searcher mismatch\n", tag);
                std::abort();
            }
        }
    }
}

void verify_absent(const std::string& ndl,
                   const std::vector<std::string>& hay,
                   const char* tag)
{
    memepp::searcher ms(UB(ndl), static_cast<mmint_t>(ndl.size()));

    for (const std::string& h : hay)
    {
        if (h.find(ndl) != std::string::npos)
        {
            std::fprintf(stderr, "verify_absent %s: needle present\n", tag);
            std::abort();
        }
        if (std::strstr(h.c_str(), ndl.c_str()) != nullptr)
        {
            std::fprintf(stderr, "verify_absent %s: C found it\n", tag);
            std::abort();
        }
        if (ms.find(memepp::string_view(h.data(), h.size())) != -1)
        {
            std::fprintf(stderr, "verify_absent %s: memepp found it\n", tag);
            std::abort();
        }
    }
}

void verify_ci(const DataSet& ds, const std::string& ndl_upper, const char* tag)
{
    memepp::searcher ms(
        UB(ndl_upper), static_cast<mmint_t>(ndl_upper.size()),
        memepp::case_sensitivity_t::all_insensitive);

    const std::vector<const std::vector<std::string>*> sets =
        { &ds.hay512, &ds.hay4k, &ds.hay16k };
    const std::vector<const std::vector<std::size_t>*> poss =
        { &ds.pos512, &ds.pos4k, &ds.pos16k };

    for (std::size_t k = 0; k < sets.size(); ++k)
    {
        const std::vector<std::string>& hay = *sets[k];
        const std::vector<std::size_t>& pos = *poss[k];

        for (std::size_t i = 0; i < hay.size(); ++i)
        {
            const std::size_t want = pos[i];
            const char* p = c_strcasestr(hay[i].c_str(), ndl_upper.c_str());
            if (p == nullptr ||
                static_cast<std::size_t>(p - hay[i].c_str()) != want)
            {
                std::fprintf(stderr, "verify_ci %s: hand-rolled C mismatch\n", tag);
                std::abort();
            }
            if (ms.find(memepp::string_view(hay[i].data(), hay[i].size()))
                != static_cast<mmint_t>(want))
            {
                std::fprintf(stderr, "verify_ci %s: memepp CI mismatch\n", tag);
                std::abort();
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Amortised macro: build once, search kNHay haystacks of one size
// ---------------------------------------------------------------------------
#define SRCH_SIZE(NAME, DS, HAYSZ, CS)                                        \
static void BM_SrchC_##NAME(benchmark::State& state)                          \
{                                                                             \
    init_data();                                                              \
    for (auto _ : state)                                                      \
        benchmark::DoNotOptimize(c_find_all(DS.HAYSZ, DS.needle.c_str()));    \
}                                                                             \
BENCHMARK(BM_SrchC_##NAME);                                                   \
                                                                              \
static void BM_SrchStd_##NAME(benchmark::State& state)                        \
{                                                                             \
    init_data();                                                              \
    std::boyer_moore_searcher bm(DS.needle.begin(), DS.needle.end());         \
    for (auto _ : state)                                                      \
        benchmark::DoNotOptimize(std_find_all(bm, DS.HAYSZ));                 \
}                                                                             \
BENCHMARK(BM_SrchStd_##NAME);                                                 \
                                                                              \
static void BM_SrchMeme_##NAME(benchmark::State& state)                       \
{                                                                             \
    init_data();                                                              \
    memepp::searcher s(UB(DS.needle),                                         \
        static_cast<mmint_t>(DS.needle.size()), CS);                          \
    for (auto _ : state)                                                      \
        benchmark::DoNotOptimize(meme_find_all(s, DS.HAYSZ));                 \
}                                                                             \
BENCHMARK(BM_SrchMeme_##NAME);

// Absent-needle variant: plain filler haystacks, needle never present.
#define SRCH_ABSENT(NAME, HAYSZ, NDL, CS)                                     \
static void BM_SrchC_##NAME(benchmark::State& state)                          \
{                                                                             \
    init_data();                                                              \
    for (auto _ : state)                                                      \
        benchmark::DoNotOptimize(c_find_all(HAYSZ, NDL.c_str()));             \
}                                                                             \
BENCHMARK(BM_SrchC_##NAME);                                                   \
                                                                              \
static void BM_SrchStd_##NAME(benchmark::State& state)                        \
{                                                                             \
    init_data();                                                              \
    std::boyer_moore_searcher bm(NDL.begin(), NDL.end());                     \
    for (auto _ : state)                                                      \
        benchmark::DoNotOptimize(std_find_all(bm, HAYSZ));                    \
}                                                                             \
BENCHMARK(BM_SrchStd_##NAME);                                                 \
                                                                              \
static void BM_SrchMeme_##NAME(benchmark::State& state)                       \
{                                                                             \
    init_data();                                                              \
    memepp::searcher s(UB(NDL), static_cast<mmint_t>(NDL.size()), CS);        \
    for (auto _ : state)                                                      \
        benchmark::DoNotOptimize(meme_find_all(s, HAYSZ));                    \
}                                                                             \
BENCHMARK(BM_SrchMeme_##NAME);

// Case-insensitive: no std cell exists, only hand-rolled C and memepp.
#define SRCH_CI(NAME, DS, HAYSZ)                                              \
static void BM_SrchC_##NAME(benchmark::State& state)                          \
{                                                                             \
    init_data();                                                              \
    const std::string ndl = upper_of(DS.needle);                              \
    for (auto _ : state)                                                      \
        benchmark::DoNotOptimize(c_find_all_ci(DS.HAYSZ, ndl.c_str()));       \
}                                                                             \
BENCHMARK(BM_SrchC_##NAME);                                                   \
                                                                              \
static void BM_SrchMeme_##NAME(benchmark::State& state)                       \
{                                                                             \
    init_data();                                                              \
    const std::string ndl = upper_of(DS.needle);                              \
    memepp::searcher s(UB(ndl), static_cast<mmint_t>(ndl.size()),             \
        memepp::case_sensitivity_t::all_insensitive);                         \
    for (auto _ : state)                                                      \
        benchmark::DoNotOptimize(meme_find_all(s, DS.HAYSZ));                 \
}                                                                             \
BENCHMARK(BM_SrchMeme_##NAME);

// ---------------------------------------------------------------------------
// Single-shot: build + one search, vs the per-call APIs
// ---------------------------------------------------------------------------
#define SRCH_SINGLE(NAME, HAY, NDL, CS)                                       \
static void BM_SrchSingleC_##NAME(benchmark::State& state)                    \
{                                                                             \
    init_data();                                                              \
    for (auto _ : state)                                                      \
        benchmark::DoNotOptimize(                                             \
            std::strstr(HAY.c_str(), NDL.c_str()));                           \
}                                                                             \
BENCHMARK(BM_SrchSingleC_##NAME);                                             \
                                                                              \
static void BM_SrchSingleStd_##NAME(benchmark::State& state)                  \
{                                                                             \
    init_data();                                                              \
    for (auto _ : state)                                                      \
        benchmark::DoNotOptimize(HAY.find(NDL));                              \
}                                                                             \
BENCHMARK(BM_SrchSingleStd_##NAME);                                           \
                                                                              \
static void BM_SrchSingleMeme_##NAME(benchmark::State& state)                 \
{                                                                             \
    init_data();                                                              \
    const memepp::string_view hv(HAY.data(), HAY.size());                     \
    const memepp::string_view nv(NDL.data(), NDL.size());                     \
    for (auto _ : state)                                                      \
        benchmark::DoNotOptimize(hv.index_of(nv, CS));                        \
}                                                                             \
BENCHMARK(BM_SrchSingleMeme_##NAME);                                          \
                                                                              \
static void BM_SrchSingleBuildMeme_##NAME(benchmark::State& state)            \
{                                                                             \
    init_data();                                                              \
    const memepp::string_view hv(HAY.data(), HAY.size());                     \
    for (auto _ : state)                                                      \
    {                                                                         \
        memepp::searcher s(UB(NDL), static_cast<mmint_t>(NDL.size()), CS);    \
        benchmark::DoNotOptimize(s.find(hv));                                 \
    }                                                                         \
}                                                                             \
BENCHMARK(BM_SrchSingleBuildMeme_##NAME);

} // namespace

// ===========================================================================
// Amortised: build once, search 16 haystacks
// ===========================================================================
SRCH_SIZE(Find5_512B,  g_s5,  hay512, memepp::case_sensitivity_t::all_sensitive)
SRCH_SIZE(Find5_4K,    g_s5,  hay4k,  memepp::case_sensitivity_t::all_sensitive)
SRCH_SIZE(Find5_16K,   g_s5,  hay16k, memepp::case_sensitivity_t::all_sensitive)
SRCH_SIZE(Find17_512B, g_s17, hay512, memepp::case_sensitivity_t::all_sensitive)
SRCH_SIZE(Find17_4K,   g_s17, hay4k,  memepp::case_sensitivity_t::all_sensitive)
SRCH_SIZE(Find17_16K,  g_s17, hay16k, memepp::case_sensitivity_t::all_sensitive)
SRCH_SIZE(Find58_512B, g_s58, hay512, memepp::case_sensitivity_t::all_sensitive)
SRCH_SIZE(Find58_4K,   g_s58, hay4k,  memepp::case_sensitivity_t::all_sensitive)
SRCH_SIZE(Find58_16K,  g_s58, hay16k, memepp::case_sensitivity_t::all_sensitive)

// ===========================================================================
// Absent needles (worst case: full scan, no match)
// ===========================================================================
SRCH_ABSENT(Find5Abs_512B,  g_plain512, g_abs5,  memepp::case_sensitivity_t::all_sensitive)
SRCH_ABSENT(Find17Abs_512B, g_plain512, g_abs17, memepp::case_sensitivity_t::all_sensitive)
SRCH_ABSENT(Find58Abs_512B, g_plain512, g_abs58, memepp::case_sensitivity_t::all_sensitive)
SRCH_ABSENT(Find58Abs_16K,  g_plain16k, g_abs58, memepp::case_sensitivity_t::all_sensitive)

// ===========================================================================
// Amortised, case-insensitive (std has no equivalent)
// ===========================================================================
SRCH_CI(Find5CI_512B,  g_s5,  hay512)
SRCH_CI(Find5CI_16K,   g_s5,  hay16k)
SRCH_CI(Find17CI_512B, g_s17, hay512)
SRCH_CI(Find17CI_16K,  g_s17, hay16k)
SRCH_CI(Find58CI_512B, g_s58, hay512)
SRCH_CI(Find58CI_16K,  g_s58, hay16k)

// ===========================================================================
// Single-shot break-even
// ===========================================================================
SRCH_SINGLE(5_512B,  g_s5.hay512[0],  g_s5.needle,  memepp::case_sensitivity_t::all_sensitive)
SRCH_SINGLE(17_512B, g_s17.hay512[0], g_s17.needle, memepp::case_sensitivity_t::all_sensitive)
SRCH_SINGLE(58_512B, g_s58.hay512[0], g_s58.needle, memepp::case_sensitivity_t::all_sensitive)
SRCH_SINGLE(58_16K,  g_s58.hay16k[0], g_s58.needle, memepp::case_sensitivity_t::all_sensitive)

// ===========================================================================
// Construction cost alone
// ===========================================================================
static void BM_SrchCtorStd_5(benchmark::State& state)
{
    init_data();
    for (auto _ : state)
    {
        std::boyer_moore_searcher bm(g_s5.needle.begin(), g_s5.needle.end());
        benchmark::DoNotOptimize(&bm);
    }
}
BENCHMARK(BM_SrchCtorStd_5);

static void BM_SrchCtorMeme_5(benchmark::State& state)
{
    init_data();
    for (auto _ : state)
    {
        memepp::searcher s(UB(g_s5.needle),
            static_cast<mmint_t>(g_s5.needle.size()));
        benchmark::DoNotOptimize(s.valid());
    }
}
BENCHMARK(BM_SrchCtorMeme_5);

static void BM_SrchCtorStd_17(benchmark::State& state)
{
    init_data();
    for (auto _ : state)
    {
        std::boyer_moore_searcher bm(g_s17.needle.begin(), g_s17.needle.end());
        benchmark::DoNotOptimize(&bm);
    }
}
BENCHMARK(BM_SrchCtorStd_17);

static void BM_SrchCtorMeme_17(benchmark::State& state)
{
    init_data();
    for (auto _ : state)
    {
        memepp::searcher s(UB(g_s17.needle),
            static_cast<mmint_t>(g_s17.needle.size()));
        benchmark::DoNotOptimize(s.valid());
    }
}
BENCHMARK(BM_SrchCtorMeme_17);

static void BM_SrchCtorStd_58(benchmark::State& state)
{
    init_data();
    for (auto _ : state)
    {
        std::boyer_moore_searcher bm(g_s58.needle.begin(), g_s58.needle.end());
        benchmark::DoNotOptimize(&bm);
    }
}
BENCHMARK(BM_SrchCtorStd_58);

static void BM_SrchCtorMeme_58(benchmark::State& state)
{
    init_data();
    for (auto _ : state)
    {
        memepp::searcher s(UB(g_s58.needle),
            static_cast<mmint_t>(g_s58.needle.size()));
        benchmark::DoNotOptimize(s.valid());
    }
}
BENCHMARK(BM_SrchCtorMeme_58);

// ===========================================================================
// Lifecycle: move / copy / swap  (no C or std analogue exists)
// ===========================================================================
static void BM_SrchMoveCtor_58(benchmark::State& state)
{
    init_data();
    for (auto _ : state)
    {
        memepp::searcher a(UB(g_s58.needle),
            static_cast<mmint_t>(g_s58.needle.size()));
        memepp::searcher b(std::move(a));
        benchmark::DoNotOptimize(b.valid());
    }
}
BENCHMARK(BM_SrchMoveCtor_58);

static void BM_SrchMoveCtor_5(benchmark::State& state)
{
    init_data();
    for (auto _ : state)
    {
        memepp::searcher a(UB(g_s5.needle),
            static_cast<mmint_t>(g_s5.needle.size()));
        memepp::searcher b(std::move(a));
        benchmark::DoNotOptimize(b.valid());
    }
}
BENCHMARK(BM_SrchMoveCtor_5);

static void BM_SrchMoveAssign_58(benchmark::State& state)
{
    init_data();
    memepp::searcher dst(UB(g_s5.needle),
        static_cast<mmint_t>(g_s5.needle.size()));
    for (auto _ : state)
    {
        memepp::searcher src(UB(g_s58.needle),
            static_cast<mmint_t>(g_s58.needle.size()));
        dst = std::move(src);
        benchmark::DoNotOptimize(dst.valid());
    }
}
BENCHMARK(BM_SrchMoveAssign_58);

/*
 * The source is constructed INSIDE the loop for both move and copy, so the
 * two numbers are directly comparable.  A move benchmark cannot hoist the
 * source out of the loop -- after the first iteration the source is empty --
 * so hoisting only the copy side would make the pair incomparable.
 */
static void BM_SrchCopyCtor_58(benchmark::State& state)
{
    init_data();
    for (auto _ : state)
    {
        const memepp::searcher a(UB(g_s58.needle),
            static_cast<mmint_t>(g_s58.needle.size()));
        memepp::searcher b(a);
        benchmark::DoNotOptimize(b.valid());
    }
}
BENCHMARK(BM_SrchCopyCtor_58);

static void BM_SrchCopyCtor_5(benchmark::State& state)
{
    init_data();
    for (auto _ : state)
    {
        const memepp::searcher a(UB(g_s5.needle),
            static_cast<mmint_t>(g_s5.needle.size()));
        memepp::searcher b(a);
        benchmark::DoNotOptimize(b.valid());
    }
}
BENCHMARK(BM_SrchCopyCtor_5);

/*
 * FastScan-path needles (case-sensitive) build no tables, so copy and move
 * both reduce to one 344-byte memcpy plus pointer relocation and cost nearly
 * the same.  The difference only appears on the Boyer-Moore path, where copy
 * must rebuild the bad-char and good-suffix tables while move just relocates
 * pointers.  A case-insensitive 58 B needle is what selects Boyer-Moore.
 */
static void BM_SrchCtorStd_58CI(benchmark::State& state)
{
    init_data();
    for (auto _ : state)
    {
        const std::string ndl = upper_of(g_s58.needle);
        std::boyer_moore_searcher bm(ndl.begin(), ndl.end());
        benchmark::DoNotOptimize(&bm);
    }
}
BENCHMARK(BM_SrchCtorStd_58CI);

static void BM_SrchCtorMeme_58CI(benchmark::State& state)
{
    init_data();
    for (auto _ : state)
    {
        const std::string ndl = upper_of(g_s58.needle);
        memepp::searcher s(UB(ndl), static_cast<mmint_t>(ndl.size()),
            memepp::case_sensitivity_t::all_insensitive);
        benchmark::DoNotOptimize(s.valid());
    }
}
BENCHMARK(BM_SrchCtorMeme_58CI);

static void BM_SrchMoveCtor_58CI(benchmark::State& state)
{
    init_data();
    for (auto _ : state)
    {
        const std::string ndl = upper_of(g_s58.needle);
        memepp::searcher a(UB(ndl), static_cast<mmint_t>(ndl.size()),
            memepp::case_sensitivity_t::all_insensitive);
        memepp::searcher b(std::move(a));
        benchmark::DoNotOptimize(b.valid());
    }
}
BENCHMARK(BM_SrchMoveCtor_58CI);

static void BM_SrchCopyCtor_58CI(benchmark::State& state)
{
    init_data();
    for (auto _ : state)
    {
        const std::string ndl = upper_of(g_s58.needle);
        const memepp::searcher a(UB(ndl), static_cast<mmint_t>(ndl.size()),
            memepp::case_sensitivity_t::all_insensitive);
        memepp::searcher b(a);
        benchmark::DoNotOptimize(b.valid());
    }
}
BENCHMARK(BM_SrchCopyCtor_58CI);

static void BM_SrchCopyAssign_58(benchmark::State& state)
{
    init_data();
    const memepp::searcher a(UB(g_s58.needle),
        static_cast<mmint_t>(g_s58.needle.size()));
    memepp::searcher dst(UB(g_s5.needle),
        static_cast<mmint_t>(g_s5.needle.size()));
    for (auto _ : state)
    {
        dst = a;
        benchmark::DoNotOptimize(dst.valid());
    }
}
BENCHMARK(BM_SrchCopyAssign_58);

static void BM_SrchSwap_58(benchmark::State& state)
{
    init_data();
    memepp::searcher a(UB(g_s58.needle),
        static_cast<mmint_t>(g_s58.needle.size()));
    memepp::searcher b(UB(g_s17.needle),
        static_cast<mmint_t>(g_s17.needle.size()));
    for (auto _ : state)
    {
        a.swap(b);
        benchmark::DoNotOptimize(a.valid());
    }
}
BENCHMARK(BM_SrchSwap_58);

// ===========================================================================
// density() / reset_density() / valid() / native_handle()
// ===========================================================================
static void BM_SrchDensity(benchmark::State& state)
{
    init_data();
    memepp::searcher s(UB(g_s5.needle),
        static_cast<mmint_t>(g_s5.needle.size()));
    const memepp::string_view h(g_s5.hay512[0].data(), g_s5.hay512[0].size());
    for (auto _ : state)
    {
        s.find(h);
        benchmark::DoNotOptimize(s.density());
    }
}
BENCHMARK(BM_SrchDensity);

static void BM_SrchResetDensity(benchmark::State& state)
{
    init_data();
    memepp::searcher s(UB(g_s5.needle),
        static_cast<mmint_t>(g_s5.needle.size()));
    for (auto _ : state)
        s.reset_density();
}
BENCHMARK(BM_SrchResetDensity);

static void BM_SrchValid(benchmark::State& state)
{
    init_data();
    memepp::searcher s(UB(g_s5.needle),
        static_cast<mmint_t>(g_s5.needle.size()));
    for (auto _ : state)
        benchmark::DoNotOptimize(s.valid());
}
BENCHMARK(BM_SrchValid);

static void BM_SrchNativeHandle(benchmark::State& state)
{
    init_data();
    memepp::searcher s(UB(g_s5.needle),
        static_cast<mmint_t>(g_s5.needle.size()));
    for (auto _ : state)
        benchmark::DoNotOptimize(s.native_handle());
}
BENCHMARK(BM_SrchNativeHandle);

// ===========================================================================
// find() overloads
// ===========================================================================
static void BM_SrchFindOV_MemeView(benchmark::State& state)
{
    init_data();
    memepp::searcher s(UB(g_s17.needle),
        static_cast<mmint_t>(g_s17.needle.size()));
    const memepp::string_view h(g_s17.hay512[0].data(), g_s17.hay512[0].size());
    for (auto _ : state)
        benchmark::DoNotOptimize(s.find(h));
}
BENCHMARK(BM_SrchFindOV_MemeView);

static void BM_SrchFindOV_CStr(benchmark::State& state)
{
    init_data();
    memepp::searcher s(UB(g_s17.needle),
        static_cast<mmint_t>(g_s17.needle.size()));
    const std::string h = g_s17.hay512[0];
    for (auto _ : state)
        benchmark::DoNotOptimize(s.find(h.c_str()));
}
BENCHMARK(BM_SrchFindOV_CStr);

static void BM_SrchFindOV_Bytes(benchmark::State& state)
{
    init_data();
    memepp::searcher s(UB(g_s17.needle),
        static_cast<mmint_t>(g_s17.needle.size()));
    const std::string h = g_s17.hay512[0];
    for (auto _ : state)
        benchmark::DoNotOptimize(
            s.find(UB(h), static_cast<mmint_t>(h.size())));
}
BENCHMARK(BM_SrchFindOV_Bytes);

static void BM_SrchFindOV_Offset(benchmark::State& state)
{
    init_data();
    memepp::searcher s(UB(g_s17.needle),
        static_cast<mmint_t>(g_s17.needle.size()));
    const memepp::string_view h(g_s17.hay512[0].data(), g_s17.hay512[0].size());
    for (auto _ : state)
        benchmark::DoNotOptimize(s.find(h, 128));
}
BENCHMARK(BM_SrchFindOV_Offset);

// ===========================================================================
// Correctness gate, run once at static-init time.
// ===========================================================================
namespace {
struct SrchVerifier {
    SrchVerifier(void)
    {
        init_data();
        verify(g_s5,  "5B");
        verify(g_s17, "17B");
        verify(g_s58, "58B");

        verify_ci(g_s5,  upper_of(g_s5.needle),  "5B");
        verify_ci(g_s17, upper_of(g_s17.needle), "17B");
        verify_ci(g_s58, upper_of(g_s58.needle), "58B");

        verify_absent(g_abs5,  g_plain512, "5B-absent");
        verify_absent(g_abs17, g_plain512, "17B-absent");
        verify_absent(g_abs58, g_plain512, "58B-absent");
        verify_absent(g_abs58, g_plain16k, "58B-absent-16K");
    }
} g_srch_verifier;
} // namespace
