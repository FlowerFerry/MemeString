#include <catch2/catch.hpp>

#include <memepp/varts.hpp>
#include <memepp/variant.hpp>
#include <memepp/string.hpp>

// ---------------------------------------------------------------------------
// default constructor
// ---------------------------------------------------------------------------

TEST_CASE("memepp::varts default constructor", "[varts]")
{
    memepp::varts vts;
    // ts_ud_value_=0 means ts_=0 (Unix epoch), which is valid, not MMVTS__TS_INVALID
    REQUIRE(vts.is_ts_valid());
    REQUIRE(vts.timestamp() == 0);
    REQUIRE(vts.userdata() == 0);
    REQUIRE(vts.var().is_null());
}

// ---------------------------------------------------------------------------
// construct from variant only
// ---------------------------------------------------------------------------

TEST_CASE("memepp::varts construct from variant (copy)", "[varts]")
{
    memepp::variant var(static_cast<int64_t>(42LL));
    memepp::varts vts(var);

    // variant-only ctor sets ts_ud_value_=0, so ts_=0 (epoch) which is valid
    REQUIRE(vts.is_ts_valid());
    REQUIRE(vts.timestamp() == 0);
    REQUIRE(vts.userdata() == 0);
    REQUIRE(vts.var().is_type(memepp::meta::typid::int64));

    int64_t out = 0;
    REQUIRE(vts.var().try_get(out) == 0);
    REQUIRE(out == 42LL);
}

TEST_CASE("memepp::varts construct from variant (move)", "[varts]")
{
    memepp::variant var(static_cast<double>(3.14));
    memepp::varts vts(std::move(var));

    // variant-only ctor sets ts_ud_value_=0, so ts_=0 (epoch) which is valid
    REQUIRE(vts.is_ts_valid());
    REQUIRE(vts.timestamp() == 0);
    REQUIRE(vts.var().is_type(memepp::meta::typid::double_t));

    double out = 0.0;
    REQUIRE(vts.var().try_get(out) == 0);
    REQUIRE(out == Approx(3.14));
}

// ---------------------------------------------------------------------------
// construct from variant + timestamp
// ---------------------------------------------------------------------------

TEST_CASE("memepp::varts construct from variant and timestamp", "[varts]")
{
    memepp::variant var(static_cast<int64_t>(7LL));
    mgu_timestamp_t ts = mgu_timestamp_get();

    memepp::varts vts(var, ts);

    REQUIRE(vts.is_ts_valid());
    REQUIRE(vts.timestamp() == ts);
    REQUIRE(vts.userdata() == 0);

    int64_t out = 0;
    REQUIRE(vts.var().try_get(out) == 0);
    REQUIRE(out == 7LL);
}

TEST_CASE("memepp::varts construct from variant and invalid timestamp (-1)", "[varts]")
{
    memepp::variant var(static_cast<int64_t>(0LL));
    memepp::varts vts(var, static_cast<mgu_timestamp_t>(-1));

    REQUIRE(!vts.is_ts_valid());
    REQUIRE(vts.timestamp() == -1);
}

// ---------------------------------------------------------------------------
// construct from variant + timestamp + userdata
// ---------------------------------------------------------------------------

TEST_CASE("memepp::varts construct from variant, timestamp, and userdata", "[varts]")
{
    memepp::variant var(memepp::string("hello"));
    mgu_timestamp_t ts = mgu_timestamp_get();

    memepp::varts vts(var, ts, static_cast<int8_t>(5));

    REQUIRE(vts.is_ts_valid());
    REQUIRE(vts.timestamp() == ts);
    REQUIRE(vts.userdata() == 5);

    memepp::string out;
    REQUIRE(vts.var().try_get(out) == 0);
    REQUIRE(out == "hello");
}

TEST_CASE("memepp::varts construct from variant (move), timestamp, and userdata", "[varts]")
{
    memepp::variant var(static_cast<uint64_t>(999ULL));
    mgu_timestamp_t ts = mgu_timestamp_get();

    memepp::varts vts(std::move(var), ts, static_cast<int8_t>(-1));

    REQUIRE(vts.is_ts_valid());
    REQUIRE(vts.userdata() == static_cast<int8_t>(-1));

    uint64_t out = 0;
    REQUIRE(vts.var().try_get(out) == 0);
    REQUIRE(out == 999ULL);
}

