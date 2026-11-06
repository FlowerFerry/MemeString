// String relational operators and C-side addendum.
//
// 1. String relational operators: !=, <, >, <=, >= (30+ overloads in
//    string_def.hpp:463-477) — previously zero coverage.
// 2. operator+(char), +(string_view), +(const string&) variants.
// 3. C-side equivalents for operations that exist in ISO C but had no
//    benchmark counterpart.

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/to_string.hpp>
#include <string>
#include <vector>
#include <cstring>
#include <cctype>
#include <cstdio>

namespace {

const char* kA = "Hello World!";
const char* kB = "Hello World!";
const char* kC = "Hello Apple!";   // 'A' < 'W' at pos 6
const char* kD = "Hello World!!";  // longer

// ===========================================================================
// 1. String relational operators
// ===========================================================================

// --- operator!= (string vs string) ---
static void MemeStr_OpNe_StrStr(benchmark::State& state) {
    memepp::string a(kA), b(kB);
    for (auto _ : state) benchmark::DoNotOptimize(a != b);
}
BENCHMARK(MemeStr_OpNe_StrStr)->Iterations(MMBM_ITERATION_COUNT);

// --- operator!= (string vs char*) ---
static void MemeStr_OpNe_StrCstr(benchmark::State& state) {
    memepp::string a(kA);
    for (auto _ : state) benchmark::DoNotOptimize(a != kB);
}
BENCHMARK(MemeStr_OpNe_StrCstr)->Iterations(MMBM_ITERATION_COUNT);

// --- operator!= (char* vs string) ---
static void MemeStr_OpNe_CstrStr(benchmark::State& state) {
    memepp::string a(kA);
    for (auto _ : state) benchmark::DoNotOptimize(kB != a);
}
BENCHMARK(MemeStr_OpNe_CstrStr)->Iterations(MMBM_ITERATION_COUNT);

// --- operator< (string vs string) ---
static void MemeStr_OpLt_StrStr(benchmark::State& state) {
    memepp::string a(kA), c(kC);  // 'W' > 'A', so a > c
    for (auto _ : state) benchmark::DoNotOptimize(a < c);
}
BENCHMARK(MemeStr_OpLt_StrStr)->Iterations(MMBM_ITERATION_COUNT);

// --- operator< (string vs char*) ---
static void MemeStr_OpLt_StrCstr(benchmark::State& state) {
    memepp::string a(kA);
    for (auto _ : state) benchmark::DoNotOptimize(a < kC);
}
BENCHMARK(MemeStr_OpLt_StrCstr)->Iterations(MMBM_ITERATION_COUNT);

// --- operator<= (string vs string) ---
static void MemeStr_OpLe_StrStr(benchmark::State& state) {
    memepp::string a(kA), b(kB);
    for (auto _ : state) benchmark::DoNotOptimize(a <= b);
}
BENCHMARK(MemeStr_OpLe_StrStr)->Iterations(MMBM_ITERATION_COUNT);

// --- operator> (string vs string) ---
static void MemeStr_OpGt_StrStr(benchmark::State& state) {
    memepp::string a(kA), c(kC);
    for (auto _ : state) benchmark::DoNotOptimize(a > c);
}
BENCHMARK(MemeStr_OpGt_StrStr)->Iterations(MMBM_ITERATION_COUNT);

// --- operator>= (string vs string) ---
static void MemeStr_OpGe_StrStr(benchmark::State& state) {
    memepp::string a(kA), b(kB);
    for (auto _ : state) benchmark::DoNotOptimize(a >= b);
}
BENCHMARK(MemeStr_OpGe_StrStr)->Iterations(MMBM_ITERATION_COUNT);

// C equivalents
static void C_OpEq(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(std::strcmp(kA, kB) == 0);
}
BENCHMARK(C_OpEq)->Iterations(MMBM_ITERATION_COUNT);

static void C_OpNe(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(std::strcmp(kA, kB) != 0);
}
BENCHMARK(C_OpNe)->Iterations(MMBM_ITERATION_COUNT);

static void C_OpLt(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(std::strcmp(kA, kC) < 0);
}
BENCHMARK(C_OpLt)->Iterations(MMBM_ITERATION_COUNT);

static void C_OpLe(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(std::strcmp(kA, kB) <= 0);
}
BENCHMARK(C_OpLe)->Iterations(MMBM_ITERATION_COUNT);

// ===========================================================================
// 2. operator+ variants
// ===========================================================================

// string + char (single char)
// No operator+(string, char) in this library; the closest is
// string_builder += char.
static void MemeStr_PlusChar(benchmark::State& state) {
    for (auto _ : state) {
        // Constructed per iteration: declaring it outside would accumulate one
        // string + one char per round and the builder would grow unboundedly.
        memepp::string_builder sb;
        sb += memepp::string(kA);
        sb += '!';
        benchmark::DoNotOptimize(sb.size());
    }
}
BENCHMARK(MemeStr_PlusChar)->Iterations(MMBM_ITERATION_COUNT / 10);

// string + string_view
static void MemeStr_PlusView(benchmark::State& state) {
    memepp::string a(kA);
    memepp::string_view sv(kB);
    for (auto _ : state) benchmark::DoNotOptimize(a + sv);
}
BENCHMARK(MemeStr_PlusView)->Iterations(MMBM_ITERATION_COUNT / 10);

// string + const string& (already tested as MemeStr_Concat, but confirm
// operator+ specifically)
static void MemeStr_OpPlusStr(benchmark::State& state) {
    memepp::string a(kA), b(kB);
    for (auto _ : state) benchmark::DoNotOptimize(a + b);
}
BENCHMARK(MemeStr_OpPlusStr)->Iterations(MMBM_ITERATION_COUNT / 10);

// C equivalents: malloc+memcpy for concat
static void C_Concat2(benchmark::State& state) {
    for (auto _ : state) {
        size_t n1 = std::strlen(kA), n2 = std::strlen(kB);
        char* out = static_cast<char*>(std::malloc(n1 + n2 + 1));
        std::memcpy(out, kA, n1);
        std::memcpy(out + n1, kB, n2);
        out[n1 + n2] = '\0';
        benchmark::DoNotOptimize(out[0]);
        std::free(out);
    }
}
BENCHMARK(C_Concat2)->Iterations(MMBM_ITERATION_COUNT / 10);

// ===========================================================================
// 3. C-side addendum: operations that have ISO C equivalents but lacked
//    benchmarks.
// ===========================================================================

// 3a. at() vs s[i]  (boundary-checked vs raw)
static void C_At(benchmark::State& state) {
    // ISO C: raw pointer access (no bounds check — the "C equivalent")
    const char* s = kA;
    for (auto _ : state) benchmark::DoNotOptimize(s[6]);
}
BENCHMARK(C_At)->Iterations(MMBM_ITERATION_COUNT);

// 3b. front()/back() vs s[0]/s[n-1]
static void C_Front(benchmark::State& state) {
    for (auto _ : state) benchmark::DoNotOptimize(kA[0]);
}
BENCHMARK(C_Front)->Iterations(MMBM_ITERATION_COUNT);

static void C_Back(benchmark::State& state) {
    size_t n = std::strlen(kA);
    for (auto _ : state) benchmark::DoNotOptimize(kA[n - 1]);
}
BENCHMARK(C_Back)->Iterations(MMBM_ITERATION_COUNT);

// 3c. find(str, pos) vs strstr(s+pos, needle)
static void C_FindFromPos(benchmark::State& state) {
    const char* hay = "The quick brown fox jumps over the lazy dog.";
    for (auto _ : state) {
        const char* p = std::strstr(hay + 10, "fox");
        benchmark::DoNotOptimize(p ? static_cast<int>(p - hay) : -1);
    }
}
BENCHMARK(C_FindFromPos)->Iterations(MMBM_ITERATION_COUNT);

static void C_FindCharFromPos(benchmark::State& state) {
    const char* hay = "The quick brown fox jumps over the lazy dog.";
    for (auto _ : state) {
        const char* p = std::strchr(hay + 10, 'o');
        benchmark::DoNotOptimize(p ? static_cast<int>(p - hay) : -1);
    }
}
BENCHMARK(C_FindCharFromPos)->Iterations(MMBM_ITERATION_COUNT);

// 3d. rfind(str, pos) — hand-written reverse strstr from pos
static const char* c_rfind_str_pos(const char* hay, size_t n, const char* nd,
                                    size_t ndl, size_t pos)
{
    if (pos + ndl > n) pos = n - ndl;
    for (size_t i = pos + 1; i-- > 0; )
        if (std::memcmp(hay + i, nd, ndl) == 0)
            return hay + i;
    return nullptr;
}

static void C_RfindFromPos(benchmark::State& state) {
    const char* hay = "The quick brown fox jumps over the lazy dog.";
    size_t n = std::strlen(hay);
    for (auto _ : state) {
        const char* p = c_rfind_str_pos(hay, n, "fox", 3, 30);
        benchmark::DoNotOptimize(p ? static_cast<int>(p - hay) : -1);
    }
}
BENCHMARK(C_RfindFromPos)->Iterations(MMBM_ITERATION_COUNT);

static void C_RfindCharFromPos(benchmark::State& state) {
    const char* hay = "The quick brown fox jumps over the lazy dog.";
    size_t n = std::strlen(hay);
    for (auto _ : state) {
        const char* p = std::strrchr(hay + 10, 'o');
        benchmark::DoNotOptimize(p ? static_cast<int>(p - hay) : -1);
    }
}
BENCHMARK(C_RfindCharFromPos)->Iterations(MMBM_ITERATION_COUNT);

// 3e. substr(pos) / substr(pos,count) vs malloc+memcpy
static void C_SubstrPos(benchmark::State& state) {
    const char* s = "The quick brown fox jumps over the lazy dog.";
    size_t n = std::strlen(s);
    for (auto _ : state) {
        size_t sub_len = n - 10;
        char* sub = static_cast<char*>(std::malloc(sub_len + 1));
        std::memcpy(sub, s + 10, sub_len);
        sub[sub_len] = '\0';
        benchmark::DoNotOptimize(sub[0]);
        std::free(sub);
    }
}
BENCHMARK(C_SubstrPos)->Iterations(MMBM_ITERATION_COUNT / 10);

static void C_SubstrPosCount(benchmark::State& state) {
    const char* s = "The quick brown fox jumps over the lazy dog.";
    for (auto _ : state) {
        char* sub = static_cast<char*>(std::malloc(21));
        std::memcpy(sub, s + 10, 20);
        sub[20] = '\0';
        benchmark::DoNotOptimize(sub[0]);
        std::free(sub);
    }
}
BENCHMARK(C_SubstrPosCount)->Iterations(MMBM_ITERATION_COUNT / 10);

// 3f. to_upper vs toupper loop
static void C_ToUpper(benchmark::State& state) {
    const char* s = "The quick brown fox jumps over the lazy dog.";
    size_t n = std::strlen(s);
    for (auto _ : state) {
        char* out = static_cast<char*>(std::malloc(n + 1));
        std::memcpy(out, s, n + 1);
        for (size_t i = 0; i < n; ++i)
            out[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(out[i])));
        benchmark::DoNotOptimize(out[0]);
        std::free(out);
    }
}
BENCHMARK(C_ToUpper)->Iterations(MMBM_ITERATION_COUNT / 10);

