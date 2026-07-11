#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/buffer.hpp>
#include <memepp/buffer_view.hpp>
#include <memepp/buffer_span.hpp>

#include <cstring>

// ============================================================================
// variable_buffer: append(const variable_buffer&)
// ============================================================================

TEST_CASE("variable_buffer append variable_buffer", "[variable_buffer][append]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);
    buf.push_back(2);

    memepp::variable_buffer other;
    const uint8_t tail[] = { 3, 4, 5 };
    other.append(tail, 3);

    buf.append(other);
    REQUIRE(buf.size() == 5);
    REQUIRE(buf.at(0) == 1);
    REQUIRE(buf.at(2) == 3);
    REQUIRE(buf.at(4) == 5);
}

// ============================================================================
// variable_buffer: append(const string&)
// ============================================================================

TEST_CASE("variable_buffer append string", "[variable_buffer][append]")
{
    memepp::variable_buffer buf;
    buf.push_back(0);

    memepp::string s = "abc";
    buf.append(s);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 0);
    REQUIRE(buf.at(1) == 'a');
    REQUIRE(buf.at(3) == 'c');
}

// ============================================================================
// variable_buffer: append(const string_view&)
// ============================================================================

TEST_CASE("variable_buffer append string_view", "[variable_buffer][append]")
{
    memepp::variable_buffer buf;
    memepp::string_view sv("xyz");
    buf.append(sv);
    REQUIRE(buf.size() == 3);
    const uint8_t expected[] = { 'x', 'y', 'z' };
    REQUIRE(memcmp(buf.data(), expected, 3) == 0);
}

// ============================================================================
// variable_buffer: append(const buffer&)
// ============================================================================

TEST_CASE("variable_buffer append buffer", "[variable_buffer][append]")
{
    memepp::variable_buffer buf;
    buf.push_back(0xFF);

    const uint8_t raw[] = { 0xAA, 0xBB };
    memepp::buffer b(raw, 2);
    buf.append(b);
    REQUIRE(buf.size() == 3);
    REQUIRE(buf.at(0) == 0xFF);
    REQUIRE(buf.at(1) == 0xAA);
    REQUIRE(buf.at(2) == 0xBB);
}

// ============================================================================
// variable_buffer: append(const buffer_view&)
// ============================================================================

TEST_CASE("variable_buffer append buffer_view", "[variable_buffer][append]")
{
    memepp::variable_buffer buf;
    const uint8_t raw[] = { 0x10, 0x20, 0x30 };
    memepp::buffer_view bv(raw, 3);
    buf.append(bv);
    REQUIRE(buf.size() == 3);
    REQUIRE(buf.at(0) == 0x10);
    REQUIRE(buf.at(2) == 0x30);
}

// ============================================================================
// variable_buffer: append(const buffer_span&)
// ============================================================================

TEST_CASE("variable_buffer append buffer_span", "[variable_buffer][append]")
{
    memepp::variable_buffer buf;
    const uint8_t raw[] = { 0x01, 0x02, 0x03, 0x04 };
    memepp::buffer_span bs(raw, 4);
    buf.append(bs);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(3) == 0x04);
}

// ============================================================================
// variable_buffer: append empty
// ============================================================================

TEST_CASE("variable_buffer append empty buffer_view", "[variable_buffer][append]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);

    memepp::buffer_view empty;
    buf.append(empty);
    REQUIRE(buf.size() == 1);
    REQUIRE(buf.at(0) == 1);
}

// ============================================================================
// variable_buffer: insert(const_iterator, ...) — multi-type overloads
// ============================================================================

TEST_CASE("variable_buffer insert const_iterator variable_buffer", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);
    buf.push_back(4);

    memepp::variable_buffer mid;
    const uint8_t ins[] = { 2, 3 };
    mid.append(ins, 2);

    buf.insert(buf.cbegin() + 1, mid);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 1);
    REQUIRE(buf.at(1) == 2);
    REQUIRE(buf.at(2) == 3);
    REQUIRE(buf.at(3) == 4);
}

TEST_CASE("variable_buffer insert const_iterator string", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x10);
    buf.push_back(0x40);

    memepp::string s("Mid");
    buf.insert(buf.cbegin() + 1, s);
    REQUIRE(buf.size() == 5);
    REQUIRE(buf.at(0) == 0x10);
    REQUIRE(buf.at(1) == 'M');
    REQUIRE(buf.at(4) == 0x40);
}

TEST_CASE("variable_buffer insert const_iterator string_view", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x10);
    buf.push_back(0x20);

    memepp::string_view sv("INS");
    buf.insert(buf.cbegin() + 1, sv);
    REQUIRE(buf.size() == 5);
    REQUIRE(buf.at(0) == 0x10);
    REQUIRE(buf.at(1) == 'I');
    REQUIRE(buf.at(3) == 'S');
    REQUIRE(buf.at(4) == 0x20);
}

TEST_CASE("variable_buffer insert const_iterator buffer", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x10);
    buf.push_back(0x30);

    const uint8_t ins[] = { 0x20 };
    memepp::buffer b(ins, 1);
    buf.insert(buf.cbegin() + 1, b);
    REQUIRE(buf.size() == 3);
    REQUIRE(buf.at(0) == 0x10);
    REQUIRE(buf.at(1) == 0x20);
    REQUIRE(buf.at(2) == 0x30);
}

