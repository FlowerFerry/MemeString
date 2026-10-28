#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>

// Error path tests for variable_buffer.
//
// Scope: only paths where the C layer actually returns an error code.
// Many "invalid" inputs are silently tolerated by design (remove out-of-range
// is a no-op, slice out-of-range returns empty, append(nullptr, 5) is a no-op)
// so those are NOT tested as errors.
//
// Two modes are exercised:
//   - exceptions enabled:  REQUIRE_THROWS_AS(..., memepp::errc_except)
//   - exceptions disabled: check memepp::get_errc() != 0

#if !MMOPT__EXCEPTION_DISABLED
#  define EXPECT_ERR(stmt) REQUIRE_THROWS_AS(stmt, memepp::errc_except)
#else
#  define EXPECT_ERR(stmt) do { stmt; REQUIRE(memepp::get_errc() != 0); } while (0)
#endif

// ---------------------------------------------------------------------------
// insert(const_iterator, const_pointer, size_type)
// ---------------------------------------------------------------------------

TEST_CASE("variable_buffer::insert — nullptr with nonzero count fails",
          "[variable_buffer][error]")
{
    memepp::variable_buffer buf;
    buf.push_back(static_cast<uint8_t>(1));

    EXPECT_ERR(buf.insert(buf.cbegin(), static_cast<const uint8_t*>(nullptr), 4));
}

TEST_CASE("variable_buffer::insert — zero count fails",
          "[variable_buffer][error]")
{
    memepp::variable_buffer buf;
    buf.push_back(static_cast<uint8_t>(1));
    const uint8_t data[] = { 0x09, 0x09 };

    EXPECT_ERR(buf.insert(static_cast<memepp::variable_buffer::size_type>(0),
                          data,
                          static_cast<memepp::variable_buffer::size_type>(0)));
}

TEST_CASE("variable_buffer::insert — pos beyond size fails",
          "[variable_buffer][error]")
{
    memepp::variable_buffer buf;
    buf.push_back(static_cast<uint8_t>(1));
    const uint8_t data[] = { 0x09, 0x09 };

    EXPECT_ERR(buf.insert(static_cast<memepp::variable_buffer::size_type>(100),
                          data,
                          static_cast<memepp::variable_buffer::size_type>(2)));
}

TEST_CASE("variable_buffer::insert — pos == size is valid (append semantics)",
          "[variable_buffer][error]")
{
    memepp::variable_buffer buf;
    buf.push_back(static_cast<uint8_t>(1));
    const uint8_t data[] = { 0x09 };

    // pos == size is allowed (append at end), must NOT throw.
    buf.insert(buf.cend(), data, 1);
    REQUIRE(buf.size() == 2);
    REQUIRE(buf[0] == 1);
    REQUIRE(buf[1] == 0x09);
}

// ---------------------------------------------------------------------------
// insert(size_type, const_pointer, size_type)
// ---------------------------------------------------------------------------

TEST_CASE("variable_buffer::insert(size_type) — pos beyond size fails",
          "[variable_buffer][error]")
{
    memepp::variable_buffer buf;
    buf.push_back(static_cast<uint8_t>(1));
    const uint8_t data[] = { 0x09, 0x09 };

    EXPECT_ERR(buf.insert(static_cast<memepp::variable_buffer::size_type>(100),
                          data,
                          static_cast<memepp::variable_buffer::size_type>(2)));
}

// ---------------------------------------------------------------------------
// append(const_pointer, size_type)
// ---------------------------------------------------------------------------

TEST_CASE("variable_buffer::append — nullptr is silently tolerated",
          "[variable_buffer][error]")
{
    memepp::variable_buffer buf;
    buf.push_back(static_cast<uint8_t>(1));

    // Design: append(nullptr, n) is silently converted to append(nullptr, 0)
    // by the C layer — a no-op, not an error.
    buf.append(static_cast<const uint8_t*>(nullptr),
               static_cast<memepp::variable_buffer::size_type>(5));

    REQUIRE(buf.size() == 1);
    REQUIRE(buf[0] == 1);
}

// ---------------------------------------------------------------------------
// remove / slice / append(nullptr, 0) — silently tolerated, no error expected
// ---------------------------------------------------------------------------

TEST_CASE("variable_buffer::remove — pos beyond size is silent no-op",
          "[variable_buffer][error]")
{
    memepp::variable_buffer buf;
    buf.push_back(static_cast<uint8_t>(1));
    buf.push_back(static_cast<uint8_t>(2));

    // Design: remove out-of-range is a no-op, not an error.
    buf.remove(static_cast<memepp::variable_buffer::size_type>(10),
               static_cast<memepp::variable_buffer::size_type>(1));

    REQUIRE(buf.size() == 2);
    REQUIRE(buf[0] == 1);
    REQUIRE(buf[1] == 2);
}

TEST_CASE("variable_buffer::slice — pos beyond size returns empty",
          "[variable_buffer][error]")
{
    memepp::variable_buffer buf;
    buf.push_back(static_cast<uint8_t>(1));

    // Design: slice out-of-range returns empty buffer, not an error.
    auto s = buf.slice(static_cast<memepp::variable_buffer::size_type>(50),
                       static_cast<memepp::variable_buffer::size_type>(2));
    REQUIRE(s.size() == 0);
    REQUIRE(s.empty());
}
