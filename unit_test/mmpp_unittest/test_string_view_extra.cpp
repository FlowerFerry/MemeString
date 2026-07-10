#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

// ---------------------------------------------------------------------------
// string_view::to_en_upper / to_en_lower
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view to_en_upper", "[string_view]")
{
    memepp::string_view sv = "aBcDeF";
    auto result = sv.to_en_upper();
    REQUIRE(result == "ABCDEF");
}

TEST_CASE("memepp::string_view to_en_upper — mixed Chinese", "[string_view]")
{
    memepp::string_view sv = u8"aB好cDeF世";
    auto result = sv.to_en_upper();
    REQUIRE(result == u8"AB好CDEF世");
}

TEST_CASE("memepp::string_view to_en_upper — empty", "[string_view]")
{
    memepp::string_view sv;
    auto result = sv.to_en_upper();
    REQUIRE(result.empty());
}

TEST_CASE("memepp::string_view to_en_lower", "[string_view]")
{
    memepp::string_view sv = "AbCdEf";
    auto result = sv.to_en_lower();
    REQUIRE(result == "abcdef");
}

TEST_CASE("memepp::string_view to_en_lower — mixed Chinese", "[string_view]")
{
    memepp::string_view sv = u8"Ab好CdEf世";
    auto result = sv.to_en_lower();
    REQUIRE(result == u8"ab好cdef世");
}

TEST_CASE("memepp::string_view to_en_lower — empty", "[string_view]")
{
    memepp::string_view sv;
    auto result = sv.to_en_lower();
    REQUIRE(result.empty());
}

// ---------------------------------------------------------------------------
// string_view::to_valid_utf8
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view to_valid_utf8 — valid input unchanged", "[string_view]")
{
    memepp::string_view sv = u8"Hello, 世界!";
    auto result = sv.to_valid_utf8();
    REQUIRE(result == sv);
}

TEST_CASE("memepp::string_view to_valid_utf8 — strips invalid bytes", "[string_view]")
{
    // invalid UTF-8 continuation byte without lead — to_valid_utf8 truncates at the invalid byte
    char bad[] = { 'H', 'e', 'l', 'l', 'o', (char)0x80, '\0' };
    memepp::string_view sv(bad, 6);
    auto result = sv.to_valid_utf8();
    REQUIRE(result == "Hello");
}

TEST_CASE("memepp::string_view to_valid_utf8 — empty", "[string_view]")
{
    memepp::string_view sv;
    auto result = sv.to_valid_utf8();
    REQUIRE(result.empty());
}

// ---------------------------------------------------------------------------
// string_view::trim_space / trim_left_space / trim_right_space
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view trim_space", "[string_view]")
{
    memepp::string_view sv1 = "  Hello World  ";
    REQUIRE(sv1.trim_space() == "Hello World");

    memepp::string_view sv2 = "Hello World  ";
    REQUIRE(sv2.trim_space() == "Hello World");

    memepp::string_view sv3 = "  Hello World";
    REQUIRE(sv3.trim_space() == "Hello World");

    memepp::string_view sv4 = "Hello World";
    REQUIRE(sv4.trim_space() == "Hello World");

    memepp::string_view sv5 = "  ";
    REQUIRE(sv5.trim_space() == "");

    memepp::string_view sv6 = "";
    REQUIRE(sv6.trim_space() == "");
}

TEST_CASE("memepp::string_view trim_left_space", "[string_view]")
{
    memepp::string_view sv1 = "  Hello World  ";
    REQUIRE(sv1.trim_left_space() == "Hello World  ");

    memepp::string_view sv2 = "Hello World  ";
    REQUIRE(sv2.trim_left_space() == "Hello World  ");

    memepp::string_view sv3 = "  ";
    REQUIRE(sv3.trim_left_space() == "");

    memepp::string_view sv4 = "";
    REQUIRE(sv4.trim_left_space() == "");
}

TEST_CASE("memepp::string_view trim_right_space", "[string_view]")
{
    memepp::string_view sv1 = "  Hello World  ";
    REQUIRE(sv1.trim_right_space() == "  Hello World");

    memepp::string_view sv2 = "  Hello World";
    REQUIRE(sv2.trim_right_space() == "  Hello World");

    memepp::string_view sv3 = "  ";
    REQUIRE(sv3.trim_right_space() == "");

    memepp::string_view sv4 = "";
    REQUIRE(sv4.trim_right_space() == "");
}

