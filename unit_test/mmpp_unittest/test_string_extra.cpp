#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/string_builder.hpp>
#include <memepp/rune.hpp>

// ---------------------------------------------------------------------------
// string constructors from rune and string_builder
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string constructor from rune", "[string]")
{
    memepp::rune r('A');
    memepp::string s(r);
    REQUIRE(s == "A");
    REQUIRE(s.size() == 1);
}

TEST_CASE("memepp::string constructor from rune — multi-byte", "[string]")
{
    memepp::rune r{ reinterpret_cast<const uint8_t*>(u8"中"), -1 };
    memepp::string s(r);
    REQUIRE(s == u8"中");
    REQUIRE(s.size() == 3);
}

TEST_CASE("memepp::string constructor from string_builder", "[string]")
{
    memepp::string_builder sb;
    sb += "Hello";
    sb += " ";
    sb += "World";
    memepp::string s(sb);
    REQUIRE(s == "Hello World");
}

TEST_CASE("memepp::string construct from string_view — from char* with length", "[string]")
{
    memepp::string s("Hello World", 5);
    REQUIRE(s == "Hello");
    REQUIRE(s.storage_type() == memepp::string_storage_t::small);
}

TEST_CASE("memepp::string construct from string_view — with storage hint", "[string]")
{
    memepp::string s("Hi", 2, memepp::string_storage_t::large);
    REQUIRE(s == "Hi");
    REQUIRE(s.storage_type() == memepp::string_storage_t::large);
}

TEST_CASE("memepp::string construct from const_pointer", "[string]")
{
    const memepp::string::const_pointer p =
        reinterpret_cast<const memepp::string::const_pointer>("Hello");
    memepp::string s(p);
    REQUIRE(s == "Hello");
}

TEST_CASE("memepp::string construct from const_pointer with size", "[string]")
{
    const memepp::string::const_pointer p =
        reinterpret_cast<const memepp::string::const_pointer>("Hello");
    memepp::string s(p, 2);
    REQUIRE(s == "He");
}

TEST_CASE("memepp::string construct from const_pointer with size and storage hint", "[string]")
{
    const memepp::string::const_pointer p =
        reinterpret_cast<const memepp::string::const_pointer>("Hello");
    memepp::string s(p, 4, memepp::string_storage_t::large);
    REQUIRE(s == "Hell");
    REQUIRE(s.storage_type() == memepp::string_storage_t::large);
}

TEST_CASE("memepp::string construct from const_pointer with storage hint (empty)", "[string]")
{
    const memepp::string::const_pointer p =
        reinterpret_cast<const memepp::string::const_pointer>("abc");
    memepp::string s(p, 0, memepp::string_storage_t::large);
    REQUIRE(s.empty());
    REQUIRE(s.storage_type() == memepp::string_storage_t::large);
}

TEST_CASE("memepp::string construct from mmstrstk_t (move)", "[string]")
{
    mmstrstk_t stk;
    MemeStringStack_initByU8bytes(&stk, MMSTR__OBJ_SIZE, (const uint8_t*)"Hello", -1);
    memepp::string s(std::move(stk));
    REQUIRE(s == "Hello");
}

TEST_CASE("memepp::string construct from mmstrstk_t (copy)", "[string]")
{
    mmstrstk_t stk;
    MemeStringStack_initByU8bytes(&stk, MMSTR__OBJ_SIZE, (const uint8_t*)"Hello", -1);
    memepp::string s(stk);
    REQUIRE(s == "Hello");
    MemeStringStack_unInit(&stk, MMSTR__OBJ_SIZE);
}

// ---------------------------------------------------------------------------
// string::cheap_copy / to_large_or_user
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string cheap_copy — small string", "[string]")
{
    memepp::string s = "Hi";
    auto copy = s.cheap_copy();
    REQUIRE(copy == "Hi");
}

TEST_CASE("memepp::string cheap_copy — empty string", "[string]")
{
    memepp::string s;
    auto copy = s.cheap_copy();
    REQUIRE(copy.empty());
}

TEST_CASE("memepp::string to_large_or_user — small", "[string]")
{
    memepp::string s = "Hi";
    auto result = s.to_large_or_user();
    REQUIRE(result == "Hi");
    // For small → promotes to large (or stays same if already large/user)
    // The exact behavior depends on implementation — verify it doesn't crash
    REQUIRE(result.empty() == false);
}

TEST_CASE("memepp::string to_large_or_user — empty", "[string]")
{
    memepp::string s;
    auto result = s.to_large_or_user();
    REQUIRE(result.empty());
}

TEST_CASE("memepp::string to_large_or_user — already large", "[string]")
{
    // need a string exceeding MemeStringOption_getStorageMediumLimit()
    memepp::string s = "This is a very long string that should definitely be stored in large storage mode because it exceeds the medium limit on all platforms by a comfortable margin, hopefully this is enough bytes to guarantee that.";
    REQUIRE(s.storage_type() == memepp::string_storage_t::large);
    auto result = s.to_large_or_user();
    REQUIRE(result == s);
    REQUIRE(result.storage_type() == memepp::string_storage_t::large);
}