static void C_ToLower(benchmark::State& state) {
    const char* s = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.";
    size_t n = std::strlen(s);
    for (auto _ : state) {
        char* out = static_cast<char*>(std::malloc(n + 1));
        std::memcpy(out, s, n + 1);
        for (size_t i = 0; i < n; ++i)
            out[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(out[i])));
        benchmark::DoNotOptimize(out[0]);
        std::free(out);
    }
}
BENCHMARK(C_ToLower)->Iterations(MMBM_ITERATION_COUNT / 10);

// 3g. reverse — two-pointer swap
static void C_Reverse(benchmark::State& state) {
    const char* s = "The quick brown fox jumps over the lazy dog.";
    size_t n = std::strlen(s);
    for (auto _ : state) {
        char* out = static_cast<char*>(std::malloc(n + 1));
        std::memcpy(out, s, n + 1);
        for (size_t i = 0, j = n - 1; i < j; ++i, --j) {
            char tmp = out[i];
            out[i] = out[j];
            out[j] = tmp;
        }
        benchmark::DoNotOptimize(out[0]);
        std::free(out);
    }
}
BENCHMARK(C_Reverse)->Iterations(MMBM_ITERATION_COUNT / 10);

// 3h. trim_space — skip leading/trailing whitespace
static void C_TrimSpace(benchmark::State& state) {
    const char* s = "      Hello World!      ";
    size_t n = std::strlen(s);
    for (auto _ : state) {
        size_t b = 0;
        while (b < n && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
        size_t e = n;
        while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) --e;
        benchmark::DoNotOptimize(e - b);
    }
}
BENCHMARK(C_TrimSpace)->Iterations(MMBM_ITERATION_COUNT / 10);

