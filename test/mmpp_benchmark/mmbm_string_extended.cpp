// Extended memepp::string benchmarks covering missing interfaces
// Compares memepp::string vs std::string vs C stdlib where applicable

#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <cstdlib>

// Test data
static const char* g_medium = "The quick brown fox jumps over the lazy dog. " "Pack my box with five dozen liquor jugs.";
static const char* g_with_spaces = "   Hello World!   ";
static const char* g_csv = "apple,banana,cherry,date,elderberry";
static const char* g_repeat_src = "ab";

// ============================================================================
// REPLACE
static void MemeStr_Replace(benchmark::State& state) {
    memepp::string s("Hello World! Hello World! Hello World!");
    memepp::string_view from("World");
    memepp::string_view to("Universe");
    for (auto _ : state) {
        memepp::string result = s.replace(from, to);
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(MemeStr_Replace)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Replace(benchmark::State& state) {
    std::string s("Hello World! Hello World! Hello World!");
    for (auto _ : state) {
        std::string result = s;
        size_t pos = 0;
        while ((pos = result.find("World", pos)) != std::string::npos) {
            result.replace(pos, 5, "Universe");
            pos += 8;
        }
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(StdStr_Replace)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// TRIM SPACE
static void MemeStr_TrimSpace(benchmark::State& state) {
    memepp::string s(g_with_spaces);
    for (auto _ : state) {
        memepp::string result = s.trim_space();
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(MemeStr_TrimSpace)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_TrimSpace(benchmark::State& state) {
    std::string s(g_with_spaces);
    for (auto _ : state) {
        size_t start = s.find_first_not_of(" \t\n\r");
        size_t end = s.find_last_not_of(" \t\n\r");
        std::string result = (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(StdStr_TrimSpace)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// TRIM LEFT
static void MemeStr_TrimLeft(benchmark::State& state) {
    memepp::string s(g_with_spaces);
    for (auto _ : state) {
        memepp::string result = s.trim_left_space();
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(MemeStr_TrimLeft)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_TrimLeft(benchmark::State& state) {
    std::string s(g_with_spaces);
    for (auto _ : state) {
        size_t start = s.find_first_not_of(" \t\n\r");
        std::string result = (start == std::string::npos) ? "" : s.substr(start);
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(StdStr_TrimLeft)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// TRIM RIGHT
static void MemeStr_TrimRight(benchmark::State& state) {
    memepp::string s(g_with_spaces);
    for (auto _ : state) {
        memepp::string result = s.trim_right_space();
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(MemeStr_TrimRight)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_TrimRight(benchmark::State& state) {
    std::string s(g_with_spaces);
    for (auto _ : state) {
        size_t end = s.find_last_not_of(" \t\n\r");
        std::string result = (end == std::string::npos) ? "" : s.substr(0, end + 1);
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(StdStr_TrimRight)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// TO UPPER
static void MemeStr_ToUpper(benchmark::State& state) {
    memepp::string s("Hello World!");
    for (auto _ : state) {
        memepp::string result = s.to_en_upper();
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(MemeStr_ToUpper)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_ToUpper(benchmark::State& state) {
    std::string s("Hello World!");
    for (auto _ : state) {
        std::string result = s;
        for (auto& c : result) c = static_cast<char>(std::toupper(c));
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(StdStr_ToUpper)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// TO LOWER
static void MemeStr_ToLower(benchmark::State& state) {
    memepp::string s("HELLO WORLD!");
    for (auto _ : state) {
        memepp::string result = s.to_en_lower();
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(MemeStr_ToLower)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_ToLower(benchmark::State& state) {
    std::string s("HELLO WORLD!");
    for (auto _ : state) {
        std::string result = s;
        for (auto& c : result) c = static_cast<char>(std::tolower(c));
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(StdStr_ToLower)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// REVERSE
static void MemeStr_Reverse(benchmark::State& state) {
    memepp::string s("Hello World!");
    for (auto _ : state) {
        memepp::string result = s.reverse();
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(MemeStr_Reverse)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Reverse(benchmark::State& state) {
    std::string s("Hello World!");
    for (auto _ : state) {
        std::string result = s;
        std::reverse(result.begin(), result.end());
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(StdStr_Reverse)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// REPEAT
static void MemeStr_Repeat(benchmark::State& state) {
    memepp::string s(g_repeat_src);
    for (auto _ : state) {
        memepp::string result = s.repeat(10);
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(MemeStr_Repeat)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Repeat(benchmark::State& state) {
    std::string s(g_repeat_src);
    for (auto _ : state) {
        std::string result;
        for (int i = 0; i < 10; ++i) result += s;
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(StdStr_Repeat)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// STARTS WITH
static void MemeStr_StartsWith(benchmark::State& state) {
    memepp::string s(g_medium);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.starts_with("The quick"));
}
BENCHMARK(MemeStr_StartsWith)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_StartsWith(benchmark::State& state) {
    std::string s(g_medium);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.compare(0, 9, "The quick") == 0);
}
BENCHMARK(StdStr_StartsWith)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// ENDS WITH
static void MemeStr_EndsWith(benchmark::State& state) {
    memepp::string s(g_medium);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.ends_with("jugs."));
}
BENCHMARK(MemeStr_EndsWith)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_EndsWith(benchmark::State& state) {
    std::string s(g_medium);
    for (auto _ : state) {
        const char* suffix = "jugs.";
        size_t len = strlen(suffix);
        benchmark::DoNotOptimize(s.size() >= len && s.compare(s.size() - len, len, suffix) == 0);
    }
}
BENCHMARK(StdStr_EndsWith)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// CONTAINS
static void MemeStr_Contains(benchmark::State& state) {
    memepp::string s(g_medium);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.contains("brown"));
}
BENCHMARK(MemeStr_Contains)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Contains(benchmark::State& state) {
    std::string s(g_medium);
    for (auto _ : state)
        benchmark::DoNotOptimize(s.find("brown") != std::string::npos);
}
BENCHMARK(StdStr_Contains)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// SPLIT (by comma)
static void MemeStr_Split(benchmark::State& state) {
    memepp::string s(g_csv);
    for (auto _ : state) {
        std::vector<memepp::string_view> parts;
        s.split(",", memepp::split_behavior_t::skip_empty_parts,
                std::back_inserter(parts));
        benchmark::DoNotOptimize(parts.size());
    }
}
BENCHMARK(MemeStr_Split)->Iterations(MMBM_ITERATION_COUNT / 10);

static void StdStr_Split(benchmark::State& state) {
    std::string s(g_csv);
    for (auto _ : state) {
        std::vector<std::string> parts;
        size_t start = 0;
        size_t end = s.find(',');
        while (end != std::string::npos) {
            parts.push_back(s.substr(start, end - start));
            start = end + 1;
            end = s.find(',', start);
        }
        parts.push_back(s.substr(start));
        benchmark::DoNotOptimize(parts.size());
    }
}
BENCHMARK(StdStr_Split)->Iterations(MMBM_ITERATION_COUNT / 10);

// ============================================================================
// JOIN
static void MemeStr_Join(benchmark::State& state) {
    std::vector<memepp::string> parts = {"apple", "banana", "cherry", "date"};
    memepp::string sep(", ");
    for (auto _ : state) {
        memepp::string result = sep.join(parts);
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(MemeStr_Join)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Join(benchmark::State& state) {
    std::vector<std::string> parts = {"apple", "banana", "cherry", "date"};
    for (auto _ : state) {
        std::string result;
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i > 0) result += ", ";
            result += parts[i];
        }
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(StdStr_Join)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// LEFT / RIGHT
static void MemeStr_Left(benchmark::State& state) {
    memepp::string s(g_medium);
    for (auto _ : state) {
        memepp::string result = s.left(10);
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(MemeStr_Left)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Left(benchmark::State& state) {
    std::string s(g_medium);
    for (auto _ : state) {
        std::string result = s.substr(0, 10);
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(StdStr_Left)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_Right(benchmark::State& state) {
    memepp::string s(g_medium);
    for (auto _ : state) {
        memepp::string result = s.right(10);
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(MemeStr_Right)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Right(benchmark::State& state) {
    std::string s(g_medium);
    for (auto _ : state) {
        std::string result = s.substr(s.size() - 10);
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(StdStr_Right)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// IS ONLY ASCII
static void MemeStr_IsOnlyAscii(benchmark::State& state) {
    memepp::string s("Hello World! This is ASCII only.");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.is_only_ascii());
}
BENCHMARK(MemeStr_IsOnlyAscii)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_IsOnlyAscii(benchmark::State& state) {
    std::string s("Hello World! This is ASCII only.");
    for (auto _ : state) {
        bool ascii = true;
        for (unsigned char c : s) {
            if (c > 127) { ascii = false; break; }
        }
        benchmark::DoNotOptimize(ascii);
    }
}
BENCHMARK(StdStr_IsOnlyAscii)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// RUNE SIZE (UTF-8 code point count)
static void MemeStr_RuneSize(benchmark::State& state) {
    memepp::string s("Hello \xE4\xB8\xAD\xE6\x96\x87!");  // Hello 中文!
    for (auto _ : state)
        benchmark::DoNotOptimize(s.rune_size());
}
BENCHMARK(MemeStr_RuneSize)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// TO SHARED STORAGE
static void MemeStr_ToShared(benchmark::State& state) {
    memepp::string s("Hello World!");
    for (auto _ : state) {
        memepp::string result = s.to_shared_storage();
        benchmark::DoNotOptimize(result.data());
    }
}
BENCHMARK(MemeStr_ToShared)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// CHEAP COPY (refcount bump on shared storage)
static void MemeStr_CheapCopy(benchmark::State& state) {
    memepp::string s("Hello World! This is a longer string.");
    memepp::string shared = s.to_shared_storage();
    for (auto _ : state) {
        memepp::string copy = shared.cheap_copy();
        benchmark::DoNotOptimize(copy.data());
    }
}
BENCHMARK(MemeStr_CheapCopy)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Copy(benchmark::State& state) {
    std::string s("Hello World! This is a longer string.");
    for (auto _ : state) {
        std::string copy = s;
        benchmark::DoNotOptimize(copy.data());
    }
}
BENCHMARK(StdStr_Copy)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// CUT (split into two parts)
static void MemeStr_Cut(benchmark::State& state) {
    memepp::string s("Hello=World");
    for (auto _ : state) {
        auto [left, right, found] = s.cut("=");
        benchmark::DoNotOptimize(left.data());
        benchmark::DoNotOptimize(right.data());
        benchmark::DoNotOptimize(found);
    }
}
BENCHMARK(MemeStr_Cut)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// ASSIGNMENT
static void MemeStr_Assign(benchmark::State& state) {
    memepp::string s1("Hello");
    memepp::string s2("World!");
    for (auto _ : state) {
        s1 = s2;
        benchmark::DoNotOptimize(s1.data());
    }
}
BENCHMARK(MemeStr_Assign)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Assign(benchmark::State& state) {
    std::string s1("Hello");
    std::string s2("World!");
    for (auto _ : state) {
        s1 = s2;
        benchmark::DoNotOptimize(s1.data());
    }
}
BENCHMARK(StdStr_Assign)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// CAPACITY
static void MemeStr_Capacity(benchmark::State& state) {
    memepp::string s("Hello World!");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.capacity());
}
BENCHMARK(MemeStr_Capacity)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Capacity(benchmark::State& state) {
    std::string s("Hello World!");
    for (auto _ : state)
        benchmark::DoNotOptimize(s.capacity());
}
BENCHMARK(StdStr_Capacity)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// FRONT/BACK
static void MemeStr_FrontBack(benchmark::State& state) {
    memepp::string s("Hello World!");
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.front());
        benchmark::DoNotOptimize(s.back());
    }
}
BENCHMARK(MemeStr_FrontBack)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_FrontBack(benchmark::State& state) {
    std::string s("Hello World!");
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.front());
        benchmark::DoNotOptimize(s.back());
    }
}
BENCHMARK(StdStr_FrontBack)->Iterations(MMBM_ITERATION_COUNT);

// ============================================================================
// SWAP
static void MemeStr_Swap(benchmark::State& state) {
    memepp::string s1("Hello");
    memepp::string s2("World!");
    for (auto _ : state) {
        s1.swap(s2);
        benchmark::DoNotOptimize(s1.data());
    }
}
BENCHMARK(MemeStr_Swap)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_Swap(benchmark::State& state) {
    std::string s1("Hello");
    std::string s2("World!");
    for (auto _ : state) {
        s1.swap(s2);
        benchmark::DoNotOptimize(s1.data());
    }
}
BENCHMARK(StdStr_Swap)->Iterations(MMBM_ITERATION_COUNT);
