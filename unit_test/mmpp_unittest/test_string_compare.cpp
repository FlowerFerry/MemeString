
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/convert/std/string.hpp>

#include <string>

TEST_CASE("memepp::string compare", "[string]")
{
    // --- shared data ---
    memepp::string s_world  = "Hello World.";
    memepp::string s_world_ = "Hello World!";
    const char* sz_world  = "Hello World.";
    const char* sz_world_ = "Hello World!";
    memepp::string_view sv_world  = sz_world;
    memepp::string_view sv_world_ = sz_world_;

    memepp::string s_hello = "Hello";
    const char* sz_hello = "Hello";
    memepp::string_view sv_hello = sz_hello;

    memepp::string s_long = "Hello World...";
    const char* sz_long = "Hello World...";
    memepp::string_view sv_long = sz_long;

    SECTION("differ at last char: 'Hello World.' vs 'Hello World!'")
    {
        // string vs string
        REQUIRE(s_world >  s_world_);
        REQUIRE(s_world >= s_world_);
        REQUIRE(s_world_ <  s_world);
        REQUIRE(s_world_ <= s_world);

        // string vs const char*
        REQUIRE(s_world >  sz_world_);
        REQUIRE(s_world >= sz_world_);
        REQUIRE(sz_world_ <  s_world);
        REQUIRE(sz_world_ <= s_world);

        // string vs string_view
        REQUIRE(s_world >  sv_world_);
        REQUIRE(s_world >= sv_world_);
        REQUIRE(sv_world_ <  s_world);
        REQUIRE(sv_world_ <= s_world);

        // equal case (same content, different types)
        REQUIRE(s_world <= sz_world);
        REQUIRE(s_world >= sz_world);
        REQUIRE(sz_world <= s_world);
        REQUIRE(sz_world >= s_world);

        REQUIRE(s_world <= sv_world);
        REQUIRE(s_world >= sv_world);
        REQUIRE(sv_world <= s_world);
        REQUIRE(sv_world >= s_world);
    }

    SECTION("prefix shorter: 'Hello World.' vs 'Hello'")
    {
        // string vs string
        REQUIRE(s_hello <  s_world);
        REQUIRE(s_hello <= s_world);
        REQUIRE(s_world >  s_hello);
        REQUIRE(s_world >= s_hello);

        // string vs const char*
        REQUIRE(s_hello <  sz_world);
        REQUIRE(s_hello <= sz_world);
        REQUIRE(sz_world >  s_hello);
        REQUIRE(sz_world >= s_hello);

        // string vs string_view
        REQUIRE(s_hello <  sv_world);
        REQUIRE(s_hello <= sv_world);
        REQUIRE(sv_world >  s_hello);
        REQUIRE(sv_world >= s_hello);

        // equal case
        REQUIRE(s_hello <= sz_hello);
        REQUIRE(sz_hello >= s_hello);
        REQUIRE(s_hello <= sv_hello);
        REQUIRE(sv_hello >= s_hello);
    }

    SECTION("prefix longer: 'Hello World.' vs 'Hello World...'")
    {
        // string vs string
        REQUIRE(s_long >  s_world);
        REQUIRE(s_long >= s_world);
        REQUIRE(s_world <  s_long);
        REQUIRE(s_world <= s_long);

        // string vs const char*
        REQUIRE(s_long >  sz_world);
        REQUIRE(s_long >= sz_world);
        REQUIRE(sz_world <  s_long);
        REQUIRE(sz_world <= s_long);

        // string vs string_view
        REQUIRE(s_long >  sv_world);
        REQUIRE(s_long >= sv_world);
        REQUIRE(sv_world <  s_long);
        REQUIRE(sv_world <= s_long);

        // equal case
        REQUIRE(s_long >= sz_long);
        REQUIRE(sz_long <= s_long);
        REQUIRE(s_long >= sv_long);
        REQUIRE(sv_long <= s_long);
    }

    SECTION("empty strings and default rune")
    {
        memepp::string s_empty1;
        memepp::string s_empty2;
        const char* sz_empty = "";
        memepp::string_view sv_empty = sz_empty;
        memepp::rune r_default;

        // string vs string (both empty)
        REQUIRE(s_empty1 <= s_empty2);
        REQUIRE(s_empty1 >= s_empty2);
        REQUIRE(s_empty2 <= s_empty1);
        REQUIRE(s_empty2 >= s_empty1);

        // string vs const char*
        REQUIRE(s_empty1 <= sz_empty);
        REQUIRE(s_empty1 >= sz_empty);
        REQUIRE(sz_empty <= s_empty1);
        REQUIRE(sz_empty >= s_empty1);

        // string vs string_view
        REQUIRE(s_empty1 <= sv_empty);
        REQUIRE(s_empty1 >= sv_empty);
        REQUIRE(sv_empty <= s_empty1);
        REQUIRE(sv_empty >= s_empty1);

        // string vs rune (default)
        REQUIRE(s_empty1 <= r_default);
        REQUIRE(s_empty1 >= r_default);
        REQUIRE(r_default <= s_empty1);
        REQUIRE(r_default >= s_empty1);
    }

    SECTION("string vs rune")
    {
        memepp::rune r_H = 'H';

        // string vs rune
        REQUIRE(s_world >  r_H);
        REQUIRE(s_world >= r_H);
        REQUIRE(r_H <  s_world);
        REQUIRE(r_H <= s_world);
    }
}

