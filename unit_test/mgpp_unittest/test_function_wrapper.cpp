#include <catch2/catch.hpp>
#include <megopp/util/function_wrapper.h>

#include <functional>
#include <memory>
#include <tuple>

namespace {

// Transformer that passes arguments through unchanged
struct IdentityTransformer
{
    static std::tuple<int> transform_in(int a)
    {
        return std::make_tuple(a);
    }
};

// Transformer that doubles the return value
struct DoubleOutTransformer
{
    static std::tuple<int> transform_in(int a)
    {
        return std::make_tuple(a);
    }

    static int transform_out(int result)
    {
        return result * 2;
    }
};

// Transformer for two-argument functions
struct TwoArgTransformer
{
    static std::tuple<int, int> transform_in(int a, int b)
    {
        return std::make_tuple(a, b);
    }
};

} // namespace

using mgpp::util::function_wrapper;

// ============================================================
// Identity transformer (no transform_out)
// ============================================================

TEST_CASE("function_wrapper - identity transformer invoke returns correct result", "[function_wrapper]")
{
    using FW = function_wrapper<int(int), IdentityTransformer>;
    auto* fw = FW{ [](int x) { return x + 1; } }.to_heap_object();
    int result = FW::invoke(fw, 5);
    REQUIRE(result == 6);
    FW::destroy(fw);
}

TEST_CASE("function_wrapper - two-argument transformer invoke", "[function_wrapper]")
{
    using FW = function_wrapper<int(int, int), TwoArgTransformer>;
    auto* fw = FW{ [](int a, int b) { return a + b; } }.to_heap_object();
    int result = FW::invoke(fw, 3, 4);
    REQUIRE(result == 7);
    FW::destroy(fw);
}

// ============================================================
// transform_out transformer
// ============================================================

TEST_CASE("function_wrapper - transform_out doubles the return value", "[function_wrapper]")
{
    using FW = function_wrapper<int(int), DoubleOutTransformer>;
    auto* fw = FW{ [](int x) { return x + 1; } }.to_heap_object();
    // (5 + 1) = 6, then doubled = 12
    int result = FW::invoke(fw, 5);
    REQUIRE(result == 12);
    FW::destroy(fw);
}

// ============================================================
// Null / empty cases
// ============================================================

TEST_CASE("function_wrapper - invoke with null user_data returns default", "[function_wrapper]")
{
    using FW = function_wrapper<int(int), IdentityTransformer>;
    int result = FW::invoke(nullptr, 5);
    REQUIRE(result == 0);
}

TEST_CASE("function_wrapper - destroy with null is safe", "[function_wrapper]")
{
    using FW = function_wrapper<int(int), IdentityTransformer>;
    REQUIRE_NOTHROW(FW::destroy(nullptr));
}

// ============================================================
// Heap object lifetime
// ============================================================

TEST_CASE("function_wrapper - to_heap_object and destroy do not leak", "[function_wrapper]")
{
    using FW = function_wrapper<int(int), IdentityTransformer>;

    auto alive = std::make_shared<bool>(true);
    std::weak_ptr<bool> weak = alive;

    {
        auto* fw = FW{ [alive](int x) mutable { return x; } }.to_heap_object();
        alive.reset(); // drop our external reference
        REQUIRE_FALSE(weak.expired()); // fw still holds it
        int r = FW::invoke(fw, 7);
        REQUIRE(r == 7);
        FW::destroy(fw); // should release the captured shared_ptr
    }

    REQUIRE(weak.expired()); // lambda destroyed, shared_ptr released
}

TEST_CASE("function_wrapper - function constructed from lvalue", "[function_wrapper]")
{
    using FW = function_wrapper<int(int), IdentityTransformer>;
    std::function<int(int)> fn = [](int x) { return x * 3; };
    auto* fw = FW{ fn }.to_heap_object();
    REQUIRE(FW::invoke(fw, 4) == 12);
    FW::destroy(fw);
}