// string::to_shared_storage
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string to_shared_storage — small", "[string]")
{
    memepp::string s = "Hi";
    auto result = s.to_shared_storage();
    REQUIRE(result == "Hi");
    REQUIRE(result.empty() == false);
    REQUIRE(result.storage_type() == memepp::string_storage_t::large);
}

TEST_CASE("memepp::string to_shared_storage — empty", "[string]")
{
    memepp::string s;
    auto result = s.to_shared_storage();
    REQUIRE(result.empty());
    REQUIRE(result.storage_type() == memepp::string_storage_t::large);
}

TEST_CASE("memepp::string to_shared_storage — already large", "[string]")
{
    memepp::string s = "This is a very long string that should definitely be stored in large storage mode because it exceeds the medium limit on all platforms by a comfortable margin, hopefully this is enough bytes to guarantee that.";
    REQUIRE(s.storage_type() == memepp::string_storage_t::large);
    auto result = s.to_shared_storage();
    REQUIRE(result == s);
    REQUIRE(result.storage_type() == memepp::string_storage_t::large);
}

// ---------------------------------------------------------------------------
// string::is_only_ascii
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string is_only_ascii — all ASCII", "[string]")
{
    memepp::string s1;
    REQUIRE(s1.is_only_ascii() == true);

    memepp::string s2 = "Hello, World! 123";
    REQUIRE(s2.is_only_ascii() == true);
}

TEST_CASE("memepp::string is_only_ascii — non-ASCII", "[string]")
{
    memepp::string s = u8"Hello, 世界!";
    REQUIRE(s.is_only_ascii() == false);
}

TEST_CASE("memepp::string is_only_ascii — binary bytes", "[string]")
{
    // bytes > 127 should be non-ASCII
    const char bytes[] = { 'H', 'e', (char)0x80, 'l', 'o', '\0' };
    memepp::string s(bytes, 5);
    REQUIRE(s.is_only_ascii() == false);
}

// ---------------------------------------------------------------------------
// string::operator+ with string_view
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string operator+ with string_view", "[string]")
{
    memepp::string s = "Hello";
    memepp::string_view sv = " World";
    auto result = s + sv;
    REQUIRE(result == "Hello World");
}

TEST_CASE("memepp::string operator+ with const char*", "[string]")
{
    memepp::string s = "Hello";
    auto result = s + " World";
    REQUIRE(result == "Hello World");
}

TEST_CASE("memepp::string operator+ with string", "[string]")
{
    memepp::string s = "Hello";
    memepp::string other = " World";
    auto result = s + other;
    REQUIRE(result == "Hello World");
    REQUIRE(result.size() == 11);
}

// ---------------------------------------------------------------------------
// string::compare
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string compare -- basic", "[string]")
{
    memepp::string a = "abc";
    memepp::string b = "abd";
    memepp::string c = "abc";

    REQUIRE(a.compare(b) < 0);
    REQUIRE(b.compare(a) > 0);
    REQUIRE(c.compare(c) == 0);
}

TEST_CASE("memepp::string compare — empty", "[string]")
{
    memepp::string empty;
    memepp::string s = "abc";
    REQUIRE(empty.compare(s) < 0);
    REQUIRE(s.compare(empty) > 0);
    REQUIRE(empty.compare(empty) == 0);
}

// ---------------------------------------------------------------------------
// string::rune_front / rune_back
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string rune_front/rune_back", "[string]")
{
    memepp::string s = u8"你好世界";
    auto front = s.rune_front();
    auto back = s.rune_back();
    REQUIRE(front == u8"你");
    REQUIRE(back == u8"界");
}

TEST_CASE("memepp::string rune_front/rune_back — single ASCII", "[string]")
{
    memepp::string s = "H";
    auto front = s.rune_front();
    auto back = s.rune_back();
    REQUIRE(front == 'H');
    REQUIRE(back == 'H');
}

// ---------------------------------------------------------------------------
// string::trim_prefix / trim_suffix (for string, returns string_view)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string trim_prefix", "[string]")
{
    memepp::string s = "Hello World";
    REQUIRE(s.trim_prefix("Hello ") == memepp::string_view{ "World" });
    REQUIRE(s.trim_prefix("World") == memepp::string_view{ "Hello World" }); // not found
    REQUIRE(s.trim_prefix("") == memepp::string_view{ "Hello World" });
}

TEST_CASE("memepp::string trim_suffix", "[string]")
{
    memepp::string s = "Hello World";
    REQUIRE(s.trim_suffix(" World") == memepp::string_view{ "Hello" });
    REQUIRE(s.trim_suffix("Hello") == memepp::string_view{ "Hello World" }); // not found
    REQUIRE(s.trim_suffix("") == memepp::string_view{ "Hello World" });
}

// ---------------------------------------------------------------------------
// string::mapping_convert — advanced edge cases
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string mapping_convert — empty", "[string]")
{
    memepp::string s;
    auto result = s.mapping_convert([](memepp::rune&) { return 0; });
    REQUIRE(result.empty());
}

