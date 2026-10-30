#include <catch2/catch.hpp>

#include <memepp/errc.hpp>

#include <mego/err/ec.h>

#include <cstring>
#include <thread>

// ============================================================================
// memepp errc: the thread-local error slot (set_errc / get_errc) and the
// throw_errc mapping documented in AGENTS.md:
//   NOMEM -> memepp::bad_alloc, other -> memepp::errc_except, 0 -> no throw.
// ============================================================================

namespace {

// RAII guard: clears the slot on entry and exit so one test case cannot leak
// its error state into the next (Catch2 runs cases on the same thread).
struct errc_guard
{
    errc_guard() { memepp::set_errc(MGEC__OK); }
    ~errc_guard() { memepp::set_errc(MGEC__OK); }
};

} // anonymous namespace

// ---------------------------------------------------------------------------
// set_errc / get_errc round trip
// ---------------------------------------------------------------------------

TEST_CASE("memepp errc set/get round trip", "[errc]")
{
    errc_guard g;

    memepp::set_errc(MGEC__INVAL);
    REQUIRE(memepp::get_errc() == MGEC__INVAL);

    memepp::set_errc(MGEC__RANGE);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);

    memepp::set_errc(MGEC__OK);
    REQUIRE(memepp::get_errc() == MGEC__OK);
}

// ---------------------------------------------------------------------------
// thread_local isolation: a code set in one thread must not be visible in
// another, and the child must not disturb the parent's slot.
// ---------------------------------------------------------------------------

TEST_CASE("memepp errc is thread_local", "[errc][thread]")
{
    errc_guard g;

    memepp::set_errc(MGEC__NOMEM);
    REQUIRE(memepp::get_errc() == MGEC__NOMEM);

    int child_initial = 0x7ead;   // sentinel no real code matches
    int child_after_set = 0;

    std::thread t([&] {
        child_initial = memepp::get_errc();     // fresh slot: must be OK
        memepp::set_errc(MGEC__INVAL);          // write in the child
        child_after_set = memepp::get_errc();
    });
    t.join();

    // The child started clean: it never saw the parent's NOMEM.
    REQUIRE(child_initial == MGEC__OK);
    REQUIRE(child_after_set == MGEC__INVAL);

    // The child's write did not reach the parent's slot.
    REQUIRE(memepp::get_errc() == MGEC__NOMEM);
}

#if !MMOPT__EXCEPTION_DISABLED

// ---------------------------------------------------------------------------
// throw_errc(code): NOMEM -> memepp::bad_alloc, other -> errc_except,
// 0 -> no throw.
// ---------------------------------------------------------------------------

TEST_CASE("memepp throw_errc maps NOMEM to bad_alloc", "[errc][throw]")
{
    errc_guard g;

    REQUIRE_THROWS_AS(memepp::throw_errc(MGEC__NOMEM), memepp::bad_alloc);

    // bad_alloc is an errc_except carrying the NOMEM code.
    try {
        memepp::throw_errc(MGEC__NOMEM);
        FAIL("expected throw");
    } catch (const memepp::errc_except& e) {
        REQUIRE(e.code() == MGEC__NOMEM);
        REQUIRE(std::strcmp(e.what(), "bad_alloc") == 0);
    }
}

TEST_CASE("memepp throw_errc maps other codes to errc_except", "[errc][throw]")
{
    errc_guard g;

    const int codes[] = { MGEC__ERR, MGEC__INVAL, MGEC__RANGE, MGEC__OPNOTSUPP };
    for (int c : codes) {
        REQUIRE_THROWS_AS(memepp::throw_errc(c), memepp::errc_except);
        // Must NOT be misclassified as bad_alloc.
        try {
            memepp::throw_errc(c);
            FAIL("expected throw");
        } catch (const memepp::bad_alloc&) {
            FAIL("code was misclassified as bad_alloc");
        } catch (const memepp::errc_except& e) {
            REQUIRE(e.code() == c);
            REQUIRE(std::strcmp(e.what(), "errc_exception") == 0);
        }
    }
}

TEST_CASE("memepp throw_errc does not throw for OK", "[errc][throw]")
{
    errc_guard g;

    REQUIRE_NOTHROW(memepp::throw_errc(MGEC__OK));
}

// ---------------------------------------------------------------------------
// throw_errc(code, what): custom message, same code mapping.
// ---------------------------------------------------------------------------

TEST_CASE("memepp throw_errc with custom what", "[errc][throw]")
{
    errc_guard g;

    try {
        memepp::throw_errc(MGEC__INVAL, "custom diagnostic");
        FAIL("expected throw");
    } catch (const memepp::errc_except& e) {
        REQUIRE(e.code() == MGEC__INVAL);
        REQUIRE(std::strcmp(e.what(), "custom diagnostic") == 0);
    }

    // NOMEM keeps the bad_alloc class but takes the custom message.
    try {
        memepp::throw_errc(MGEC__NOMEM, "out of heap");
        FAIL("expected throw");
    } catch (const memepp::bad_alloc& e) {
        REQUIRE(e.code() == MGEC__NOMEM);
        REQUIRE(std::strcmp(e.what(), "out of heap") == 0);
    }

    // OK still does not throw even with a message supplied.
    REQUIRE_NOTHROW(memepp::throw_errc(MGEC__OK, "ignored"));
}

// ---------------------------------------------------------------------------
// errc_except accessors
// ---------------------------------------------------------------------------

TEST_CASE("memepp errc_except code and what", "[errc][throw]")
{
    memepp::errc_except a{ MGEC__RANGE };
    REQUIRE(a.code() == MGEC__RANGE);
    REQUIRE(std::strcmp(a.what(), "errc_exception") == 0);

    memepp::errc_except b{ MGEC__RANGE, "result too large" };
    REQUIRE(b.code() == MGEC__RANGE);
    REQUIRE(std::strcmp(b.what(), "result too large") == 0);

    // It is a std::runtime_error, so generic handlers can catch it.
    try {
        throw b;
    } catch (const std::runtime_error& e) {
        REQUIRE(std::strcmp(e.what(), "result too large") == 0);
    }
}

TEST_CASE("memepp bad_alloc default what", "[errc][throw]")
{
    memepp::bad_alloc a;
    REQUIRE(a.code() == MGEC__NOMEM);
    REQUIRE(std::strcmp(a.what(), "bad_alloc") == 0);

    memepp::bad_alloc b{ "named" };
    REQUIRE(b.code() == MGEC__NOMEM);
    REQUIRE(std::strcmp(b.what(), "named") == 0);
}

#endif // !MMOPT__EXCEPTION_DISABLED
