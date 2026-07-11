#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>
#include <memepp/buffer.hpp>
#include <memepp/string.hpp>
#include <stdexcept>

// ============================================================================
// variable_buffer: move constructor
// ============================================================================

TEST_CASE("variable_buffer move constructor", "[variable_buffer]")
{
    memepp::variable_buffer src;
    src.push_back(10);
    src.push_back(20);
    src.push_back(30);

    memepp::variable_buffer dst(std::move(src));
    REQUIRE(dst.size() == 3);
    REQUIRE(dst.at(0) == 10);
    REQUIRE(dst.at(2) == 30);
}

// ============================================================================
// variable_buffer: fill constructor
// ============================================================================

TEST_CASE("variable_buffer fill constructor", "[variable_buffer]")
{
    memepp::variable_buffer buf(static_cast<memepp::variable_buffer::size_type>(5),
                                 static_cast<uint8_t>(0xAB));
    REQUIRE(buf.size() == 5);
    for (memepp::variable_buffer::size_type i = 0; i < 5; ++i)
        REQUIRE(buf.at(i) == 0xAB);
}

TEST_CASE("variable_buffer fill constructor — zero count", "[variable_buffer]")
{
    memepp::variable_buffer buf(static_cast<memepp::variable_buffer::size_type>(0),
                                 static_cast<uint8_t>(0xFF));
    REQUIRE(buf.empty());
    REQUIRE(buf.size() == 0);
}

// ============================================================================
// variable_buffer: at() out-of-range
// ============================================================================

TEST_CASE("variable_buffer at basic", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(10);
    buf.push_back(20);

    REQUIRE(buf.at(0) == 10);
    REQUIRE(buf.at(1) == 20);
}

TEST_CASE("variable_buffer at mutable", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(10);

    buf.at(0) = 99;
    REQUIRE(buf.at(0) == 99);
}

TEST_CASE("variable_buffer at out-of-range throws", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);

#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(buf.at(1), std::out_of_range);
    REQUIRE_THROWS_AS(buf.at(100), std::out_of_range);
#endif
}

// ============================================================================
// variable_buffer: max_size()
// ============================================================================

TEST_CASE("variable_buffer max_size", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    REQUIRE(buf.max_size() > 0);
}

// ============================================================================
// variable_buffer: iterators (begin/end/cbegin/cend)
// ============================================================================

TEST_CASE("variable_buffer iterators forward", "[variable_buffer]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5 };
    memepp::variable_buffer buf(data, 5);

    memepp::variable_buffer::size_type idx = 0;
    for (auto it = buf.begin(); it != buf.end(); ++it, ++idx)
        REQUIRE(*it == data[idx]);
    REQUIRE(idx == 5);
}

TEST_CASE("variable_buffer const iterators forward", "[variable_buffer]")
{
    const uint8_t data[] = { 10, 20, 30 };
    memepp::variable_buffer buf(data, 3);

    memepp::variable_buffer::size_type idx = 0;
    for (auto it = buf.cbegin(); it != buf.cend(); ++it, ++idx)
        REQUIRE(*it == data[idx]);
    REQUIRE(idx == 3);
}

TEST_CASE("variable_buffer iterators empty", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    REQUIRE(buf.begin() == buf.end());
    REQUIRE(buf.cbegin() == buf.cend());
}

TEST_CASE("variable_buffer iterator mutable", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(10);
    buf.push_back(20);

    *buf.begin() = 99;
    REQUIRE(buf.at(0) == 99);
}

// ============================================================================
// variable_buffer: pop_front()
// ============================================================================

TEST_CASE("variable_buffer pop_front", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);
    buf.push_back(2);
    buf.push_back(3);

    buf.pop_front();
    REQUIRE(buf.size() == 2);
    REQUIRE(buf.at(0) == 2);
    REQUIRE(buf.at(1) == 3);

    buf.pop_front();
    REQUIRE(buf.size() == 1);
    REQUIRE(buf.at(0) == 3);

    buf.pop_front();
    REQUIRE(buf.size() == 0);
}

TEST_CASE("variable_buffer pop_front — empty does not crash", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.pop_front();
    REQUIRE(buf.empty());
}

// ============================================================================
// variable_buffer: remove(size_type, size_type)
// ============================================================================

TEST_CASE("variable_buffer remove from middle", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);
    buf.push_back(2);
    buf.push_back(3);
    buf.push_back(4);
    buf.push_back(5);

    buf.remove(1, 2); // remove 2,3
    REQUIRE(buf.size() == 3);
    REQUIRE(buf.at(0) == 1);
    REQUIRE(buf.at(1) == 4);
    REQUIRE(buf.at(2) == 5);
}

TEST_CASE("variable_buffer remove from start", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);
    buf.push_back(2);
    buf.push_back(3);

    buf.remove(0, 2);
    REQUIRE(buf.size() == 1);
    REQUIRE(buf.at(0) == 3);
}

TEST_CASE("variable_buffer remove from end", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);
    buf.push_back(2);
    buf.push_back(3);

    buf.remove(1, 2);
    REQUIRE(buf.size() == 1);
    REQUIRE(buf.at(0) == 1);
}

TEST_CASE("variable_buffer remove all", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);
    buf.push_back(2);
    buf.push_back(3);

    buf.remove(0, 3);
    REQUIRE(buf.size() == 0);
    REQUIRE(buf.empty());
}

TEST_CASE("variable_buffer remove zero count", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);
    buf.push_back(2);

    buf.remove(0, 0);
    REQUIRE(buf.size() == 2);
}

// ============================================================================
// variable_buffer: release(buffer&) and release(string&)
// ============================================================================

TEST_CASE("variable_buffer release to buffer", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(0xAA);
    buf.push_back(0xBB);
    buf.push_back(0xCC);

    memepp::buffer released;
    mmint_t rc = buf.release(released);
    REQUIRE(rc == 0);
    REQUIRE(released.size() == 3);
    REQUIRE(released.at(0) == 0xAA);
    REQUIRE(released.at(2) == 0xCC);

    // source buffer should be empty after release
    REQUIRE(buf.empty());
}

TEST_CASE("variable_buffer release to string", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 'H', 'i' };
    buf.append(data, 2);

    memepp::string released;
    mmint_t rc = buf.release(released);
    REQUIRE(rc == 0);
    REQUIRE(released == "Hi");
    REQUIRE(released.size() == 2);

    REQUIRE(buf.empty());
}

TEST_CASE("variable_buffer release empty to buffer", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    memepp::buffer released;
    mmint_t rc = buf.release(released);
    REQUIRE(rc == 0);
    REQUIRE(released.empty());
}

// ============================================================================
// variable_buffer: native_handle()
// ============================================================================

TEST_CASE("variable_buffer native_handle accessor", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);

    const auto& handle = buf.native_handle();
    (void)handle;
    REQUIRE(buf.size() == 1);
}
