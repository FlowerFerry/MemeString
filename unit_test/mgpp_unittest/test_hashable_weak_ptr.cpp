#include <catch2/catch.hpp>

#include <megopp/memory/hashable_weak_ptr.hpp>

#include <functional>
#include <map>
#include <memory>
#include <unordered_map>

using mgpp::mem::hashable_weak_ptr;

namespace {

struct Foo {
    int id;
    explicit Foo(int i) : id(i) {}
};

} // namespace

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

TEST_CASE("hashable_weak_ptr - default construction is expired with hash 0", "[hashable_weak_ptr]")
{
    hashable_weak_ptr<Foo> hwp;
    REQUIRE(hwp.hash_value() == 0);
    REQUIRE(hwp.expired());
    REQUIRE(hwp.lock() == nullptr);
}

TEST_CASE("hashable_weak_ptr - construct from shared_ptr", "[hashable_weak_ptr]")
{
    auto sp = std::make_shared<Foo>(1);
    hashable_weak_ptr<Foo> hwp(sp);

    REQUIRE(!hwp.expired());
    REQUIRE(hwp.lock() == sp);
    REQUIRE(hwp.hash_value() == std::hash<std::shared_ptr<Foo>>{}(sp));
}

TEST_CASE("hashable_weak_ptr - construct from weak_ptr", "[hashable_weak_ptr]")
{
    auto sp = std::make_shared<Foo>(2);
    std::weak_ptr<Foo> wp = sp;
    hashable_weak_ptr<Foo> hwp(wp);

    REQUIRE(!hwp.expired());
    REQUIRE(hwp.lock() == sp);
    REQUIRE(hwp.hash_value() == std::hash<std::shared_ptr<Foo>>{}(sp));
}

TEST_CASE("hashable_weak_ptr - copy construction preserves hash and ptr", "[hashable_weak_ptr]")
{
    auto sp = std::make_shared<Foo>(3);
    hashable_weak_ptr<Foo> hwp1(sp);
    hashable_weak_ptr<Foo> hwp2(hwp1);

    REQUIRE(hwp2.hash_value() == hwp1.hash_value());
    REQUIRE(hwp2.lock() == sp);
}

// ---------------------------------------------------------------------------
// hash_value stability
// ---------------------------------------------------------------------------

TEST_CASE("hashable_weak_ptr - hash_value is stable after managed object is destroyed", "[hashable_weak_ptr]")
{
    std::shared_ptr<Foo> sp = std::make_shared<Foo>(10);
    hashable_weak_ptr<Foo> hwp(sp);
    size_t hash_before = hwp.hash_value();

    sp.reset(); // destroy the managed object

    REQUIRE(hwp.expired());
    REQUIRE(hwp.hash_value() == hash_before);
}

// ---------------------------------------------------------------------------
// operator== / != / <
// ---------------------------------------------------------------------------

TEST_CASE("hashable_weak_ptr - two wrappers from the same shared_ptr are equal", "[hashable_weak_ptr]")
{
    auto sp = std::make_shared<Foo>(1);
    hashable_weak_ptr<Foo> a(sp);
    hashable_weak_ptr<Foo> b(sp);

    REQUIRE(a == b);
    REQUIRE(!(a != b));
}

TEST_CASE("hashable_weak_ptr - wrappers from different shared_ptrs are not equal", "[hashable_weak_ptr]")
{
    auto sp1 = std::make_shared<Foo>(1);
    auto sp2 = std::make_shared<Foo>(2);
    hashable_weak_ptr<Foo> a(sp1);
    hashable_weak_ptr<Foo> b(sp2);

    REQUIRE(a != b);
    REQUIRE(!(a == b));
}

TEST_CASE("hashable_weak_ptr - operator== is stable after managed object is destroyed", "[hashable_weak_ptr]")
{
    auto sp = std::make_shared<Foo>(1);
    hashable_weak_ptr<Foo> a(sp);
    hashable_weak_ptr<Foo> b(sp);

    sp.reset(); // both a and b are now expired

    REQUIRE(a == b); // control-block identity is preserved
}

TEST_CASE("hashable_weak_ptr - operator< provides strict weak ordering", "[hashable_weak_ptr]")
{
    auto sp1 = std::make_shared<Foo>(1);
    auto sp2 = std::make_shared<Foo>(2);
    hashable_weak_ptr<Foo> a(sp1);
    hashable_weak_ptr<Foo> b(sp2);

    // Exactly one of a<b or b<a must be true (they are distinct objects)
    bool ab = a < b;
    bool ba = b < a;
    REQUIRE(ab != ba); // strict: one is less, the other is not

    // Reflexivity: a < a must be false
    REQUIRE(!(a < a));
}

TEST_CASE("hashable_weak_ptr - default-constructed instances are equal to each other", "[hashable_weak_ptr]")
{
    hashable_weak_ptr<Foo> a;
    hashable_weak_ptr<Foo> b;
    // Both are default (null); owner_before(nullptr,...) is implementation-defined but
    // both share the same null state so neither should owner_before the other.
    REQUIRE(!(a < b));
    REQUIRE(!(b < a));
}

// ---------------------------------------------------------------------------
// reset
// ---------------------------------------------------------------------------

TEST_CASE("hashable_weak_ptr - reset() makes it expired with hash 0", "[hashable_weak_ptr]")
{
    auto sp = std::make_shared<Foo>(5);
    hashable_weak_ptr<Foo> hwp(sp);
    REQUIRE(!hwp.expired());

    hwp.reset();

    REQUIRE(hwp.expired());
    REQUIRE(hwp.hash_value() == 0);
    REQUIRE(hwp.lock() == nullptr);
}