// ---------------------------------------------------------------------------
// operator== / operator!= (equality)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string operator== (string, string)", "[string]")
{
    memepp::string a("Hello");
    memepp::string b("Hello");
    memepp::string c("World");

    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
}

TEST_CASE("memepp::string operator!= (string, string)", "[string]")
{
    memepp::string a("Hello");
    memepp::string b("World");
    memepp::string c("Hello");

    REQUIRE(a != b);
    REQUIRE_FALSE(a != c);
}

TEST_CASE("memepp::string operator== (const char* lhs, string rhs)", "[string]")
{
    memepp::string s("Hello");

    REQUIRE("Hello" == s);
    REQUIRE_FALSE("World" == s);
}

TEST_CASE("memepp::string operator!= (const char* lhs, string rhs)", "[string]")
{
    memepp::string s("Hello");

    REQUIRE("World" != s);
    REQUIRE_FALSE("Hello" != s);
}

TEST_CASE("memepp::string operator== (string lhs, const char* rhs)", "[string]")
{
    memepp::string s("Hello");

    REQUIRE(s == "Hello");
    REQUIRE_FALSE(s == "World");
}

TEST_CASE("memepp::string operator!= (string lhs, const char* rhs)", "[string]")
{
    memepp::string s("Hello");

    REQUIRE(s != "World");
    REQUIRE_FALSE(s != "Hello");
}

TEST_CASE("memepp::string operator== (string::const_pointer lhs, string rhs)", "[string]")
{
    memepp::string s("Hello");
    memepp::string s_same("Hello");
    memepp::string s_diff("World");
    memepp::string::const_pointer p = s.bytes();

    REQUIRE(p == s_same);
    REQUIRE_FALSE(p == s_diff);
}

TEST_CASE("memepp::string operator!= (string::const_pointer lhs, string rhs)", "[string]")
{
    memepp::string s("Hello");
    memepp::string s_same("Hello");
    memepp::string s_diff("World");
    memepp::string::const_pointer p = s.bytes();

    REQUIRE(p != s_diff);
    REQUIRE_FALSE(p != s_same);
}

TEST_CASE("memepp::string operator== (string lhs, string::const_pointer rhs)", "[string]")
{
    memepp::string s("Hello");
    memepp::string s_same("Hello");
    memepp::string s_diff("World");
    memepp::string::const_pointer p = s.bytes();

    REQUIRE(s_same == p);
    REQUIRE_FALSE(s_diff == p);
}

TEST_CASE("memepp::string operator!= (string lhs, string::const_pointer rhs)", "[string]")
{
    memepp::string s("Hello");
    memepp::string s_same("Hello");
    memepp::string s_diff("World");
    memepp::string::const_pointer p = s.bytes();

    REQUIRE(s_diff != p);
    REQUIRE_FALSE(s_same != p);
}

