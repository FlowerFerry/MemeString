// Extra-large haystack search benchmarks: 4 KiB / 16 KiB / 64 KiB.
//
// Complements mmbm_search_matrix.cpp (which stops at 512 bytes).  At these
// sizes the fixed per-call preprocessing cost is amortised against a real
// full-length scan, so the numbers answer a different question:
//
//   small haystack -> "how expensive is setting up the search?"
//   huge haystack  -> "how fast is the scan itself?"
//
// ---------------------------------------------------------------------------
// FILLER CORRECTNESS REQUIREMENT
// ---------------------------------------------------------------------------
// The filler alphabet is deliberately restricted to the characters
//     { x, y, z, para }   (no a-w, no q)
// so that NONE of the needles used here can occur accidentally in the filler:
//
//     needles contain only a-w letters  ->  cannot match filler
//     "absent" needle contains q        ->  cannot match filler
//
// An earlier version of this file used ordinary prose as filler.  That prose
// contained the needle "fox", so every "mid", "tail" and "absent" case in fact
// returned a match within the first ~20 bytes and the whole position dimension
// silently collapsed (4 KiB and 64 KiB measured identically).  The
// `verify_plant()` helper below now asserts the invariants at runtime so the
// same mistake cannot pass unnoticed again.
//
// Match position is a first-class dimension: head / mid / tail / absent
// exercise very different amounts of scanning work.

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <string>
#include <cstring>
#include <cctype>
#include <cassert>
#include <cstdio>

namespace {

// Filler alphabet: only x / y / z and the paragraph mark's UTF-8 bytes are
// used, and the needles below never contain those characters.  Segments are
// alternated so the buffer is not a single repeated byte (which would let a
// Boyer-Moore bad-character table degenerate to a trivial shift).
const char* const kSeg[] = {
    "xyzzyxyzzyxyzzyxyzzy",   // x y z only
    "zzzzxyzzzzxyzzzzxyzz",   // x y z only
    "xyzxyzxyzxyzxyzxyzxy",   // x y z only
};
constexpr std::size_t kSegCount = sizeof(kSeg) / sizeof(kSeg[0]);

constexpr std::size_t kHay4K  = 4 * 1024;
constexpr std::size_t kHay16K = 16 * 1024;
constexpr std::size_t kHay64K = 64 * 1024;

// Needles.  Letters are restricted to a-w so they can never match filler.
const char* const kNeedleShort  = "fox";                                    // 3,  f/o/x -> 'x' collides!
const char* const kNeedleMedium = "brown fox jumps o";                      // 17
const char* const kNeedleLong   =
    "jumps over the lazy dog. Pack my box with five dozen liquor";          // 58
const char* const kNeedleAbsent = "QZQZQZQZQZQZQZQZQZQZ";                   // 20, has 'q'/'Q'

// NOTE on kNeedleShort: 'x' appears in the filler alphabet, so a 3-byte needle
// containing 'x' CAN partially match filler, but never fully ("fox" needs
// f-o-x in sequence and filler never has 'f' or 'o').  Full matches are what
// matter, and those are impossible.  Still, to keep the reasoning airtight we
// also provide a needle with no filler characters at all.
const char* const kNeedlePure   = "brown";                                  // 5, no x/y/z/q

// ---------------------------------------------------------------------------
// Haystack builder: exactly `size` bytes of filler, with `needle` planted so
// its first byte lands at `pos`.
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

// ---------------------------------------------------------------------------
// Runtime invariants.  If any of these trips, the position dimension is
// meaningless and the benchmark numbers must not be trusted.
// ---------------------------------------------------------------------------
void verify_plant(const std::string& hay, const char* needle, std::size_t want_pos)
{
    const std::string n(needle);
    const std::size_t found = hay.find(n);

    if (found != want_pos) {
        std::fprintf(stderr,
            "FATAL: plant invariant broken -- needle \"%s\" first found at %zu, "
            "expected %zu (haystack %zu bytes). Filler accidentally contains "
            "the needle; fix kSeg or the needle.\n",
            needle, found, want_pos, hay.size());
        std::abort();
    }
}

// Verify the needle really does NOT occur when we ask for an absent case.
void verify_absent(const std::string& hay, const char* needle)
{
    if (hay.find(needle) != std::string::npos) {
        std::fprintf(stderr,
            "FATAL: absent invariant broken -- needle \"%s\" occurs in a "
            "%zu-byte haystack that should not contain it.\n",
            needle, hay.size());
        std::abort();
    }
}

std::size_t pos_head(std::size_t)        { return 0; }
std::size_t pos_mid (std::size_t size)   { return size / 2; }
std::size_t pos_tail(std::size_t size)   { return size - 64; }

// ---------------------------------------------------------------------------
// Paired macro: identical name stem, memepp left, std right.
// ---------------------------------------------------------------------------
#define BENCH_PAIR_XL(NAME, HAY_EXPR, CHECK, MEME_BODY, STD_BODY)             \
    static void MemeStr_##NAME(benchmark::State& state) {                     \
        const std::string hay = (HAY_EXPR);                                   \
        CHECK(hay);                                                           \
        memepp::string s(hay.data(),                                          \
            static_cast<memepp::string::size_type>(hay.size()));              \
        for (auto _ : state) { MEME_BODY; }                                   \
    }                                                                         \
    BENCHMARK(MemeStr_##NAME);                                                \
    static void StdStr_##NAME(benchmark::State& state) {                      \
        const std::string hay = (HAY_EXPR);                                   \
        CHECK(hay);                                                           \
        std::string s(hay);                                                   \
        for (auto _ : state) { STD_BODY; }                                    \
    }                                                                         \
    BENCHMARK(StdStr_##NAME);

#define CHECK_PLANT(NEEDLE, POSFN)                                            \
    [](const std::string& h) { verify_plant(h, NEEDLE, POSFN(h.size())); }
#define CHECK_ABSENT(NEEDLE)                                                  \
    [](const std::string& h) { verify_absent(h, NEEDLE); }

#define HAY_AT(SIZE, NEEDLE, POS_FN) make_hay((SIZE), (NEEDLE), (POS_FN)(SIZE))

} // namespace

