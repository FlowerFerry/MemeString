#include <catch2/catch.hpp>

#include <memepp/variant.hpp>
#include <memepp/string.hpp>
#include <memepp/buffer.hpp>
#include <memepp/variable_buffer.hpp>
#include <memepp/rune.hpp>

// ---------------------------------------------------------------------------
// default constructor — null variant
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant default constructor is null", "[variant]")
{
    memepp::variant v;
    REQUIRE(v.is_null());
    REQUIRE(!v.is_type(memepp::meta::typid::byte));
}

// ---------------------------------------------------------------------------
// mmbyte_t
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant from mmbyte_t", "[variant]")
{
    memepp::variant v(static_cast<mmbyte_t>(0xAB));
    REQUIRE(!v.is_null());
    REQUIRE(v.is_type(memepp::meta::typid::byte));

    mmbyte_t out = 0;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == static_cast<mmbyte_t>(0xAB));
    REQUIRE(v.get_or<mmbyte_t>() == static_cast<mmbyte_t>(0xAB));
}

// ---------------------------------------------------------------------------
// char
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant from char", "[variant]")
{
    memepp::variant v('Z');
    REQUIRE(v.is_type(memepp::meta::typid::char_t));

    char out = 0;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == 'Z');
    REQUIRE(v.get_or<char>() == 'Z');
}

// ---------------------------------------------------------------------------
// int64_t
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant from int64_t", "[variant]")
{
    memepp::variant v(static_cast<int64_t>(-9876543210LL));
    REQUIRE(v.is_type(memepp::meta::typid::int64));

    int64_t out = 0;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == -9876543210LL);
    REQUIRE(v.get_or<int64_t>() == -9876543210LL);
}

// ---------------------------------------------------------------------------
// uint64_t
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant from uint64_t", "[variant]")
{
    memepp::variant v(static_cast<uint64_t>(12345678901234ULL));
    REQUIRE(v.is_type(memepp::meta::typid::uint64));

    uint64_t out = 0;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == 12345678901234ULL);
}

// ---------------------------------------------------------------------------
// double
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant from double", "[variant]")
{
    memepp::variant v(3.14159);
    REQUIRE(v.is_type(memepp::meta::typid::double_t));

    double out = 0.0;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == Approx(3.14159));
    REQUIRE(v.get_or<double>() == Approx(3.14159));
}

// ---------------------------------------------------------------------------
// string
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant from string", "[variant]")
{
    memepp::string s("hello variant");
    memepp::variant v(s);
    REQUIRE(v.is_type(memepp::meta::typid::string));

    memepp::string out;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == "hello variant");
    REQUIRE(v.get_or<memepp::string>() == "hello variant");
}

// ---------------------------------------------------------------------------
// rune
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant from rune", "[variant]")
{
    memepp::rune r('A');
    memepp::variant v(r);
    REQUIRE(v.is_type(memepp::meta::typid::rune));

    memepp::rune out;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == 'A');
}

// ---------------------------------------------------------------------------
// copy & move
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant copy constructor", "[variant]")
{
    memepp::variant src(static_cast<int64_t>(42LL));
    memepp::variant dst(src);

    REQUIRE(dst.is_type(memepp::meta::typid::int64));
    int64_t out = 0;
    REQUIRE(dst.try_get(out) == 0);
    REQUIRE(out == 42LL);
}

TEST_CASE("memepp::variant move constructor", "[variant]")
{
    memepp::variant src(static_cast<int64_t>(99LL));
    memepp::variant dst(std::move(src));

    REQUIRE(dst.is_type(memepp::meta::typid::int64));
    int64_t out = 0;
    REQUIRE(dst.try_get(out) == 0);
    REQUIRE(out == 99LL);
}

