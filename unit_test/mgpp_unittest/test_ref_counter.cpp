#include <catch2/catch.hpp>
#include <megopp/util/simple_counter.h>

using mgpp::util::ref_counter;

TEST_CASE("ref_counter - default construction: count=0, compare_value=0", "[ref_counter]")
{
    ref_counter<> rc;
    REQUIRE(rc.count() == 0);
    REQUIRE(rc.compare_value() == 0);
}

TEST_CASE("ref_counter - single-value constructor sets count, compare_value=0", "[ref_counter]")
{
    ref_counter<> rc{5};
    REQUIRE(rc.count() == 5);
    REQUIRE(rc.compare_value() == 0);
}

TEST_CASE("ref_counter - two-value constructor sets count and compare_value", "[ref_counter]")
{
    ref_counter<> rc{3, 10};
    REQUIRE(rc.count() == 3);
    REQUIRE(rc.compare_value() == 10);
}

TEST_CASE("ref_counter - increment increases count by one", "[ref_counter]")
{
    ref_counter<> rc{0};
    rc.increment();
    REQUIRE(rc.count() == 1);
}

TEST_CASE("ref_counter - decrement decreases count by one", "[ref_counter]")
{
    ref_counter<> rc{5};
    rc.decrement();
    REQUIRE(rc.count() == 4);
}

TEST_CASE("ref_counter - set_count changes count", "[ref_counter]")
{
    ref_counter<> rc;
    rc.set_count(42);
    REQUIRE(rc.count() == 42);
}

TEST_CASE("ref_counter - set_compare_value changes compare_value", "[ref_counter]")
{
    ref_counter<> rc;
    rc.set_compare_value(7);
    REQUIRE(rc.compare_value() == 7);
}

TEST_CASE("ref_counter - is_equal when count == compare_value", "[ref_counter]")
{
    ref_counter<> rc{5, 5};
    REQUIRE(rc.is_equal());
    REQUIRE_FALSE(rc.is_not_equal());
}

TEST_CASE("ref_counter - is_not_equal when count != compare_value", "[ref_counter]")
{
    ref_counter<> rc{4, 5};
    REQUIRE(rc.is_not_equal());
    REQUIRE_FALSE(rc.is_equal());
}

TEST_CASE("ref_counter - is_greater when count > compare_value", "[ref_counter]")
{
    ref_counter<> rc{6, 5};
    REQUIRE(rc.is_greater());
    REQUIRE(rc.is_greater_or_equal());
    REQUIRE_FALSE(rc.is_less());
    REQUIRE_FALSE(rc.is_less_or_equal());
}

TEST_CASE("ref_counter - is_less when count < compare_value", "[ref_counter]")
{
    ref_counter<> rc{4, 5};
    REQUIRE(rc.is_less());
    REQUIRE(rc.is_less_or_equal());
    REQUIRE_FALSE(rc.is_greater());
    REQUIRE_FALSE(rc.is_greater_or_equal());
}

TEST_CASE("ref_counter - is_greater_or_equal when count == compare_value", "[ref_counter]")
{
    ref_counter<> rc{5, 5};
    REQUIRE(rc.is_greater_or_equal());
    REQUIRE(rc.is_less_or_equal());
}

TEST_CASE("ref_counter - callback fires on increment reaching compare_value from below", "[ref_counter]")
{
    bool fired = false;
    ref_counter<> rc{4, 5};
    rc.set_callback([&](const ref_counter<>& r) {
        fired = true;
        REQUIRE(r.count() == 5);
    });
    rc.increment(); // count: 4 -> 5
    REQUIRE(fired);
}

TEST_CASE("ref_counter - callback fires on decrement reaching compare_value from above", "[ref_counter]")
{
    bool fired = false;
    ref_counter<> rc{6, 5};
    rc.set_callback([&](const ref_counter<>& r) {
        fired = true;
        REQUIRE(r.count() == 5);
    });
    rc.decrement(); // count: 6 -> 5
    REQUIRE(fired);
}

TEST_CASE("ref_counter - callback NOT fired when incrementing away from compare_value", "[ref_counter]")
{
    bool fired = false;
    ref_counter<> rc{5, 5}; // already at compare_value
    rc.set_callback([&](const ref_counter<>&) { fired = true; });
    rc.increment(); // count: 5 -> 6, NOT reaching from below
    REQUIRE_FALSE(fired);
}

TEST_CASE("ref_counter - callback NOT fired when decrementing away from compare_value", "[ref_counter]")
{
    bool fired = false;
    ref_counter<> rc{5, 5}; // already at compare_value
    rc.set_callback([&](const ref_counter<>&) { fired = true; });
    rc.decrement(); // count: 5 -> 4, NOT reaching from above
    REQUIRE_FALSE(fired);
}

TEST_CASE("ref_counter - callback fires only once per crossing", "[ref_counter]")
{
    int fire_count = 0;
    ref_counter<> rc{3, 5};
    rc.set_callback([&](const ref_counter<>&) { ++fire_count; });
    rc.increment(); // 3->4: not reached
    rc.increment(); // 4->5: fires
    rc.increment(); // 5->6: not reached (starting above)
    rc.decrement(); // 6->5: fires (reaching from above)
    REQUIRE(fire_count == 2);
}
