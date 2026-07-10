#include <catch2/catch.hpp>

#include <memepp/string_span.hpp>
#include <memepp/string.hpp>

// ============================================================================
//  member types & npos
// ============================================================================

TEST_CASE("string_span member types", "[string_span]")
{
    using sv = memepp::string_span;
    static_assert(sizeof(sv::value_type) == 1, "");
    static_assert(sizeof(sv::size_type) >= 4, "");
    static_assert(sizeof(sv::difference_type) >= 4, "");
    SUCCEED();
}

TEST_CASE("string_span npos", "[string_span]")
{
    REQUIRE(memepp::string_span::npos == static_cast<memepp::string_span::size_type>(-1));
}

// ============================================================================
//  constructors
// ============================================================================

TEST_CASE("string_span default constructor", "[string_span]")
{
    memepp::string_span sv;
    REQUIRE(sv.empty());
    REQUIRE(sv.size() == 0);
}

TEST_CASE("string_span constructor from const char*", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.size() == 5);
    REQUIRE(sv.length() == 5);
    REQUIRE(sv == "hello");
}

TEST_CASE("string_span constructor from const char* with count", "[string_span]")
{
    memepp::string_span sv("hello world", 5);
    REQUIRE(sv.size() == 5);
    REQUIRE(sv == "hello");
}

TEST_CASE("string_span constructor from const_pointer", "[string_span]")
{
    const MemeByte_t bytes[] = {'a', 'b', 'c', '\0'};
    memepp::string_span sv(bytes);
    REQUIRE(sv.size() == 3);
    REQUIRE(sv[0] == 'a');
    REQUIRE(sv[2] == 'c');
}

TEST_CASE("string_span constructor from const_pointer with count", "[string_span]")
{
    const MemeByte_t bytes[] = {'x', 'y', 'z'};
    memepp::string_span sv(bytes, 2);
    REQUIRE(sv.size() == 2);
    REQUIRE(sv == "xy");
}

TEST_CASE("string_span constructor from memepp::string", "[string_span]")
{
    memepp::string s("test123");
    memepp::string_span sv(s);
    REQUIRE(sv.size() == 7);
    REQUIRE(sv == "test123");
}

TEST_CASE("string_span copy constructor", "[string_span]")
{
    memepp::string_span a("abc");
    memepp::string_span b(a);
    REQUIRE(b == "abc");
    REQUIRE(b.data() == a.data());
}

TEST_CASE("string_span copy assignment", "[string_span]")
{
    memepp::string_span a("hello");
    memepp::string_span b("world");
    b = a;
    REQUIRE(b == "hello");
}

// ============================================================================
//  element access
// ============================================================================

TEST_CASE("string_span operator[]", "[string_span]")
{
    memepp::string_span sv("abc");
    REQUIRE(sv[0] == 'a');
    REQUIRE(sv[1] == 'b');
    REQUIRE(sv[2] == 'c');
}

TEST_CASE("string_span at", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.at(0) == 'h');
    REQUIRE(sv.at(4) == 'o');
}

TEST_CASE("string_span at out_of_range", "[string_span]")
{
    memepp::string_span sv("abc");
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(sv.at(3), std::out_of_range);
    REQUIRE_THROWS_AS(sv.at(-1), std::out_of_range);
#endif
}

TEST_CASE("string_span front", "[string_span]")
{
    memepp::string_span sv("xyz");
    REQUIRE(sv.front() == 'x');
}

TEST_CASE("string_span back", "[string_span]")
{
    memepp::string_span sv("xyz");
    REQUIRE(sv.back() == 'z');
}

TEST_CASE("string_span data", "[string_span]")
{
    memepp::string_span sv("foo");
    REQUIRE(std::strncmp(sv.data(), "foo", 3) == 0);
}

TEST_CASE("string_span bytes", "[string_span]")
{
    memepp::string_span sv("bar");
    REQUIRE(std::memcmp(sv.bytes(), "bar", 3) == 0);
}

// ============================================================================
//  capacity
// ============================================================================

TEST_CASE("string_span size", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.size() == 5);
}

TEST_CASE("string_span length", "[string_span]")
{
    memepp::string_span sv("test");
    REQUIRE(sv.length() == 4u);
}