// ===========================================================================
// 4 KiB
// ===========================================================================
BENCH_PAIR_XL(FindShort_4K_Head,
    HAY_AT(kHay4K, kNeedleShort, pos_head),
    CHECK_PLANT(kNeedleShort, pos_head),
    benchmark::DoNotOptimize(s.find(kNeedleShort)),
    benchmark::DoNotOptimize(s.find(kNeedleShort)))

BENCH_PAIR_XL(FindShort_4K_Mid,
    HAY_AT(kHay4K, kNeedleShort, pos_mid),
    CHECK_PLANT(kNeedleShort, pos_mid),
    benchmark::DoNotOptimize(s.find(kNeedleShort)),
    benchmark::DoNotOptimize(s.find(kNeedleShort)))

BENCH_PAIR_XL(FindShort_4K_Tail,
    HAY_AT(kHay4K, kNeedleShort, pos_tail),
    CHECK_PLANT(kNeedleShort, pos_tail),
    benchmark::DoNotOptimize(s.find(kNeedleShort)),
    benchmark::DoNotOptimize(s.find(kNeedleShort)))

// Absent: haystack has no needle anywhere, so this is a guaranteed full scan.
BENCH_PAIR_XL(FindShort_4K_Absent,
    HAY_AT(kHay4K, nullptr, pos_head),
    CHECK_ABSENT(kNeedleShort),
    benchmark::DoNotOptimize(s.find(kNeedleShort)),
    benchmark::DoNotOptimize(s.find(kNeedleShort)))

BENCH_PAIR_XL(FindLong_4K_Mid,
    HAY_AT(kHay4K, kNeedleLong, pos_mid),
    CHECK_PLANT(kNeedleLong, pos_mid),
    benchmark::DoNotOptimize(s.find(kNeedleLong)),
    benchmark::DoNotOptimize(s.find(kNeedleLong)))

// ===========================================================================
// 16 KiB
// ===========================================================================
BENCH_PAIR_XL(FindShort_16K_Head,
    HAY_AT(kHay16K, kNeedleShort, pos_head),
    CHECK_PLANT(kNeedleShort, pos_head),
    benchmark::DoNotOptimize(s.find(kNeedleShort)),
    benchmark::DoNotOptimize(s.find(kNeedleShort)))

BENCH_PAIR_XL(FindShort_16K_Mid,
    HAY_AT(kHay16K, kNeedleShort, pos_mid),
    CHECK_PLANT(kNeedleShort, pos_mid),
    benchmark::DoNotOptimize(s.find(kNeedleShort)),
    benchmark::DoNotOptimize(s.find(kNeedleShort)))