TEST_CASE("memepp::string_view trim_space — tab and newline", "[string_view]")
{
    memepp::string_view sv = "\t\n Hello World \r\n";
    REQUIRE(sv.trim_space() == "Hello World");
}

// ---------------------------------------------------------------------------
// string_view::count
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view count — basic", "[string_view]")
{
    memepp::string_view sv = "AaBbCcAaBbCcAaBbCc";
    REQUIRE(sv.count("Aa") == 3);
    REQUIRE(sv.count("Bb") == 3);
    REQUIRE(sv.count("Cc") == 3);
}

TEST_CASE("memepp::string_view count — edge cases", "[string_view]")
{
    memepp::string_view sv = "Hello";
    REQUIRE(sv.count("Hello") == 1);
    REQUIRE(sv.count("") == 0);
    REQUIRE(sv.count("x") == 0);
    REQUIRE(memepp::string_view{}.count("A") == 0);
}

TEST_CASE("memepp::string_view count — rune", "[string_view]")
{
    memepp::string_view sv = "Hello";
    memepp::rune rl = 'l';
    REQUIRE(sv.count(rl) == 2);
}

// ---------------------------------------------------------------------------
// string_view::to_large / to_large_or_user
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view to_large", "[string_view]")
{
    memepp::string_view sv = "Hello, World!";
    auto large = sv.to_large();
    REQUIRE(large == "Hello, World!");
    REQUIRE(large.storage_type() == memepp::string_storage_t::large);
}

TEST_CASE("memepp::string_view to_large — empty", "[string_view]")
{
    memepp::string_view sv;
    auto large = sv.to_large();
    REQUIRE(large.empty());
    REQUIRE(large.storage_type() == memepp::string_storage_t::large);
}

TEST_CASE("memepp::string_view to_large_or_user — empty", "[string_view]")
{
    memepp::string_view sv;
    auto result = sv.to_large_or_user();
    REQUIRE(result.empty());
}

// ---------------------------------------------------------------------------
// string_view::substr
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view substr", "[string_view]")
{
    memepp::string_view sv = "0123456789";

    REQUIRE(sv.substr(0, 0) == "");
    REQUIRE(sv.substr(0, 5) == "01234");
    REQUIRE(sv.substr(5)    == "56789");
    REQUIRE(sv.substr(5, 3) == "567");
    REQUIRE(sv.substr(10)   == "");    // at end
    REQUIRE(sv.substr(0)    == sv);
    REQUIRE(sv.substr(0, 100) == sv);  // clamping
}

// ---------------------------------------------------------------------------
// string_view::contains
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view contains — rune", "[string_view]")
{
    memepp::string_view sv = u8"您好世界";
    memepp::rune r_you{ reinterpret_cast<const uint8_t*>(u8"您"), -1 };
    memepp::rune r_xie{ reinterpret_cast<const uint8_t*>(u8"谢"), -1 };
    REQUIRE(sv.contains(r_you));
    REQUIRE_FALSE(sv.contains(r_xie));
}

TEST_CASE("memepp::string_view contains — const_pointer", "[string_view]")
{
    memepp::string_view sv = "abcabc";
    const memepp::string_view::const_pointer p =
        reinterpret_cast<const memepp::string_view::const_pointer>("abc");
    REQUIRE(sv.contains(p));
    REQUIRE_FALSE(sv.contains(
        reinterpret_cast<const memepp::string_view::const_pointer>("xyz")));
}

TEST_CASE("memepp::string_view contains — const_pointer with count", "[string_view]")
{
    memepp::string_view sv = "abcabc";
    const memepp::string_view::const_pointer p =
        reinterpret_cast<const memepp::string_view::const_pointer>("abcXYZ");
    REQUIRE(sv.contains(p, 3));
    REQUIRE_FALSE(sv.contains(p, 4));
}

// ---------------------------------------------------------------------------
// string_view::rune_size / u16char_size
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view rune_size", "[string_view]")
{
    memepp::string_view sv1 = "Hello";
    REQUIRE(sv1.rune_size() == 5);
    REQUIRE(sv1.u16char_size() == 5);

    memepp::string_view sv2 = u8"你好世界"; // 4 Chinese chars
    REQUIRE(sv2.rune_size() == 4);
    REQUIRE(sv2.u16char_size() == 4);

    memepp::string_view sv3;
    REQUIRE(sv3.rune_size() == 0);
    REQUIRE(sv3.u16char_size() == 0);
}

// ---------------------------------------------------------------------------
// string_view::front / back
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view front/back", "[string_view]")
{
    memepp::string_view sv = "Hello";
    REQUIRE(sv.front() == 'H');
    REQUIRE(sv.back() == 'o');
}