TEST_CASE("string_span empty", "[string_span]")
{
    REQUIRE(memepp::string_span().empty());
    REQUIRE_FALSE(memepp::string_span("x").empty());
}

// ============================================================================
//  modifiers
// ============================================================================

TEST_CASE("string_span remove_prefix", "[string_span]")
{
    memepp::string_span sv("hello");
    sv.remove_prefix(2);
    REQUIRE(sv.size() == 3);
    REQUIRE(sv == "llo");
}

TEST_CASE("string_span remove_prefix zero", "[string_span]")
{
    memepp::string_span sv("hello");
    sv.remove_prefix(0);
    REQUIRE(sv == "hello");
}

TEST_CASE("string_span remove_suffix", "[string_span]")
{
    memepp::string_span sv("hello");
    sv.remove_suffix(2);
    REQUIRE(sv.size() == 3);
    REQUIRE(sv == "hel");
}

TEST_CASE("string_span remove_suffix zero", "[string_span]")
{
    memepp::string_span sv("hello");
    sv.remove_suffix(0);
    REQUIRE(sv == "hello");
}

TEST_CASE("string_span swap", "[string_span]")
{
    memepp::string_span a("AAA");
    memepp::string_span b("BBB");
    a.swap(b);
    REQUIRE(a == "BBB");
    REQUIRE(b == "AAA");
}

TEST_CASE("string_span swap self", "[string_span]")
{
    memepp::string_span a("test");
    a.swap(a);
    REQUIRE(a == "test");
}

// ============================================================================
//  iterators
// ============================================================================

TEST_CASE("string_span begin / end", "[string_span]")
{
    memepp::string_span sv("abc");
    REQUIRE(*sv.begin() == 'a');
    REQUIRE(*(sv.end() - 1) == 'c');
    REQUIRE(sv.end() - sv.begin() == 3);
}

TEST_CASE("string_span cbegin / cend", "[string_span]")
{
    memepp::string_span sv("xyz");
    REQUIRE(*sv.cbegin() == 'x');
    REQUIRE(*(sv.cend() - 1) == 'z');
}

TEST_CASE("string_span rbegin / rend", "[string_span]")
{
    memepp::string_span sv("abc");
    REQUIRE(*sv.rbegin() == 'c');
    REQUIRE(*(sv.rend() - 1) == 'a');
}

TEST_CASE("string_span crbegin / crend", "[string_span]")
{
    memepp::string_span sv("xyz");
    REQUIRE(*sv.crbegin() == 'z');
    REQUIRE(*(sv.crend() - 1) == 'x');
}

TEST_CASE("string_span rune_begin / rune_end ASCII", "[string_span]")
{
    memepp::string_span sv("hello");
    auto it = sv.rune_begin();
    auto end = sv.rune_end();
    REQUIRE(it != end);
    int count = 0;
    for (; it != end; ++it) ++count;
    REQUIRE(count == 5);
}

TEST_CASE("string_span rune_cbegin / rune_cend", "[string_span]")
{
    memepp::string_span sv("abc");
    REQUIRE(sv.rune_cbegin() != sv.rune_cend());
}

TEST_CASE("string_span to_rune_iterator at pos", "[string_span]")
{
    memepp::string_span sv("abcdef");
    auto it = sv.to_rune_iterator(3);
    REQUIRE(it != sv.rune_end());
}

TEST_CASE("string_span to_rune_iterator past end", "[string_span]")
{
    memepp::string_span sv("abc");
    auto it = sv.to_rune_iterator(10);
    REQUIRE(it == sv.rune_end());
}

TEST_CASE("string_span to_rune_iterator negative", "[string_span]")
{
    memepp::string_span sv("abc");
    auto it = sv.to_rune_iterator(-1);
    REQUIRE(it == sv.rune_end());
}

// ============================================================================
//  string operations — rune / u16
// ============================================================================

TEST_CASE("string_span rune_size ASCII", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.rune_size() == 5);
}

TEST_CASE("string_span rune_size multi-byte", "[string_span]")
{
    memepp::string_span sv(u8"\u4f60\u597d");  // 2 code points
    REQUIRE(sv.rune_size() == 2);
}

TEST_CASE("string_span u16char_size", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.u16char_size() == 5);
}

TEST_CASE("string_span rune_front ASCII", "[string_span]")
{
    memepp::string_span sv("abc");
    auto r = sv.rune_front();
    REQUIRE(r.size() == 1);
    REQUIRE(r.data()[0] == 'a');
}