// ---------------------------------------------------------------------------
// operator<  operator>  operator<=  operator>=  with string::const_pointer
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string compare with string::const_pointer", "[string]")
{
    memepp::string s_apple("Apple");
    memepp::string s_banana("Banana");
    memepp::string s_apple2("Apple");

    memepp::string::const_pointer p_apple  = s_apple.bytes();
    memepp::string::const_pointer p_banana = s_banana.bytes();

    // string op const_pointer
    REQUIRE(s_apple  <  p_banana);
    REQUIRE(s_banana >  p_apple);
    REQUIRE(s_apple  <= p_banana);
    REQUIRE(s_banana >= p_apple);

    // const_pointer op string
    REQUIRE(p_apple  <  s_banana);
    REQUIRE(p_banana >  s_apple);
    REQUIRE(p_apple  <= s_banana);
    REQUIRE(p_banana >= s_apple);

    // equal cases (<=, >= true; <, > false)
    REQUIRE(s_apple  <= p_apple);
    REQUIRE(s_apple  >= p_apple);
    REQUIRE(p_apple  <= s_apple2);
    REQUIRE(p_apple  >= s_apple2);
    REQUIRE_FALSE(s_apple  <  p_apple);
    REQUIRE_FALSE(s_apple  >  p_apple);
    REQUIRE_FALSE(p_apple  <  s_apple2);
    REQUIRE_FALSE(p_apple  >  s_apple2);
}

// ---------------------------------------------------------------------------
// Non-ASCII / high bytes, embedded NUL, and string::compare() return value
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string compare: high bytes, embedded NUL, compare() value",
          "[string]")
{
    SECTION("non-ASCII / high bytes: memcmp is unsigned so 0xFF > 'a'")
    {
        // Single-byte: 0xFF (255) > 'a' (97) under unsigned comparison
        memepp::string s_ff("\xFF", 1);
        memepp::string s_a("a");
        REQUIRE(s_ff.compare(s_a) == 1);
        REQUIRE(s_a.compare(s_ff) == -1);
        REQUIRE(s_ff > s_a);
        REQUIRE(s_a < s_ff);

        // 0x80 (128) > 'z' (122)
        memepp::string s_80("\x80", 1);
        memepp::string s_z("z");
        REQUIRE(s_80.compare(s_z) == 1);
        REQUIRE(s_z.compare(s_80) == -1);

        // Two high-byte strings differing at second byte
        memepp::string s_ab("\xFF\x00", 2);
        memepp::string s_ac("\xFF\x01", 2);
        REQUIRE(s_ab.compare(s_ac) == -1);
        REQUIRE(s_ac.compare(s_ab) == 1);
    }

    SECTION("embedded NUL: compare treats it as a 0 byte, not terminator")
    {
        // "a\0b" (3 bytes) vs "a\0c" (3 bytes) — differ at byte 2
        memepp::string s_abc("a\0b", 3);
        memepp::string s_abz("a\0c", 3);
        REQUIRE(s_abc.compare(s_abz) == -1);   // 'b' < 'c'
        REQUIRE(s_abz.compare(s_abc) == 1);

        // "a" (1 byte) vs "a\0b" (3 bytes) — prefix match, shorter is less
        memepp::string s_a("a");
        REQUIRE(s_a.compare(s_abc) == -1);
        REQUIRE(s_abc.compare(s_a) == 1);

        // "\0" (1 byte) vs "" (0 bytes)
        memepp::string s_nul("\0", 1);
        memepp::string s_empty;
        REQUIRE(s_nul.compare(s_empty) == 1);
        REQUIRE(s_empty.compare(s_nul) == -1);
    }

    SECTION("string::compare() return value: exactly -1 / 0 / +1")
    {
        memepp::string s1("Hello");
        memepp::string s2("Hello");
        memepp::string s3("World");

        // Equal → 0
        REQUIRE(s1.compare(s2) == 0);

        // Less → -1 (MemeString_compare returns exactly -1/0/1)
        REQUIRE(s1.compare(s3) == -1);

        // Greater → +1
        REQUIRE(s3.compare(s1) == 1);

        // Symmetry: compare(a,b) == -compare(b,a)
        REQUIRE(s1.compare(s3) == -s3.compare(s1));
    }
}

// ---------------------------------------------------------------------------
// Equality / ordering across storage tiers
// (small / medium / large / user / view)
// ---------------------------------------------------------------------------

