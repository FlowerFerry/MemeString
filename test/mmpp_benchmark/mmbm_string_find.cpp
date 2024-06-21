
#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>

static void MemeStr_FindChar(benchmark::State& _state)
{
    memepp::string s("Hello!");
    for (auto _ : _state)
        s.find("l");
}
BENCHMARK(MemeStr_FindChar)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_FindChar(benchmark::State& _state)
{
    std::string s("Hello!");
    for (auto _ : _state)
        s.find("l");
}
BENCHMARK(StdStr_FindChar)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_FindShortStr(benchmark::State& _state)
{
    memepp::string s("Hello!");
    for (auto _ : _state)
        s.find("lo");
}
BENCHMARK(MemeStr_FindShortStr)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_FindShortStr(benchmark::State& _state)
{
    std::string s("Hello!");
    for (auto _ : _state)
        s.find("lo");
}
BENCHMARK(StdStr_FindShortStr)->Iterations(MMBM_ITERATION_COUNT);