TEST_CASE("string_span rune_back ASCII", "[string_span]")
{
    memepp::string_span sv("abc");
    auto r = sv.rune_back();
    REQUIRE(r.size() == 1);
    REQUIRE(r.data()[0] == 'c');
}

// ============================================================================
//  substr
// ============================================================================

TEST_CASE("string_span substr basic", "[string_span]")
{
    memepp::string_span sv("hello world");
    auto sub = sv.substr(0, 5);
    REQUIRE(sub == "hello");
}

TEST_CASE("string_span substr default count", "[string_span]")
{
    memepp::string_span sv("hello world");
    auto sub = sv.substr(6);
    REQUIRE(sub == "world");
}

TEST_CASE("string_span substr pos past end", "[string_span]")
{
    memepp::string_span sv("abc");
    auto sub = sv.substr(10);
    REQUIRE(sub.empty());
}

TEST_CASE("string_span substr count exceeds", "[string_span]")
{
    memepp::string_span sv("hello");
    auto sub = sv.substr(3, 100);
    REQUIRE(sub == "lo");
}

// ============================================================================
//  copy
// ============================================================================

TEST_CASE("string_span copy basic", "[string_span]")
{
    memepp::string_span sv("hello");
    MemeByte_t buf[16] = {};
    auto n = sv.copy(buf, 5);
    REQUIRE(n == 5);
    REQUIRE(std::memcmp(buf, "hello", 5) == 0);
}

TEST_CASE("string_span copy with pos", "[string_span]")
{
    memepp::string_span sv("hello");
    MemeByte_t buf[16] = {};
    auto n = sv.copy(buf, 3, 1);
    REQUIRE(n == 3);
    REQUIRE(std::memcmp(buf, "ell", 3) == 0);
}

TEST_CASE("string_span copy count larger than remaining", "[string_span]")
{
    memepp::string_span sv("hello");
    MemeByte_t buf[16] = {};
    auto n = sv.copy(buf, 100, 3);
    REQUIRE(n == 2);
    REQUIRE(std::memcmp(buf, "lo", 2) == 0);
}

TEST_CASE("string_span copy zero count", "[string_span]")
{
    memepp::string_span sv("hello");
    MemeByte_t buf[16] = {};
    auto n = sv.copy(buf, 0);
    REQUIRE(n == 0);
}

TEST_CASE("string_span copy pos out of range", "[string_span]")
{
    memepp::string_span sv("abc");
    MemeByte_t buf[16] = {};
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(sv.copy(buf, 1, 10), std::out_of_range);
#endif
}

// ============================================================================
//  compare — all 6 overloads
// ============================================================================

TEST_CASE("string_span compare string_span", "[string_span]")
{
    memepp::string_span a("hello");
    REQUIRE(a.compare(memepp::string_span("hello")) == 0);
    REQUIRE(a.compare(memepp::string_span("hell")) > 0);
    REQUIRE(a.compare(memepp::string_span("helloo")) < 0);
}

TEST_CASE("string_span compare pos+count vs string_span", "[string_span]")
{
    memepp::string_span a("hello");
    REQUIRE(a.compare(1, 2, memepp::string_span("el")) == 0);
}

TEST_CASE("string_span compare pos+count vs pos+count", "[string_span]")
{
    memepp::string_span a("abcde");
    memepp::string_span b("xxcdexx");
    REQUIRE(a.compare(2, 3, b, 2, 3) == 0);
}

TEST_CASE("string_span compare const char*", "[string_span]")
{
    memepp::string_span a("hello");
    REQUIRE(a.compare("hello") == 0);
    REQUIRE(a.compare("hell") > 0);
}

TEST_CASE("string_span compare pos+count vs const char*", "[string_span]")
{
    memepp::string_span a("hello");
    REQUIRE(a.compare(0, 2, "he") == 0);
}

TEST_CASE("string_span compare pos+count vs const char*+count", "[string_span]")
{
    memepp::string_span a("hello");
    REQUIRE(a.compare(0, 5, "helloXXX", 5) == 0);
}

// ============================================================================
//  starts_with — all 4 overloads
// ============================================================================

