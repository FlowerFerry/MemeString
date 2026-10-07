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

// ---------------------------------------------------------------------------
// variable_buffer::slice — additional edge cases
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer slice — empty buffer", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    auto s  = buf.slice(0, 3);
    auto s2 = buf.slice(2, 1);
    REQUIRE(s.empty());
    REQUIRE(s2.empty());
}

TEST_CASE("memepp::variable_buffer slice — whole buffer", "[variable_buffer]")
{
    memepp::variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::const_pointer>("abcde"), 5);
    auto s = buf.slice(0, 5);
    REQUIRE(s.size() == 5);
    REQUIRE(std::memcmp(s.data(), "abcde", 5) == 0);
}

TEST_CASE("memepp::variable_buffer slice — last single element", "[variable_buffer]")
{
    memepp::variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::const_pointer>("abcde"), 5);
    auto s = buf.slice(4, 1);
    REQUIRE(s.size() == 1);
    REQUIRE(s.data()[0] == 'e');
}

TEST_CASE("memepp::variable_buffer slice — medium storage", "[variable_buffer]")
{
    const uint8_t data[] = {
        'T','h','e',' ','q','u','i','c','k',' ','b','r','o','w','n',
        ' ','f','o','x',' ','j','u','m','p','s',' ','o','v','e','r',
        ' ','t','h','e',' ','l','a','z','y',' ','d','o','g','.'
    };
    memepp::variable_buffer buf(data, static_cast<memepp::variable_buffer::size_type>(sizeof(data)));
    auto s = buf.slice(4, 5);  // "quick"
    REQUIRE(s.size() == 5);
    REQUIRE(std::memcmp(s.data(), "quick", 5) == 0);
    REQUIRE(buf.size() == static_cast<memepp::variable_buffer::size_type>(sizeof(data)));
}
