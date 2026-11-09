/* Null-safe extension tests: every memepp pointer overload treats a null
 * C string as the empty string, so no call reaches strlen(nullptr) or
 * memcmp(nullptr, ...). */

#include <catch2/catch.hpp>

#include <memepp/rune.hpp>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/string_span.hpp>

using namespace memepp;

namespace {

const char* null_cstr()
{
    return nullptr;
}

const MemeByte_t* null_bytes()
{
    return nullptr;
}

} // namespace

// ---------------------------------------------------------------------------
//  1. rune comparison with a null C string
// ---------------------------------------------------------------------------

TEST_CASE("rune == nullptr compares against the empty string", "[rune][nullsafe]")
{
    rune r('a');

    /* A non-empty rune is never equal to "". */
    REQUIRE_FALSE(r == null_cstr());
    REQUIRE(r != null_cstr());

    /* An empty rune equals "". */
    rune empty;
    REQUIRE(empty == null_cstr());
    REQUIRE_FALSE(empty != null_cstr());

    /* Must stay consistent with an explicit empty string. */
    REQUIRE((r == null_cstr()) == (r == ""));
    REQUIRE((empty == null_cstr()) == (empty == ""));
}

#if defined(__cpp_char8_t)
TEST_CASE("rune == char8_t nullptr", "[rune][nullsafe]")
{
    rune r('a');
    REQUIRE((r == static_cast<const char8_t*>(nullptr)) == (r == ""));
}
#endif

// ---------------------------------------------------------------------------
//  2. count(nullptr)
// ---------------------------------------------------------------------------

TEST_CASE("count nullptr returns zero", "[string][string_view][nullsafe]")
{
    string s("abcabcabc");
    string_view sv("abcabcabc");

    REQUIRE(s.count(null_cstr()) == 0);
    REQUIRE(s.count(null_bytes()) == 0);
    REQUIRE(s.count(null_cstr(), 0) == 0);
    REQUIRE(s.count(null_bytes(), 0) == 0);

    REQUIRE(sv.count(null_cstr()) == 0);
    REQUIRE(sv.count(null_bytes()) == 0);
    REQUIRE(sv.count(null_cstr(), 0) == 0);
    REQUIRE(sv.count(null_bytes(), 0) == 0);

    /* An explicit empty needle counts as "not found", same as nullptr. */
    REQUIRE(s.count(null_cstr()) == s.count(""));
    REQUIRE(sv.count(null_cstr()) == sv.count(""));
}

// ---------------------------------------------------------------------------
//  3. string_view::rfind(nullptr)
// ---------------------------------------------------------------------------

TEST_CASE("string_view rfind nullptr returns the empty-needle position",
          "[string_view][nullsafe]")
{
    string_view sv("hello");
    const string_view::size_type npos = string_view::npos;

    /* Empty needle: min(pos, size()). */
    REQUIRE(sv.rfind(null_cstr()) == sv.size());
    REQUIRE(sv.rfind(null_cstr(), 3) == 3);
    REQUIRE(sv.rfind(null_cstr(), 0) == 0);
    REQUIRE(sv.rfind(null_cstr(), 99) == sv.size());

    REQUIRE(sv.rfind(null_cstr(), npos, 0) == sv.size());
    REQUIRE(sv.rfind(null_cstr(), 3, 0) == 3);

    REQUIRE(sv.rfind(null_bytes()) == sv.size());
    REQUIRE(sv.rfind(null_bytes(), 3) == 3);
    REQUIRE(sv.rfind(null_bytes(), npos, 0) == sv.size());

    /* Agreement with an explicit empty needle. */
    REQUIRE(sv.rfind(null_cstr(), 3) == sv.rfind("", 3));
    REQUIRE(sv.rfind(null_cstr()) == sv.rfind(""));
}

// ---------------------------------------------------------------------------
//  4. span / view construction from (nullptr, count)
// ---------------------------------------------------------------------------

