#include <catch2/catch.hpp>
#include <megopp/util/atomic_counter.h>

#include <thread>
#include <vector>

using mgpp::util::atomic_counter;

TEST_CASE("atomic_counter - default construction is zero", "[atomic_counter]")
{
    atomic_counter<int> c;
    REQUIRE(static_cast<int>(c) == 0);
}

TEST_CASE("atomic_counter - value construction", "[atomic_counter]")
{
    atomic_counter<int> c{42};
    REQUIRE(static_cast<int>(c) == 42);
}

TEST_CASE("atomic_counter - out-of-range construction resets to zero", "[atomic_counter]")
{
    // val=100 but maxVal=50, so it should reset to 0
    atomic_counter<int> c{100, 0, 50};
    REQUIRE(static_cast<int>(c) == 0);
}

TEST_CASE("atomic_counter - prefix increment", "[atomic_counter]")
{
    atomic_counter<int> c{0};
    ++c;
    REQUIRE(static_cast<int>(c) == 1);
}

TEST_CASE("atomic_counter - prefix decrement", "[atomic_counter]")
{
    atomic_counter<int> c{5};
    --c;
    REQUIRE(static_cast<int>(c) == 4);
}

TEST_CASE("atomic_counter - postfix increment returns old value", "[atomic_counter]")
{
    atomic_counter<int> c{3};
    int old = c++;
    REQUIRE(old == 3);
    REQUIRE(static_cast<int>(c) == 4);
}

TEST_CASE("atomic_counter - postfix decrement returns old value", "[atomic_counter]")
{
    atomic_counter<int> c{3};
    int old = c--;
    REQUIRE(old == 3);
    REQUIRE(static_cast<int>(c) == 2);
}

TEST_CASE("atomic_counter - operator+=", "[atomic_counter]")
{
    atomic_counter<int> c{10};
    c += 5;
    REQUIRE(static_cast<int>(c) == 15);
}

TEST_CASE("atomic_counter - operator-=", "[atomic_counter]")
{
    atomic_counter<int> c{10};
    c -= 3;
    REQUIRE(static_cast<int>(c) == 7);
}

TEST_CASE("atomic_counter - reset", "[atomic_counter]")
{
    atomic_counter<int> c{99};
    c.reset(0);
    REQUIRE(static_cast<int>(c) == 0);
}

TEST_CASE("atomic_counter - operator== and !=", "[atomic_counter]")
{
    atomic_counter<int> a{5}, b{5}, d{6};
    REQUIRE(a == b);
    REQUIRE(a != d);
}

TEST_CASE("atomic_counter - wraps to minVal when exceeding maxVal on increment", "[atomic_counter]")
{
    // min=0, max=3, start at 3; incrementing should wrap to 0
    atomic_counter<int> c{3, 0, 3};
    c++;
    REQUIRE(static_cast<int>(c) == 0);
}

TEST_CASE("atomic_counter - wraps to maxVal when exceeding minVal on decrement", "[atomic_counter]")
{
    // min=0, max=3, start at 0; decrementing should wrap to 3
    atomic_counter<int> c{0, 0, 3};
    c--;
    REQUIRE(static_cast<int>(c) == 3);
}

TEST_CASE("atomic_counter - get_and_inc returns old value", "[atomic_counter]")
{
    atomic_counter<int> c{10};
    int old = c.get_and_inc(3);
    REQUIRE(old == 10);
    REQUIRE(static_cast<int>(c) == 13);
}

TEST_CASE("atomic_counter - get_and_dec returns old value", "[atomic_counter]")
{
    atomic_counter<int> c{10};
    int old = c.get_and_dec(4);
    REQUIRE(old == 10);
    REQUIRE(static_cast<int>(c) == 6);
}

TEST_CASE("atomic_counter - get_and_inc with zero is no-op", "[atomic_counter]")
{
    atomic_counter<int> c{7};
    int old = c.get_and_inc(0);
    REQUIRE(old == 7);
    REQUIRE(static_cast<int>(c) == 7);
}

TEST_CASE("atomic_counter - thread safety: concurrent increments", "[atomic_counter]")
{
    atomic_counter<int> c{0};
    constexpr int N_THREADS = 4;
    constexpr int N_INC = 100;

    std::vector<std::thread> threads;
    threads.reserve(N_THREADS);
    for (int i = 0; i < N_THREADS; ++i) {
        threads.emplace_back([&] {
            for (int j = 0; j < N_INC; ++j)
                ++c;
        });
    }
    for (auto& t : threads)
        t.join();

    REQUIRE(static_cast<int>(c) == N_THREADS * N_INC);
}
