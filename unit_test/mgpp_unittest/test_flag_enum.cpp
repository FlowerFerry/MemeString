#include <catch2/catch.hpp>
#include <megopp/util/flag_enum.h>

// ============================================================
// 1. Global-scope enum (backward compatible via global using declarations)
// ============================================================

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
// 2. Namespace-scoped enum using MGPP_FLAG_ENUM (at global scope) + MGPP_DECLARE_FLAG_ENUM_OPS (in namespace)
// ============================================================

namespace test_ns {

enum class NsFlags : uint32_t {
    None = 0,
    X = 1 << 0,
    Y = 1 << 1,
    Z = 1 << 2,
    All = X | Y | Z
};

MGPP_DECLARE_FLAG_ENUM_OPS()

} // namespace test_ns

MGPP_FLAG_ENUM(test_ns::NsFlags)

// ============================================================
// 3. Namespace-scoped enum using separate macro + trait
// ============================================================

namespace test_ns2 {

enum class Ns2Flags : uint8_t {
    None = 0,
    P = 1 << 0,
    Q = 1 << 1
};

} // namespace test_ns2

namespace mgpp {
namespace util {
template <> struct is_flag_enum<test_ns2::Ns2Flags> : std::true_type {};
}
}

namespace test_ns2 {
MGPP_DECLARE_FLAG_ENUM_OPS()
}

// ============================================================
// Operator tests (global scope)
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
// Operator tests (namespace scope via MGPP_FLAG_ENUM)
// ============================================================

TEST_CASE("mgpp::util::flag_enum operators in namespace (MGPP_FLAG_ENUM)", "[flag_enum]") {
    test_ns::NsFlags x = test_ns::NsFlags::X;
    test_ns::NsFlags y = test_ns::NsFlags::Y;

    SECTION("Bitwise OR") {
        REQUIRE((x | y) == static_cast<test_ns::NsFlags>(3));
    }

    SECTION("Bitwise AND") {
        REQUIRE((x & y) == test_ns::NsFlags::None);
        REQUIRE(((x | y) & x) == test_ns::NsFlags::X);
    }

    SECTION("Assignment operators") {
        test_ns::NsFlags val = test_ns::NsFlags::None;
        val |= test_ns::NsFlags::X;
        REQUIRE(val == test_ns::NsFlags::X);
        val |= test_ns::NsFlags::Y;
        REQUIRE(val == test_ns::NsFlags::All);
    }

    SECTION("Bitwise NOT") {
        REQUIRE(~test_ns::NsFlags::None == static_cast<test_ns::NsFlags>(~0u));
    }
}

// ============================================================
// Operator tests (namespace scope via MGPP_DECLARE_FLAG_ENUM_OPS)
// ============================================================

TEST_CASE("mgpp::util::flag_enum operators in namespace (MGPP_DECLARE_FLAG_ENUM_OPS)", "[flag_enum]") {
    test_ns2::Ns2Flags p = test_ns2::Ns2Flags::P;
    test_ns2::Ns2Flags q = test_ns2::Ns2Flags::Q;

    SECTION("Bitwise OR") {
        REQUIRE((p | q) == static_cast<test_ns2::Ns2Flags>(3));
        REQUIRE(static_cast<int>(p | q) == 3);
    }

    SECTION("Bitwise AND") {
        REQUIRE((p & q) == test_ns2::Ns2Flags::None);
    }

    SECTION("Assignment") {
        test_ns2::Ns2Flags val = test_ns2::Ns2Flags::None;
        val |= test_ns2::Ns2Flags::P;
        val |= test_ns2::Ns2Flags::Q;
        REQUIRE(static_cast<int>(val) == 3);
    }
}

// ============================================================
// Queries
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
// Mutators and Converters
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
// Bit Analysis
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

// ============================================================
// Named bitwise functions
// ============================================================

TEST_CASE("mgpp::util::flag_enum named functions", "[flag_enum]") {
    TestFlags a = TestFlags::A;
    TestFlags b = TestFlags::B;
    TestFlags c = TestFlags::C;

    SECTION("flag_or") {
        REQUIRE(mgpp::util::flag_or(a, b) == (TestFlags::A | TestFlags::B));
        REQUIRE(mgpp::util::flag_or(a, TestFlags::None) == a);
    }

    SECTION("flag_and") {
        REQUIRE(mgpp::util::flag_and(a, b) == TestFlags::None);
        REQUIRE(mgpp::util::flag_and(TestFlags::A | TestFlags::B, a) == a);
    }

    SECTION("flag_xor") {
        REQUIRE(mgpp::util::flag_xor(a, a) == TestFlags::None);
        REQUIRE(mgpp::util::flag_xor(a, b) == (TestFlags::A | TestFlags::B));
    }

    SECTION("flag_not") {
        REQUIRE(mgpp::util::flag_not(TestFlags::None) == ~TestFlags::None);
        REQUIRE(mgpp::util::flag_not(a) == ~a);
    }

    SECTION("flag_or_eq") {
        TestFlags val = TestFlags::None;
        mgpp::util::flag_or_eq(val, a);
        REQUIRE(val == a);
        mgpp::util::flag_or_eq(val, b);
        REQUIRE(val == (TestFlags::A | TestFlags::B));
    }

    SECTION("flag_and_eq") {
        TestFlags val = TestFlags::A | TestFlags::B;
        mgpp::util::flag_and_eq(val, a);
        REQUIRE(val == a);
        mgpp::util::flag_and_eq(val, TestFlags::C);
        REQUIRE(val == TestFlags::None);
    }

    SECTION("flag_xor_eq") {
        TestFlags val = TestFlags::A;
        mgpp::util::flag_xor_eq(val, b);
        REQUIRE(val == (TestFlags::A | TestFlags::B));
        mgpp::util::flag_xor_eq(val, a);
        REQUIRE(val == TestFlags::B);
    }
}

// ============================================================
// Named functions on namespace-scoped enum (no operator visibility needed)
// ============================================================

TEST_CASE("mgpp::util::flag_enum named functions in namespace", "[flag_enum]") {
    SECTION("flag_or without operators") {
        auto x = mgpp::util::flag_or(test_ns::NsFlags::X, test_ns::NsFlags::Y);
        REQUIRE(x == static_cast<test_ns::NsFlags>(3));
    }

    SECTION("flag_and_eq without operators") {
        test_ns::NsFlags val = test_ns::NsFlags::All;
        mgpp::util::flag_and_eq(val, test_ns::NsFlags::X);
        REQUIRE(val == test_ns::NsFlags::X);
    }

    SECTION("flag_not without operators") {
        auto x = mgpp::util::flag_not(test_ns::NsFlags::None);
        REQUIRE(x == static_cast<test_ns::NsFlags>(~0u));
    }
}