TEST_CASE("hashable_weak_ptr - reset(shared_ptr) re-points to new object", "[hashable_weak_ptr]")
{
    auto sp1 = std::make_shared<Foo>(1);
    auto sp2 = std::make_shared<Foo>(2);
    hashable_weak_ptr<Foo> hwp(sp1);

    hwp.reset(sp2);

    REQUIRE(hwp.lock() == sp2);
    REQUIRE(hwp.hash_value() == std::hash<std::shared_ptr<Foo>>{}(sp2));
}

// ---------------------------------------------------------------------------
// Assignment operators
// ---------------------------------------------------------------------------

TEST_CASE("hashable_weak_ptr - operator= from shared_ptr", "[hashable_weak_ptr]")
{
    auto sp = std::make_shared<Foo>(7);
    hashable_weak_ptr<Foo> hwp;
    hwp = sp;

    REQUIRE(hwp.lock() == sp);
    REQUIRE(hwp.hash_value() == std::hash<std::shared_ptr<Foo>>{}(sp));
}

TEST_CASE("hashable_weak_ptr - operator= copy assignment", "[hashable_weak_ptr]")
{
    auto sp = std::make_shared<Foo>(8);
    hashable_weak_ptr<Foo> src(sp);
    hashable_weak_ptr<Foo> dst;
    dst = src;

    REQUIRE(dst.lock() == sp);
    REQUIRE(dst.hash_value() == src.hash_value());
}

TEST_CASE("hashable_weak_ptr - operator= from weak_ptr", "[hashable_weak_ptr]")
{
    auto sp = std::make_shared<Foo>(9);
    std::weak_ptr<Foo> wp = sp;
    hashable_weak_ptr<Foo> hwp;
    hwp = wp;

    REQUIRE(hwp.lock() == sp);
    REQUIRE(hwp.hash_value() == std::hash<std::shared_ptr<Foo>>{}(sp));
}

// ---------------------------------------------------------------------------
// weak_ptr() accessor
// ---------------------------------------------------------------------------

TEST_CASE("hashable_weak_ptr - weak_ptr() accessor returns an equivalent weak_ptr", "[hashable_weak_ptr]")
{
    auto sp = std::make_shared<Foo>(4);
    hashable_weak_ptr<Foo> hwp(sp);

    auto wp = hwp.weak_ptr();
    REQUIRE(wp.lock() == sp);
}

// ---------------------------------------------------------------------------
// std::hash specialization
// ---------------------------------------------------------------------------

TEST_CASE("hashable_weak_ptr - std::hash specialization matches hash_value()", "[hashable_weak_ptr]")
{
    auto sp = std::make_shared<Foo>(42);
    hashable_weak_ptr<Foo> hwp(sp);

    size_t h1 = std::hash<hashable_weak_ptr<Foo>>{}(hwp);
    size_t h2 = hwp.hash_value();

    REQUIRE(h1 == h2);
}

// ---------------------------------------------------------------------------
// Use as std::unordered_map key
// ---------------------------------------------------------------------------

TEST_CASE("hashable_weak_ptr - usable as key in std::unordered_map", "[hashable_weak_ptr]")
{
    std::unordered_map<hashable_weak_ptr<Foo>, int> m;

    auto sp1 = std::make_shared<Foo>(1);
    auto sp2 = std::make_shared<Foo>(2);
    hashable_weak_ptr<Foo> k1(sp1);
    hashable_weak_ptr<Foo> k2(sp2);

    m[k1] = 10;
    m[k2] = 20;

    REQUIRE(m.size() == 2);
    REQUIRE(m.at(k1) == 10);
    REQUIRE(m.at(k2) == 20);
}

TEST_CASE("hashable_weak_ptr - unordered_map lookup survives managed object destruction", "[hashable_weak_ptr]")
{
    std::unordered_map<hashable_weak_ptr<Foo>, int> m;

    auto sp = std::make_shared<Foo>(99);
    hashable_weak_ptr<Foo> key(sp);
    m[key] = 42;

    sp.reset(); // managed object destroyed; key is now expired

    REQUIRE(key.expired());

    // The entry must still be findable using the same (expired) key
    auto it = m.find(key);
    REQUIRE(it != m.end());
    REQUIRE(it->second == 42);
}

// ---------------------------------------------------------------------------
// Use as std::map key
// ---------------------------------------------------------------------------

TEST_CASE("hashable_weak_ptr - usable as key in std::map", "[hashable_weak_ptr]")
{
    std::map<hashable_weak_ptr<Foo>, int> m;

    auto sp1 = std::make_shared<Foo>(1);
    auto sp2 = std::make_shared<Foo>(2);
    hashable_weak_ptr<Foo> k1(sp1);
    hashable_weak_ptr<Foo> k2(sp2);

    m[k1] = 1;
    m[k2] = 2;

    REQUIRE(m.size() == 2);
    REQUIRE(m[k1] == 1);
    REQUIRE(m[k2] == 2);
}

TEST_CASE("hashable_weak_ptr - map lookup survives managed object destruction", "[hashable_weak_ptr]")
{
    std::map<hashable_weak_ptr<Foo>, int> m;

    auto sp = std::make_shared<Foo>(77);
    hashable_weak_ptr<Foo> key(sp);
    m[key] = 55;

    sp.reset();

    REQUIRE(key.expired());
    auto it = m.find(key);
    REQUIRE(it != m.end());
    REQUIRE(it->second == 55);
}
