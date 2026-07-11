#include <catch2/catch.hpp>

#include <memepp/buffer_view.hpp>
#include <memepp/buffer.hpp>
#include <memepp/variable_buffer.hpp>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>

// ============================================================================
// buffer_view: rvalue constructors
// ============================================================================

TEST_CASE("buffer_view construct from buffer&& small", "[buffer_view][ctor]")
{
    const uint8_t raw[] = { 'A', 'B', 'C' };
    memepp::buffer_view bv(memepp::buffer(raw, 3));
    REQUIRE(bv.size() == 3);
    REQUIRE(bv.at(0) == 'A');
    REQUIRE(bv.at(2) == 'C');
}

TEST_CASE("buffer_view construct from buffer&& large", "[buffer_view][ctor]")
{
    const uint8_t raw[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                            31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
    memepp::buffer buf(raw, static_cast<memepp::buffer::size_type>(sizeof(raw)));
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::large);
    memepp::buffer_view bv(std::move(buf));
    REQUIRE(bv.size() == sizeof(raw));
    REQUIRE(bv.at(0) == 1);
}

TEST_CASE("buffer_view construct from string&& small", "[buffer_view][ctor]")
{
    memepp::buffer_view bv(memepp::string("hello", 5));
    REQUIRE(bv.size() == 5);
    REQUIRE(bv.at(0) == 'h');
}

TEST_CASE("buffer_view construct from string_view&&", "[buffer_view][ctor]")
{
    memepp::buffer_view bv(memepp::string_view("world", 5));
    REQUIRE(bv.size() == 5);
    REQUIRE(bv.at(0) == 'w');
}

TEST_CASE("buffer_view construct from buffer_view&&", "[buffer_view][ctor]")
{
    memepp::string s("move");
    memepp::buffer_view src(s);
    memepp::buffer_view dst(std::move(src));
    REQUIRE(dst.size() == 4);
    REQUIRE(dst.at(0) == 'm');
}

TEST_CASE("buffer_view construct from variable_buffer", "[buffer_view][ctor]")
{
    const uint8_t raw[] = { 0x11, 0x22, 0x33 };
    memepp::variable_buffer varBuf(raw, 3);
    memepp::buffer_view bv(varBuf);
    REQUIRE(bv.size() == 3);
    REQUIRE(bv.at(0) == 0x11);
    REQUIRE(bv.at(1) == 0x22);
    REQUIRE(bv.at(2) == 0x33);
}

// ============================================================================
// buffer_view: storage_type()
// ============================================================================

TEST_CASE("buffer_view storage_type — from small buffer", "[buffer_view]")
{
    const uint8_t raw[] = { 1, 2, 3 };
    memepp::buffer buf(raw, 3);
    memepp::buffer_view bv(buf);
    REQUIRE(bv.storage_type() == memepp::buffer_storage_t::large);
}

TEST_CASE("buffer_view storage_type — raw pointer", "[buffer_view]")
{
    const uint8_t raw[] = { 1, 2, 3 };
    memepp::buffer_view bv(raw, 3);
    REQUIRE(bv.storage_type() == memepp::buffer_storage_t::view_unsafe);
}

// ============================================================================
// buffer_view: data() and native_handle()
// ============================================================================

TEST_CASE("buffer_view data() returns correct pointer", "[buffer_view]")
{
    const uint8_t raw[] = { 'X', 'Y', 'Z' };
    memepp::buffer_view bv(raw, 3);
    REQUIRE(bv.data() != nullptr);
    REQUIRE(bv.data()[0] == 'X');
    REQUIRE(bv.data()[2] == 'Z');
}

TEST_CASE("buffer_view native_handle", "[buffer_view]")
{
    memepp::string s("test");
    memepp::buffer_view bv(s);

    const auto& handle = bv.native_handle();
    // basic sanity: the handle holds view data
    (void)handle;
    REQUIRE(bv.size() == 4);
}