TEST_CASE("string_span starts_with string_span", "[string_span]")
{
    memepp::string_span sv("hello world");
    REQUIRE(sv.starts_with(memepp::string_span("hello")));
    REQUIRE(sv.starts_with(memepp::string_span("")));
    REQUIRE_FALSE(sv.starts_with(memepp::string_span("world")));
    REQUIRE_FALSE(sv.starts_with(memepp::string_span("hello world!")));
}

TEST_CASE("string_span starts_with char", "[string_span]")
{
    memepp::string_span sv("abc");
    REQUIRE(sv.starts_with('a'));
    REQUIRE_FALSE(sv.starts_with('b'));
    REQUIRE_FALSE(memepp::string_span().starts_with('a'));
}

TEST_CASE("string_span starts_with const char*", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.starts_with("he"));
    REQUIRE(sv.starts_with(""));
    REQUIRE_FALSE(sv.starts_with("lo"));
    REQUIRE_FALSE(sv.starts_with("hello!"));
}

TEST_CASE("string_span starts_with const_pointer", "[string_span]")
{
    memepp::string_span sv("hello");
    const MemeByte_t* p = reinterpret_cast<const MemeByte_t*>("hel");
    REQUIRE(sv.starts_with(p));
    REQUIRE_FALSE(sv.starts_with(reinterpret_cast<const MemeByte_t*>("zzz")));
}

// ============================================================================
//  ends_with — all 4 overloads
// ============================================================================

TEST_CASE("string_span ends_with string_span", "[string_span]")
{
    memepp::string_span sv("hello world");
    REQUIRE(sv.ends_with(memepp::string_span("world")));
    REQUIRE(sv.ends_with(memepp::string_span("")));
    REQUIRE_FALSE(sv.ends_with(memepp::string_span("hello")));
    REQUIRE_FALSE(sv.ends_with(memepp::string_span("xhello world")));
}

TEST_CASE("string_span ends_with char", "[string_span]")
{
    memepp::string_span sv("abc");
    REQUIRE(sv.ends_with('c'));
    REQUIRE_FALSE(sv.ends_with('a'));
    REQUIRE_FALSE(memepp::string_span().ends_with('x'));
}

TEST_CASE("string_span ends_with const char*", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.ends_with("lo"));
    REQUIRE(sv.ends_with(""));
    REQUIRE_FALSE(sv.ends_with("he"));
}

TEST_CASE("string_span ends_with const_pointer", "[string_span]")
{
    memepp::string_span sv("hello");
    const MemeByte_t* p = reinterpret_cast<const MemeByte_t*>("lo");
    REQUIRE(sv.ends_with(p));
}

// ============================================================================
//  contains — all 4 overloads
// ============================================================================

TEST_CASE("string_span contains string_span", "[string_span]")
{
    memepp::string_span sv("hello world");
    REQUIRE(sv.contains(memepp::string_span("lo w")));
    REQUIRE(sv.contains(memepp::string_span("")));
    REQUIRE_FALSE(sv.contains(memepp::string_span("xyz")));
}

TEST_CASE("string_span contains char", "[string_span]")
{
    memepp::string_span sv("abc");
    REQUIRE(sv.contains('b'));
    REQUIRE_FALSE(sv.contains('z'));
}

TEST_CASE("string_span contains const char*", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.contains("ell"));
    REQUIRE_FALSE(sv.contains("zz"));
}

TEST_CASE("string_span contains const_pointer", "[string_span]")
{
    memepp::string_span sv("hello");
    const MemeByte_t* p = reinterpret_cast<const MemeByte_t*>("ell");
    REQUIRE(sv.contains(p));
}

// ============================================================================
//  find — all 6 overloads
// ============================================================================

TEST_CASE("string_span find string_span", "[string_span]")
{
    memepp::string_span sv("hello world");
    REQUIRE(sv.find(memepp::string_span("world")) == 6);
    REQUIRE(sv.find(memepp::string_span("hello")) == 0);
    REQUIRE(sv.find(memepp::string_span("")) == 0);
    REQUIRE(sv.find(memepp::string_span("xyz")) == memepp::string_span::npos);
}

TEST_CASE("string_span find string_span with pos", "[string_span]")
{
    memepp::string_span sv("aaa bbb aaa");
    REQUIRE(sv.find(memepp::string_span("aaa"), 4) == 8);
    REQUIRE(sv.find(memepp::string_span("aaa"), 9) == memepp::string_span::npos);
}

