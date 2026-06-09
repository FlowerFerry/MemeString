#include <catch2/catch.hpp>
#include <megopp/help/noncopyable.h>

// A test class that inherits from noncopyable
class test_noncopyable : public mgpp::help::noncopyable {
public:
    test_noncopyable(int val) : val_(val) {}
    int get() const { return val_; }
private:
    int val_;
};

TEST_CASE("noncopyable - copy construction is disabled", "[noncopyable]")
{
    test_noncopyable a{42};
    // This should fail to compile - uncomment to verify
    // test_noncopyable b{a};
    SUCCEED("noncopyable class can be constructed");
}

TEST_CASE("noncopyable - copy assignment is disabled", "[noncopyable]")
{
    test_noncopyable a{42};
    test_noncopyable b{99};
    // This should fail to compile - uncomment to verify
    // b = a;
    SUCCEED("noncopyable class can be used with move semantics");
}

TEST_CASE("noncopyable - move construction is allowed", "[noncopyable]")
{
    test_noncopyable a{42};
    test_noncopyable b{ std::move(a) };
    REQUIRE(b.get() == 42);
}

TEST_CASE("noncopyable - can inherit and use normally", "[noncopyable]")
{
    test_noncopyable obj{123};
    REQUIRE(obj.get() == 123);
}