BENCH_PAIR_XL(FindShort_16K_Tail,
    HAY_AT(kHay16K, kNeedleShort, pos_tail),
    CHECK_PLANT(kNeedleShort, pos_tail),
    benchmark::DoNotOptimize(s.find(kNeedleShort)),
    benchmark::DoNotOptimize(s.find(kNeedleShort)))

BENCH_PAIR_XL(FindShort_16K_Absent,
    HAY_AT(kHay16K, nullptr, pos_head),
    CHECK_ABSENT(kNeedleShort),
    benchmark::DoNotOptimize(s.find(kNeedleShort)),
    benchmark::DoNotOptimize(s.find(kNeedleShort)))

BENCH_PAIR_XL(FindPure_16K_Absent,
    HAY_AT(kHay16K, nullptr, pos_head),
    CHECK_ABSENT(kNeedlePure),
    benchmark::DoNotOptimize(s.find(kNeedlePure)),
    benchmark::DoNotOptimize(s.find(kNeedlePure)))

BENCH_PAIR_XL(FindMedium_16K_Mid,
    HAY_AT(kHay16K, kNeedleMedium, pos_mid),
    CHECK_PLANT(kNeedleMedium, pos_mid),
    benchmark::DoNotOptimize(s.find(kNeedleMedium)),
    benchmark::DoNotOptimize(s.find(kNeedleMedium)))

BENCH_PAIR_XL(FindLong_16K_Mid,
    HAY_AT(kHay16K, kNeedleLong, pos_mid),
    CHECK_PLANT(kNeedleLong, pos_mid),
    benchmark::DoNotOptimize(s.find(kNeedleLong)),
    benchmark::DoNotOptimize(s.find(kNeedleLong)))

BENCH_PAIR_XL(RfindShort_16K_Tail,
    HAY_AT(kHay16K, kNeedleShort, pos_tail),
    CHECK_PLANT(kNeedleShort, pos_tail),
    benchmark::DoNotOptimize(s.rfind(kNeedleShort)),
    benchmark::DoNotOptimize(s.rfind(kNeedleShort)))

BENCH_PAIR_XL(ContainsShort_16K_Absent,
    HAY_AT(kHay16K, nullptr, pos_head),
    CHECK_ABSENT(kNeedleShort),
    benchmark::DoNotOptimize(s.contains(kNeedleShort)),
    benchmark::DoNotOptimize(s.find(kNeedleShort) != std::string::npos))

// ===========================================================================
// 64 KiB — exceeds L1/L2; measures sustained scan throughput
// ===========================================================================
BENCH_PAIR_XL(FindShort_64K_Head,
    HAY_AT(kHay64K, kNeedleShort, pos_head),
    CHECK_PLANT(kNeedleShort, pos_head),
    benchmark::DoNotOptimize(s.find(kNeedleShort)),
    benchmark::DoNotOptimize(s.find(kNeedleShort)))

BENCH_PAIR_XL(FindShort_64K_Mid,
    HAY_AT(kHay64K, kNeedleShort, pos_mid),
    CHECK_PLANT(kNeedleShort, pos_mid),
    benchmark::DoNotOptimize(s.find(kNeedleShort)),
    benchmark::DoNotOptimize(s.find(kNeedleShort)))

BENCH_PAIR_XL(FindShort_64K_Tail,
    HAY_AT(kHay64K, kNeedleShort, pos_tail),
    CHECK_PLANT(kNeedleShort, pos_tail),
    benchmark::DoNotOptimize(s.find(kNeedleShort)),
    benchmark::DoNotOptimize(s.find(kNeedleShort)))

BENCH_PAIR_XL(FindShort_64K_Absent,
    HAY_AT(kHay64K, nullptr, pos_head),
    CHECK_ABSENT(kNeedleShort),
    benchmark::DoNotOptimize(s.find(kNeedleShort)),
    benchmark::DoNotOptimize(s.find(kNeedleShort)))

BENCH_PAIR_XL(FindMedium_64K_Mid,
    HAY_AT(kHay64K, kNeedleMedium, pos_mid),
    CHECK_PLANT(kNeedleMedium, pos_mid),
    benchmark::DoNotOptimize(s.find(kNeedleMedium)),
    benchmark::DoNotOptimize(s.find(kNeedleMedium)))