// 3i. is_only_ascii — byte > 127 check
static void C_IsAscii(benchmark::State& state) {
    const char* s = "The quick brown fox jumps over the lazy dog.";
    size_t n = std::strlen(s);
    for (auto _ : state) {
        bool ascii = true;
        for (size_t i = 0; i < n; ++i)
            if (static_cast<unsigned char>(s[i]) > 127) { ascii = false; break; }
        benchmark::DoNotOptimize(ascii);
    }
}
BENCHMARK(C_IsAscii)->Iterations(MMBM_ITERATION_COUNT / 10);

// 3j. count(needle) — loop strstr
static void C_Count(benchmark::State& state) {
    const char* hay = "The quick brown fox jumps over the lazy dog. Pack my box with five dozen liquor jugs.";
    const char* nd = "the";
    for (auto _ : state) {
        int count = 0;
        const char* p = hay;
        while ((p = std::strstr(p, nd)) != nullptr) {
            ++count;
            ++p;
        }
        benchmark::DoNotOptimize(count);
    }
}
BENCHMARK(C_Count)->Iterations(MMBM_ITERATION_COUNT / 10);

// 3k. repeat(n) — loop memcpy
static void C_Repeat(benchmark::State& state) {
    const char* src = "ab";
    size_t sl = 2;
    size_t reps = 20;
    for (auto _ : state) {
        char* out = static_cast<char*>(std::malloc(sl * reps + 1));
        for (size_t i = 0; i < reps; ++i)
            std::memcpy(out + i * sl, src, sl);
        out[sl * reps] = '\0';
        benchmark::DoNotOptimize(out[0]);
        std::free(out);
    }
}
BENCHMARK(C_Repeat)->Iterations(MMBM_ITERATION_COUNT / 10);

