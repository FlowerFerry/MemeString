#include <catch2/catch.hpp>
#include <megopp/util/flag_enum.h>

enum class TestFlags : uint32_t {
    None = 0,
    A = 1 << 0,
    B = 1 << 1,
    C = 1 << 2,
    D = 1 << 3,
    All = A | B | C | D
};

namespace mgpp {
namespace util {
template <> struct is_flag_enum<TestFlags> : std::true_type {};
}
}

// ============================================================
// 1. Operators
// ============================================================

TEST_CASE("mgpp::util::flag_enum operators", "[flag_enum]") {
    TestFlags a = TestFlags::A;
    TestFlags b = TestFlags::B;
    
    SECTION("Bitwise OR") {
        REQUIRE((a | b) == static_cast<TestFlags>(3));
    }
    
    SECTION("Bitwise AND") {
        REQUIRE((a & b) == TestFlags::None);
        REQUIRE(((a | b) & a) == TestFlags::A);
    }
    
    SECTION("Bitwise XOR") {
        REQUIRE((a ^ a) == TestFlags::None);
        REQUIRE((a ^ b) == (a | b));
    }
    
    SECTION("Bitwise NOT") {
        REQUIRE(~TestFlags::None == static_cast<TestFlags>(~0u));
        REQUIRE(~a == static_cast<TestFlags>(~1u));
    }
    
    SECTION("Assignment operators") {
        TestFlags val = TestFlags::None;
        
        val |= TestFlags::A;
        REQUIRE(val == TestFlags::A);
        
        val |= TestFlags::B;
        REQUIRE(val == (TestFlags::A | TestFlags::B));
        
        val &= TestFlags::A;
        REQUIRE(val == TestFlags::A);
        
        val ^= TestFlags::B;
        REQUIRE(val == (TestFlags::A | TestFlags::B));
    }
}

// ============================================================
// 2. Queries
// ============================================================

TEST_CASE("mgpp::util::flag_enum queries", "[flag_enum]") {
    TestFlags none = TestFlags::None;
    TestFlags ab = TestFlags::A | TestFlags::B;
    TestFlags bc = TestFlags::B | TestFlags::C;

    SECTION("has_any_flag / has_no_flag base") {
        REQUIRE_FALSE(mgpp::util::has_any_flag(none));
        REQUIRE(mgpp::util::has_no_flag(none));

        REQUIRE(mgpp::util::has_any_flag(ab));
        REQUIRE_FALSE(mgpp::util::has_no_flag(ab));
    }

    SECTION("has_flag") {
        REQUIRE(mgpp::util::has_flag(ab, TestFlags::A));
        REQUIRE(mgpp::util::has_flag(ab, TestFlags::B));
        REQUIRE(mgpp::util::has_flag(ab, ab));
        REQUIRE_FALSE(mgpp::util::has_flag(ab, TestFlags::C));
        REQUIRE_FALSE(mgpp::util::has_flag(ab, TestFlags::All));
    }

    SECTION("has_any_flag with mask") {
        REQUIRE(mgpp::util::has_any_flag(ab, TestFlags::A));
        REQUIRE(mgpp::util::has_any_flag(ab, bc)); // shares B
        REQUIRE_FALSE(mgpp::util::has_any_flag(ab, TestFlags::C));
        REQUIRE_FALSE(mgpp::util::has_any_flag(none, TestFlags::A));
    }

    SECTION("has_no_flag with mask") {
        REQUIRE_FALSE(mgpp::util::has_no_flag(ab, TestFlags::A));
        REQUIRE_FALSE(mgpp::util::has_no_flag(ab, bc)); // shares B
        REQUIRE(mgpp::util::has_no_flag(ab, TestFlags::C));
        REQUIRE(mgpp::util::has_no_flag(none, TestFlags::A));
    }
}

// ============================================================
// 3. Mutators and Converters
// ============================================================

TEST_CASE("mgpp::util::flag_enum mutators", "[flag_enum]") {
    SECTION("to_underlying") {
        REQUIRE(mgpp::util::to_underlying(TestFlags::None) == 0);
        REQUIRE(mgpp::util::to_underlying(TestFlags::C) == 4);
    }

    SECTION("set_flag_if") {
        TestFlags val = TestFlags::None;
        
        mgpp::util::set_flag_if(val, TestFlags::A, true);
        REQUIRE(val == TestFlags::A);
        
        mgpp::util::set_flag_if(val, TestFlags::B, false);
        REQUIRE(val == TestFlags::A);
        
        mgpp::util::set_flag_if(val, TestFlags::A, false);
        REQUIRE(val == TestFlags::None);
    }

    SECTION("toggle_flag") {
        TestFlags val = TestFlags::A;
        
        mgpp::util::toggle_flag(val, TestFlags::A);
        REQUIRE(val == TestFlags::None);
        
        mgpp::util::toggle_flag(val, TestFlags::B);
        REQUIRE(val == TestFlags::B);
    }

    SECTION("add_flags") {
        TestFlags val = TestFlags::A;
        mgpp::util::add_flags(val, TestFlags::B | TestFlags::C);
        REQUIRE(val == (TestFlags::A | TestFlags::B | TestFlags::C));
    }

    SECTION("remove_flags") {
        TestFlags val = TestFlags::A | TestFlags::B | TestFlags::C;
        mgpp::util::remove_flags(val, TestFlags::B);
        REQUIRE(val == (TestFlags::A | TestFlags::C));
    }

    SECTION("keep_only_flags") {
        TestFlags val = TestFlags::A | TestFlags::B | TestFlags::C;
        mgpp::util::keep_only_flags(val, TestFlags::B | TestFlags::D);
        REQUIRE(val == TestFlags::B);
    }
}

// ============================================================
// 4. Bit Analysis
// ============================================================

TEST_CASE("mgpp::util::flag_enum bit analysis", "[flag_enum]") {
    SECTION("extract_lowest_flag") {
        REQUIRE(mgpp::util::extract_lowest_flag(TestFlags::None) == TestFlags::None);
        REQUIRE(mgpp::util::extract_lowest_flag(TestFlags::C) == TestFlags::C);
        REQUIRE(mgpp::util::extract_lowest_flag(TestFlags::A | TestFlags::C) == TestFlags::A);
        
        TestFlags bcd = TestFlags::B | TestFlags::C | TestFlags::D;
        REQUIRE(mgpp::util::extract_lowest_flag(bcd) == TestFlags::B);
    }

    SECTION("has_single_flag") {
        REQUIRE_FALSE(mgpp::util::has_single_flag(TestFlags::None));
        REQUIRE(mgpp::util::has_single_flag(TestFlags::A));
        REQUIRE(mgpp::util::has_single_flag(TestFlags::D));
        REQUIRE_FALSE(mgpp::util::has_single_flag(TestFlags::A | TestFlags::B));
        REQUIRE_FALSE(mgpp::util::has_single_flag(TestFlags::All));
    }

    SECTION("count_flags") {
        REQUIRE(mgpp::util::count_flags(TestFlags::None) == 0);
        REQUIRE(mgpp::util::count_flags(TestFlags::B) == 1);
        REQUIRE(mgpp::util::count_flags(TestFlags::A | TestFlags::C) == 2);
        REQUIRE(mgpp::util::count_flags(TestFlags::All) == 4);
    }
}