TEST_CASE("string_span find char", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.find('e') == 1);
    REQUIRE(sv.find('o') == 4);
    REQUIRE(sv.find('z') == memepp::string_span::npos);
}

TEST_CASE("string_span find char with pos", "[string_span]")
{
    memepp::string_span sv("abca");
    REQUIRE(sv.find('a', 1) == 3);
}

TEST_CASE("string_span find const char* with pos and count", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.find("ellXX", 0, 3) == 1);
}

TEST_CASE("string_span find const char*", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.find("lo") == 3);
    REQUIRE(sv.find("") == 0);
    REQUIRE(sv.find("xyz") == memepp::string_span::npos);
}

TEST_CASE("string_span find const_pointer with pos and count", "[string_span]")
{
    memepp::string_span sv("hello");
    const MemeByte_t* p = reinterpret_cast<const MemeByte_t*>("ellXX");
    REQUIRE(sv.find(p, 0, 3) == 1);
}

TEST_CASE("string_span find const_pointer", "[string_span]")
{
    memepp::string_span sv("hello");
    const MemeByte_t* p = reinterpret_cast<const MemeByte_t*>("lo");
    REQUIRE(sv.find(p) == 3);
}

TEST_CASE("string_span find empty in empty", "[string_span]")
{
    memepp::string_span sv;
    REQUIRE(sv.find(memepp::string_span("")) == 0);
    REQUIRE(sv.find(memepp::string_span("x")) == memepp::string_span::npos);
}

// ============================================================================
//  rfind — all 6 overloads
// ============================================================================

TEST_CASE("string_span rfind string_span", "[string_span]")
{
    memepp::string_span sv("hello hello");
    REQUIRE(sv.rfind(memepp::string_span("hello")) == 6);
    REQUIRE(sv.rfind(memepp::string_span("")) == 11);
    REQUIRE(sv.rfind(memepp::string_span("xyz")) == memepp::string_span::npos);
}

TEST_CASE("string_span rfind string_span with pos", "[string_span]")
{
    memepp::string_span sv("hello hello");
    REQUIRE(sv.rfind(memepp::string_span("hello"), 5) == 0);
}

TEST_CASE("string_span rfind char", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.rfind('l') == 3);
    REQUIRE(sv.rfind('h') == 0);
    REQUIRE(sv.rfind('z') == memepp::string_span::npos);
}

TEST_CASE("string_span rfind char with pos", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.rfind('l', 2) == 2);
}

TEST_CASE("string_span rfind const char* with pos and count", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.rfind("ellXX", 5, 3) == 1);
}

TEST_CASE("string_span rfind const char*", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.rfind("el") == 1);
    REQUIRE(sv.rfind("") == 5);
}

TEST_CASE("string_span rfind const_pointer with pos and count", "[string_span]")
{
    memepp::string_span sv("hello");
    const MemeByte_t* p = reinterpret_cast<const MemeByte_t*>("ellXX");
    REQUIRE(sv.rfind(p, 5, 3) == 1);
}

TEST_CASE("string_span rfind const_pointer", "[string_span]")
{
    memepp::string_span sv("hello");
    const MemeByte_t* p = reinterpret_cast<const MemeByte_t*>("el");
    REQUIRE(sv.rfind(p) == 1);
}

TEST_CASE("string_span rfind empty string", "[string_span]")
{
    memepp::string_span sv;
    REQUIRE(sv.rfind('x') == memepp::string_span::npos);
    REQUIRE(sv.rfind(memepp::string_span("")) == 0);
}

// ============================================================================
//  find_first_of — all 4 overloads
// ============================================================================

TEST_CASE("string_span find_first_of string_span", "[string_span]")
{
    memepp::string_span sv("hello world");
    REQUIRE(sv.find_first_of(memepp::string_span("aeiou")) == 1);
    REQUIRE(sv.find_first_of(memepp::string_span("")) == memepp::string_span::npos);
}

TEST_CASE("string_span find_first_of string_span with pos", "[string_span]")
{
    memepp::string_span sv("abc abc");
    REQUIRE(sv.find_first_of(memepp::string_span("bc"), 2) == 2);
}

TEST_CASE("string_span find_first_of char", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.find_first_of('e') == 1);
    REQUIRE(sv.find_first_of('z') == memepp::string_span::npos);
}