// ---------------------------------------------------------------------------
// construct from timestamp only
// ---------------------------------------------------------------------------

TEST_CASE("memepp::varts construct from timestamp only", "[varts]")
{
    mgu_timestamp_t ts = mgu_timestamp_get();
    memepp::varts vts(ts);

    REQUIRE(vts.is_ts_valid());
    REQUIRE(vts.timestamp() == ts);
    REQUIRE(vts.userdata() == 0);
    REQUIRE(vts.var().is_null());
}

TEST_CASE("memepp::varts construct from invalid timestamp only", "[varts]")
{
    memepp::varts vts(static_cast<mgu_timestamp_t>(-1));

    REQUIRE(!vts.is_ts_valid());
    REQUIRE(vts.timestamp() == -1);
    REQUIRE(vts.var().is_null());
}

TEST_CASE("memepp::varts construct from timestamp and userdata", "[varts]")
{
    mgu_timestamp_t ts = mgu_timestamp_get();
    memepp::varts vts(ts, static_cast<int8_t>(127));

    REQUIRE(vts.is_ts_valid());
    REQUIRE(vts.timestamp() == ts);
    REQUIRE(vts.userdata() == 127);
    REQUIRE(vts.var().is_null());
}

// ---------------------------------------------------------------------------
// copy and move constructors
// ---------------------------------------------------------------------------

TEST_CASE("memepp::varts copy constructor", "[varts]")
{
    mgu_timestamp_t ts = mgu_timestamp_get();
    memepp::variant var(static_cast<int64_t>(55LL));
    memepp::varts src(var, ts, static_cast<int8_t>(3));

    memepp::varts dst(src);

    REQUIRE(dst.is_ts_valid());
    REQUIRE(dst.timestamp() == src.timestamp());
    REQUIRE(dst.userdata() == 3);

    int64_t out = 0;
    REQUIRE(dst.var().try_get(out) == 0);
    REQUIRE(out == 55LL);
}

TEST_CASE("memepp::varts move constructor", "[varts]")
{
    mgu_timestamp_t ts = mgu_timestamp_get();
    memepp::variant var(static_cast<int64_t>(88LL));
    memepp::varts src(var, ts, static_cast<int8_t>(9));

    memepp::varts dst(std::move(src));

    REQUIRE(dst.is_ts_valid());
    REQUIRE(dst.timestamp() == ts);
    REQUIRE(dst.userdata() == 9);

    int64_t out = 0;
    REQUIRE(dst.var().try_get(out) == 0);
    REQUIRE(out == 88LL);
}

// ---------------------------------------------------------------------------
// copy and move assignment
// ---------------------------------------------------------------------------

TEST_CASE("memepp::varts copy assignment", "[varts]")
{
    mgu_timestamp_t ts = mgu_timestamp_get();
    memepp::variant var(static_cast<double>(1.5));
    memepp::varts src(var, ts, static_cast<int8_t>(2));

    memepp::varts dst;
    dst = src;

    REQUIRE(dst.is_ts_valid());
    REQUIRE(dst.timestamp() == src.timestamp());
    REQUIRE(dst.userdata() == 2);

    double out = 0.0;
    REQUIRE(dst.var().try_get(out) == 0);
    REQUIRE(out == Approx(1.5));
}

TEST_CASE("memepp::varts move assignment", "[varts]")
{
    mgu_timestamp_t ts = mgu_timestamp_get();
    memepp::variant var(static_cast<int64_t>(100LL));
    memepp::varts src(var, ts, static_cast<int8_t>(7));

    memepp::varts dst;
    dst = std::move(src);

    REQUIRE(dst.is_ts_valid());
    REQUIRE(dst.timestamp() == ts);
    REQUIRE(dst.userdata() == 7);

    int64_t out = 0;
    REQUIRE(dst.var().try_get(out) == 0);
    REQUIRE(out == 100LL);
}

