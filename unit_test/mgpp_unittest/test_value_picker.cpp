#include <catch2/catch.hpp>
#include <megopp/util/value_picker.h>
#include <mego/err/ec.h>

using mgpp::util::small_range_value_picker;
using mgpp::util::large_range_value_picker;
using mgpp::util::value_picker;

// ============================================================
// small_range_value_picker
// ============================================================

TEST_CASE("small_range_value_picker - pick returns value in range", "[value_picker]")
{
    small_range_value_picker<int> picker(1, 1, 5, 0);
    mgec_t ec = MGEC__ERR;
    int v = picker.pick(&ec);
    REQUIRE(ec == MGEC__OK);
    REQUIRE(v >= 1);
    REQUIRE(v <= 5);
}

TEST_CASE("small_range_value_picker - is_used after pick", "[value_picker]")
{
    small_range_value_picker<int> picker(1, 1, 5, 0);
    int v = picker.pick();
    REQUIRE(picker.is_used(v));
}

TEST_CASE("small_range_value_picker - has_available transitions", "[value_picker]")
{
    small_range_value_picker<int> picker(1, 1, 3, 0);
    REQUIRE(picker.has_available());
    picker.pick();
    picker.pick();
    picker.pick();
    REQUIRE_FALSE(picker.has_available());
}

TEST_CASE("small_range_value_picker - exhaustion returns invalid and MGEC__RANGE", "[value_picker]")
{
    small_range_value_picker<int> picker(1, 1, 3, 0);
    picker.pick();
    picker.pick();
    picker.pick();
    mgec_t ec = MGEC__OK;
    int v = picker.pick(&ec);
    REQUIRE(v == 0);
    REQUIRE(ec == MGEC__RANGE);
}

TEST_CASE("small_range_value_picker - release and re-pick", "[value_picker]")
{
    small_range_value_picker<int> picker(1, 1, 5, 0);
    int v1 = picker.pick();
    REQUIRE(picker.is_used(v1));
    picker.release(v1);
    REQUIRE_FALSE(picker.is_used(v1));
    int v2 = picker.pick();
    REQUIRE(v2 >= 1);
    REQUIRE(v2 <= 5);
}

TEST_CASE("small_range_value_picker - pick avoids invalid value", "[value_picker]")
{
    // invalid = 2, range [1,4]; all valid values (1,3,4) should never be 2
    small_range_value_picker<int> picker(1, 1, 4, 2);
    for (int i = 0; i < 3; ++i) {
        int v = picker.pick();
        REQUIRE(v != 2);
        REQUIRE(v >= 1);
        REQUIRE(v <= 4);
    }
}

TEST_CASE("small_range_value_picker - release out-of-range is safe", "[value_picker]")
{
    small_range_value_picker<int> picker(1, 1, 5, 0);
    REQUIRE_NOTHROW(picker.release(999));
    REQUIRE_NOTHROW(picker.release(0)); // 0 is invalid, should be no-op
}

// ============================================================
// large_range_value_picker
// ============================================================

TEST_CASE("large_range_value_picker - pick returns value in range", "[value_picker]")
{
    large_range_value_picker<int> picker(1, 1, 1000, 0);
    mgec_t ec = MGEC__ERR;
    int v = picker.pick(&ec);
    REQUIRE(ec == MGEC__OK);
    REQUIRE(v >= 1);
    REQUIRE(v <= 1000);
}

TEST_CASE("large_range_value_picker - is_used after pick", "[value_picker]")
{
    large_range_value_picker<int> picker(1, 1, 1000, 0);
    int v = picker.pick();
    REQUIRE(picker.is_used(v));
}

TEST_CASE("large_range_value_picker - has_available transitions", "[value_picker]")
{
    large_range_value_picker<int> picker(1, 1, 3, 0);
    REQUIRE(picker.has_available());
    picker.pick();
    picker.pick();
    picker.pick();
    REQUIRE_FALSE(picker.has_available());
}

TEST_CASE("large_range_value_picker - release and re-pick", "[value_picker]")
{
    large_range_value_picker<int> picker(1, 1, 1000, 0);
    int v1 = picker.pick();
    REQUIRE(picker.is_used(v1));
    picker.release(v1);
    REQUIRE_FALSE(picker.is_used(v1));
    int v2 = picker.pick();
    REQUIRE(v2 >= 1);
    REQUIRE(v2 <= 1000);
}

TEST_CASE("large_range_value_picker - released values are re-used", "[value_picker]")
{
    large_range_value_picker<int> picker(1, 1, 3, 0);
    int v1 = picker.pick(); // picks from [1,3]
    int v2 = picker.pick();
    int v3 = picker.pick();
    // All 3 used; release v1 and pick again
    picker.release(v1);
    mgec_t ec = MGEC__ERR;
    int v4 = picker.pick(&ec);
    REQUIRE(ec == MGEC__OK);
    (void)v2; (void)v3; (void)v4;
}

// ============================================================
// value_picker wrapper
// ============================================================

TEST_CASE("value_picker - pick returns value in range and marks it used", "[value_picker]")
{
    value_picker<int, 1, 10> vp{1, 0};
    mgec_t ec = MGEC__ERR;
    int v = vp.pick(&ec);
    REQUIRE(ec == MGEC__OK);
    REQUIRE(v >= 1);
    REQUIRE(v <= 10);
    REQUIRE(vp.is_used(v));
}

TEST_CASE("value_picker - has_available and exhaustion", "[value_picker]")
{
    value_picker<int, 1, 3> vp{1, 0};
    REQUIRE(vp.has_available());
    vp.pick();
    vp.pick();
    vp.pick();
    REQUIRE_FALSE(vp.has_available());
    mgec_t ec = MGEC__OK;
    int v = vp.pick(&ec);
    REQUIRE(v == 0);
    REQUIRE(ec == MGEC__RANGE);
}

TEST_CASE("value_picker - release makes value available again", "[value_picker]")
{
    value_picker<int, 1, 10> vp{1, 0};
    int v = vp.pick();
    REQUIRE(vp.is_used(v));
    vp.release(v);
    REQUIRE_FALSE(vp.is_used(v));
    REQUIRE(vp.has_available());
}
