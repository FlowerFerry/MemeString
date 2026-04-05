#include <catch2/catch.hpp>

#include <megopp/help/traverse.h>

#include <map>
#include <mutex>
#include <optional>
#include <string>

using Container = std::map<int, std::string>;
using Lock      = std::unique_lock<std::mutex>;

TEST_CASE("mgpp::help::foreach_with_small_step - empty container returns nullopt", "[traverse]")
{
    std::mutex mtx;
    Lock lk(mtx);
    Container c;
    std::optional<int> out;

    mgpp::help::foreach_with_small_step(
        lk, c, std::optional<int>{}, 10, out,
        [](const Container::value_type&) { return true; });

    REQUIRE(!out.has_value());
}

TEST_CASE("mgpp::help::foreach_with_small_step - count exhausted, returns next element key", "[traverse]")
{
    std::mutex mtx;
    Lock lk(mtx);
    Container c = { {1,"a"}, {2,"b"}, {3,"c"}, {4,"d"}, {5,"e"} };
    std::optional<int> out;

    // Process first 3 (all pred=true); returns the key of the 4th element
    mgpp::help::foreach_with_small_step(
        lk, c, std::optional<int>{}, 3, out,
        [](const Container::value_type&) { return true; });

    REQUIRE(out.has_value());
    REQUIRE(*out == 4);
}

TEST_CASE("mgpp::help::foreach_with_small_step - count covers all elements, returns nullopt", "[traverse]")
{
    std::mutex mtx;
    Lock lk(mtx);
    Container c = { {1,"a"}, {2,"b"}, {3,"c"} };
    std::optional<int> out;

    mgpp::help::foreach_with_small_step(
        lk, c, std::optional<int>{}, 10, out,
        [](const Container::value_type&) { return true; });

    REQUIRE(!out.has_value());
}

TEST_CASE("mgpp::help::foreach_with_small_step - pred returns false, skips to next element", "[traverse]")
{
    std::mutex mtx;
    Lock lk(mtx);
    Container c = { {1,"a"}, {2,"b"}, {3,"c"}, {4,"d"} };
    std::optional<int> out;

    // pred returns false on key==2, so the element at key==3 is stored in out
    mgpp::help::foreach_with_small_step(
        lk, c, std::optional<int>{}, 10, out,
        [](const Container::value_type& e) { return e.first != 2; });

    REQUIRE(out.has_value());
    REQUIRE(*out == 3);
}

TEST_CASE("mgpp::help::foreach_with_small_step - pred returns false on last element, returns nullopt", "[traverse]")
{
    std::mutex mtx;
    Lock lk(mtx);
    Container c = { {1,"a"}, {2,"b"}, {3,"c"} };
    std::optional<int> out;

    mgpp::help::foreach_with_small_step(
        lk, c, std::optional<int>{}, 10, out,
        [](const Container::value_type& e) { return e.first != 3; });

    REQUIRE(!out.has_value());
}

TEST_CASE("mgpp::help::foreach_with_small_step - begin key found, iterates from that point", "[traverse]")
{
    std::mutex mtx;
    Lock lk(mtx);
    Container c = { {1,"a"}, {2,"b"}, {3,"c"}, {4,"d"}, {5,"e"} };
    std::optional<int> out;

    // Start at key=3, count=1 (pred=true): processes {3}, then returns key of {4}
    mgpp::help::foreach_with_small_step(
        lk, c, std::optional<int>{3}, 1, out,
        [](const Container::value_type&) { return true; });

    REQUIRE(out.has_value());
    REQUIRE(*out == 4);
}

TEST_CASE("mgpp::help::foreach_with_small_step - begin key not found, returns nullopt", "[traverse]")
{
    std::mutex mtx;
    Lock lk(mtx);
    Container c = { {1,"a"}, {2,"b"}, {3,"c"} };
    std::optional<int> out = 99;

    mgpp::help::foreach_with_small_step(
        lk, c, std::optional<int>{99}, 5, out,
        [](const Container::value_type&) { return true; });

    REQUIRE(!out.has_value());
}

TEST_CASE("mgpp::help::foreach_with_small_step - lock state preserved when entering locked", "[traverse]")
{
    std::mutex mtx;
    Lock lk(mtx);   // locked
    Container c = { {1,"a"}, {2,"b"} };
    std::optional<int> out;

    REQUIRE(lk.owns_lock());
    mgpp::help::foreach_with_small_step(
        lk, c, std::optional<int>{}, 5, out,
        [](const Container::value_type&) { return true; });
    REQUIRE(lk.owns_lock());
}

TEST_CASE("mgpp::help::foreach_with_small_step - lock state preserved when entering unlocked", "[traverse]")
{
    std::mutex mtx;
    Lock lk(mtx, std::defer_lock);   // NOT locked
    Container c = { {1,"a"}, {2,"b"} };
    std::optional<int> out;

    REQUIRE(!lk.owns_lock());
    mgpp::help::foreach_with_small_step(
        lk, c, std::optional<int>{}, 5, out,
        [](const Container::value_type&) { return true; });
    REQUIRE(!lk.owns_lock());
}