TEST_CASE("memepp::varts assign from variant (copy)", "[varts]")
{
    memepp::varts vts;
    memepp::variant var(static_cast<int64_t>(11LL));
    vts = var;

    REQUIRE(vts.var().is_type(memepp::meta::typid::int64));
    int64_t out = 0;
    REQUIRE(vts.var().try_get(out) == 0);
    REQUIRE(out == 11LL);
}

TEST_CASE("memepp::varts assign from variant (move)", "[varts]")
{
    memepp::varts vts;
    memepp::variant var(static_cast<uint64_t>(22ULL));
    vts = std::move(var);

    REQUIRE(vts.var().is_type(memepp::meta::typid::uint64));
    uint64_t out = 0;
    REQUIRE(vts.var().try_get(out) == 0);
    REQUIRE(out == 22ULL);
}

// ---------------------------------------------------------------------------
// setters
// ---------------------------------------------------------------------------

TEST_CASE("memepp::varts set_timestamp", "[varts]")
{
    memepp::varts vts;
    // Default has ts_=0 (epoch), which is valid
    REQUIRE(vts.is_ts_valid());
    REQUIRE(vts.timestamp() == 0);

    mgu_timestamp_t ts = mgu_timestamp_get();
    vts.set_timestamp(ts);
    REQUIRE(vts.is_ts_valid());
    REQUIRE(vts.timestamp() == ts);

    vts.set_timestamp(static_cast<mgu_timestamp_t>(-1));
    REQUIRE(!vts.is_ts_valid());
    REQUIRE(vts.timestamp() == -1);
}

TEST_CASE("memepp::varts set_userdata", "[varts]")
{
    memepp::varts vts;
    REQUIRE(vts.userdata() == 0);

    vts.set_userdata(static_cast<int8_t>(42));
    REQUIRE(vts.userdata() == 42);

    vts.set_userdata(static_cast<int8_t>(-128));
    REQUIRE(vts.userdata() == static_cast<int8_t>(-128));
}

TEST_CASE("memepp::varts set_var (copy)", "[varts]")
{
    memepp::varts vts;
    memepp::variant var(static_cast<int64_t>(33LL));

    vts.set_var(var);
    REQUIRE(vts.var().is_type(memepp::meta::typid::int64));

    int64_t out = 0;
    REQUIRE(vts.var().try_get(out) == 0);
    REQUIRE(out == 33LL);
}

TEST_CASE("memepp::varts set_var (move)", "[varts]")
{
    memepp::varts vts;
    memepp::variant var(static_cast<double>(9.99));

    vts.set_var(std::move(var));
    REQUIRE(vts.var().is_type(memepp::meta::typid::double_t));

    double out = 0.0;
    REQUIRE(vts.var().try_get(out) == 0);
    REQUIRE(out == Approx(9.99));
}

// ---------------------------------------------------------------------------
// reset
// ---------------------------------------------------------------------------

TEST_CASE("memepp::varts reset", "[varts]")
{
    mgu_timestamp_t ts = mgu_timestamp_get();
    memepp::variant var(static_cast<int64_t>(1LL));
    memepp::varts vts(var, ts, static_cast<int8_t>(4));

    REQUIRE(vts.is_ts_valid());
    REQUIRE(!vts.var().is_null());
    REQUIRE(vts.userdata() == 4);

    vts.reset();

    REQUIRE(!vts.is_ts_valid());
    REQUIRE(vts.timestamp() == -1);
    REQUIRE(vts.userdata() == 0);
    REQUIRE(vts.var().is_null());
}

// ---------------------------------------------------------------------------
// swap
// ---------------------------------------------------------------------------

TEST_CASE("memepp::varts swap", "[varts]")
{
    mgu_timestamp_t ts_a = mgu_timestamp_get();
    memepp::variant var_a(static_cast<int64_t>(10LL));
    memepp::varts a(var_a, ts_a, static_cast<int8_t>(1));

    memepp::varts b;
    memepp::variant var_b(static_cast<double>(2.0));
    b.set_var(var_b);
    // b was default-constructed: ts_=0 (epoch), valid

    a.swap(b);

    // a now has b's state: ts_=0 (epoch), valid
    REQUIRE(a.is_ts_valid());
    REQUIRE(a.timestamp() == 0);
    REQUIRE(a.var().is_type(memepp::meta::typid::double_t));

    // b now has a's original state
    REQUIRE(b.is_ts_valid());
    REQUIRE(b.timestamp() == ts_a);
    REQUIRE(b.userdata() == 1);
    REQUIRE(b.var().is_type(memepp::meta::typid::int64));

    int64_t out = 0;
    REQUIRE(b.var().try_get(out) == 0);
    REQUIRE(out == 10LL);
}