namespace {

// `view` has no enumerator in string_storage_t: the C layer tags it with the
// unsafe-storage value, so cast it the same way test_string_at.cpp does.
constexpr memepp::string_storage_t view_storage =
    static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view);

//! Asserts the full relational matrix for two equal-content strings.
template<typename _Lhs, typename _Rhs>
void check_equal(const _Lhs& _lhs, const _Rhs& _rhs)
{
    REQUIRE(_lhs == _rhs);
    REQUIRE_FALSE(_lhs != _rhs);
    REQUIRE(_lhs <= _rhs);
    REQUIRE(_lhs >= _rhs);
    REQUIRE_FALSE(_lhs < _rhs);
    REQUIRE_FALSE(_lhs > _rhs);
}

//! Asserts two different-content strings are unequal and order strictly.
template<typename _Lhs, typename _Rhs>
void check_not_equal(const _Lhs& _lhs, const _Rhs& _rhs)
{
    REQUIRE(_lhs != _rhs);
    REQUIRE_FALSE(_lhs == _rhs);
    // Distinct content must order strictly, in exactly one direction.
    REQUIRE((_lhs < _rhs) != (_lhs > _rhs));
}

} // namespace

TEST_CASE("memepp::string equality across storage tiers", "[string]")
{
    SECTION("short content: small vs medium vs large vs view")
    {
        // 10 bytes fit the inline small buffer, yet the storage hint still forces
        // the medium / large tiers, so all four tiers hold identical bytes.
        const char* data = "0123456789";

        memepp::string      s_small (data, 10);
        memepp::string      s_medium(data, 10, memepp::string_storage_t::medium);
        memepp::string      s_large (data, 10, memepp::string_storage_t::large);
        memepp::string_view s_view  (data, 10);

        // Guard the matrix itself: if a tier silently degraded, the comparisons
        // below would no longer be cross-storage at all.
        REQUIRE(s_small .storage_type() == memepp::string_storage_t::small);
        REQUIRE(s_medium.storage_type() == memepp::string_storage_t::medium);
        REQUIRE(s_large .storage_type() == memepp::string_storage_t::large);
        REQUIRE(s_view  .storage_type() == view_storage);

        check_equal(s_small, s_medium);
        check_equal(s_small, s_large);
        check_equal(s_small, s_view);
        check_equal(s_medium, s_large);
        check_equal(s_medium, s_view);
        check_equal(s_large, s_view);

        // Negative: same length, last byte differs -> every tier must disagree.
        const char* other = "0123456788";

        memepp::string      d_small (other, 10);
        memepp::string      d_medium(other, 10, memepp::string_storage_t::medium);
        memepp::string      d_large (other, 10, memepp::string_storage_t::large);
        memepp::string_view d_view  (other, 10);

        check_not_equal(s_small, d_small);
        check_not_equal(s_small, d_medium);
        check_not_equal(s_small, d_large);
        check_not_equal(s_small, d_view);
    }

    SECTION("long content: medium vs large vs user vs view")
    {
        // The user tier is only produced above medium_limit + MMSTR__OBJ_SIZE, so
        // a long payload is needed to exercise all owning tiers at once. Read the
        // limit instead of hard-coding it: the MediumLimitGuard listener in
        // mmpp_unittest.cpp saves/restores it around every test case.
        const memepp::string::size_type len =
            static_cast<memepp::string::size_type>(MemeStringOption_getStorageMediumLimit())
            + static_cast<memepp::string::size_type>(MMSTR__OBJ_SIZE)
            + 16;

        const std::string payload(static_cast<std::size_t>(len), 'y');

        memepp::string      l_medium(payload.data(), static_cast<mmint_t>(payload.size()),
                                     memepp::string_storage_t::medium);
        memepp::string      l_large (payload.data(), static_cast<mmint_t>(payload.size()),
                                     memepp::string_storage_t::large);
        memepp::string      l_auto  (payload.data(), static_cast<mmint_t>(payload.size()));
        memepp::string_view l_view  (payload.data(), static_cast<mmint_t>(payload.size()));

        std::string   user_src(payload);
        memepp::string l_user = memepp::from(std::move(user_src));

        REQUIRE(l_medium.storage_type() == memepp::string_storage_t::medium);
        REQUIRE(l_large .storage_type() == memepp::string_storage_t::large);
        REQUIRE(l_auto  .storage_type() == memepp::string_storage_t::large);
        REQUIRE(l_user  .storage_type() == memepp::string_storage_t::user);
        REQUIRE(l_view  .storage_type() == view_storage);

        check_equal(l_medium, l_large);
        check_equal(l_medium, l_user);
        check_equal(l_medium, l_view);
        check_equal(l_medium, l_auto);
        check_equal(l_large,  l_user);
        check_equal(l_large,  l_view);
        check_equal(l_large,  l_auto);
        check_equal(l_user,   l_view);
        check_equal(l_user,   l_auto);
        check_equal(l_view,   l_auto);

        // Negative: differ in the very last byte, i.e. past the 90%-offset memcmp
        // probe the C helper uses first, across every owning tier.
        std::string other_src(payload);
        other_src.back() = 'z';

        memepp::string      o_medium(other_src.data(), static_cast<mmint_t>(other_src.size()),
                                     memepp::string_storage_t::medium);
        memepp::string      o_large (other_src.data(), static_cast<mmint_t>(other_src.size()),
                                     memepp::string_storage_t::large);
        memepp::string_view o_view  (other_src.data(), static_cast<mmint_t>(other_src.size()));

        std::string   other_user_src(other_src);
        memepp::string o_user = memepp::from(std::move(other_user_src));

        check_not_equal(l_medium, o_medium);
        check_not_equal(l_medium, o_large);
        check_not_equal(l_medium, o_user);
        check_not_equal(l_medium, o_view);
        check_not_equal(l_large,  o_user);
        check_not_equal(l_large,  o_view);
        check_not_equal(l_user,   o_view);
    }

    SECTION("empty strings across tiers")
    {
        memepp::string      e_auto;
        memepp::string      e_medium("", 0, memepp::string_storage_t::medium);
        memepp::string      e_large ("", 0, memepp::string_storage_t::large);
        // A default-constructed view stays small; an explicit 0-length one is a view.
        memepp::string_view e_view  ("", 0);

        REQUIRE(e_auto  .storage_type() == memepp::string_storage_t::small);
        REQUIRE(e_medium.storage_type() == memepp::string_storage_t::medium);
        REQUIRE(e_large .storage_type() == memepp::string_storage_t::large);
        REQUIRE(e_view  .storage_type() == view_storage);

        // Zero-length hits the early `lhslen == 0` branch of the C equality helper.
        check_equal(e_auto, e_medium);
        check_equal(e_auto, e_large);
        check_equal(e_auto, e_view);
        check_equal(e_medium, e_large);
        check_equal(e_medium, e_view);
        check_equal(e_large, e_view);
    }

    SECTION("embedded NUL compares by byteSize, not by strlen")
    {
        // "a\0b" is 3 bytes with an interior NUL. A strlen-based comparison would
        // truncate it to "a" and wrongly report equality.
        const char* nul_data = "a\0b";

        memepp::string n_small (nul_data, 3);
        memepp::string n_medium(nul_data, 3, memepp::string_storage_t::medium);
        memepp::string n_large (nul_data, 3, memepp::string_storage_t::large);

        REQUIRE(n_small .storage_type() == memepp::string_storage_t::small);
        REQUIRE(n_medium.storage_type() == memepp::string_storage_t::medium);
        REQUIRE(n_large .storage_type() == memepp::string_storage_t::large);
        REQUIRE(n_small .size() == 3);
        REQUIRE(n_medium.size() == 3);
        REQUIRE(n_large .size() == 3);

        check_equal(n_small, n_medium);
        check_equal(n_small, n_large);
        check_equal(n_medium, n_large);

        memepp::string n_truncated("a");     // would match under strlen semantics
        memepp::string n_diff("a\0c", 3);    // differs in the last byte only

        check_not_equal(n_small, n_truncated);
        check_not_equal(n_medium, n_truncated);
        check_not_equal(n_large, n_truncated);
        check_not_equal(n_small, n_diff);
        check_not_equal(n_medium, n_diff);
        check_not_equal(n_large, n_diff);

        // Length dominates: the 1-byte prefix orders before the 3-byte string.
        REQUIRE(n_truncated < n_small);
        REQUIRE(n_truncated < n_medium);
        REQUIRE(n_truncated < n_large);
    }
}
