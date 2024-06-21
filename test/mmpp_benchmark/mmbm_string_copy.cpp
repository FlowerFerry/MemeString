
#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>

static void MemeStr_ShortStrCopy(benchmark::State& _state)
{
    memepp::string s("Hello!");
    for (auto _ : _state)
        memepp::string copy(s);
}
BENCHMARK(MemeStr_ShortStrCopy)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_ShortStrCopy(benchmark::State& _state)
{
    std::string s("Hello!");
    for (auto _ : _state)
        std::string copy(s);
}
BENCHMARK(StdStr_ShortStrCopy)->Iterations(MMBM_ITERATION_COUNT);