// ---------------------------------------------------------------------------
// make_now static factories
// ---------------------------------------------------------------------------

TEST_CASE("memepp::varts make_now no args", "[varts]")
{
    auto before = mgu_timestamp_get();
    auto vts = memepp::varts::make_now();
    auto after = mgu_timestamp_get();

    REQUIRE(vts.is_ts_valid());
    REQUIRE(vts.timestamp() >= before);
    REQUIRE(vts.timestamp() <= after);
    REQUIRE(vts.userdata() == 0);
    REQUIRE(vts.var().is_null());
}

TEST_CASE("memepp::varts make_now with userdata", "[varts]")
{
    auto vts = memepp::varts::make_now(static_cast<int8_t>(11));

    REQUIRE(vts.is_ts_valid());
    REQUIRE(vts.userdata() == 11);
    REQUIRE(vts.var().is_null());
}

TEST_CASE("memepp::varts make_now with variant (copy)", "[varts]")
{
    memepp::variant var(static_cast<int64_t>(77LL));
    auto before = mgu_timestamp_get();
    auto vts = memepp::varts::make_now(var);
    auto after = mgu_timestamp_get();

    REQUIRE(vts.is_ts_valid());
    REQUIRE(vts.timestamp() >= before);
    REQUIRE(vts.timestamp() <= after);
    REQUIRE(vts.userdata() == 0);

    int64_t out = 0;
    REQUIRE(vts.var().try_get(out) == 0);
    REQUIRE(out == 77LL);
}

TEST_CASE("memepp::varts make_now with variant (move)", "[varts]")
{
    memepp::variant var(static_cast<double>(1.23));
    auto vts = memepp::varts::make_now(std::move(var));

    REQUIRE(vts.is_ts_valid());
    double out = 0.0;
    REQUIRE(vts.var().try_get(out) == 0);
    REQUIRE(out == Approx(1.23));
}

TEST_CASE("memepp::varts make_now with variant and userdata (copy)", "[varts]")
{
    memepp::variant var(memepp::string("world"));
    auto vts = memepp::varts::make_now(var, static_cast<int8_t>(99));

    REQUIRE(vts.is_ts_valid());
    REQUIRE(vts.userdata() == 99);

    memepp::string out;
    REQUIRE(vts.var().try_get(out) == 0);
    REQUIRE(out == "world");
}

TEST_CASE("memepp::varts make_now with variant and userdata (move)", "[varts]")
{
    memepp::variant var(static_cast<uint64_t>(55ULL));
    auto vts = memepp::varts::make_now(std::move(var), static_cast<int8_t>(-5));

    REQUIRE(vts.is_ts_valid());
    REQUIRE(vts.userdata() == static_cast<int8_t>(-5));

    uint64_t out = 0;
    REQUIRE(vts.var().try_get(out) == 0);
    REQUIRE(out == 55ULL);
}

// ---------------------------------------------------------------------------
// non-const var() mutability
// ---------------------------------------------------------------------------

TEST_CASE("memepp::varts var() non-const allows mutation", "[varts]")
{
    memepp::variant var(static_cast<int64_t>(1LL));
    memepp::varts vts(var);

    vts.var() = static_cast<double>(3.14);
    REQUIRE(vts.var().is_type(memepp::meta::typid::double_t));

    double out = 0.0;
    REQUIRE(vts.var().try_get(out) == 0);
    REQUIRE(out == Approx(3.14));
}

// ---------------------------------------------------------------------------
// size invariant: sizeof(varts) == sizeof(mmvtsstk_t)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::varts size matches mmvtsstk_t", "[varts]")
{
    REQUIRE(sizeof(memepp::varts) == sizeof(mmvtsstk_t));
}