TEST_CASE("memepp::string mapping_convert — delete all", "[string]")
{
    memepp::string s = "Hello";
    auto result = s.mapping_convert([](memepp::rune&) { return -1; });
    REQUIRE(result.empty());
}

TEST_CASE("memepp::string mapping_convert — replace with larger rune", "[string]")
{
    memepp::string s = u8"ab";
    auto result = s.mapping_convert([](memepp::rune& r) {
        if (r == 'a')
            r = memepp::rune{ reinterpret_cast<const uint8_t*>(u8"中"), -1 };
        return 0;
    });
    REQUIRE(result == u8"中b");
}

TEST_CASE("memepp::string mapping_convert — multiple modifications", "[string]")
{
    memepp::string s = u8"ab零cd";
    auto result = s.mapping_convert([](memepp::rune& r) {
        if (r == 'b') r = 'B';
        if (r == u8"零") return -1; // delete 零
        return 0;
    });
    REQUIRE(result == u8"aBcd");
}

// ---------------------------------------------------------------------------
// string trim_if / trim_if_rune
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string trim_if — predicate", "[string]")
{
    memepp::string s = "***Hello***";
    auto result = s.trim_if([](const mmbyte_t& b) { return b == '*'; });
    REQUIRE(result == "Hello");
}

TEST_CASE("memepp::string trim_if — only matching chars", "[string]")
{
    memepp::string s = "****";
    auto result = s.trim_if([](const mmbyte_t& b) { return b == '*'; });
    REQUIRE(result == "");
}

TEST_CASE("memepp::string trim_if — no trim needed", "[string]")
{
    memepp::string s = "Hello";
    auto result = s.trim_if([](const mmbyte_t& b) { return b == '*'; });
    REQUIRE(result == "Hello");
}

TEST_CASE("memepp::string trim_if_rune — predicate", "[string]")
{
    memepp::string s = u8"*你*好*";
    auto result = s.trim_if_rune([](const memepp::rune& r) { return r == '*'; });
    REQUIRE(result == u8"你*好");
}

TEST_CASE("memepp::string trim_if_rune — empty string", "[string]")
{
    memepp::string s;
    auto result = s.trim_if_rune([](const memepp::rune&) { return true; });
    REQUIRE(result.empty());
}

// ---------------------------------------------------------------------------
// string::split with default behavior (skip_empty_parts)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string split — default behavior", "[string]")
{
    memepp::string s = "a,b,c";
    std::vector<memepp::string> parts;
    s.split(",", memepp::split_behav_t::skip_empty_parts, std::back_inserter(parts));
    REQUIRE(parts.size() == 3);
    REQUIRE(parts[0] == "a");
    REQUIRE(parts[1] == "b");
    REQUIRE(parts[2] == "c");
}

TEST_CASE("memepp::string split — keep empty parts", "[string]")
{
    memepp::string s = "a,,b,";
    std::vector<memepp::string> parts;
    s.split(",", memepp::split_behav_t::keep_empty_parts, std::back_inserter(parts));
    REQUIRE(parts.size() == 4);
    REQUIRE(parts[0] == "a");
    REQUIRE(parts[1] == "");
    REQUIRE(parts[2] == "b");
    REQUIRE(parts[3] == "");
}

// ---------------------------------------------------------------------------
// string::join
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string join", "[string]")
{
    memepp::string sep = ",";
    std::vector<memepp::string> items = { "a", "b", "c" };
    auto result = sep.join(items);
    REQUIRE(result == "a,b,c");
}

TEST_CASE("memepp::string join — empty separator", "[string]")
{
    memepp::string sep;
    std::vector<memepp::string> items = { "a", "b", "c" };
    auto result = sep.join(items);
    REQUIRE(result == "abc");
}

TEST_CASE("memepp::string join — single item", "[string]")
{
    memepp::string sep = ",";
    std::vector<memepp::string> items = { "only" };
    auto result = sep.join(items);
    REQUIRE(result == "only");
}

TEST_CASE("memepp::string join — empty list", "[string]")
{
    memepp::string sep = ",";
    std::vector<memepp::string> items;
    auto result = sep.join(items);
    REQUIRE(result.empty());
}

TEST_CASE("memepp::string join — string_view items", "[string]")
{
    memepp::string sep = "-";
    std::vector<memepp::string_view> items = { "a", "b", "c" };
    auto result = sep.join(items);
    REQUIRE(result == "a-b-c");
}

// ---------------------------------------------------------------------------
// string::operator=(const string_builder&)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string operator= with string_builder", "[string]")
{
    memepp::string_builder sb;
    sb += "Built ";
    sb += "string";

    memepp::string s;
    s = sb;
    REQUIRE(s == "Built string");
}

TEST_CASE("memepp::string operator= with string_builder — empty builder", "[string]")
{
    memepp::string_builder sb;
    memepp::string s = "existing";
    s = sb;
    REQUIRE(s.empty());
}

// ---------------------------------------------------------------------------
// string::replace with count parameter
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string replace with count", "[string]")
{
    memepp::string s = "aa_aa_aa";
    // Replace all occurrences (count=-1)
    auto r3 = s.replace("_", "-", -1);
    REQUIRE(r3 == "aa-aa-aa");
}
