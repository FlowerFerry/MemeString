#include <catch2/catch.hpp>
#include <megopp/util/c_wrap_weak_ptr.h>

#include <memory>

// C struct with size == sizeof(std::weak_ptr<T>) == 2 * sizeof(void*)
struct alignas(void*) FakeCWeakStruct
{
    char data[2 * sizeof(void*)];
};
static_assert(std::is_trivial_v<FakeCWeakStruct>, "FakeCWeakStruct must be trivial");

namespace {
struct Node { int id; };
} // namespace

using Wrapper = mgpp::util::c_wrap_weak_ptr<Node, FakeCWeakStruct>;

TEST_CASE("c_wrap_weak_ptr - null_struct produces an expired/empty weak_ptr", "[c_wrap_weak_ptr]")
{
    auto st = Wrapper::null_struct();
    auto wp = Wrapper::unwrap_struct(&st);
    REQUIRE(wp.expired());
}

TEST_CASE("c_wrap_weak_ptr - wrap_struct from shared_ptr is not expired", "[c_wrap_weak_ptr]")
{
    auto sp = std::make_shared<Node>();
    sp->id = 10;

    auto st = Wrapper::wrap_struct(sp);
    REQUIRE_FALSE(Wrapper::unwrap_struct(&st).expired());

    auto locked = Wrapper::unwrap_struct(&st).lock();
    REQUIRE(locked != nullptr);
    REQUIRE(locked->id == 10);
}

TEST_CASE("c_wrap_weak_ptr - expired() after shared_ptr is reset", "[c_wrap_weak_ptr]")
{
    std::shared_ptr<Node> sp = std::make_shared<Node>();
    auto st = Wrapper::wrap_struct(sp);
    sp.reset(); // drop the last strong reference

    auto* w = reinterpret_cast<Wrapper*>(&st);
    REQUIRE(w->expired());
}

TEST_CASE("c_wrap_weak_ptr - copy_struct while alive returns valid copy", "[c_wrap_weak_ptr]")
{
    auto sp = std::make_shared<Node>();
    sp->id = 20;

    auto st1 = Wrapper::wrap_struct(sp);
    auto st2 = Wrapper::copy_struct(&st1);

    auto locked = Wrapper::unwrap_struct(&st2).lock();
    REQUIRE(locked != nullptr);
    REQUIRE(locked->id == 20);
}

TEST_CASE("c_wrap_weak_ptr - copy_struct after expiry returns null_struct", "[c_wrap_weak_ptr]")
{
    auto st = Wrapper::null_struct();
    {
        auto sp = std::make_shared<Node>();
        st = Wrapper::wrap_struct(sp);
    } // sp out of scope, weak_ptr expired

    auto copy = Wrapper::copy_struct(&st);
    REQUIRE(Wrapper::unwrap_struct(&copy).expired());
}

TEST_CASE("c_wrap_weak_ptr - copy_struct with nullptr is safe", "[c_wrap_weak_ptr]")
{
    auto st = Wrapper::copy_struct(static_cast<const FakeCWeakStruct*>(nullptr));
    REQUIRE(Wrapper::unwrap_struct(&st).expired());
}

TEST_CASE("c_wrap_weak_ptr - reset_struct clears the embedded weak_ptr", "[c_wrap_weak_ptr]")
{
    auto sp = std::make_shared<Node>();
    auto st = Wrapper::wrap_struct(sp);
    REQUIRE_FALSE(Wrapper::unwrap_struct(&st).expired());

    Wrapper::reset_struct(&st, std::weak_ptr<Node>{});
    REQUIRE(Wrapper::unwrap_struct(&st).expired());
}

TEST_CASE("c_wrap_weak_ptr - reset_struct with nullptr is safe", "[c_wrap_weak_ptr]")
{
    REQUIRE_NOTHROW(Wrapper::reset_struct(nullptr, std::weak_ptr<Node>{}));
}

TEST_CASE("c_wrap_weak_ptr - unwrap_struct with nullptr returns expired weak_ptr", "[c_wrap_weak_ptr]")
{
    auto wp = Wrapper::unwrap_struct(nullptr);
    REQUIRE(wp.expired());
}
