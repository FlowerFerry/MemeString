#include <catch2/catch.hpp>
#include <megopp/util/c_wrap_shared_ptr.h>

#include <memory>

// C struct with size == sizeof(std::shared_ptr<T>) == 2 * sizeof(void*)
struct alignas(void*) FakeCSharedStruct
{
    char data[2 * sizeof(void*)];
};
static_assert(std::is_trivial_v<FakeCSharedStruct>, "FakeCSharedStruct must be trivial");

namespace {
struct Obj { int value; };
} // namespace

using Wrapper = mgpp::util::c_wrap_shared_ptr<Obj, FakeCSharedStruct>;

TEST_CASE("c_wrap_shared_ptr - null_struct produces null shared_ptr", "[c_wrap_shared_ptr]")
{
    auto st = Wrapper::null_struct();
    const auto& sp = Wrapper::unwrap_struct(st);
    REQUIRE_FALSE(static_cast<bool>(sp));
    // Clean up (no-op for null)
    Wrapper::reset_struct(st);
}

TEST_CASE("c_wrap_shared_ptr - wrap_struct embeds shared_ptr", "[c_wrap_shared_ptr]")
{
    auto obj = std::make_shared<Obj>();
    obj->value = 42;

    auto st = Wrapper::wrap_struct(obj);
    const auto& sp = Wrapper::unwrap_struct(st);
    REQUIRE(static_cast<bool>(sp));
    REQUIRE(sp->value == 42);

    Wrapper::reset_struct(st);
}

TEST_CASE("c_wrap_shared_ptr - unwrap_struct ref-shares ownership", "[c_wrap_shared_ptr]")
{
    auto obj = std::make_shared<Obj>();
    obj->value = 7;

    auto st = Wrapper::wrap_struct(obj);
    {
        const auto& sp = Wrapper::unwrap_struct(st);
        // obj + st + sp (from unwrap reference) all share the same control block
        REQUIRE(sp.use_count() >= 2); // obj + embedded in st
    }

    Wrapper::reset_struct(st);
}

TEST_CASE("c_wrap_shared_ptr - copy_struct copies ownership", "[c_wrap_shared_ptr]")
{
    auto obj = std::make_shared<Obj>();
    obj->value = 99;

    auto st1 = Wrapper::wrap_struct(obj);
    auto st2 = Wrapper::copy_struct(st1);
    const auto& sp1 = Wrapper::unwrap_struct(st1);
    const auto& sp2 = Wrapper::unwrap_struct(st2);
    REQUIRE(sp1.get() == sp2.get());

    Wrapper::reset_struct(st1);
    Wrapper::reset_struct(st2);
}

TEST_CASE("c_wrap_shared_ptr - copy_struct of null produces null", "[c_wrap_shared_ptr]")
{
    auto st = Wrapper::null_struct();
    auto copied = Wrapper::copy_struct(st);
    const auto& sp = Wrapper::unwrap_struct(copied);
    REQUIRE_FALSE(static_cast<bool>(sp));
}

TEST_CASE("c_wrap_shared_ptr - reset_struct releases the embedded pointer", "[c_wrap_shared_ptr]")
{
    std::weak_ptr<Obj> weak;
    {
        auto obj = std::make_shared<Obj>();
        weak = obj;
        auto st = Wrapper::wrap_struct(obj);
        obj.reset(); // drop external reference
        REQUIRE_FALSE(weak.expired()); // st still holds it
        Wrapper::reset_struct(st);
        REQUIRE(weak.expired()); // released
    }
}

TEST_CASE("c_wrap_shared_ptr - reset_struct with null pointer is safe", "[c_wrap_shared_ptr]")
{
    auto st = Wrapper::null_struct();
    REQUIRE_NOTHROW(Wrapper::reset_struct(st));
}

TEST_CASE("c_wrap_shared_ptr - reset_struct pointer overload with nullptr is safe", "[c_wrap_shared_ptr]")
{
    REQUIRE_NOTHROW(Wrapper::reset_struct(nullptr));
}

TEST_CASE("c_wrap_shared_ptr - unwrap_struct with nullptr returns null", "[c_wrap_shared_ptr]")
{
    auto sp = Wrapper::unwrap_struct(nullptr);
    REQUIRE_FALSE(static_cast<bool>(sp));
}

TEST_CASE("c_wrap_shared_ptr - copy_struct with nullptr is safe", "[c_wrap_shared_ptr]")
{
    auto st = Wrapper::copy_struct(static_cast<const FakeCSharedStruct*>(nullptr));
    const auto& sp = Wrapper::unwrap_struct(st);
    REQUIRE_FALSE(static_cast<bool>(sp));
}