BENCH_PAIR_XL(FindLong_64K_Mid,
    HAY_AT(kHay64K, kNeedleLong, pos_mid),
    CHECK_PLANT(kNeedleLong, pos_mid),
    benchmark::DoNotOptimize(s.find(kNeedleLong)),
    benchmark::DoNotOptimize(s.find(kNeedleLong)))

BENCH_PAIR_XL(RfindShort_64K_Tail,
    HAY_AT(kHay64K, kNeedleShort, pos_tail),
    CHECK_PLANT(kNeedleShort, pos_tail),
    benchmark::DoNotOptimize(s.rfind(kNeedleShort)),
    benchmark::DoNotOptimize(s.rfind(kNeedleShort)))

// ===========================================================================
// 64 KiB with the case flag — exercises the new mmsimd_u8_find2 path over a
// full-length scan rather than a 91-byte one.
// ===========================================================================
BENCH_PAIR_XL(IndexOfCharInsensitive_64K_Mid,
    HAY_AT(kHay64K, "B", pos_mid),
    CHECK_PLANT("B", pos_mid),
    benchmark::DoNotOptimize(s.index_of(
        "b", memepp::case_sensitivity_t::all_insensitive)),
    benchmark::DoNotOptimize([&] {
        for (std::size_t i = 0; i < s.size(); ++i)
            if (std::tolower((unsigned char)s[i]) == 'b') return i;
        return std::string::npos;
    }()))

BENCH_PAIR_XL(IndexOfCharInsensitive_64K_Absent,
    HAY_AT(kHay64K, nullptr, pos_head),
    [](const std::string& h) {
        if (h.find('b') != std::string::npos || h.find('B') != std::string::npos) {
            std::fprintf(stderr, "FATAL: filler contains 'b'/'B'\n");
            std::abort();
        }
    },
    benchmark::DoNotOptimize(s.index_of(
        "b", memepp::case_sensitivity_t::all_insensitive)),
    benchmark::DoNotOptimize([&] {
        for (std::size_t i = 0; i < s.size(); ++i)
            if (std::tolower((unsigned char)s[i]) == 'b') return i;
        return std::string::npos;
    }()))

BENCH_PAIR_XL(LastIndexOfCharInsensitive_64K_Mid,
    HAY_AT(kHay64K, "B", pos_mid),
    CHECK_PLANT("B", pos_mid),
    benchmark::DoNotOptimize(s.last_index_of(
        "b", memepp::case_sensitivity_t::all_insensitive)),
    benchmark::DoNotOptimize([&] {
        for (std::size_t i = s.size(); i-- > 0; )
            if (std::tolower((unsigned char)s[i]) == 'b') return i;
        return std::string::npos;
    }()))

// ===========================================================================
// Construction / copy at XL size
// ===========================================================================
static void MemeStr_Ctor_64K(benchmark::State& state) {
    const std::string hay = make_hay(kHay64K, nullptr, 0);
    for (auto _ : state) {
        memepp::string a(hay.data(),
            static_cast<memepp::string::size_type>(hay.size()));
        benchmark::DoNotOptimize(a.data());
    }
}
BENCHMARK(MemeStr_Ctor_64K);

static void StdStr_Ctor_64K(benchmark::State& state) {
    const std::string hay = make_hay(kHay64K, nullptr, 0);
    for (auto _ : state) {
        std::string a(hay);
        benchmark::DoNotOptimize(a.data());
    }
}
BENCHMARK(StdStr_Ctor_64K);

static void MemeStr_CopyCtor_64K(benchmark::State& state) {
    const std::string hay = make_hay(kHay64K, nullptr, 0);
    for (auto _ : state) {
        memepp::string a(hay.data(),
            static_cast<memepp::string::size_type>(hay.size()));
        memepp::string b(a);
        benchmark::DoNotOptimize(b.data());
    }
}
BENCHMARK(MemeStr_CopyCtor_64K);

static void StdStr_CopyCtor_64K(benchmark::State& state) {
    const std::string hay = make_hay(kHay64K, nullptr, 0);
    for (auto _ : state) {
        std::string a(hay);
        std::string b(a);
        benchmark::DoNotOptimize(b.data());
    }
}
BENCHMARK(StdStr_CopyCtor_64K);

#undef BENCH_PAIR_XL
#undef CHECK_PLANT
#undef CHECK_ABSENT
#undef HAY_AT
