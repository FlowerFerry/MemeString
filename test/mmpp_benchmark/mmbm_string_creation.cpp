
#include <benchmark/benchmark.h>
#include "common.hpp"

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/variable_buffer.hpp>

static void MemeStr_CreationDefault(benchmark::State& _state)
{
    for (auto _ : _state)
        memepp::string empty_string;
}
BENCHMARK(MemeStr_CreationDefault)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_CreationDefault(benchmark::State& _state)
{
    for (auto _ : _state)
        std::string empty_string;
}
BENCHMARK(StdStr_CreationDefault)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_CreationByShortCCStr(benchmark::State& _state)
{
    for (auto _ : _state)
        memepp::string s{ "Hello!" };
}
BENCHMARK(MemeStr_CreationByShortCCStr)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_CreationByShortCStr(benchmark::State& _state)
{
    for (auto _ : _state)
        std::string s{ "Hello!" };
}
BENCHMARK(StdStr_CreationByShortCStr)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_CreationByMudiumCStr(benchmark::State& _state)
{
    const char* str = "Hello, my name is John Doe!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    for (auto _ : _state)
        memepp::string s{ str };
}
BENCHMARK(MemeStr_CreationByMudiumCStr)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_CreationByMudiumCStr(benchmark::State& _state)
{
    const char* str = "Hello, my name is John Doe!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    for (auto _ : _state)
        std::string s{ str };
}
BENCHMARK(StdStr_CreationByMudiumCStr)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_CreationByLargeCStr(benchmark::State& _state)
{
    const char* str = "Hello, my name is John Doe! "
        "I am a software developer and I am working on a project called Meme++! "
        "It is a C++ library that provides a lot of useful tools for developers! "
        "It is a very cool project and I am very proud of it! "
        "I hope you will like it too! "
        "Have a nice day!"
        "Hello, my name is John Doe! "
        "I am a software developer and I am working on a project called Meme++! "
        "It is a C++ library that provides a lot of useful tools for developers! "
        "It is a very cool project and I am very proud of it! "
        "I hope you will like it too! "
        "Have a nice day!";
    for (auto _ : _state)
        memepp::string s{ str };
}
BENCHMARK(MemeStr_CreationByLargeCStr)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_CreationByLargeCStr(benchmark::State& _state)
{
    const char* str = "Hello, my name is John Doe! "
        "I am a software developer and I am working on a project called Meme++! "
        "It is a C++ library that provides a lot of useful tools for developers! "
        "It is a very cool project and I am very proud of it! "
        "I hope you will like it too! "
        "Have a nice day!"
        "Hello, my name is John Doe! "
        "I am a software developer and I am working on a project called Meme++! "
        "It is a C++ library that provides a lot of useful tools for developers! "
        "It is a very cool project and I am very proud of it! "
        "I hope you will like it too! "
        "Have a nice day!";
    for (auto _ : _state)
        std::string s{ str };
}
BENCHMARK(StdStr_CreationByLargeCStr)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_CreationByShortStr(benchmark::State& _state)
{
    memepp::string s0("Hello!");
    for (auto _ : _state)
        memepp::string s{ s0 };
}
BENCHMARK(MemeStr_CreationByShortStr)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_CreationByShortStr(benchmark::State& _state)
{
    std::string s0("Hello!");
    for (auto _ : _state)
        std::string s{ s0 };
}
BENCHMARK(StdStr_CreationByShortStr)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_CreationByMudiumStr(benchmark::State& _state)
{
    memepp::string str =
        "Four-leaf clover is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
        "In the West, it is considered a sign of great good fortune to find four-leaf clover.";
    for (auto _ : _state)
        memepp::string s{ str };
}
BENCHMARK(MemeStr_CreationByMudiumStr)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_CreationByMudiumStr(benchmark::State& _state)
{
    std::string str =
        "Four-leaf clover is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
        "In the West, it is considered a sign of great good fortune to find four-leaf clover.";
    for (auto _ : _state)
        std::string s{ str };
}
BENCHMARK(StdStr_CreationByMudiumStr)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_CreationByLargeStr(benchmark::State& _state)
{
    memepp::string str = "Hello, my name is John Doe! "
        "I am a software developer and I am working on a project called Meme++! "
        "It is a C++ library that provides a lot of useful tools for developers! "
        "It is a very cool project and I am very proud of it! "
        "I hope you will like it too! "
        "Have a nice day!"
        "Hello, my name is John Doe! "
        "I am a software developer and I am working on a project called Meme++! "
        "It is a C++ library that provides a lot of useful tools for developers! "
        "It is a very cool project and I am very proud of it! "
        "I hope you will like it too! "
        "Have a nice day!";
    for (auto _ : _state)
        memepp::string s{ str };
}
BENCHMARK(MemeStr_CreationByLargeStr)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_CreationByLargeStr(benchmark::State& _state)
{
    std::string str = "Hello, my name is John Doe! "
        "I am a software developer and I am working on a project called Meme++! "
        "It is a C++ library that provides a lot of useful tools for developers! "
        "It is a very cool project and I am very proud of it! "
        "I hope you will like it too! "
        "Have a nice day!"
        "Hello, my name is John Doe! "
        "I am a software developer and I am working on a project called Meme++! "
        "It is a C++ library that provides a lot of useful tools for developers! "
        "It is a very cool project and I am very proud of it! "
        "I hope you will like it too! "
        "Have a nice day!";
    for (auto _ : _state)
        std::string s{ str };
}
BENCHMARK(StdStr_CreationByLargeStr)->Iterations(MMBM_ITERATION_COUNT);

static void MemeStr_CreationByEmptyMove(benchmark::State& _state)
{
    memepp::string s0;
    for (auto _ : _state)
        memepp::string s{ std::move(s0) };
}
BENCHMARK(MemeStr_CreationByEmptyMove)->Iterations(MMBM_ITERATION_COUNT);

static void StdStr_CreationByEmptyMove(benchmark::State& _state)
{
    std::string s0;
    for (auto _ : _state)
        std::string s{ std::move(s0) };
}
BENCHMARK(StdStr_CreationByEmptyMove)->Iterations(MMBM_ITERATION_COUNT);