TEST_CASE("variable_buffer insert const_iterator buffer_view", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    buf.push_back(0xAA);
    buf.push_back(0xCC);

    const uint8_t ins[] = { 0xBB };
    memepp::buffer_view bv(ins, 1);
    buf.insert(buf.cbegin() + 1, bv);
    REQUIRE(buf.size() == 3);
    REQUIRE(buf.at(0) == 0xAA);
    REQUIRE(buf.at(1) == 0xBB);
    REQUIRE(buf.at(2) == 0xCC);
}

TEST_CASE("variable_buffer insert const_iterator buffer_span", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x01);
    buf.push_back(0x04);

    const uint8_t ins[] = { 0x02, 0x03 };
    memepp::buffer_span bs(ins, 2);
    buf.insert(buf.cbegin() + 1, bs);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 0x01);
    REQUIRE(buf.at(1) == 0x02);
    REQUIRE(buf.at(2) == 0x03);
    REQUIRE(buf.at(3) == 0x04);
}

TEST_CASE("variable_buffer insert const_iterator const_pointer+count", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x01);
    buf.push_back(0x04);

    const uint8_t ins[] = { 0x02, 0x03 };
    buf.insert(buf.cbegin() + 1, ins, 2);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 0x01);
    REQUIRE(buf.at(1) == 0x02);
    REQUIRE(buf.at(2) == 0x03);
    REQUIRE(buf.at(3) == 0x04);
}

// ============================================================================
// variable_buffer: insert(size_type, ...) — multi-type overloads
// ============================================================================

TEST_CASE("variable_buffer insert size_type variable_buffer", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);
    buf.push_back(4);

    memepp::variable_buffer mid;
    const uint8_t ins[] = { 2, 3 };
    mid.append(ins, 2);

    buf.insert(static_cast<memepp::variable_buffer::size_type>(1), mid);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 1);
    REQUIRE(buf.at(1) == 2);
    REQUIRE(buf.at(3) == 4);
}

TEST_CASE("variable_buffer insert size_type string", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x10);
    buf.push_back(0x30);

    memepp::string s("XX");
    buf.insert(static_cast<memepp::variable_buffer::size_type>(1), s);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 0x10);
    REQUIRE(buf.at(1) == 'X');
    REQUIRE(buf.at(2) == 'X');
    REQUIRE(buf.at(3) == 0x30);
}

TEST_CASE("variable_buffer insert size_type string_view", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x10);
    buf.push_back(0x20);

    memepp::string_view sv("YY");
    buf.insert(static_cast<memepp::variable_buffer::size_type>(1), sv);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 0x10);
    REQUIRE(buf.at(1) == 'Y');
    REQUIRE(buf.at(2) == 'Y');
    REQUIRE(buf.at(3) == 0x20);
}

TEST_CASE("variable_buffer insert size_type buffer", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    buf.push_back(0xAA);
    buf.push_back(0xDD);

    const uint8_t ins[] = { 0xBB, 0xCC };
    memepp::buffer b(ins, 2);
    buf.insert(static_cast<memepp::variable_buffer::size_type>(1), b);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 0xAA);
    REQUIRE(buf.at(1) == 0xBB);
    REQUIRE(buf.at(2) == 0xCC);
    REQUIRE(buf.at(3) == 0xDD);
}

TEST_CASE("variable_buffer insert size_type buffer_view", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x01);
    buf.push_back(0x04);

    const uint8_t ins[] = { 0x02, 0x03 };
    memepp::buffer_view bv(ins, 2);
    buf.insert(static_cast<memepp::variable_buffer::size_type>(1), bv);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 0x01);
    REQUIRE(buf.at(2) == 0x03);
    REQUIRE(buf.at(3) == 0x04);
}

TEST_CASE("variable_buffer insert size_type buffer_span", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x10);
    buf.push_back(0x40);

    const uint8_t ins[] = { 0x20, 0x30 };
    memepp::buffer_span bs(ins, 2);
    buf.insert(static_cast<memepp::variable_buffer::size_type>(1), bs);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 0x10);
    REQUIRE(buf.at(1) == 0x20);
    REQUIRE(buf.at(2) == 0x30);
    REQUIRE(buf.at(3) == 0x40);
}

TEST_CASE("variable_buffer insert size_type const_pointer+count", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x01);
    buf.push_back(0x04);

    const uint8_t ins[] = { 0x02, 0x03 };
    buf.insert(static_cast<memepp::variable_buffer::size_type>(1), ins, 2);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 0x01);
    REQUIRE(buf.at(1) == 0x02);
    REQUIRE(buf.at(3) == 0x04);
}

// ============================================================================
// variable_buffer: insert at end via size_type
// ============================================================================

TEST_CASE("variable_buffer insert size_type at end", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);
    buf.push_back(2);

    const uint8_t tail[] = { 3, 4 };
    buf.insert(buf.size(), tail, 2);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(2) == 3);
    REQUIRE(buf.at(3) == 4);
}