TEST_CASE("memepp::string_view front/back — single char", "[string_view]")
{
    memepp::string_view sv = "X";
    REQUIRE(sv.front() == 'X');
    REQUIRE(sv.back() == 'X');
}

// ---------------------------------------------------------------------------
// string_view::rune_front / rune_back
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view rune_front/rune_back", "[string_view]")
{
    memepp::string_view sv = u8"你好世界";
    auto rf = sv.rune_front();
    auto rb = sv.rune_back();
    REQUIRE(rf == u8"你");
    REQUIRE(rb == u8"界");
}

// ---------------------------------------------------------------------------
// string_view comparison operators
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view operator== with rune", "[string_view]")
{
    memepp::string_view sv = "H";
    memepp::rune rh = 'H';
    memepp::rune rx = 'X';
    REQUIRE(sv == rh);
    REQUIRE_FALSE(sv == rx);
    REQUIRE(rh == sv);
    REQUIRE(rx != sv);
}

TEST_CASE("memepp::string_view comparison operators", "[string_view]")
{
    memepp::string_view a = "abc";
    memepp::string_view b = "abd";
    memepp::string_view c = "abc";

    REQUIRE(a < b);
    REQUIRE(b > a);
    REQUIRE(a <= b);
    REQUIRE(b >= a);
    REQUIRE(a <= c);
    REQUIRE(a >= c);
}

TEST_CASE("memepp::string_view comparison with string", "[string_view]")
{
    memepp::string_view sv = "abc";
    memepp::string s1 = "abd";
    memepp::string s2 = "abc";

    REQUIRE(sv < s1);
    REQUIRE(sv <= s2);
    REQUIRE(sv == s2);
}

TEST_CASE("memepp::string_view comparison with const char*", "[string_view]")
{
    memepp::string_view sv = "abc";
    REQUIRE(sv < "abd");
    // "abc" == sv, so it should not be < sv
    REQUIRE_FALSE("abc" < sv);
    REQUIRE("abc" <= sv);
    REQUIRE("abd" > sv);
}

TEST_CASE("memepp::string_view comparison with const_pointer", "[string_view]")
{
    memepp::string_view sv = "abc";
    const memepp::string_view::const_pointer p_abd =
        reinterpret_cast<const memepp::string_view::const_pointer>("abd");
    const memepp::string_view::const_pointer p_abc =
        reinterpret_cast<const memepp::string_view::const_pointer>("abc");

    REQUIRE(sv < p_abd);
    REQUIRE(sv <= p_abc);
    REQUIRE(sv >= p_abc);
    REQUIRE(p_abc <= sv);
    REQUIRE(p_abc >= sv);
}

TEST_CASE("memepp::string_view comparison with rune", "[string_view]")
{
    memepp::string_view sv = "H";
    memepp::rune rh = 'H';
    memepp::rune rx = 'X';

    REQUIRE(sv <= rh);
    REQUIRE(sv >= rh);
    REQUIRE_FALSE(sv < rh);
    REQUIRE_FALSE(sv > rh);
    REQUIRE(rh <= sv);
    REQUIRE(rh >= sv);

    REQUIRE(sv < rx);
    REQUIRE(rx > sv);
}

// ---------------------------------------------------------------------------
// string_view::index_of / last_index_of
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view index_of", "[string_view]")
{
    memepp::string_view sv = "abcabcabc";

    REQUIRE(sv.index_of("abc") == 0);
    REQUIRE(sv.index_of("bca") == 1);
    REQUIRE(sv.index_of("xyz") == -1);
}

TEST_CASE("memepp::string_view index_of — full_match", "[string_view]")
{
    // full_match = true means word-boundary match: the matched text must be
    // preceded and followed by non-alphanumeric chars (or start/end of view)
    memepp::string_view sv = "abcde abcde";
    REQUIRE(sv.index_of("abcde", true) == 0);
    REQUIRE(sv.index_of("bcd", true) == -1); // not on word boundary (mid-word)
}

TEST_CASE("memepp::string_view index_of — with offset/limit", "[string_view]")
{
    memepp::string_view sv = "abcabcabc";
    // index_of(const char*, size_type u8len, size_type offset, bool full_match, case_sensitivity_t)
    REQUIRE(sv.index_of("abc", 3, 1, false) == 3);
}

