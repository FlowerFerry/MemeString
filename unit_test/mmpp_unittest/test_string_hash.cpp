#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/hash/std/hash.hpp>
#include <unordered_map>

// ---------------------------------------------------------------------------
// std::hash<memepp::string>
// ---------------------------------------------------------------------------

TEST_CASE("std::hash<memepp::string> same content produces same hash", "[string][hash]")
{
    memepp::string a("Hello");
    memepp::string b("Hello");
    REQUIRE(std::hash<memepp::string>{}(a) == std::hash<memepp::string>{}(b));
}

TEST_CASE("std::hash<memepp::string> different content produces different hash", "[string][hash]")
{
    memepp::string a("Hello");
    memepp::string b("World");
    REQUIRE(std::hash<memepp::string>{}(a) != std::hash<memepp::string>{}(b));
}

TEST_CASE("std::hash<memepp::string> empty string has stable hash", "[string][hash]")
{
    memepp::string a;
    memepp::string b;
    REQUIRE(std::hash<memepp::string>{}(a) == std::hash<memepp::string>{}(b));
}

TEST_CASE("std::hash<memepp::string> is deterministic across calls", "[string][hash]")
{
    memepp::string s("deterministic");
    size_t h1 = std::hash<memepp::string>{}(s);
    size_t h2 = std::hash<memepp::string>{}(s);
    REQUIRE(h1 == h2);
}

// ---------------------------------------------------------------------------
// std::hash<memepp::string_view>
// ---------------------------------------------------------------------------

TEST_CASE("std::hash<memepp::string_view> same content produces same hash", "[string_view][hash]")
{
    memepp::string_view a("Hello");
    memepp::string_view b("Hello");
    REQUIRE(std::hash<memepp::string_view>{}(a) == std::hash<memepp::string_view>{}(b));
}

TEST_CASE("std::hash<memepp::string_view> different content produces different hash", "[string_view][hash]")
{
    memepp::string_view a("Hello");
    memepp::string_view b("World");
    REQUIRE(std::hash<memepp::string_view>{}(a) != std::hash<memepp::string_view>{}(b));
}

TEST_CASE("std::hash<memepp::string_view> is deterministic across calls", "[string_view][hash]")
{
    memepp::string_view sv("deterministic");
    size_t h1 = std::hash<memepp::string_view>{}(sv);
    size_t h2 = std::hash<memepp::string_view>{}(sv);
    REQUIRE(h1 == h2);
}

TEST_CASE("std::hash string and string_view agree on same content", "[string][string_view][hash]")
{
    memepp::string      s ("Hello");
    memepp::string_view sv("Hello");
    REQUIRE(std::hash<memepp::string>{}(s) == std::hash<memepp::string_view>{}(sv));
}

TEST_CASE("std::hash string and string_view agree on UTF-8 content", "[string][string_view][hash]")
{
    memepp::string      s (u8"\u4F60\u597D\u4E16\u754C");
    memepp::string_view sv(u8"\u4F60\u597D\u4E16\u754C");
    REQUIRE(std::hash<memepp::string>{}(s) == std::hash<memepp::string_view>{}(sv));
}

// ---------------------------------------------------------------------------
// std::unordered_map<memepp::string, int>
// ---------------------------------------------------------------------------

TEST_CASE("std::unordered_map<memepp::string, int> insert and lookup", "[string][hash][unordered_map]")
{
    std::unordered_map<memepp::string, int> m;
    m[memepp::string("apple")]  = 1;
    m[memepp::string("banana")] = 2;
    m[memepp::string("cherry")] = 3;

    REQUIRE(m.at(memepp::string("apple"))  == 1);
    REQUIRE(m.at(memepp::string("banana")) == 2);
    REQUIRE(m.at(memepp::string("cherry")) == 3);
    REQUIRE(m.find(memepp::string("mango")) == m.end());
}

TEST_CASE("std::unordered_map<memepp::string, int> update existing key", "[string][hash][unordered_map]")
{
    std::unordered_map<memepp::string, int> m;
    m[memepp::string("key")] = 10;
    m[memepp::string("key")] = 20;

    REQUIRE(m.size() == 1);
    REQUIRE(m.at(memepp::string("key")) == 20);
}

