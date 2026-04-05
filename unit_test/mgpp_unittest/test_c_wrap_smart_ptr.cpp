#include <catch2/catch.hpp>
#include <megopp/util/c_wrap_smart_ptr.h>

#include <memory>

// C struct with size == sizeof(std::shared_ptr<T>) + sizeof(size_t) == 3 * sizeof(void*)
struct alignas(void*) FakeCSmartStruct
{
    char data[3 * sizeof(void*)];
};
static_assert(std::is_trivial_v<FakeCSmartStruct>, "FakeCSmartStruct must be trivial");

namespace {
struct Item { int val; };
} // namespace

using Wrapper = mgpp::util::c_wrap_smart_ptr<Item, FakeCSmartStruct>;

TEST_CASE("c_wrap_smart_ptr - null_struct produces null shared_ptr", "[c_wrap_smart_ptr]")
{
    auto st = Wrapper::null_struct();
    auto sp = Wrapper::unwrap_struct(&st);
    REQUIRE_FALSE(static_cast<bool>(sp));
}

TEST_CASE("c_wrap_smart_ptr - wrap_struct embeds shared_ptr", "[c_wrap_smart_ptr]")
{
    auto obj = std::make_shared<Item>();
    obj->val = 55;

    auto st = Wrapper::wrap_struct(obj);
    auto sp = Wrapper::unwrap_struct(&st);
    REQUIRE(static_cast<bool>(sp));
    REQUIRE(sp->val == 55);

    Wrapper::unref_struct(&st);
}

TEST_CASE("c_wrap_smart_ptr - is_null on a null struct", "[c_wrap_smart_ptr]")
{
    auto st = Wrapper::null_struct();
    auto* w = reinterpret_cast<Wrapper*>(&st);
    REQUIRE(w->is_null());
}

TEST_CASE("c_wrap_smart_ptr - is_null false when populated", "[c_wrap_smart_ptr]")
{
    auto obj = std::make_shared<Item>();
    auto st = Wrapper::wrap_struct(obj);
    auto* w = reinterpret_cast<Wrapper*>(&st);
    REQUIRE_FALSE(w->is_null());
    Wrapper::unref_struct(&st);
}

TEST_CASE("c_wrap_smart_ptr - ref_struct copies ownership", "[c_wrap_smart_ptr]")
{
    auto obj = std::make_shared<Item>();
    obj->val = 13;

    auto st1 = Wrapper::wrap_struct(obj);
    auto st2 = Wrapper::ref_struct(&st1);

    auto sp1 = Wrapper::unwrap_struct(&st1);
    auto sp2 = Wrapper::unwrap_struct(&st2);
    REQUIRE(sp1.get() == sp2.get());
    REQUIRE(sp2->val == 13);

    Wrapper::unref_struct(&st1);
    Wrapper::unref_struct(&st2);
}

TEST_CASE("c_wrap_smart_ptr - ref_struct on null returns null", "[c_wrap_smart_ptr]")
{
    auto st = Wrapper::null_struct();
    auto st2 = Wrapper::ref_struct(&st);
    auto sp = Wrapper::unwrap_struct(&st2);
    REQUIRE_FALSE(static_cast<bool>(sp));
}

TEST_CASE("c_wrap_smart_ptr - copy_struct copies ownership", "[c_wrap_smart_ptr]")
{
    auto obj = std::make_shared<Item>();
    obj->val = 77;

    auto st1 = Wrapper::wrap_struct(obj);
    auto st2 = Wrapper::copy_struct(&st1);

    auto sp1 = Wrapper::unwrap_struct(&st1);
    auto sp2 = Wrapper::unwrap_struct(&st2);
    REQUIRE(sp1.get() == sp2.get());

    Wrapper::unref_struct(&st1);
    Wrapper::unref_struct(&st2);
}

TEST_CASE("c_wrap_smart_ptr - unref_struct releases embedded pointer", "[c_wrap_smart_ptr]")
{
    std::weak_ptr<Item> weak;
    {
        auto obj = std::make_shared<Item>();
        weak = obj;
        auto st = Wrapper::wrap_struct(obj);
        obj.reset();
        REQUIRE_FALSE(weak.expired()); // st still holds it
        Wrapper::unref_struct(&st);
        REQUIRE(weak.expired()); // released
    }
}

TEST_CASE("c_wrap_smart_ptr - unref_struct with nullptr is safe", "[c_wrap_smart_ptr]")
{
    REQUIRE_NOTHROW(Wrapper::unref_struct(nullptr));
}

TEST_CASE("c_wrap_smart_ptr - unwrap_struct with nullptr returns null", "[c_wrap_smart_ptr]")
{
    auto sp = Wrapper::unwrap_struct(nullptr);
    REQUIRE_FALSE(static_cast<bool>(sp));
}