TEST_CASE("string_span find_first_of char with pos", "[string_span]")
{
    memepp::string_span sv("abcabc");
    REQUIRE(sv.find_first_of('b', 2) == 4);
}

TEST_CASE("string_span find_first_of const char* with pos and count", "[string_span]")
{
    memepp::string_span sv("abcdef");
    REQUIRE(sv.find_first_of("zzyx", 0, 2) == memepp::string_span::npos);
}

TEST_CASE("string_span find_first_of const char*", "[string_span]")
{
    memepp::string_span sv("hello world");
    REQUIRE(sv.find_first_of("aeiou") == 1);
}

// ============================================================================
//  find_first_not_of — all 4 overloads
// ============================================================================

TEST_CASE("string_span find_first_not_of string_span", "[string_span]")
{
    memepp::string_span sv("aaabbbccc");
    REQUIRE(sv.find_first_not_of(memepp::string_span("a")) == 3);
    REQUIRE(sv.find_first_not_of(memepp::string_span("abc")) == memepp::string_span::npos);
}

TEST_CASE("string_span find_first_not_of string_span with pos", "[string_span]")
{
    memepp::string_span sv("aaabbb");
    REQUIRE(sv.find_first_not_of(memepp::string_span("a"), 3) == 3);
}

TEST_CASE("string_span find_first_not_of char", "[string_span]")
{
    memepp::string_span sv("aaaaab");
    REQUIRE(sv.find_first_not_of('a') == 5);
    REQUIRE(sv.find_first_not_of('a', 6) == memepp::string_span::npos);
}

TEST_CASE("string_span find_first_not_of const char* with pos and count", "[string_span]")
{
    memepp::string_span sv("abcdef");
    REQUIRE(sv.find_first_not_of("abx", 0, 2) == 2);
}

TEST_CASE("string_span find_first_not_of const char*", "[string_span]")
{
    memepp::string_span sv("aaabc");
    REQUIRE(sv.find_first_not_of("a") == 3);
}

// ============================================================================
//  find_last_of — all 4 overloads
// ============================================================================

TEST_CASE("string_span find_last_of string_span", "[string_span]")
{
    memepp::string_span sv("hello world");
    REQUIRE(sv.find_last_of(memepp::string_span("aeiou")) == 7);
    REQUIRE(sv.find_last_of(memepp::string_span("")) == memepp::string_span::npos);
}

TEST_CASE("string_span find_last_of char", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.find_last_of('l') == 3);
    REQUIRE(sv.find_last_of('z') == memepp::string_span::npos);
}

TEST_CASE("string_span find_last_of char with pos", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.find_last_of('l', 2) == 2);
}

TEST_CASE("string_span find_last_of const char* with pos and count", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.find_last_of("lz", 4, 2) == 3);
}

TEST_CASE("string_span find_last_of const char*", "[string_span]")
{
    memepp::string_span sv("hello world");
    REQUIRE(sv.find_last_of("aeiou") == 7);
}

// ============================================================================
//  find_last_not_of — all 4 overloads
// ============================================================================

TEST_CASE("string_span find_last_not_of string_span", "[string_span]")
{
    memepp::string_span sv("aaabbbc");
    REQUIRE(sv.find_last_not_of(memepp::string_span("c")) == 5);
    REQUIRE(sv.find_last_not_of(memepp::string_span("abc")) == memepp::string_span::npos);
}

TEST_CASE("string_span find_last_not_of char", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.find_last_not_of('o') == 3);
    REQUIRE(sv.find_last_not_of('h', 0) == memepp::string_span::npos);
}

TEST_CASE("string_span find_last_not_of char with pos", "[string_span]")
{
    memepp::string_span sv("abb");
    REQUIRE(sv.find_last_not_of('b', 2) == 0);
}

TEST_CASE("string_span find_last_not_of const char* with pos and count", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE(sv.find_last_not_of("oz", 4, 2) == 3);
}

TEST_CASE("string_span find_last_not_of const char*", "[string_span]")
{
    memepp::string_span sv("aaabbbc");
    REQUIRE(sv.find_last_not_of("c") == 5);
}

TEST_CASE("string_span find_last_not_of empty", "[string_span]")
{
    memepp::string_span sv;
    REQUIRE(sv.find_last_not_of('x') == memepp::string_span::npos);
    REQUIRE(sv.find_last_not_of(memepp::string_span("")) == memepp::string_span::npos);
}