TEST_CASE("std::unordered_map<memepp::string, int> erase", "[string][hash][unordered_map]")
{
    std::unordered_map<memepp::string, int> m;
    m[memepp::string("x")] = 99;
    REQUIRE(m.count(memepp::string("x")) == 1);

    m.erase(memepp::string("x"));
    REQUIRE(m.count(memepp::string("x")) == 0);
    REQUIRE(m.empty());
}

TEST_CASE("std::unordered_map<memepp::string, int> Chinese keys", "[string][hash][unordered_map]")
{
    std::unordered_map<memepp::string, int> m;
    m[memepp::string(u8"\u4F60\u597D")] = 1;
    m[memepp::string(u8"\u4E16\u754C")] = 2;
    m[memepp::string(u8"\u518D\u89C1")] = 3;

    REQUIRE(m.at(memepp::string(u8"\u4F60\u597D")) == 1);
    REQUIRE(m.at(memepp::string(u8"\u4E16\u754C")) == 2);
    REQUIRE(m.at(memepp::string(u8"\u518D\u89C1")) == 3);
    REQUIRE(m.find(memepp::string(u8"\u8C22\u8C22")) == m.end());
}

TEST_CASE("std::unordered_map<memepp::string, int> duplicate key preserves size", "[string][hash][unordered_map]")
{
    std::unordered_map<memepp::string, int> m;
    m[memepp::string("a")] = 1;
    m[memepp::string("b")] = 2;
    m[memepp::string("c")] = 3;
    m[memepp::string("a")] = 4;

    REQUIRE(m.size() == 3);
}

// ---------------------------------------------------------------------------
// std::unordered_map<memepp::string_view, int>
// ---------------------------------------------------------------------------

TEST_CASE("std::unordered_map<memepp::string_view, int> insert and lookup", "[string_view][hash][unordered_map]")
{
    memepp::string s_apple("apple");
    memepp::string s_banana("banana");
    memepp::string s_cherry("cherry");

    std::unordered_map<memepp::string_view, int> m;
    m[memepp::string_view(s_apple)]  = 10;
    m[memepp::string_view(s_banana)] = 20;
    m[memepp::string_view(s_cherry)] = 30;

    REQUIRE(m.at(memepp::string_view(s_apple))  == 10);
    REQUIRE(m.at(memepp::string_view(s_banana)) == 20);
    REQUIRE(m.at(memepp::string_view(s_cherry)) == 30);

    memepp::string s_mango("mango");
    REQUIRE(m.find(memepp::string_view(s_mango)) == m.end());
}

TEST_CASE("std::unordered_map<memepp::string_view, int> update existing key", "[string_view][hash][unordered_map]")
{
    memepp::string s_key("key");
    std::unordered_map<memepp::string_view, int> m;
    m[memepp::string_view(s_key)] = 1;
    m[memepp::string_view(s_key)] = 2;

    REQUIRE(m.size() == 1);
    REQUIRE(m.at(memepp::string_view(s_key)) == 2);
}

TEST_CASE("std::unordered_map<memepp::string_view, int> erase", "[string_view][hash][unordered_map]")
{
    memepp::string s("key");
    std::unordered_map<memepp::string_view, int> m;
    m[memepp::string_view(s)] = 42;
    REQUIRE(m.count(memepp::string_view(s)) == 1);

    m.erase(memepp::string_view(s));
    REQUIRE(m.count(memepp::string_view(s)) == 0);
}

TEST_CASE("std::unordered_map<memepp::string_view, int> literal view as key", "[string_view][hash][unordered_map]")
{
    std::unordered_map<memepp::string_view, int> m;
    m[memepp::string_view("alpha")] = 100;
    m[memepp::string_view("beta")]  = 200;

    REQUIRE(m.at(memepp::string_view("alpha")) == 100);
    REQUIRE(m.at(memepp::string_view("beta"))  == 200);
}
