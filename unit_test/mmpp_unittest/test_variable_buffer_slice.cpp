#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>

// ---------------------------------------------------------------------------
// variable_buffer::slice -- extract a contiguous range as a new buffer
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer slice normal range", "[variable_buffer]")
{
    memepp::variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::const_pointer>("hello world"), 11);

    auto s = buf.slice(0, 5);

    REQUIRE(s.size() == 5);
    REQUIRE(std::memcmp(s.data(), "hello", 5) == 0);
}

TEST_CASE("memepp::variable_buffer slice count clamped", "[variable_buffer]")
{
    memepp::variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::const_pointer>("hello world"), 11);

    auto s = buf.slice(6, 100);

    REQUIRE(s.size() == 5);
    REQUIRE(std::memcmp(s.data(), "world", 5) == 0);
}

TEST_CASE("memepp::variable_buffer slice pos at size returns empty", "[variable_buffer]")
{
    memepp::variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::const_pointer>("hello"), 5);

    auto s = buf.slice(5, 3);

    REQUIRE(s.empty());
}

TEST_CASE("memepp::variable_buffer slice pos out of range returns empty", "[variable_buffer]")
{
    memepp::variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::const_pointer>("hello"), 5);

    auto s = buf.slice(10, 3);

    REQUIRE(s.empty());
}

TEST_CASE("memepp::variable_buffer slice count zero returns empty", "[variable_buffer]")
{
    memepp::variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::const_pointer>("hello"), 5);

    auto s = buf.slice(2, 0);

    REQUIRE(s.empty());
}

TEST_CASE("memepp::variable_buffer slice leaves source unchanged", "[variable_buffer]")
{
    memepp::variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::const_pointer>("hello"), 5);

    auto s = buf.slice(0, 3);

    REQUIRE(s.size() == 3);
    REQUIRE(buf.size() == 5);
    REQUIRE(std::memcmp(buf.data(), "hello", 5) == 0);
}