// ============================================================================
//  to_string
// ============================================================================

TEST_CASE("string_span to_string", "[string_span]")
{
    memepp::string_span sv("hello");
    auto s = sv.to_string();
    REQUIRE(s.size() == 5);
    REQUIRE(s == "hello");
}

TEST_CASE("string_span to_string empty", "[string_span]")
{
    memepp::string_span sv;
    auto s = sv.to_string();
    REQUIRE(s.empty());
}

// ============================================================================
//  comparison operators — string_span vs string_span
// ============================================================================

TEST_CASE("string_span operator==  string_span", "[string_span]")
{
    memepp::string_span a("abc"), b("abc"), c("abd");
    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
}

TEST_CASE("string_span operator!=  string_span", "[string_span]")
{
    memepp::string_span a("abc"), b("abd");
    REQUIRE(a != b);
    REQUIRE_FALSE(a != memepp::string_span("abc"));
}

TEST_CASE("string_span operator<   string_span", "[string_span]")
{
    REQUIRE(memepp::string_span("abc") < memepp::string_span("abd"));
    REQUIRE_FALSE(memepp::string_span("abd") < memepp::string_span("abc"));
}

TEST_CASE("string_span operator>   string_span", "[string_span]")
{
    REQUIRE(memepp::string_span("abd") > memepp::string_span("abc"));
}

TEST_CASE("string_span operator<=  string_span", "[string_span]")
{
    memepp::string_span a("abc"), b("abc"), c("abd");
    REQUIRE(a <= b);
    REQUIRE(a <= c);
    REQUIRE_FALSE(c <= a);
}

TEST_CASE("string_span operator>=  string_span", "[string_span]")
{
    memepp::string_span a("abc"), b("abc"), c("abb");
    REQUIRE(a >= b);
    REQUIRE(a >= c);
    REQUIRE_FALSE(c >= a);
}

// ============================================================================
//  comparison operators — string vs string_span
// ============================================================================

TEST_CASE("string_span operator==  string vs string_span", "[string_span]")
{
    memepp::string s("hello");
    memepp::string_span sv("hello");
    REQUIRE(s == sv);
    REQUIRE(sv == s);
}

TEST_CASE("string_span operator!=  string vs string_span", "[string_span]")
{
    REQUIRE(memepp::string("a") != memepp::string_span("b"));
    REQUIRE(memepp::string_span("a") != memepp::string("b"));
}

TEST_CASE("string_span operator<   string vs string_span", "[string_span]")
{
    REQUIRE(memepp::string("a") < memepp::string_span("b"));
    REQUIRE(memepp::string_span("a") < memepp::string("b"));
}

TEST_CASE("string_span operator>   string vs string_span", "[string_span]")
{
    REQUIRE(memepp::string("b") > memepp::string_span("a"));
    REQUIRE(memepp::string_span("b") > memepp::string("a"));
}

TEST_CASE("string_span operator<=  string vs string_span", "[string_span]")
{
    REQUIRE(memepp::string("a") <= memepp::string_span("a"));
    REQUIRE(memepp::string_span("a") <= memepp::string("b"));
}

TEST_CASE("string_span operator>=  string vs string_span", "[string_span]")
{
    REQUIRE(memepp::string("b") >= memepp::string_span("a"));
    REQUIRE(memepp::string_span("b") >= memepp::string("b"));
}

// ============================================================================
//  comparison operators — const char* vs string_span
// ============================================================================

TEST_CASE("string_span operator==  const char* vs string_span", "[string_span]")
{
    memepp::string_span sv("hello");
    REQUIRE("hello" == sv);
    REQUIRE(sv == "hello");
}

TEST_CASE("string_span operator!=  const char* vs string_span", "[string_span]")
{
    REQUIRE("a" != memepp::string_span("b"));
    REQUIRE(memepp::string_span("a") != "b");
}

TEST_CASE("string_span operator<   const char* vs string_span", "[string_span]")
{
    REQUIRE("a" < memepp::string_span("b"));
    REQUIRE(memepp::string_span("a") < "b");
}

TEST_CASE("string_span operator>   const char* vs string_span", "[string_span]")
{
    REQUIRE("b" > memepp::string_span("a"));
    REQUIRE(memepp::string_span("b") > "a");
}