// ---------------------------------------------------------------------------
// copy & move assignment
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant copy assignment", "[variant]")
{
    memepp::variant src(static_cast<double>(2.71828));
    memepp::variant dst;
    dst = src;

    REQUIRE(dst.is_type(memepp::meta::typid::double_t));
    double out = 0.0;
    REQUIRE(dst.try_get(out) == 0);
    REQUIRE(out == Approx(2.71828));
}

TEST_CASE("memepp::variant move assignment", "[variant]")
{
    memepp::variant src(static_cast<int64_t>(7LL));
    memepp::variant dst;
    dst = std::move(src);

    REQUIRE(dst.is_type(memepp::meta::typid::int64));
    int64_t out = 0;
    REQUIRE(dst.try_get(out) == 0);
    REQUIRE(out == 7LL);
}

// ---------------------------------------------------------------------------
// type operator= overloads
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant operator= primitive types", "[variant]")
{
    memepp::variant v;

    v = static_cast<mmbyte_t>(0x01);
    REQUIRE(v.is_type(memepp::meta::typid::byte));

    v = 'X';
    REQUIRE(v.is_type(memepp::meta::typid::char_t));

    v = static_cast<int64_t>(1000LL);
    REQUIRE(v.is_type(memepp::meta::typid::int64));

    v = static_cast<uint64_t>(2000ULL);
    REQUIRE(v.is_type(memepp::meta::typid::uint64));

    v = 1.5;
    REQUIRE(v.is_type(memepp::meta::typid::double_t));
}

// ---------------------------------------------------------------------------
// reset
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant reset", "[variant]")
{
    memepp::variant v(static_cast<int64_t>(123LL));
    REQUIRE(!v.is_null());
    v.reset();
    REQUIRE(v.is_null());
}

// ---------------------------------------------------------------------------
// swap
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant swap", "[variant]")
{
    memepp::variant a(static_cast<int64_t>(1LL));
    memepp::variant b(static_cast<double>(9.9));

    a.swap(b);

    REQUIRE(a.is_type(memepp::meta::typid::double_t));
    REQUIRE(b.is_type(memepp::meta::typid::int64));

    double da = 0.0;
    REQUIRE(a.try_get(da) == 0);
    REQUIRE(da == Approx(9.9));

    int64_t db = 0;
    REQUIRE(b.try_get(db) == 0);
    REQUIRE(db == 1LL);
}

// ---------------------------------------------------------------------------
// try_convert: int64 -> double, double -> bool
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant try_convert int64 to double", "[variant]")
{
    memepp::variant v(static_cast<int64_t>(42LL));
    double out = 0.0;
    REQUIRE(v.try_convert(out) == 0);
    REQUIRE(out == Approx(42.0));
    REQUIRE(v.convert_or<double>() == Approx(42.0));
}

TEST_CASE("memepp::variant try_convert double to bool", "[variant]")
{
    memepp::variant v_true(1.0);
    bool b = false;
    REQUIRE(v_true.try_convert(b) == 0);
    REQUIRE(b == true);

    memepp::variant v_false(0.0);
    REQUIRE(v_false.try_convert(b) == 0);
    REQUIRE(b == false);
}

TEST_CASE("memepp::variant convert_or returns default on wrong type get", "[variant]")
{
    memepp::variant v('A');

    // try_get for wrong type should fail
    int64_t out = 0;
    REQUIRE(v.try_get(out) != 0);
    REQUIRE(v.get_or<int64_t>(static_cast<int64_t>(-1)) == static_cast<int64_t>(-1));
}

// ---------------------------------------------------------------------------
// set<T> template
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant set<T>", "[variant]")
{
    memepp::variant v;

    REQUIRE(v.set(static_cast<mmbyte_t>(0xFE)) == 0);
    REQUIRE(v.is_type(memepp::meta::typid::byte));

    REQUIRE(v.set(static_cast<int64_t>(777LL)) == 0);
    REQUIRE(v.is_type(memepp::meta::typid::int64));
    int64_t out = 0;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == 777LL);
}
