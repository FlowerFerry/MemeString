#include <catch2/catch.hpp>
#include <megopp/thrd/numeric_id.h>

#include <thread>
#include <vector>

using mgpp::thrd::numeric_id;

TEST_CASE("numeric_id - returns non-zero for the main thread", "[numeric_id]")
{
    REQUIRE(numeric_id() != 0u);
}

TEST_CASE("numeric_id - same thread returns same id on repeated calls", "[numeric_id]")
{
    size_t id1 = numeric_id();
    size_t id2 = numeric_id();
    REQUIRE(id1 == id2);
}

TEST_CASE("numeric_id - two concurrent threads have different ids", "[numeric_id]")
{
    size_t id_main = numeric_id();
    size_t id_other = 0;

    std::thread t([&] { id_other = numeric_id(); });
    t.join();

    REQUIRE(id_other != 0u);
    REQUIRE(id_main != id_other);
}

TEST_CASE("numeric_id - four threads all have unique ids", "[numeric_id]")
{
    constexpr int N = 4;
    std::vector<size_t> ids(N, 0);

    std::vector<std::thread> threads;
    threads.reserve(N);
    for (int i = 0; i < N; ++i) {
        threads.emplace_back([&ids, i] { ids[i] = numeric_id(); });
    }
    for (auto& t : threads)
        t.join();

    // All ids must be non-zero and distinct
    for (int i = 0; i < N; ++i)
        REQUIRE(ids[i] != 0u);

    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            REQUIRE(ids[i] != ids[j]);
}
