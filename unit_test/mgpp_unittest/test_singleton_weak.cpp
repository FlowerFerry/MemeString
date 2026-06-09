#include <catch2/catch.hpp>
#include <megopp/help/singleton_weak.h>

TEST_CASE("singleton_weak - install and get_or_throw", "[singleton_weak]")
{
    auto sp = std::make_shared<int>(42);
    mgpp::help::singleton_weak<int>::install(sp);

    auto retrieved = mgpp::help::singleton_weak<int>::get_or_throw();
    REQUIRE(retrieved);
    REQUIRE(*retrieved == 42);
}

TEST_CASE("singleton_weak - get returns null before install", "[singleton_weak]")
{
    auto retrieved = mgpp::help::singleton_weak<short>::get();
    REQUIRE(retrieved == nullptr);
}

TEST_CASE("singleton_weak - get_or_throw throws before install", "[singleton_weak]")
{
    REQUIRE_THROWS_AS(mgpp::help::singleton_weak<long>::get_or_throw(), std::runtime_error);
}

TEST_CASE("singleton_weak - double install throws", "[singleton_weak]")
{
    auto sp = std::make_shared<double>(1.0);
    mgpp::help::singleton_weak<double>::install(sp);
    REQUIRE_THROWS_AS(mgpp::help::singleton_weak<double>::install(sp), std::runtime_error);
}

TEST_CASE("singleton_weak - get after install returns valid pointer", "[singleton_weak]")
{
    auto sp = std::make_shared<std::string>("test");
    mgpp::help::singleton_weak<std::string>::install(sp);

    auto retrieved = mgpp::help::singleton_weak<std::string>::get();
    REQUIRE(retrieved);
    REQUIRE(*retrieved == "test");
}

TEST_CASE("singleton_weak - is_installed", "[singleton_weak]")
{
    REQUIRE_FALSE(mgpp::help::singleton_weak<float>::is_installed());
    {
        auto sp = std::make_shared<float>(7.0f);
        mgpp::help::singleton_weak<float>::install(sp);
        REQUIRE(mgpp::help::singleton_weak<float>::is_installed());
    }
    // Instance still counts as installed even after shared_ptr expires
    REQUIRE(mgpp::help::singleton_weak<float>::is_installed());
}

TEST_CASE("singleton_weak - after target expires, get_or_throw throws", "[singleton_weak]")
{
    {
        auto sp = std::make_shared<unsigned>(99);
        mgpp::help::singleton_weak<unsigned>::install(sp);
    }
    REQUIRE_THROWS_AS(mgpp::help::singleton_weak<unsigned>::get_or_throw(), std::runtime_error);
}