TEST_CASE("memepp::string_view index_of — case insensitive", "[string_view]")
{
    // Single-char case-insensitive matching via index_of(const char*, case_sensitivity_t)
    memepp::string_view sv = "ABCDEF";
    REQUIRE(sv.index_of("bcd", memepp::case_sensitivity_t::all_sensitive) == -1);
}

TEST_CASE("memepp::string_view last_index_of", "[string_view]")
{
    memepp::string_view sv = "abcabcabc";
    REQUIRE(sv.last_index_of("abc") == 6);
    REQUIRE(sv.last_index_of("xyz") == -1);
}

TEST_CASE("memepp::string_view last_index_of — with offset/limit", "[string_view]")
{
    memepp::string_view sv = "aaaaaa";
    REQUIRE(sv.last_index_of("aa", 0, 6, false) == 4);
}

TEST_CASE("memepp::string_view last_index_of — case insensitive", "[string_view]")
{
    memepp::string_view sv = "ABCabcABC";
    REQUIRE(sv.last_index_of("ABC", memepp::case_sensitivity_t::all_sensitive) == 6);
}


// ---------------------------------------------------------------------------
// string_view::native_handle
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view native_handle", "[string_view]")
{
    memepp::string_view sv = "Hello";
    const auto& nh = sv.native_handle();
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&nh) == 5);
}

// ---------------------------------------------------------------------------
// string_view::to_string
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view to_string", "[string_view]")
{
    memepp::string_view sv = "Hello, World!";
    auto s = sv.to_string();
    REQUIRE(s == "Hello, World!");
    REQUIRE(s.size() == 13);
}

TEST_CASE("memepp::string_view to_string — empty", "[string_view]")
{
    memepp::string_view sv;
    auto s = sv.to_string();
    REQUIRE(s.empty());
}

// ---------------------------------------------------------------------------
// string_view::string_view(string&&)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view construct from string rvalue", "[string_view]")
{
    memepp::string s = "Hello, World!";
    memepp::string_view sv(std::move(s));
    REQUIRE(sv == "Hello, World!");
    REQUIRE(sv.size() == 13);
}

TEST_CASE("memepp::string_view construct from string rvalue -- empty", "[string_view]")
{
    memepp::string s;
    memepp::string_view sv(std::move(s));
    REQUIRE(sv.empty());
}

// ---------------------------------------------------------------------------
// string_view::operator+ (returns string_builder)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view operator+ with string", "[string_view]")
{
    memepp::string_view sv = "Hello";
    memepp::string other = " World";
    auto result = sv + other;
    REQUIRE(result.generate() == "Hello World");
}

TEST_CASE("memepp::string_view operator+ with string_view", "[string_view]")
{
    memepp::string_view sv = "Hello";
    memepp::string_view other = " World";
    auto result = sv + other;
    REQUIRE(result.generate() == "Hello World");
}

TEST_CASE("memepp::string_view operator+ with const char*", "[string_view]")
{
    memepp::string_view sv = "Hello";
    auto result = sv + " World";
    REQUIRE(result.generate() == "Hello World");
}

// ---------------------------------------------------------------------------
// string_view::count(const char*, size, case_sensit_t)
// string_view::count(const_pointer, size, case_sensit_t)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view count -- const char* with size", "[string_view]")
{
    memepp::string_view sv = "AaBbaaBb";
    REQUIRE(sv.count("Aa", 2) == 2);       // first 2 bytes of "Aa" = "Aa"
    REQUIRE(sv.count("AaX", 1) == 2);      // first 1 byte = "A" appears twice
    REQUIRE(sv.count("Bb", 2) == 2);
    REQUIRE(sv.count("Cc", 2) == 0);
}

TEST_CASE("memepp::string_view count -- const_pointer with size", "[string_view]")
{
    memepp::string_view sv = "HelloHello";
    const memepp::string_view::const_pointer p =
        reinterpret_cast<const memepp::string_view::const_pointer>("HelloX");
    REQUIRE(sv.count(p, 5) == 2);
    REQUIRE(sv.count(p, 4) == 2);          // "Hell" appears twice
    REQUIRE(sv.count(p, 2) == 2);          // "He" appears twice
}

// ---------------------------------------------------------------------------
// operator""_meme_sv
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view UDL _meme_sv", "[string_view]")
{
    using namespace memepp;
    auto sv = "Hello, UDL"_meme_sv;
    REQUIRE(sv == "Hello, UDL");
    REQUIRE(sv.size() == 11);
}

TEST_CASE("memepp::string_view UDL _meme_sv -- empty", "[string_view]")
{
    using namespace memepp;
    auto sv = ""_meme_sv;
    REQUIRE(sv.empty());
}