TEST_CASE("string_span operator<=  const char* vs string_span", "[string_span]")
{
    REQUIRE("a" <= memepp::string_span("a"));
    REQUIRE(memepp::string_span("a") <= "b");
}

TEST_CASE("string_span operator>=  const char* vs string_span", "[string_span]")
{
    REQUIRE("b" >= memepp::string_span("a"));
    REQUIRE(memepp::string_span("b") >= "b");
}

// ============================================================================
//  comparison operators — const_pointer vs string_span
// ============================================================================

TEST_CASE("string_span operator==  const_pointer vs string_span", "[string_span]")
{
    const MemeByte_t* p = reinterpret_cast<const MemeByte_t*>("hello");
    memepp::string_span sv("hello");
    REQUIRE(p == sv);
    REQUIRE(sv == p);
}

TEST_CASE("string_span operator!=  const_pointer vs string_span", "[string_span]")
{
    const MemeByte_t* p = reinterpret_cast<const MemeByte_t*>("a");
    const MemeByte_t* q = reinterpret_cast<const MemeByte_t*>("b");
    REQUIRE(memepp::string_span(p) != memepp::string_span(q));
    REQUIRE(memepp::string_span(q) != memepp::string_span(p));
}

TEST_CASE("string_span operator<   const_pointer vs string_span", "[string_span]")
{
    const MemeByte_t* pa = reinterpret_cast<const MemeByte_t*>("a");
    const MemeByte_t* pb = reinterpret_cast<const MemeByte_t*>("b");
    REQUIRE(pa < memepp::string_span("b"));
    REQUIRE(memepp::string_span("a") < pb);
}

TEST_CASE("string_span operator>   const_pointer vs string_span", "[string_span]")
{
    const MemeByte_t* pa = reinterpret_cast<const MemeByte_t*>("a");
    const MemeByte_t* pb = reinterpret_cast<const MemeByte_t*>("b");
    REQUIRE(pb > memepp::string_span("a"));
    REQUIRE(memepp::string_span("b") > pa);
}

TEST_CASE("string_span operator<=  const_pointer vs string_span", "[string_span]")
{
    const MemeByte_t* pa = reinterpret_cast<const MemeByte_t*>("a");
    const MemeByte_t* pb = reinterpret_cast<const MemeByte_t*>("b");
    REQUIRE(pa <= memepp::string_span("a"));
    REQUIRE(pa <= memepp::string_span("b"));
    REQUIRE(memepp::string_span("a") <= pb);
}

TEST_CASE("string_span operator>=  const_pointer vs string_span", "[string_span]")
{
    const MemeByte_t* pa = reinterpret_cast<const MemeByte_t*>("a");
    const MemeByte_t* pb = reinterpret_cast<const MemeByte_t*>("b");
    REQUIRE(pb >= memepp::string_span("a"));
    REQUIRE(pb >= memepp::string_span("b"));
    REQUIRE(memepp::string_span("b") >= pa);
}

// ============================================================================
//  free functions
// ============================================================================

TEST_CASE("string_span mm_span const char*", "[string_span]")
{
    auto sv = mm_span("hello world", 5);
    REQUIRE(sv.size() == 5);
    REQUIRE(sv == "hello");
}

TEST_CASE("string_span mm_span const_pointer", "[string_span]")
{
    const MemeByte_t bytes[] = {'a', 'b', 'c'};
    auto sv = mm_span(bytes, 3);
    REQUIRE(sv.size() == 3);
    REQUIRE(sv == "abc");
}

TEST_CASE("string_span user-defined literal", "[string_span]")
{
    auto sv = "hello"_meme_span;
    REQUIRE(sv.size() == 5);
    REQUIRE(sv == "hello");
}

TEST_CASE("string_span user-defined literal empty", "[string_span]")
{
    auto sv = ""_meme_span;
    REQUIRE(sv.empty());
}

// ============================================================================
//  to_shared_storage
// ============================================================================

TEST_CASE("string_span to_shared_storage", "[string_span]")
{
    memepp::string_span sv("hello", 5);
    auto s = sv.to_shared_storage();
    REQUIRE(s.size() == 5);
    REQUIRE(std::strncmp(s.data(), "hello", 5) == 0);
}