TEST_CASE("string_span nullptr with a non-zero count is empty",
          "[string_span][nullsafe]")
{
    /* Keeping the count would give data_ == NULL with size_ > 0, and any
     * later memcmp or iteration would read through the null pointer. */
    string_span a(null_cstr(), 5);
    REQUIRE(a.empty());
    REQUIRE(a.size() == 0);
    REQUIRE(a.data() == nullptr);

    string_span b(null_bytes(), 5);
    REQUIRE(b.empty());
    REQUIRE(b.size() == 0);

    /* With count 0 the result is the same. */
    string_span c(null_cstr(), 0);
    REQUIRE(c.empty());
}

TEST_CASE("string_view nullptr with a non-zero count is empty",
          "[string_view][nullsafe]")
{
    string_view a(null_cstr(), 5);
    REQUIRE(a.empty());
    REQUIRE(a.size() == 0);

    string_view b(null_bytes(), 5);
    REQUIRE(b.empty());
    REQUIRE(b.size() == 0);

    /* A zero count was already fine and must stay so. */
    string_view c(null_cstr(), 0);
    REQUIRE(c.empty());
}

// ---------------------------------------------------------------------------
//  5 / 6. index_of / last_index_of with a null C string
// ---------------------------------------------------------------------------

TEST_CASE("index_of nullptr returns the empty-needle position",
          "[string][string_view][nullsafe]")
{
    string s("hello");
    string_view sv("hello");

    REQUIRE(s.index_of(null_cstr()) == 0);
    REQUIRE(s.index_of(null_cstr(), false) == 0);
    REQUIRE(s.index_of(null_cstr(), 0, 0, false) == 0);

    REQUIRE(sv.index_of(null_cstr()) == 0);
    REQUIRE(sv.index_of(null_cstr(), false) == 0);
    REQUIRE(sv.index_of(null_cstr(), 0, 0, false) == 0);

    /* NOTE: the C layer reports an explicit empty needle as "not found",
     * so nullptr is deliberately NOT compared against index_of("").
     * The null-safe extension only guarantees the nullptr path itself is
     * well defined and yields the empty-needle position above. */
}

TEST_CASE("last_index_of nullptr returns size()",
          "[string][string_view][nullsafe]")
{
    string s("hello");
    string_view sv("hello");

    REQUIRE(s.last_index_of(null_cstr()) == s.size());
    REQUIRE(s.last_index_of(null_cstr(), false) == s.size());
    REQUIRE(s.last_index_of(null_cstr(), 0, s.size(), s.size(), false) == s.size());

    REQUIRE(sv.last_index_of(null_cstr()) == sv.size());
    REQUIRE(sv.last_index_of(null_cstr(), false) == sv.size());
    REQUIRE(sv.last_index_of(null_cstr(), 0, sv.size(), sv.size(), false) == sv.size());

    /* NOTE: the C layer reports an explicit empty needle as "not found",
     * so nullptr is deliberately NOT compared against last_index_of("").
     * The null-safe extension guarantees the nullptr path itself is well
     * defined and returns the empty-needle position above. */
}

// ---------------------------------------------------------------------------
//  7. relational operators against a null C string
// ---------------------------------------------------------------------------

TEST_CASE("string relational operators treat nullptr as an empty string",
          "[string][nullsafe]")
{
    string abc("abc");

    /* nullptr behaves exactly like "". */
    REQUIRE((abc < null_cstr()) == (abc < ""));
    REQUIRE((abc > null_cstr()) == (abc > ""));
    REQUIRE((abc <= null_cstr()) == (abc <= ""));
    REQUIRE((abc >= null_cstr()) == (abc >= ""));
    REQUIRE((null_cstr() < abc) == ("" < abc));
    REQUIRE((null_cstr() > abc) == ("" > abc));

    /* "abc" is greater than the empty string. */
    REQUIRE(abc > null_cstr());
    REQUIRE(abc >= null_cstr());
    REQUIRE_FALSE(abc < null_cstr());
    REQUIRE_FALSE(abc == null_cstr());

    string empty_str;
    REQUIRE(empty_str == null_cstr());
    REQUIRE_FALSE(empty_str != null_cstr());
    REQUIRE(empty_str <= null_cstr());
    REQUIRE(empty_str >= null_cstr());
}