// 3l. to_string(int) vs sprintf
static void C_ToStringI32(benchmark::State& state) {
    for (auto _ : state) {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%d", 1234567890);
        benchmark::DoNotOptimize(buf[0]);
    }
}
BENCHMARK(C_ToStringI32)->Iterations(MMBM_ITERATION_COUNT / 10);

static void C_ToStringI64(benchmark::State& state) {
    for (auto _ : state) {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%lld", (long long)1234567890123456789LL);
        benchmark::DoNotOptimize(buf[0]);
    }
}
BENCHMARK(C_ToStringI64)->Iterations(MMBM_ITERATION_COUNT / 10);

// 3m. from_hexadecimals — sscanf("%02hhx") loop
static int hex_val(char c) {
    return (c >= '0' && c <= '9') ? (c - '0')
         : (c >= 'A' && c <= 'F') ? (c - 'A' + 10)
         : (c >= 'a' && c <= 'f') ? (c - 'a' + 10)
         : 0;
}
static void C_FromHex(benchmark::State& state) {
    const char* hex = "DEADBEEF0123456789ABCDEF01234567";
    size_t hexlen = std::strlen(hex);
    for (auto _ : state) {
        uint8_t* out = static_cast<uint8_t*>(std::malloc(hexlen / 2));
        for (size_t i = 0; i + 1 < hexlen; i += 2)
            out[i / 2] = static_cast<uint8_t>((hex_val(hex[i]) << 4) | hex_val(hex[i+1]));
        benchmark::DoNotOptimize(out[0]);
        std::free(out);
    }
}
BENCHMARK(C_FromHex)->Iterations(MMBM_ITERATION_COUNT / 10);

// 3n. compare(pos, len, str) vs strncmp(s+pos, t, len)
static void C_ComparePosLen(benchmark::State& state) {
    const char* s = "The quick brown fox jumps over the lazy dog.";
    const char* t = "quick brown fox";
    for (auto _ : state) benchmark::DoNotOptimize(std::strncmp(s + 4, t, 15));
}
BENCHMARK(C_ComparePosLen)->Iterations(MMBM_ITERATION_COUNT);

// 3o. find_first_of vs strpbrk
static void C_FindFirstOf(benchmark::State& state) {
    const char* s = "The quick brown fox jumps over the lazy dog.";
    for (auto _ : state) {
        const char* p = std::strpbrk(s, "aeiou");
        benchmark::DoNotOptimize(p ? static_cast<int>(p - s) : -1);
    }
}
BENCHMARK(C_FindFirstOf)->Iterations(MMBM_ITERATION_COUNT);

static void C_FindFirstNotOf(benchmark::State& state) {
    const char* s = "   \t\nHello World!";
    for (auto _ : state) {
        const char* p = s;
        while (*p && std::isspace(static_cast<unsigned char>(*p))) ++p;
        benchmark::DoNotOptimize(p ? static_cast<int>(p - s) : -1);
    }
}
BENCHMARK(C_FindFirstNotOf)->Iterations(MMBM_ITERATION_COUNT);

} // namespace
